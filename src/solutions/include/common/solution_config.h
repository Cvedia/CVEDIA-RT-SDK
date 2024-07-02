#pragma once
#define SOLUTION_THROTTLE_WARNING_INTERVAL 60		// In seconds
#define SOLUTION_THROTTLE_WARNING_WARMUP 30			// In seconds
#define SOLUTION_THROTTLE_WARNING_BANDWIDTH 0.7f	// In percentage

#define CAMERA_FPS_WARNING 9						// In frames per second
#define CAMERA_FPS_WARNING_INTERVAL 30				// In seconds
#define CAMERA_FPS_WARNING_WARMUP 30				// In seconds

#define BEST_CROP_ASPECT_RATIO 1.85f				// Width/Height ratio
#define BEST_CROP_ASPECT_SCALEBACK 1.2f				// Scale factor in percentage
#define BEST_CROP_UPDATE_DELTA 0.02f				// Minimum change in confidence to update best crop

#define DIAGNOSTICS_QUEUE_LIMIT 100 				// Maximum number of diagnostic events to keep in queue