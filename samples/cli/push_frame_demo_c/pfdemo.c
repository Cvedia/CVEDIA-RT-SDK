/*
	SPDX-FileCopyrightText: 2024 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/

#include <stdlib.h>
#include <stdio.h>

#include "core_capi.h"
#include "rt_capi.h"
#include "securt_capi.h"
#include "solutions_capi.h"

#if defined(_WIN32) || defined(_WIN64)
#define UINT64 __uint64
#else
#define UINT64 unsigned long long int
#endif

#include <stdio.h>

// X-platform sleep function
#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
	void __sleep(int milliseconds) {
		Sleep(milliseconds);
	}
#else
	#include <unistd.h>
	void __sleep(int milliseconds) {
		usleep(milliseconds * 1000);
	}
#endif

int _core_shutdown () {
	printf("Shutting down local RT\n");
	int retVal = api_shutdown();
	if (retVal != 1) {
		printf("Failed to shutdown local RT\n");
	}
	return retVal;
}

int main(int argc, const char* argv[]) {

	// ----- USER PARAMS -----
	UINT64 FPS = 30;
	const char* solutionsPath = getenv("RT_SOLUTIONS_PATH");
	if (solutionsPath == NULL) {
		printf("Please set the RT_SOLUTIONS_PATH environment variable\n");
		return -1;
	}

	// ----- DEMO CODE -----
	UINT64 timeStepMs = 1000 / FPS;
	UINT64 timestamp = 1;

	printf("Starting Push Frame C Demo\n");

	// Default Return Value for API - 1 = success
	int retVal = 0;

	// If left empty, then RT will generate its own GUID
	char const instanceGUID[] = "ffffffff-ffff-ffff-ffff-ffffffffffff\0";

	// We expect to run the RT directly from here, so we leave the IP empty
	// If set, RT will try to connect to RT Service (Server) on that IP (REMOTE MODE) - Remote mode must be turned on.
	char const remoteIP[] = "";

	printf("Initializing local RT\n");

	retVal = api_initialize();
	if (retVal != 1) {
		printf("Failed to initialize local RT\n");
		return -1;
	}

	printf("Search for solutions on: %s\n", solutionsPath);

	int scanInstances = 0;
	retVal = solutions_discover(solutionsPath, scanInstances);
	if (retVal <= 0) {
		printf("Failed to discover solutions\n");
		_core_shutdown();
		return -1;
	}
	printf("Found %d solutions\n", retVal);

	printf("Creating instance context\n");

	int const ctxId = core_create_context(instanceGUID, remoteIP);
	if (ctxId < 0) {
		printf("Failed to create instance context\n");
		_core_shutdown();
		return -1;
	}

	printf("Creating SecuRT instance\n");

	int const securtHdl = securt_create_instance(ctxId, "TEST_INSTANCE_CH1");
	if (securtHdl < 0) {
		printf("Failed to create SecuRT instance\n");
		_core_shutdown();
		return -1;
	}

	printf("Setting input to manual\n");

	retVal = core_set_input_to_manual(ctxId);
	if (retVal != 1) {
		printf("Failed to set input to manual\n");
		_core_shutdown();
		return -1;
	}

	printf("Loading instance to memory\n");

	retVal = core_load(ctxId);
	if (retVal != 1) {
		printf("Failed to load the instance to memory\n");
		_core_shutdown();
		return -1;
	}

	printf("Creating an intrusion area \n");

	char const areaId[] = "area1";
	char const areaName[] = "area1Name";
	float const coords[] = { 0.3f, 0.3f, 0.3f, 0.7f, 0.7f, 0.7f, 0.7f, 0.3f };
	int const classes[] = { 1, 2, 3 };
	double const color[] = { 1.0, 0.0, 0.0, 1.0 };
	int const numCoords = sizeof(coords) / sizeof(float);	// Note, this is the size of the floats, not points
	int const numClasses = sizeof(classes) / sizeof(int);
	retVal = securt_create_intrusion_area(securtHdl, areaId, areaName, coords, numCoords, classes, numClasses, color);
	if (retVal != 1) {
		printf("Failed to create intrusion area\n");
		_core_shutdown();
		return -1;
	}

	printf("Starting the instance\n");

	retVal = core_start(ctxId);
	if (retVal != 1) {
		printf("Failed to start\n");
		_core_shutdown();
		return -1;
	}

	printf("Pushing black frames example - 2s\n");
	// Create a frame
	int const width = 640;
	int const height = 480;
	int const channels = 3;
	size_t frameSize = width * height * channels;

	unsigned char* fakeFrame = (unsigned char*)malloc(frameSize);
	if (fakeFrame == NULL) {
		printf("Failed to allocate frame\n");
		return -1;
	}
	for (int i = 0; i < frameSize; i++) {
		fakeFrame[i] = 0;
	}

	for (int i = 0; i < FPS * 2; i++) {
		// Push the frame
		retVal = core_push_raw_frame(ctxId, fakeFrame, width, height, timestamp);
		if (retVal != 0) {
			printf("Failed to push frame\n");
			return -1;
		}

		// OTHER OPTIONS:
		// core_push_compressed_frame(ctxId, COMPRESSED_FRAME, SIZE_OF_COMPRESSED_FRAME_DATA, timestamp);
		// core_push_h264_frame(ctxId, H264_FRAME, SIZE_OF_H264_FRAME_DATA, timestamp);
		// core_push_h265_frame(ctxId, H265_FRAME, SIZE_OF_H265_FRAME_DATA, timestamp);

		__sleep(timeStepMs);
		timestamp += timeStepMs;

		char* json;
		retVal = core_consume_events(ctxId, &json);
		if (retVal == -1) {
			printf("Failed to consume events\n");
			return -1;
		}

		if (retVal == 1) {
			printf("Events: %s\n", json);
		}

		core_free_string(json);
	}
	free(fakeFrame);

	const char* imgFileName = getenv("IMAGE_BIN_CONTAINER");
	if (imgFileName == NULL) {
		printf("To test on real images, use raw 640x480x3 in single file. Set path to it in env: IMAGE_BIN_CONTAINER\n");
	} else {
		// Open the file for reading in binary mode
    	FILE* file = fopen(imgFileName, "rb");
		if (!file) {
			fprintf(stderr, "Error: Unable to open file %s\n", imgFileName);
			return -1;
		}

		printf("Pushing frames from file\n");
		while (1){
			unsigned char* frameBuffer = (unsigned char*)malloc(frameSize);
			if (frameBuffer == NULL) {
				printf("Failed to allocate frame\n");
				return -1;
			}
			size_t readSize = fread(frameBuffer, 1, frameSize, file);
			if (readSize != frameSize) {
				// Check for end of file
				if (feof(file)) {
					printf("End of file reached\n");
				} else {
					fprintf(stderr, "Error: Unable to read frame data\n");
				}
				break;
			}
			retVal = core_push_raw_frame(ctxId, frameBuffer, width, height, timestamp);
			if (retVal != 0) {
				printf("Failed to push frame\n");
				return -1;
			}

			free(frameBuffer);
			__sleep(timeStepMs);
			timestamp += timeStepMs;

			char* json;
			retVal = core_consume_events(ctxId, &json);
			if (retVal == -1) {
				printf("Failed to consume events\n");
				return -1;
			}

			if (retVal == 1) {
				printf("Events: %s\n", json);
			}

			core_free_string(json);	
		}
	}

	printf("Stopping the instance\n");

	retVal = core_stop(ctxId);
	if (retVal != 1) {
		printf("Failed to stop\n");
		_core_shutdown();
		return -1;
	}

	printf("Destroying SecuRT instance\n");

	retVal = securt_destroy_instance(securtHdl);
	if (retVal != 1) {
		printf("Failed to destroy SecuRT instance\n");
		_core_shutdown();
		return -1;
	}

	printf("Destroying instance context\n");

	retVal = core_delete_context(ctxId);
	if (retVal != 1) {
		printf("Failed to delete context\n");
		return -1;
	}

	_core_shutdown();

	printf("Exiting Push Frame C Demo\n");
	return 0;
}
