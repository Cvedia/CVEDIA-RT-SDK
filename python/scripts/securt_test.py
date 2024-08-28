import numpy as np
import time

import os
import sys

from cvedia import rt
import requests
from jsonschema import validate, ValidationError
import cv2
import json
import pytest
from time import sleep

from pathlib import Path

@pytest.fixture(scope='module')
def api_setup():
	rt.api_initialize()

	rt.solutions_discover(os.getenv("SOLUTIONS_DIR", default=""), True)

	yield rt

	rt.api_shutdown()

@pytest.fixture
def setup(api_setup):
	# Call the create method
	handle = rt.core_create_context("", "")
	assert handle != -1

	assert rt.securt_create_instance(handle, "my_instance_name") != -1

	assert rt.core_set_blocking_readahead_queue(handle, 1) != -1

	# Start processing
	assert rt.core_start(handle) != -1

	yield handle

#    x = rt.securt_stop(handle)
#    if x == -1:
#        pytest.exit("decided to stop the test run")

	# Cleanup after tests
	assert rt.securt_destroy_instance(handle) != -1

	rt.core_delete_context(handle)

def validate_json_objects(json_strings, id_to_check, schema_url):
	# Fetch the JSON schema
	schema = requests.get(schema_url).json()

	# Process each JSON string
	for json_string in json_strings:
		# Convert JSON string to Python dictionary (event)
		event = json.loads(json_string)

		# Check event by $id and validate if it matches
		if event.get('$id') == id_to_check:
			try:
				validate(instance=event, schema=schema)
			except ValidationError as e:
				print(f"Event with $id {id_to_check} is not valid. Error: {e}")
				return False
	# If all JSON strings are valid
	return True

def play_video(handle, type, speed_mul, second_object=False,steps=20, xmax=320,extra_empty_frames=0):
	script_dir = os.path.dirname(os.path.realpath(__file__))

	img_path = Path(script_dir) / 'images' / f'{type}.jpg'

	# Load the object image
	object_img = cv2.imread(str(img_path))

	# Get the dimensions of the object image
	obj_height, obj_width = object_img.shape[:2]

	# Compute the steps in x-direction to move the image from left to right
	step_size = (xmax - obj_width) // steps

	step_size = (int)(step_size * speed_mul)

	timestamp = 0

	start_time = time.time()  # Save the start time

	for frame_count in range(steps):
		# Create a 320x320 black canvas
		canvas = np.zeros((320, 320, 3), dtype='uint8')

		# Calculate the top left position of where to place the object on the canvas
		pos_x = step_size * frame_count
		pos_y = 0  # keep it at the top of the canvas

		# Place the object onto the canvas
		canvas[pos_y:pos_y+obj_height, pos_x:pos_x+obj_width] = object_img

		# Follow behind first object at 100px distance, make sure we dont go out of bounds
		if second_object:
			# Calculate the top left position of where to place the object on the canvas
			pos_x = step_size * frame_count - 100
			pos_y = 0

			if pos_x > 0:
				# Place the object onto the canvas
				canvas[pos_y:pos_y+obj_height, pos_x:pos_x+obj_width] = object_img

		#cv2.imwrite(f'd:/tmp2/canvas_frame_{frame_count}.jpg', canvas)
		timestamp += 100

		# Push the frame to the instance
		resp = False
		while not resp:
			# Check if 15 seconds have passed
			if time.time() - start_time > 15:
				raise AssertionError("Timeout: Consuming events took longer than 15 seconds")

			resp = rt.core_push_raw_frame(handle, canvas, timestamp)

	#Push empty frames with 100 ms interval
	for empty_frame_count in range(extra_empty_frames):
		# Create a 320x320 black canvas
		canvas = np.zeros((320, 320, 3), dtype='uint8')

		#cv2.imwrite(f'd:/tmp2/canvas_frame_{empty_frame_count+frame_count}.jpg', canvas)
		timestamp += 100

		resp = False
		while not resp:
			# Check if 15 seconds have passed
			if time.time() - start_time > 15:
				raise AssertionError("Timeout: Consuming events took longer than 15 seconds")

			resp = rt.core_push_raw_frame(handle, canvas, timestamp)

	# flush out any track data
	for frame_count in range(15):
		# Create a 320x320 black canvas
		canvas = np.zeros((320, 320, 3), dtype='uint8')

		timestamp += 10000

		resp = False
		while not resp:
			# Check if 15 seconds have passed
			if time.time() - start_time > 15:
				raise AssertionError("Timeout: Consuming events took longer than 15 seconds")

			resp = rt.core_push_raw_frame(handle, canvas, timestamp)

	# Call the consume_metadata method
	metadata_list = []

	while True:
		# Check if 15 seconds have passed
		if time.time() - start_time > 15:
			raise AssertionError("Timeout: Consuming events took longer than 15 seconds")

		metadata = rt.core_consume_events(handle)
#        print(metadata)
		if metadata is None:
			break

		arr = json.loads(metadata)
		for entry in arr:
			metadata_list.append(entry["object"])

	return metadata_list

def check_json_types(json_list, types):
	# Parse JSON strings and get all "$id" field values
	json_ids = [json.loads(json_str).get('$id') for json_str in json_list]
	# Check if all types are found in json "$id" fields
	return all(type_value in json_ids for type_value in types)

def count_event_type(json_list, event_type):
	# Parse JSON strings and get all "$id" field values
	json_ids = [json.loads(json_str).get('$id') for json_str in json_list]
	# Count the occurrences of the event_type in json "$id" fields
	return json_ids.count(event_type)

def check_track_has_events(json_list):
	for json_str in json_list:
		try:
			data = json.loads(json_str)
			if 'events' in data and isinstance(data['events'], list) and len(data['events']) >= 1:
				return True
		except json.JSONDecodeError:
			pass  # JSON is not valid, continue to the next one
	return False

def assert_no_events(metadata_list, event_types):
	for event_type in event_types:
		assert not check_json_types(metadata_list, [event_type])

def assert_events(metadata_list, event_types, schema_base_url='https://bin.cvedia.com/schema'):
	for event_type in event_types:
		assert check_json_types(metadata_list, [event_type]), f'Event type {event_type} not found in metadata'
		assert validate_json_objects(metadata_list, event_type, f'{schema_base_url}/{event_type}-v1.json'), f'Failed to validate JSON objects for event type {event_type}'

def assert_events_once(metadata_list, event_types, schema_base_url='https://bin.cvedia.com/schema'):
	for event_type in event_types:
		event_type_occurrences = count_event_type(metadata_list, event_type)
		assert event_type_occurrences == 1, f'Event type {event_type} does not exist exactly once in metadata (found {event_type_occurrences} times)'
		assert validate_json_objects(metadata_list, event_type, f'{schema_base_url}/{event_type}-v1.json'), f'Failed to validate JSON objects for event type {event_type}'

def assert_attribute_once(metadata_list, attribute_name, attribute_value):
	# find all attribute events
	attributes = [json.loads(json_str) for json_str in metadata_list if json.loads(json_str).get('$id') == 'attribute']
	for attribute in attributes:
		name = attribute['name']
		value = attribute['value']
		if name == attribute_name:
			if value == attribute_value:
				return
			else:
				assert False, f'Attribute {attribute_name} with {value} does not match expected value {attribute_value}'

	assert False, f'Attribute {attribute_name} does not exist in metadata'

def test_get_version(setup):
	handle = setup
	version = rt.core_get_version(handle)
	assert version != ""

def test_get_instance_id(setup):
	handle = setup

	instance_id = rt.core_get_instance_id(handle)
	# check if instance_id is a GUID
	assert len(instance_id) == 36

def test_get_instance_name(setup):
	handle = setup

	instance_name = rt.core_get_instance_name(handle)
	assert instance_name == "my_instance_name"

def test_get_frame_rate_limit(setup):
	handle = setup

	framerate_limit = rt.core_get_frame_rate_limit(handle)
	assert framerate_limit == 0

def test_set_frame_rate_limit(setup):
	handle = setup

	rt.core_set_frame_rate_limit(handle, 10)
	framerate_limit = rt.core_get_frame_rate_limit(handle)
	assert framerate_limit == 10

def test_get_frames_processed_none(setup):
	handle = setup

	frames_processed = rt.core_get_frames_processed(handle)
	assert frames_processed == 0

def test_get_frames_processed_video(setup):
	handle = setup

	metadata_list = play_video(handle, 'person', 1)

	frames_processed = rt.core_get_frames_processed(handle)
	assert frames_processed == 35

def test_need_data(setup):
	handle = setup

	need_data = rt.core_need_data(handle, 10)
	assert need_data == True

def test_create_area_twice(setup):
	handle = setup

	pts = [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]]

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", pts, [1], False, 1, [0.0,0.0,0.0])
	assert area_created != -1

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", pts, [1], False, 1, [0.0,0.0,0.0])
	assert area_created != 1

	assert rt.core_get_track_count(handle) == 0

def test_delete_area_twice(setup):
	handle = setup

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 1, [0.0,0.0,0.0])
	assert area_created != -1

	area_deleted = rt.securt_delete_area(handle, "area_id")
	assert area_deleted != -1

	area_deleted = rt.securt_delete_area(handle, "area_id")
	assert area_deleted != 1

	assert rt.core_get_track_count(handle) == 0

def test_create_area_delete(setup):
	handle = setup

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 1, [0.0,0.0,0.0])
	assert area_created != -1

	area_deleted = rt.securt_delete_area(handle, "area_id")
	assert area_deleted != -1

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 1, [0.0,0.0,0.0])
	assert area_created != -1

	assert rt.core_get_track_count(handle) == 0

@pytest.mark.parametrize("det_sensitivity", [1, 2, 3])
@pytest.mark.parametrize("move_sensitivity", [1, 2, 3])
def test_create_area_person_enter(setup, det_sensitivity, move_sensitivity):
	handle = setup

	rt.core_set_detection_sensitivity(handle, det_sensitivity)
	rt.core_set_movement_sensitivity(handle, move_sensitivity)

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 1, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 1)

	assert_no_events(metadata_list, ['event-area-exit'])
	assert_events_once(metadata_list, ['event-area-enter', 'crop'])
	assert_events(metadata_list, ['track'])
	assert check_track_has_events(metadata_list)
	assert rt.core_get_track_count(handle) == 0

def test_create_area_person_enter_stationary_ignore(setup):
	handle = setup

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], True, 1, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 0)

	assert_no_events(metadata_list, ['event-area-enter', 'event-area-exit', 'track', 'crop'])
	assert rt.core_get_track_count(handle) == 0

def test_create_area_person_enter_stationary_include(setup):
	handle = setup

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 1, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 0)

	assert_no_events(metadata_list, ['event-area-exit'])
	assert_events_once(metadata_list, ['event-area-enter', 'crop'])
	assert_events(metadata_list, ['track'])
	assert check_track_has_events(metadata_list)
	assert rt.core_get_track_count(handle) == 0

def test_create_area_person_wrong_class(setup):
	handle = setup

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [3], False, 1, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 1)

	assert_no_events(metadata_list, ['event-area-enter', 'event-area-exit', 'track', 'crop'])
	assert rt.core_get_track_count(handle) == 0

def test_create_area_vehicle(setup):
	handle = setup

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [3], False, 1, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'vehicle', 1)

	assert_no_events(metadata_list, ['event-area-exit'])
	assert_events(metadata_list, ['track'])
	assert_events_once(metadata_list, ['event-area-enter', 'crop'])
	assert check_track_has_events(metadata_list)
	assert rt.core_get_track_count(handle) == 0

def test_create_area_person_exit(setup):
	handle = setup

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 2, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 1)

	assert_no_events(metadata_list, ['event-area-enter'])
	assert_events(metadata_list, ['track'])
	assert_events_once(metadata_list, ['event-area-exit', 'crop'])
	assert check_track_has_events(metadata_list)
	assert rt.core_get_track_count(handle) == 0

def test_create_area_person_enter_exit(setup):
	handle = setup

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 3, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 1)

	assert_events_once(metadata_list, ['event-area-enter', 'event-area-exit'])
	assert_events(metadata_list, ['track', 'crop'])
	assert check_track_has_events(metadata_list)
	assert rt.core_get_track_count(handle) == 0

def test_create_intrusion_area_person(setup):
	handle = setup

	area_created = rt.securt_create_intrusion_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 1)

	assert_no_events(metadata_list, ['event-area-exit', 'event-area-enter'])
	assert_events(metadata_list, ['track', 'crop'])
	assert_events_once(metadata_list, ['event-intrusion', 'event-intrusion-end'])
	assert check_track_has_events(metadata_list)
	assert rt.core_get_track_count(handle) == 0

#In this test the person vanishes before entering the area, so we shouldn't get an event
def test_create_almost_intrusion_area_person(setup):
	handle = setup

	area_created = rt.securt_create_intrusion_area(handle, "area_id", "area_name", [[0.5, 0.0], [1.0, 0.0], [1.0, 1.0], [0.5, 1.0]], [1], [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 1,xmax=180,extra_empty_frames=10)

	assert_no_events(metadata_list, ['track', 'crop','event-intrusion', 'event-intrusion-end'])
	assert rt.core_get_track_count(handle) == 0

def test_create_intrusion_area_person_track_age(setup):
	handle = setup

	area_created = rt.securt_create_intrusion_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 1)

	lowest_age = 9999999999

	# check if metadata contains any track with age < 0.1
	for json_str in metadata_list:
		data = json.loads(json_str)
		# find all entries with $id = track
		if data.get('$id') == 'track':
			if data['age_ms'] < lowest_age:
				lowest_age = data['age_ms']

	assert lowest_age < 1000

def test_intrusion_motion_area_include(setup):
	handle = setup

	area_created = rt.securt_create_intrusion_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], [0.0,0.0,0.0])
	assert area_created != -1

	motion_created = rt.securt_set_motion_area(handle, [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]])
	assert motion_created != -1

	metadata_list = play_video(handle, 'person', 1)

	assert_no_events(metadata_list, ['event-area-exit', 'event-area-enter'])
	assert_events(metadata_list, ['track', 'crop'])
	assert_events_once(metadata_list, ['event-intrusion', 'event-intrusion-end'])
	assert check_track_has_events(metadata_list)
	assert rt.core_get_track_count(handle) == 0

def test_intrusion_motion_area_exclude(setup):
	handle = setup

	area_created = rt.securt_create_intrusion_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], [0.0,0.0,0.0])
	assert area_created != -1

	motion_created = rt.securt_set_motion_area(handle, [[0.0, 0.9], [1.0, 0.9], [1.0, 1.0], [0.0, 1.0]])
	assert motion_created

	metadata_list = play_video(handle, 'person', 1)

	assert_no_events(metadata_list, ['event-area-exit', 'event-area-enter', 'event-intrusion', 'track', 'crop'])
	assert rt.core_get_track_count(handle) == 0

def test_create_intrusion_area_person_outside(setup):
	handle = setup

	area_created = rt.securt_create_intrusion_area(handle, "area_id", "area_name", [[0.0, 0.9], [1.0, 0.9], [1.0, 1.0], [0.0, 1.0]], [1], [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 1)

	assert_no_events(metadata_list, ['event-loitering', 'event-loitering-end', 'track', 'crop'])
	assert rt.core_get_track_count(handle) == 0

def test_create_loitering_area_person_moving(setup):
	handle = setup

	area_created = rt.securt_create_loitering_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], 1, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 1)

	assert_events(metadata_list, ['track', 'crop'])
	assert_events_once(metadata_list, ['event-loitering', 'event-loitering-end'])
	assert check_track_has_events(metadata_list)
	assert rt.core_get_track_count(handle) == 0

def test_create_loitering_area_person_stationary(setup):
	handle = setup

	area_created = rt.securt_create_loitering_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], 1, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 0)

	assert not check_track_has_events(metadata_list)
	assert_no_events(metadata_list, ['event-loitering', 'event-loitering-end', 'track', 'crop'])
	assert rt.core_get_track_count(handle) == 0

def test_create_loitering_area_vehicle_moving(setup):
	handle = setup

	area_created = rt.securt_create_loitering_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [3], 1, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'vehicle', 1)

	assert_events(metadata_list, ['track', 'crop'])
	assert_events_once(metadata_list, ['event-loitering', 'event-loitering-end'])
	assert check_track_has_events(metadata_list)
	assert rt.core_get_track_count(handle) == 0

def test_create_loitering_area_vehicle_stationary(setup):
	handle = setup

	area_created = rt.securt_create_loitering_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [3], 1, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'vehicle', 0)

	assert not check_track_has_events(metadata_list)
	assert_no_events(metadata_list, ['event-loitering', 'event-loitering-end', 'track', 'crop'])
	assert rt.core_get_track_count(handle) == 0

def test_create_loitering_area_person_long(setup):
	handle = setup

	area_created = rt.securt_create_loitering_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], 60, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 1)

	assert_no_events(metadata_list, ['event-loitering', 'event-loitering-end', 'track', 'crop'])
	assert rt.core_get_track_count(handle) == 0

def test_create_crowding_area_person_low(setup):
	handle = setup

	area_created = rt.securt_create_crowding_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], 1, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 1)

	assert_events(metadata_list, ['track'])
	assert_events_once(metadata_list, ['event-crowd-detection'])
	assert check_track_has_events(metadata_list)
	assert rt.core_get_track_count(handle) == 0

def test_create_crowding_area_person_high(setup):
	handle = setup

	area_created = rt.securt_create_crowding_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], 60, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 1)
	assert_no_events(metadata_list, ['event-crowd-detection', 'track', 'crop'])
	assert rt.core_get_track_count(handle) == 0

@pytest.mark.parametrize("det_sensitivity", [1, 2, 3])
@pytest.mark.parametrize("move_sensitivity", [1, 2, 3])
def test_create_line_crossing_person(setup, det_sensitivity, move_sensitivity):
	handle = setup

	rt.core_set_detection_sensitivity(handle, det_sensitivity)
	rt.core_set_movement_sensitivity(handle, move_sensitivity)

	area_created = rt.securt_create_line_crossing(handle, "area_id", "area_name", [[0.4, 0.0], [0.4, 1.0]], [1], 1, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 1,steps=30)
	
	assert_events(metadata_list, ['track', 'crop'])
	assert_events_once(metadata_list, ['event-line-crossing'])
	assert check_track_has_events(metadata_list)
	assert rt.core_get_track_count(handle) == 0


#In this test the person vanishes before crossing the line, so we shouldn't get an event
def test_create_almost_line_crossing_person(setup):
	handle = setup

	area_created = rt.securt_create_line_crossing(handle, "area_id", "area_name", [[0.5, 0.0], [0.5, 1.0]], [1], 1, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 1,xmax=180,extra_empty_frames=10)
	
	assert_no_events(metadata_list, ['track', 'crop','event-line-crossing'])
	assert rt.core_get_track_count(handle) == 0

def test_create_line_plus_area_person(setup):
	handle = setup

	line_created = rt.securt_create_line_crossing(handle, "area_id", "area_name", [[0.4, 0.0], [0.4, 1.0]], [1], 1, [0.0,0.0,0.0])
	assert line_created

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 3, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 1)
	
	assert_events(metadata_list, ['track', 'crop'])
	assert_events_once(metadata_list, ['event-line-crossing', 'event-area-enter', 'event-area-exit'])
	assert check_track_has_events(metadata_list)
	assert rt.core_get_track_count(handle) == 0

def test_create_intrusion_plus_area_person(setup):
	handle = setup

	area_created = rt.securt_create_intrusion_area(handle, "area_id1", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], [0.0,0.0,0.0])
	assert area_created != -1

	area_created = rt.securt_create_crossing_area(handle, "area_id2", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 3, [0.0,0.0,0.0])
	assert area_created != -1

	metadata_list = play_video(handle, 'person', 1)

	assert_events(metadata_list, ['track', 'crop'])
	assert_events_once(metadata_list, ['event-area-enter', 'event-area-exit', 'event-intrusion', 'event-intrusion-end'])
	assert check_track_has_events(metadata_list)
	assert rt.core_get_track_count(handle) == 0

def test_create_object_left_short_time(setup):
	handle = setup

	area_created = rt.securt_create_object_left_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], 1, [0.0,0.0,0.0])

	metadata_list = play_video(handle, 'person', 1)

	assert_no_events(metadata_list, ['track', 'crop'])
	assert_events_once(metadata_list, ['event-object-left-removed'])
	assert rt.core_get_track_count(handle) == 0

def test_create_object_left_long_time(setup):
	handle = setup

	area_created = rt.securt_create_object_left_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], 100, [0.0,0.0,0.0])

	metadata_list = play_video(handle, 'person', 1)

	assert_no_events(metadata_list, ['event-object-left-removed', 'track', 'crop'])
	assert rt.core_get_track_count(handle) == 0

def test_create_object_removed_short_time(setup):
	handle = setup

	area_created = rt.securt_create_object_removed_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], 1, [0.0,0.0,0.0])

	metadata_list = play_video(handle, 'person', 1)

	assert_no_events(metadata_list, ['event-object-removed', 'track', 'crop'])
	assert rt.core_get_track_count(handle) == 0

def test_create_object_removed_long_time(setup):
	handle = setup

	area_created = rt.securt_create_object_removed_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], 100, [0.0,0.0,0.0])

	metadata_list = play_video(handle, 'person', 1)

	assert_no_events(metadata_list, ['event-object-removed', 'track', 'crop'])
	assert rt.core_get_track_count(handle) == 0

def test_create_object_removed_short_time_stationary(setup):
	handle = setup

	area_created = rt.securt_create_object_removed_area(handle, "area_id", "area_name", [[1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], 1, [0.0,0.0,0.0])

	metadata_list = play_video(handle, 'person', 0.2)

	assert_no_events(metadata_list, ['track', 'crop', 'event-object-removed'])
	assert rt.core_get_track_count(handle) == 0

def test_create_tailgating_area_single_person(setup):
	handle = setup

	area_created = rt.securt_create_tailgating_line(handle, "line_id", "line_name", [[0.5, 0.0], [0.5, 1.0]], [1, 3], 1, 2, [0.0,0.0,0.0])

	metadata_list = play_video(handle, 'person', 1)

	assert_no_events(metadata_list, ['track','event-tailgating', 'crop'])
	assert rt.core_get_track_count(handle) == 0

def test_create_tailgating_area_two_people_long(setup):
	handle = setup

	area_created = rt.securt_create_tailgating_line(handle, "line_id", "line_name", [[0.5, 0.0], [0.5, 1.0]], [1, 3], 1000, 2, [0.0,0.0,0.0])

	metadata_list = play_video(handle, 'person', 1, True, 30)

	assert_events(metadata_list, ['track', 'crop'])
	assert_events_once(metadata_list, ['event-tailgating'])
	assert check_track_has_events(metadata_list)
	assert rt.core_get_track_count(handle) == 0

def test_create_tailgating_area_two_people_long_wrong_class(setup):
	handle = setup

	area_created = rt.securt_create_tailgating_line(handle, "line_id", "line_name", [[0.5, 0.0], [0.5, 1.0]], [3], 1000, 2, [0.0,0.0,0.0])

	metadata_list = play_video(handle, 'person', 1, True)

	assert_no_events(metadata_list, ['track', 'crop', 'event-tailgating'])
	assert rt.core_get_track_count(handle) == 0

def test_create_armed_person_area_no_gun(setup):
	handle = setup

	area_created = rt.securt_create_armed_person_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [0.0,0.0,0.0])

	metadata_list = play_video(handle, 'person_broom', 1, 60)
	print(metadata_list)
	assert_no_events(metadata_list, ['event-armed-person', 'track', 'crop', 'attribute'])

	assert rt.core_get_track_count(handle) == 0

@pytest.mark.parametrize("det_sensitivity", [1, 2, 3])
@pytest.mark.parametrize("move_sensitivity", [1, 2, 3])
def test_create_armed_person_area_gun(setup, det_sensitivity, move_sensitivity):
	handle = setup

	rt.core_set_detection_sensitivity(handle, det_sensitivity)
	rt.core_set_movement_sensitivity(handle, move_sensitivity)

	area_created = rt.securt_create_armed_person_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [0.0,0.0,0.0])

	metadata_list = play_video(handle, 'person_gun', 1, steps=30)

	assert_events(metadata_list, ['track', 'crop'])
	assert_events_once(metadata_list, ['event-armed-person', 'event-armed-person-end'])
	assert_attribute_once(metadata_list, 'person_carrying_gun', 'CarryingGun')

	assert rt.core_get_track_count(handle) == 0

@pytest.mark.parametrize("det_sensitivity", [1, 2, 3])
@pytest.mark.parametrize("move_sensitivity", [1, 2, 3])
def test_create_attribute_person(setup, det_sensitivity, move_sensitivity):
	handle = setup

	rt.core_set_detection_sensitivity(handle, det_sensitivity)
	rt.core_set_movement_sensitivity(handle, move_sensitivity)

	rt.securt_set_appearance_search(handle, 1)

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 3, [0.0,0.0,0.0])

	metadata_list = play_video(handle, 'person', 1)

	assert_events(metadata_list, ['track', 'crop', 'attribute'])

	assert rt.core_get_track_count(handle) == 0

def test_create_attribute_person_wrong_class(setup):
	handle = setup

	rt.securt_set_appearance_search(handle, 2)

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 3, [0.0,0.0,0.0])

	metadata_list = play_video(handle, 'person', 1)

	assert_no_events(metadata_list, ['attribute'])

	assert rt.core_get_track_count(handle) == 0

def test_create_attribute_vehicle(setup):
	handle = setup

	rt.securt_set_appearance_search(handle, 2)

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [3], False, 3, [0.0,0.0,0.0])

	metadata_list = play_video(handle, 'vehicle', 1)

	assert_events(metadata_list, ['track', 'crop', 'attribute'])

	assert rt.core_get_track_count(handle) == 0

def test_create_fallen_person_area_none(setup):
	handle = setup

	area_created = rt.securt_create_fallen_person_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [0.0,0.0,0.0])

	metadata_list = play_video(handle, 'person', 1)

	assert_no_events(metadata_list, ['event-fallen-person', 'event-fallen-person-end', 'track', 'crop'])

	assert rt.core_get_track_count(handle) == 0

def test_create_fallen_person_area(setup):
	handle = setup

	area_created = rt.securt_create_fallen_person_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [0.0,0.0,0.0])

	metadata_list = play_video(handle, 'person_fallen', 1,steps=50)

	assert_events(metadata_list, ['track', 'crop'])
	assert_events_once(metadata_list, ['event-fallen-person', 'event-fallen-person-end'])

	assert rt.core_get_track_count(handle) == 0

@pytest.mark.parametrize("det_sensitivity", [1, 2, 3])
@pytest.mark.parametrize("move_sensitivity", [1, 2, 3])
def test_create_license_plate_access_control_area(setup, det_sensitivity, move_sensitivity):
	handle = setup

	rt.core_set_detection_sensitivity(handle, det_sensitivity)
	rt.core_set_movement_sensitivity(handle, move_sensitivity)
	area_created = rt.securt_create_alpr_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [0.0,0.0,0.0])

	metadata_list = play_video(handle, 'vehicle_plate', 1)

	assert_events(metadata_list, ['track', 'crop'])
	assert_attribute_once(metadata_list, 'license_plate', 'THECE01')
	
	assert rt.core_get_track_count(handle) == 0

def test_create_license_plate_and_par_area(setup):
	handle = setup

	area_created = rt.securt_create_armed_person_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [0.0,0.0,0.0])
	area_created = rt.securt_create_intrusion_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [3], [0.0,0.0,0.0])

	area_created = rt.securt_create_alpr_area(handle, "area_id2", "area_name2", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [0.0,0.0,0.0])

	rt.securt_set_appearance_search(handle, 2)

	metadata_list = play_video(handle, 'vehicle_plate', 1)

	assert_events(metadata_list, ['track', 'crop'])
	assert_attribute_once(metadata_list, 'license_plate', 'THECE01')

	assert rt.core_get_track_count(handle) == 0

def test_attributes_are_captured_if_locked_after_crossing_event(setup):
	handle = setup
 
	rt.securt_set_appearance_search(handle, 2)
 
	line_created = rt.securt_create_line_crossing(handle, "line_id", "line_name", [[0.4, 0.0], [0.4, 1.0]], [3], 1, [0.0,0.0,0.0])
	assert line_created
 
	metadata_list = play_video(handle, 'vehicle', 1,steps=50)
   
	assert_events(metadata_list, ['track', 'crop'])
	assert_events_once(metadata_list, ['event-line-crossing'])
	assert_events(metadata_list, ['attribute'])
 
	assert check_track_has_events(metadata_list)
	assert rt.core_get_track_count(handle) == 0

#def test_create_tailgating_area_two_people_short(setup):
#    handle = setup
#
#    area_created = rt.securt_create_tailgating_line(handle, "line_id", "line_name", [[0.5, 0.0], [0.5, 1.0]], [1, 3], 0, 2, [0.0,0.0,0.0])
#
#    metadata_list = play_video(handle, 'person', 1.0, True)
#
#    assert_events(metadata_list, ['track', 'crop'])
#    assert_events_once(metadata_list, ['event-tailgating'])
#    assert check_track_has_events(metadata_list)

#def test_connect_rtsp_working(api_setup):
#
#    instance = rt.securt_create_instance("my_instance_id", "my_instance_name")
#    rt.core_start(handle)
#
#    rt.core_set_frame_rate_limit(handle, 15)
#
#    ret = rt.core_set_input_to_rtsp(handle, "rtsp://192.168.1.99:8554/loitering2")
#    assert ret
#
#    area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 1, [0.0,0.0,0.0])
#    assert area_created != -1
#
#    time.sleep(10)
#
#    # Call the consume_metadata method
#    metadata_list = []
#    while True:
#        metadata = rt.securt_consume_metadata(handle)
#        if metadata is None:
#            break
#        metadata_list.append(metadata)
#
#    assert len(metadata_list) > 0
#
#    rt.securt_stop(handle)
#    rt.securt_destroy_instance(handle)

def test_connect_rtsp_invalid(setup):
	handle = setup

	rt.core_set_frame_rate_limit(handle, 15)

	ret = rt.core_set_input_to_rtsp(handle, "rtsp://124.123.123.12:8554/loitering2")
	assert ret

	area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 1, [0.0,0.0,0.0])
	assert area_created != -1
