import os
import argparse
import random
import string
import time
from cvedia import rt

time.sleep(5)

def start_instance(uri):

    # Create a context
    handle = rt.securt_create_context("", "")
    if handle == -1:
        print("Failed to create context")
        return

    # Create an instance
    name = ''.join(random.choice(string.ascii_letters + string.digits) for _ in range(8))

    if rt.securt_create_instance(handle, name) == -1:
        print("Failed to create instance")
        return

    # Start processing
    if rt.securt_start(handle) == -1:
        print("Failed to start processing")
        return

    if rt.securt_set_input_to_rtsp(handle, uri) == -1:
        print("Failed to set input to RTSP")
        return

    if rt.securt_create_intrusion_area(handle, "area_id", "area_name", [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]], [1,3], [0.0,0.0,0.0]) == -1:
        print("Failed to create intrusion area")
        return

    if rt.securt_set_render_preset(handle, "Default") == -1:
        print("Failed to set render preset")
        return

    print(uri + " --> " + rt.securt_enable_hls_output(handle))

def main():
    # Create the parser
    parser = argparse.ArgumentParser(description="Process RTSP URIs.")

    parser.add_argument('--input', nargs='+', type=str, required=True, help='List of RTSP URIs')
    parser.add_argument('--hls-ip', type=str, required=True, help='HLS IP address')
    parser.add_argument('--hls-port', type=int, required=True, help='HLS port')

    # Parse the arguments
    args = parser.parse_args()

    rt.config_set_node("/web_server/ip_address", "\"" + args.hls_ip + "\"")
    rt.config_set_node("/web_server/port", str(args.hls_port))

    # Initialize RT
    rt.api_initialize()

    # Discover solutions
    rt.solutions_discover(os.getenv("SOLUTIONS_DIR", default=""), True)

    rt.api_start_webserver()

    for uri in args.input:
        start_instance(uri)

    while True:
        pass

if __name__ == "__main__":
    main()
