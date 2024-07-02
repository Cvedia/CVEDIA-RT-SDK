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

@pytest.fixture(scope='module')
def api_setup():
    rt.api_initialize()

    rt.solutions_discover(os.getenv("SOLUTIONS_DIR", default=""), True)

    yield rt

    rt.api_shutdown()

@pytest.fixture
def setup(api_setup):
    # Call the create method
    handle = rt.securt_create_context("", "")
    assert handle != -1

    assert rt.securt_create_instance(handle, "my_instance_name") != -1

    assert rt.securt_set_blocking_readahead_queue(handle, 1) != -1

    # Start processing
    assert rt.securt_start(handle) != -1

    yield handle

    x = rt.securt_stop(handle)
    if x == -1:
        pytest.exit("decided to stop the test run")

    # Cleanup after tests
    assert rt.securt_destroy_instance(handle) != -1

    rt.securt_delete_context(handle)

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

def play_video(handle, type, stationary):
    script_dir = os.path.dirname(os.path.realpath(__file__))

    steps = 20

    # Load the object image
    object_img = cv2.imread(script_dir + '/images/' + type + '.jpg')

    # Get the dimensions of the object image
    obj_height, obj_width = object_img.shape[:2]

    # Compute the steps in x-direction to move the image from left to right
    step_size = (320 - obj_width) // steps

    if stationary:
        step_size = 0

    timestamp = 0

    for frame_count in range(steps):
        # Create a 320x320 black canvas
        canvas = np.zeros((320, 320, 3), dtype='uint8')

        # Calculate the top left position of where to place the object on the canvas
        pos_x = step_size * frame_count
        pos_y = 0  # keep it at the top of the canvas

        # Place the object onto the canvas
        canvas[pos_y:pos_y+obj_height, pos_x:pos_x+obj_width] = object_img

        # cv2.imwrite(f'd:/tmp2/canvas_frame_{frame_count}.jpg', canvas)
        timestamp += 100

        # Push the frame to the instance
        resp = False
        while not resp:
            resp = rt.securt_push_frame(handle, canvas, timestamp)

    # flush out any track data
    for frame_count in range(10):
        # Create a 320x320 black canvas
        canvas = np.zeros((320, 320, 3), dtype='uint8')

        timestamp += 10000

        resp = False
        while not resp:
            resp = rt.securt_push_frame(handle, canvas, timestamp)

    # Call the consume_metadata method
    metadata_list = []
    while True:
        metadata = rt.securt_consume_events(handle)
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
        assert event_type_occurrences == 1, f'Event type {event_type} does not exist exactly once in metadata'
        assert validate_json_objects(metadata_list, event_type, f'{schema_base_url}/{event_type}-v1.json'), f'Failed to validate JSON objects for event type {event_type}'

def test_get_version(setup):
    handle = setup
    version = rt.securt_get_version(handle)
    assert version != ""

def test_get_instance_id(setup):
    handle = setup

    instance_id = rt.securt_get_instance_id(handle)
    # check if instance_id is a GUID
    assert len(instance_id) == 36

def test_get_instance_name(setup):
    handle = setup

    instance_name = rt.securt_get_instance_name(handle)
    assert instance_name == "my_instance_name"

def test_get_frame_rate_limit(setup):
    handle = setup

    framerate_limit = rt.securt_get_frame_rate_limit(handle)
    assert framerate_limit == 0

def test_set_frame_rate_limit(setup):
    handle = setup

    rt.securt_set_frame_rate_limit(handle, 10)
    framerate_limit = rt.securt_get_frame_rate_limit(handle)
    assert framerate_limit == 10

def test_get_frames_processed_none(setup):
    handle = setup

    frames_processed = rt.securt_get_frames_processed(handle)
    assert frames_processed == 0

def test_get_frames_processed_video(setup):
    handle = setup

    metadata_list = play_video(handle, 'person', True)

    frames_processed = rt.securt_get_frames_processed(handle)
    assert frames_processed == 30

def test_get_recommended_frame_rate(setup):
    handle = setup

    recommended_frame_rate = rt.securt_get_recommended_frame_rate(handle)
    assert recommended_frame_rate == 10

def test_need_data(setup):
    handle = setup

    need_data = rt.securt_need_data(handle, 10)
    assert need_data == True

def test_create_area_twice(setup):
    handle = setup

    pts = [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]]

    area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", pts, [1], False, 1, [0.0,0.0,0.0])
    assert area_created != -1

    area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", pts, [1], False, 1, [0.0,0.0,0.0])
    assert area_created != 1

def test_delete_area_twice(setup):
    handle = setup

    area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 1, [0.0,0.0,0.0])
    assert area_created != -1

    area_deleted = rt.securt_delete_area(handle, "area_id")
    assert area_deleted != -1

    area_deleted = rt.securt_delete_area(handle, "area_id")
    assert area_deleted != 1

def test_create_area_delete(setup):
    handle = setup

    area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 1, [0.0,0.0,0.0])
    assert area_created != -1

    area_deleted = rt.securt_delete_area(handle, "area_id")
    assert area_deleted != -1

    area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 1, [0.0,0.0,0.0])
    assert area_created != -1

def test_create_area_person_enter(setup):
    handle = setup

    area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 1, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'person', False)

    assert_no_events(metadata_list, ['event-area-exit'])
    assert_events_once(metadata_list, ['event-area-enter', 'best-crop'])
    assert_events(metadata_list, ['track'])
    assert check_track_has_events(metadata_list)

def test_create_area_person_enter_stationary_ignore(setup):
    handle = setup

    area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], True, 1, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'person', True)

    assert_no_events(metadata_list, ['event-area-enter', 'event-area-exit', 'track', 'best-crop'])

def test_create_area_person_enter_stationary_include(setup):
    handle = setup

    area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 1, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'person', True)

    assert_no_events(metadata_list, ['event-area-exit'])
    assert_events_once(metadata_list, ['event-area-enter', 'best-crop'])
    assert_events(metadata_list, ['track'])
    assert check_track_has_events(metadata_list)

def test_create_area_person_wrong_class(setup):
    handle = setup

    area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [3], False, 1, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'person', False)

    assert_no_events(metadata_list, ['event-area-enter', 'event-area-exit', 'track', 'best-crop'])

def test_create_area_vehicle(setup):
    handle = setup

    area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [3], False, 1, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'vehicle', False)

    assert_no_events(metadata_list, ['event-area-exit'])
    assert_events(metadata_list, ['track'])
    assert_events_once(metadata_list, ['event-area-enter', 'best-crop'])
    assert check_track_has_events(metadata_list)

def test_create_area_person_exit(setup):
    handle = setup

    area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 2, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'person', False)

    assert_no_events(metadata_list, ['event-area-enter'])
    assert_events(metadata_list, ['track'])
    assert_events_once(metadata_list, ['event-area-exit', 'best-crop'])
    assert check_track_has_events(metadata_list)

def test_create_area_person_enter_exit(setup):
    handle = setup

    area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 3, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'person', False)

    assert_events_once(metadata_list, ['event-area-enter', 'event-area-exit'])
    assert_events(metadata_list, ['track', 'best-crop'])
    assert check_track_has_events(metadata_list)

def test_create_intrusion_area_person(setup):
    handle = setup

    area_created = rt.securt_create_intrusion_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'person', False)

    assert_no_events(metadata_list, ['event-area-exit', 'event-area-enter'])
    assert_events(metadata_list, ['track', 'best-crop'])
    assert_events_once(metadata_list, ['event-intrusion', 'event-intrusion-end'])
    assert check_track_has_events(metadata_list)

def test_intrusion_motion_area_include(setup):
    handle = setup

    area_created = rt.securt_create_intrusion_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], [0.0,0.0,0.0])
    assert area_created != -1

    motion_created = rt.securt_set_motion_area(handle, [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]])
    assert motion_created != -1

    metadata_list = play_video(handle, 'person', False)

    assert_no_events(metadata_list, ['event-area-exit', 'event-area-enter'])
    assert_events(metadata_list, ['track', 'best-crop'])
    assert_events_once(metadata_list, ['event-intrusion', 'event-intrusion-end'])
    assert check_track_has_events(metadata_list)

def test_intrusion_motion_area_exclude(setup):
    handle = setup

    area_created = rt.securt_create_intrusion_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], [0.0,0.0,0.0])
    assert area_created != -1

    motion_created = rt.securt_set_motion_area(handle, [[0.0, 0.9], [1.0, 0.9], [1.0, 1.0], [0.0, 1.0]])
    assert motion_created

    metadata_list = play_video(handle, 'person', False)

    assert_no_events(metadata_list, ['event-area-exit', 'event-area-enter', 'event-intrusion', 'track', 'best-crop'])

def test_create_intrusion_area_person_outside(setup):
    handle = setup

    area_created = rt.securt_create_intrusion_area(handle, "area_id", "area_name", [[0.0, 0.9], [1.0, 0.9], [1.0, 1.0], [0.0, 1.0]], [1], [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'person', False)

    assert_no_events(metadata_list, ['event-loitering', 'event-loitering-end', 'track', 'best-crop'])

def test_create_loitering_area_person_moving(setup):
    handle = setup

    area_created = rt.securt_create_loitering_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], 1, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'person', False)

    assert_events(metadata_list, ['track', 'best-crop'])
    assert_events_once(metadata_list, ['event-loitering', 'event-loitering-end'])
    assert check_track_has_events(metadata_list)

def test_create_loitering_area_person_stationary(setup):
    handle = setup

    area_created = rt.securt_create_loitering_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], 1, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'person', True)

    assert not check_track_has_events(metadata_list)
    assert_no_events(metadata_list, ['event-loitering', 'event-loitering-end', 'track', 'best-crop'])

def test_create_loitering_area_vehicle_moving(setup):
    handle = setup

    area_created = rt.securt_create_loitering_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [3], 1, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'vehicle', False)

    assert_events(metadata_list, ['track', 'best-crop'])
    assert_events_once(metadata_list, ['event-loitering', 'event-loitering-end'])
    assert check_track_has_events(metadata_list)

def test_create_loitering_area_vehicle_stationary(setup):
    handle = setup

    area_created = rt.securt_create_loitering_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [3], 1, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'vehicle', True)

    assert not check_track_has_events(metadata_list)
    assert_no_events(metadata_list, ['event-loitering', 'event-loitering-end', 'track', 'best-crop'])

def test_create_loitering_area_person_long(setup):
    handle = setup

    area_created = rt.securt_create_loitering_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], 60, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'person', False)

    assert_no_events(metadata_list, ['event-loitering', 'event-loitering-end', 'track', 'best-crop'])

def test_create_crowding_area_person_low(setup):
    handle = setup

    area_created = rt.securt_create_crowding_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], 1, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'person', False)

    assert_events(metadata_list, ['track'])
    assert_events_once(metadata_list, ['event-crowd-detection'])
    assert check_track_has_events(metadata_list)

def test_create_crowding_area_person_high(setup):
    handle = setup

    area_created = rt.securt_create_crowding_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], 60, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'person', False)
    assert_no_events(metadata_list, ['event-crowd-detection', 'track', 'best-crop'])

def test_create_line_crossing_person(setup):
    handle = setup

    area_created = rt.securt_create_line_crossing(handle, "area_id", "area_name", [[0.4, 0.0], [0.4, 1.0]], [1], 1, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'person', False)
    
    assert_events(metadata_list, ['track', 'best-crop'])
    assert_events_once(metadata_list, ['event-line-crossing'])
    assert check_track_has_events(metadata_list)

def test_create_line_plus_area_person(setup):
    handle = setup

    line_created = rt.securt_create_line_crossing(handle, "area_id", "area_name", [[0.4, 0.0], [0.4, 1.0]], [1], 1, [0.0,0.0,0.0])
    assert line_created

    area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 3, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'person', False)
    
    assert_events(metadata_list, ['track', 'best-crop'])
    assert_events_once(metadata_list, ['event-line-crossing', 'event-area-enter', 'event-area-exit'])
    assert check_track_has_events(metadata_list)

def test_create_intrusion_plus_area_person(setup):
    handle = setup

    area_created = rt.securt_create_intrusion_area(handle, "area_id1", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], [0.0,0.0,0.0])
    assert area_created != -1

    area_created = rt.securt_create_crossing_area(handle, "area_id2", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 3, [0.0,0.0,0.0])
    assert area_created != -1

    metadata_list = play_video(handle, 'person', False)

    assert_events(metadata_list, ['track', 'best-crop'])
    assert_events_once(metadata_list, ['event-area-enter', 'event-area-exit', 'event-intrusion', 'event-intrusion-end'])
    assert check_track_has_events(metadata_list)

#def test_connect_rtsp_working(api_setup):
#
#    instance = rt.securt_create_instance("my_instance_id", "my_instance_name")
#    rt.securt_start(handle)
#
#    rt.securt_set_frame_rate_limit(handle, 15)
#
#    ret = rt.securt_set_input_to_rtsp(handle, "rtsp://192.168.1.99:8554/loitering2")
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

    rt.securt_set_frame_rate_limit(handle, 15)

    ret = rt.securt_set_input_to_rtsp(handle, "rtsp://124.123.123.12:8554/loitering2")
    assert ret

    area_created = rt.securt_create_crossing_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1], False, 1, [0.0,0.0,0.0])
    assert area_created != -1
