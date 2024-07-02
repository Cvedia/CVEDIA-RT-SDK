#pragma once

#include "api/capi_base.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT int securt_create_context(char const* instanceUuid, char const* remoteIp);
EXPORT int securt_delete_context(int const contextHandle);

/**
 * Checks if remote connection to a CVEDIA-RT instance is alive.
 *
 * @param handle The handle of the remote connection.
 *
 * @return Returns 1 if connection is alive, 0 if handle was not found, -1 if connection is dead or an error occurred.
 */
EXPORT int securt_is_alive(int const handle);

/**
 * Enables remote mode.
 *
 */
EXPORT void securt_enable_remote_mode();

/**
 * Disables remote mode.
 *
 */
EXPORT void securt_disable_remote_mode();

/**
 * Finds a remote CVEDIA-RT instance.
 *
 * @return Returns a string with the IP address of the remote CVEDIA-RT instance.
 *
 */
EXPORT char* securt_find_remote_server();

/**
 * Creates a new SecuRT instance with a given ID and name.
 *
 * @param handle The handle of the SecuRT instance to destroy.
 * @param instanceName User friendly name for the new SecuRT instance. Must be a null-terminated string.
 *
 * @return On success, the handle (integer > 0) to the created SecuRT instance is returned. On failure, -1 is returned.
 *
 * @note The function returns -1 if the instanceId already exists or if SecuRT cannot be initialized (see the log for details).
 */
EXPORT int securt_create_instance(int const handle, char const* instanceName);

/**
 * Destroys an existing SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance to destroy.
 *
 * @return 1 if the SecuRT instance was successfully destroyed, 0 if it did not exist.
 */
EXPORT int securt_destroy_instance(int const handle);

/**
 * Starts a SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance to start.
 *
 * @return int
 *
 * @note If the handle does not exist, has already been destroyed, or is already running, the function does nothing.
 */
EXPORT int securt_start(int const handle);

/**
 * Stops an existing SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance to stop.
 *
 * @return int
 *
 * @note If the handle does not exist, has already been destroyed, or is already stopped, the function does nothing.
 */
EXPORT int securt_stop(int const handle);

/**
 * Restarts an existing SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance to restart.
 *
 * @return int
 *
 * @note If the handle does not exist, has already been destroyed, or is already stopped, the function does nothing.
 */
EXPORT int securt_restart(int const handle);

/**
 * Creates a new area in the specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param areaId Unique identifier for the new area. Must be a null-terminated string.
 * @param name Name for the new area. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the area (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param classes Pointer to an array containing the class IDs of interest (1 = Person, 2 = Animal, 3 = Vehicle, 4 = Unknown).
 * @param classesSize The number of classes in the classes array.
 * @param ignoreStationaryObjects If true, stationary objects will be ignored in the area.
 * @param eventType The type of event to trigger in this area (1 = Enter, 2 = Exit, 3 = Enter & Exit).
 * @param color Color of the area for visualization purposes
 *
 * @return Returns 1 if the area was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the areaId already exists in the instance or if the numPoints does not match the size of the coordinates arrays.
 */
EXPORT int securt_create_crossing_area(int const handle, char const* areaId, char const* name, float const* coords,
                                       int numPoints, int const* classes, int classesSize, int ignoreStationaryObjects,
                                       int eventType, double const* color);

/**
 * Creates a new intrusion area in the specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param areaId Unique identifier for the new area. Must be a null-terminated string.
 * @param name Name for the new intrusion area. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the area (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param classes Pointer to an array containing the class IDs of interest (1 = Person, 2 = Animal, 3 = Vehicle, 4 = Unknown).
 * @param classesSize The number of classes in the classes array.
 * @param color Color of the area for visualization purposes
 *
 * @return Returns 1 if the area was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the areaId already exists in the instance.
 */
EXPORT int securt_create_intrusion_area(int const handle, char const* areaId, char const* name, float const* coords,
                                        int numPoints, int const* classes, int classesSize, double const* color);


/**
 * Creates a new loitering area in the specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param areaId Unique identifier for the new area. Must be a null-terminated string.
 * @param name Name for the new loitering area. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the area (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param classes Pointer to an array containing the class IDs of interest (1 = Person, 2 = Animal, 3 = Vehicle, 4 = Unknown).
 * @param classesSize The number of classes in the classes array.
 * @param seconds The duration in seconds for an object to be considered as loitering.
 * @param color Color of the area for visualization purposes
 *
 * @return Returns 1 if the area was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the areaId already exists in the instance.
 */
EXPORT int securt_create_loitering_area(int const handle, char const* areaId, char const* name, float const* coords,
                                        int numPoints, int const* classes, int classesSize, int seconds,
                                        double const* color);


/**
 * Creates a new crowding area in the specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param areaId Unique identifier for the new area. Must be a null-terminated string.
 * @param name Name for the new crowding area. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the area (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param classes Pointer to an array containing the class IDs of interest (1 = Person, 2 = Animal, 3 = Vehicle, 4 = Unknown).
 * @param classesSize The number of classes in the classes array.
 * @param objectCnt The count of objects that constitutes crowding.
 * @param color Color of the area for visualization purposes
 *
 * @return Returns 1 if the area was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the areaId already exists in the instance.
 */
EXPORT int securt_create_crowding_area(int const handle, char const* areaId, char const* name, float const* coords,
                                       int numPoints, int const* classes, int classesSize, int objectCnt,
                                       double const* color);

/**
 * Creates a new line crossing in the specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param lineId Unique identifier for the new line crossing area. Must be a null-terminated string.
 * @param name Name for the new line crossing area. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the line (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param classes Pointer to an array containing the class IDs of interest (1 = Person, 2 = Animal, 3 = Vehicle, 4 = Unknown).
 * @param classesSize The number of classes in the classes array.
 * @param direction The direction of line crossing to be detected (1 = Up, 2 = Down, 3 = Both).
 * @param color Color of the line for visualization purposes
 *
 * @return Returns 1 if the line was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the lineId already exists in the instance.
 */
EXPORT int securt_create_line_crossing(int const handle, char const* lineId, char const* name, float const* coords,
                                       int numPoints, int const* classes, int classesSize, int direction,
                                       double const* color);

/**
 * Creates a new armed person area in the specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param areaId Unique identifier for the new area. Must be a null-terminated string.
 * @param name Name for the new area. Must be a null-terminated string.
 * @param coords Array of points containing the coordinates of the area (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @param color Color of the line for visualization purposes
 *
 * @return Returns 1 if the area was successfully created, 0 otherwise.
 *
 * @note The function returns 0 if the areaId already exists in the instance or if the numPoints does not match the size of the coordinates arrays.
 */
EXPORT int securt_create_armed_person_area(int const handle, char const* areaId, char const* name, float const* coords,
                                           int numPoints, double const* color);

/**
 * @brief Defines a motion area within a specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param coords Array of points defining the area. The array should contain (x, y) coordinates in the following format: (x1, y1, x2, y2, ...).
 * @param numPoints The total size of the coordinates array.
 * @return Returns 1 if the motion area was successfully defined, 0 otherwise.
 */
EXPORT int securt_set_motion_area(int const handle, float const* coords, int const numPoints);

/**
 * @brief Defines multiple areas to be ignored within a specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param coords The array should contain (x, y) coordinates in the following format: (x1, y1, x2, y2, ...). An entry with (-1, -1) is used to separate the different areas.
 * @param numPoints The total size of the coordinates array.
 * @return Returns 1 if the motion area was successfully defined, 0 otherwise.
 */
EXPORT int securt_set_exclusion_areas(int const handle, float const* coords, int const numPoints);


/**
 * @brief Deletes an existing area within a specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param areaId The unique identifier for the area to be deleted. This must be a null-terminated string.
 * @return Returns 1 if the area was successfully deleted, 0 otherwise.
 */
EXPORT int securt_delete_area(int const handle, char const* areaId);


/**
 * @brief Deletes an existing line within a specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param lineId The unique identifier for the line to be deleted. This must be a null-terminated string.
 * @return Returns 1 if the line was successfully deleted, 0 otherwise.
 */
EXPORT int securt_delete_line(int const handle, char const* lineId);

/**
 * @brief Pushes a frame into the specified SecuRT instance for processing.
 *
 * @param handle The handle of the SecuRT instance.
 * @param buffer The buffer containing the frame data.
 * @param width The width of the frame.
 * @param height The height of the frame.
 * @param timestampMs The timestamp of the frame in milliseconds.
 *
 * @return Returns 1 if the frame was successfully pushed, 0 is the queue was full. 
 */
EXPORT int securt_push_frame(int const handle, void const* buffer, int width, int height,
                             unsigned long long int timestampMs);

/**
 * @brief Pushes a H.264 video frame into the specified SecuRT instance for processing.
 * 
 * @param handle The handle of the SecuRT instance.
 * @param buffer The buffer containing the frame data.
 * @param dataSize The size of the frame data.
 * @param timestampMs The timestamp of the frame in milliseconds.
 * 
 * @return Returns 1 if the frame was successfully pushed, 0 is the queue was full.
 *
 */
EXPORT int securt_push_h264_frame(int const handle, void const* buffer, unsigned long long int const dataSize,
                                  unsigned long long int const timestampMs);

/**
 * @brief Pushes a H.265 video frame into the specified SecuRT instance for processing.
 *
 * @param handle The handle of the SecuRT instance.
 * @param buffer The buffer containing the frame data.
 * @param dataSize The size of the frame data.
 * @param timestampMs The timestamp of the frame in milliseconds.
 *
 * @return Returns 1 if the frame was successfully pushed, 0 is the queue was full.
 *
 */
EXPORT int securt_push_h265_frame(int const handle, void const* buffer, unsigned long long int const dataSize,
                                  unsigned long long int const timestampMs);

/**
 * @brief Returns events from the specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param outJson Pointer to a char* containing the JSON array of events serialized as a string; must be freed by the caller.
 *
 * @return Returns 1 if there is an event, 0 otherwise
*/
EXPORT int securt_consume_events(int const handle, char** outJson);

/**
 * @brief Sets the detector mode of the specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param mode The detector mode to be set (1 = Smart Detection, 2 = Detection).
 *
 * @note The default detector mode is Smart Detection.
 */
EXPORT int securt_set_detector_mode(int const handle, int mode);

/**
 * @brief Sets the sensor modality of the specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param modality The sensor modality to be set (1 = RGB, 2 = Thermal).
 *
 * @note The default sensor modality is RGB.
 */
EXPORT int securt_set_sensor_modality(int const handle, int modality);

/**
 * @brief Sets the diagnostics mode of the specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param diagnostics The diagnostics mode to be set (1 = Receive diagnostics events, 0 = Do not receive diagnostics events).
 *
 * @note The default diagnostics mode is Off.
 */
EXPORT int securt_set_diagnostics_mode(int const handle, int diagnostics);

/**
 * @brief Sets the debug mode of the specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param debugMode The debug mode to be set (1 = On, 0 = Off).
 *
 * @note The default debug mode is Off.
 */
EXPORT int securt_set_debug_mode(int const handle, int debugMode);

/**
 * @brief Sets whether to return a track history from before the moment it caused an event
 * 
 * @param handle The handle of the SecuRT instance.
 * @param enabled The enabled state to be set (1 = On, 0 = Off).
 *
 * @note The default state is On.
*/
EXPORT int securt_set_tentative_tracks(int const handle, int enabled);

/**
 * @brief Calculates motion mask for specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 *
 */
EXPORT int securt_calculate_motion_mask(int const handle);

/**
 * @brief Sets the detector sensitivity of the specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param sensitivity The detection sensitivity to be set (1 = Low, 2 = Medium, 3 = High).
 *
 * @note The default detection sensitivity is Medium.
*/
EXPORT int securt_set_detection_sensitivity(int const handle, int sensitivity);

/**
 * @brief Sets the movement sensitivity of the specified SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param sensitivity The movement sensitivity to be set (1 = Low, 2 = Medium, 3 = High).
 *
 * @note The default movement sensitivity is Medium.
 */
EXPORT int securt_set_movement_sensitivity(int const handle, int sensitivity);

/**
 * @brief Returns the instance ID of the SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 *
 * @return Returns a string with the instance ID of the SecuRT instance.
 */
EXPORT char* securt_get_instance_id(int const handle);

/**
 * @brief Returns the name of the SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 *
 * @return Returns a string with the name of the SecuRT instance.
 */
EXPORT char* securt_get_instance_name(int const handle);

/**
 * @brief Returns the version of the SecuRT solution.
 *
 * @param handle The handle of the SecuRT instance.
 *
 * @return Returns a string with the version of the SecuRT solution.
 */
EXPORT char* securt_get_version(int const handle);

/**
 * @brief Returns the active number of tracked objects.
 *
 * @param handle The handle of the SecuRT instance.
 *
 * @return Number of tracked objects in the SecuRT instance.
 *
 */
EXPORT int securt_get_track_count(int const handle);

/**
 * @brief Returns the number of frames that have been processed.
 *
 * @param handle The handle of the SecuRT instance.
 *
 * @return Number of frames.
 */
EXPORT long long int securt_get_frames_processed(int const handle);

/**
 * @brief Returns the time it took to process the last frame
 *
 * @param handle The handle of the SecuRT instance.
 *
 * @return Time in milliseconds.
 *
 */
EXPORT float securt_get_latency(int const handle);

/**
 * @brief Returns the current estimated frame rate which is based on the
 * rate of data being pushed into the SecuRT instance and the processing speed.
 *
 * @param handle The handle of the SecuRT instance.
 *
 * @return Current estimated frame rate.
 */
EXPORT float securt_get_current_frame_rate(int const handle);

/**
 * @brief Sets a frame rate limit. This will cause push_frame to return 0 if the rate is exceeded.
 *
 * @param handle The handle of the SecuRT instance.
 * @param frameRateLimit The frame rate limit to be set.
 */
EXPORT int securt_set_frame_rate_limit(int const handle, int frameRateLimit);

/**
 * @brief Returns the current frame rate limit.
 *
 * @param handle The handle of the SecuRT instance.
 *
 * @return Current frame rate limit.
 */
EXPORT int securt_get_frame_rate_limit(int const handle);

/**
 * @brief Returns the recommended frame rate based on the current configuration.
 *
 * @param handle The handle of the SecuRT instance.
 *
 * @return Recommended frame rate.
 */
EXPORT int securt_get_recommended_frame_rate(int const handle);

/**
 * @brief Checks if the SecuRT instance needs more data or is rate limited.
 *
 * @param handle The handle of the SecuRT instance.
 * @param currentFrameTime The current frame time in milliseconds.
 *
 * @return Returns 1 if more data is needed, 0 if the instance is rate limited.
 */
EXPORT int securt_need_data(int const handle, long long int const currentFrameTime);

/**
 * @brief Checks if specified SecuRT instance is running.
 *
 */
EXPORT int securt_is_instance_running(int const handle);

/**
 * @brief Frees a string allocated by the SecuRT SDK.
 *
 * @param ptr The pointer to the string to be freed.
 *
 */
EXPORT void securt_free_string(void* ptr);

EXPORT int securt_set_blocking_readahead_queue(int const handle, int const state);

/**
 * Sets the input source of a SecuRT instance to an RTSP stream.
 *
 * @param handle The handle of the SecuRT instance.
 * @param rtspUrl The URL of the RTSP stream.
 *
 * @return Returns 1 if the input source was successfully set, -1 otherwise.
 */
EXPORT int securt_set_input_to_rtsp(int const handle, char const* rtspUrl);

/**
* Sets the input source of a SecuRT instance to manual mode.
*
* @param handle The handle of the SecuRT instance.
*
* @return Returns 1 if the input source was successfully set, 0 otherwise.
*/
EXPORT int securt_set_input_to_manual(int const handle);

/**
 * Enables HLS output for a SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 *
 * @return Returns a string with the HLS output URL if successful, NULL otherwise.
 */
EXPORT char* securt_enable_hls_output(int const handle);

/**
 * Disables HLS output for a SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 *
 * @return Returns 1 if the HLS output was successfully disabled, 0 otherwise.
 */
EXPORT int securt_disable_hls_output(int const handle);

/**
 * Enables RTSP output for a SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param rtspUrl The URL of the RTSP stream.
 *
 * @return Returns 1 if the RTSP output was successfully enabled, 0 otherwise.
 */
EXPORT int securt_enable_rtsp_output(int const handle, char const* rtspUrl);

/**
 * Disables RTSP output for a SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param rtspUrl The URL of the RTSP stream.
 *
 * @return Returns 1 if the RTSP output was successfully disabled, 0 otherwise.
 */
EXPORT int securt_disable_rtsp_output(int const handle, char const* rtspUrl);

/**
 * @brief Retrieves the instance information for a SecuRT instance.
 *
 * This function retrieves various information about a SecuRT instance, including its ID, name, version, detector mode, detection sensitivity, movement sensitivity, sensor modality, frame rate limit, metadata mode, statistics mode, diagnostics mode, and debug mode.
 *
 * @param handle The handle of the SecuRT instance.
 * @param instanceId Pointer to a char* that will be populated with the instance ID. The caller is responsible for freeing the memory allocated for the instance ID.
 * @param instanceName Pointer to a char* that will be populated with the instance name. The caller is responsible for freeing the memory allocated for the instance name.
 * @param instanceVersion Pointer to a char* that will be populated with the instance version. The caller is responsible for freeing the memory allocated for the instance version.
 * @param detectorMode Pointer to an int that will be populated with the detector mode of the instance.
 * @param detectionSensitivity Pointer to an int that will be populated with the detection sensitivity of the instance.
 * @param movementSensitivity Pointer to an int that will be populated with the movement sensitivity of the instance.
 * @param sensorModality Pointer to an int that will be populated with the sensor modality of the instance.
 * @param frameRateLimit Pointer to an int that will be populated with the frame rate limit of the instance.
 * @param metadataMode Pointer to an int that will be populated with the metadata mode of the instance.
 * @param statisticsMode Pointer to an int that will be populated with the statistics mode of the instance.
 * @param diagnosticsMode Pointer to an int that will be populated with the diagnostics mode of the instance.
 * @param debugMode Pointer to an int that will be populated with the debug mode of the instance.
 *
 * @return Returns 0 if the instance handle is invalid, otherwise returns 1.
 */
EXPORT int securt_get_instance(int const handle, char** instanceId, char** instanceName, char** instanceVersion,
                               int* detectorMode, int* detectionSensitivity, int* movementSensitivity,
                               int* sensorModality, int* frameRateLimit, int* metadataMode, int* statisticsMode,
                               int* diagnosticsMode, int* debugMode);

/**
 * @brief Updates the options of a SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param detectorMode The detector mode to be set (0 = Disabled, 1 = Enabled).
 * @param detectionSensitivity The detection sensitivity to be set (0 = Low, 1 = Medium, 2 = High).
 * @param movementSensitivity The movement sensitivity to be set (0 = Low, 1 = Medium, 2 = High).
 * @param sensorModality The sensor modality to be set (0 = RGB, 1 = Thermal).
 * @param frameRateLimit The frame rate limit to be set (0 = Unlimited, positive integer = Limit in frames per second).
 * @param metadataMode The metadata mode to be set (0 = Disabled, 1 = Enabled).
 * @param statisticsMode The statistics mode to be set (0 = Disabled, 1 = Enabled).
 * @param diagnosticsMode The diagnostics mode to be set (0 = Disabled, 1 = Enabled).
 * @param debugMode The debug mode to be set (0 = Disabled, 1 = Enabled).
 *
 * @return Returns 1 if the options were successfully updated, 0 otherwise.
 */
EXPORT int securt_update_instance_options(int const handle, int const detectorMode, int const detectionSensitivity,
                                          int const movementSensitivity, int const sensorModality,
                                          int const frameRateLimit, int const metadataMode, int const statisticsMode,
                                          int const diagnosticsMode, int const debugMode);

// internal method
EXPORT void securt_reset();

EXPORT int securt_set_config_value(int const handle, char const* key, char const* value);

/**
 * Sets the statistics mode for a SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param statisticsMode The statistics mode to set. Possible values:
 *                       - 0: Disable statistics collection.
 *                       - 1: Enable statistics collection.
 *
 * @return Returns 1 if the statistics mode was successfully set, 0 otherwise.
 */
EXPORT int securt_set_statistics_mode(int const handle, int statisticsMode);

/**
 * Sets the metadata mode for a SecuRT instance.
 *
 * @param handle The handle of the SecuRT instance.
 * @param metadataMode The metadata mode to set. Possible values:
 *                     - 0: Disable metadata collection.
 *                     - 1: Enable metadata collection.
 *
 * @return Returns 1 if the metadata mode was successfully set, 0 otherwise.
 */
EXPORT int securt_set_metadata_mode(int const handle, int metadataMode);

EXPORT int securt_set_render_preset(int const contextHandle, char const* preset);

#ifdef __cplusplus
}
#endif
