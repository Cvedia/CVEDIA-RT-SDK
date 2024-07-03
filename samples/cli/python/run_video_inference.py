import cv2
import os
from cvedia import rt

def main():
    # Initialize RT
    rt.api_initialize()

    # Discover solutions
    rt.solutions_discover(os.getenv("SOLUTIONS_DIR", default=""), True)

    # Create a context
    handle = rt.securt_create_context("", "")
    if handle == -1:
        print("Failed to create context")
        return

    # Create an instance
    if rt.securt_create_instance(handle, "my_instance_name") == -1:
        print("Failed to create instance")
        return

    # Start processing
    if rt.securt_start(handle) == -1:
        print("Failed to start processing")
        return

    if rt.securt_create_intrusion_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1,3], [0.0,0.0,0.0]) == -1:
        print("Failed to create intrusion area")
        return

    # Feed frames from an MP4 video file
    video_path = "f:/video/demo1.mp4"  # Replace with your video file path
    video = cv2.VideoCapture(video_path)
    if not video.isOpened():
        print(f"Failed to open video {video_path}")
        return

    fps = video.get(cv2.CAP_PROP_FPS)

    while True:
        ret, frame = video.read()
        if not ret:
            break

        events = rt.securt_consume_events(handle)
        if events is not None:
            print(events)

        frame_number = video.get(cv2.CAP_PROP_POS_FRAMES)
        timestamp = int((frame_number / fps) * 1000)

        # Push the frame to the instance
        if rt.securt_push_frame(handle, frame, timestamp) == -1:
            print("Failed to push frame")
            continue

    video.release()

    # Consume and print output events
    while True:
        events = rt.securt_consume_events(handle)
        if events is None:
            break
        print(events)

    # Stop processing
    if rt.securt_stop(handle) == -1:
        print("Failed to stop processing")

    # Destroy the instance
    if rt.securt_destroy_instance(handle) == -1:
        print("Failed to destroy instance")

    # Delete the context
    rt.securt_delete_context(handle)

    # Shutdown RT
    rt.api_shutdown()

if __name__ == "__main__":
    main()
