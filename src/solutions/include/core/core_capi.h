#pragma once

#include "api/capi_base.h"

/*
 * The following is the aimed convention for our C API functions
 * Not all functions have this convention yet, please follow documentation for each function
 */
#define RT_C_SUCCESS 0
#define RT_C_ERROR_OPERATIONAL -1
#define RT_C_ERROR_INSTANCE_NOT_RUNNING -2

#ifdef __cplusplus
extern "C" {
#endif


    /**
     * Creates a context for a CVEDIA-RT instance. The context must be created before any other operation 
     * as most the C API functions require a context handle.
     *
     * @param instanceUuid (Optional) The UUID of the instance. If not provided, a random UUID will be generated.
     * @param remoteIp (Optional) The IP address of the remote CVEDIA-RT instance. If empty or NULL, the operations will be performed locally.
     *
     * @return int The context handle to be used in subsequent operations. Returns -1 if an error occurred.
     */
    EXPORT int core_create_context(char const* instanceUuid, char const* remoteIp);

    /**
     * Deletes a context for a CVEDIA-RT instance.
     *
     * @param contextHandle The handle of the context to delete.
     *
     * @return int Returns 1 if the context was successfully deleted, 0 if it did not exist.
     */
    EXPORT int core_delete_context(int const contextHandle);

    /**
     * Resets the CVEDIA-RT, deletes all contexts.
     *
     */
    EXPORT void core_reset();

    /**
     * Checks if remote connection to a CVEDIA-RT instance is alive.
     *
     * @param contextHandle The handle of the remote connection.
     *
     * @return Returns 1 if connection is alive, 0 if handle was not found, -1 if connection is dead or an error occurred.
     */
    EXPORT int core_is_alive(int const contextHandle);

    /**
     * Enables remote mode.
     *
     */
    EXPORT void core_enable_remote_mode();

    /**
     * Disables remote mode.
     *
     */
    EXPORT void core_disable_remote_mode();

    /**
     * Finds a remote CVEDIA-RT instance.
     *
     * @return Returns a string with the IP address of the remote CVEDIA-RT instance. The caller is responsible for freeing the memory allocated for the string.
     *
     */
    EXPORT char* core_find_remote_server();

    /**
     * Creates a new instance.
     *
     * @param contextHandle The handle of the instance to create.
     * @param instanceName User friendly name for the new instance. Must be a null-terminated string.
     * @param instanceGroup Group of the new instance. Must be a null-terminated string.
     * @param solutionId (Optional) Solution id of the new instance. Must be a null-terminated string.
     * @param persistent The persistency state to be set (1 = On, 0 = Off).
     *
     * @return On success, the handle (integer > 0) to the created instance is returned. On failure, -1 is returned.
     *
     * @note The function returns -1 if the instanceId already exists or if the instance solution cannot be initialized (see the log for details).
     */
    EXPORT int core_create_instance(int const contextHandle, char const* instanceName, char const* instanceGroup, char const* solutionId, int const persistent);

    /**
     * Deletes an existing instance.
     *
     * @param contextHandle The handle of the instance to destroy.
     *
     * @return 1 if the instance was successfully deleted, 0 if it did not exist.
     */
    EXPORT int core_delete_instance(int const contextHandle);

    /**
     * Loads an existing instance.
     *
     * @param contextHandle The handle of the instance to load.
     *
     * @return 1 if the instance was successfully loaded, -1 otherwise.
     */
    EXPORT int core_load(int const contextHandle);

    /**
     * Unloads an existing instance that is loaded.
     *
     * @param contextHandle The handle of the instance to unload.
     *
     * @return 1 if the instance was successfully unloaded, -1 otherwise.
     */
    EXPORT int core_unload(int const contextHandle);
    
    /**
     * Starts an instance.
     *
     * @param contextHandle The handle of the instance to start.
     *
     * @return 1 if the instance was successfully started, -1 otherwise.
     *
     * @note If the handle does not exist, has already been destroyed, or is already running, the function does nothing.
     */
    EXPORT int core_start(int const contextHandle);

    /**
     * Stops an existing instance.
     *
     * @param contextHandle The handle of the instance to stop.
     *
     * @return 1 if the instance was successfully stopped, -1 otherwise.
     *
     * @note If the handle does not exist, has already been destroyed, or is already stopped, the function does nothing.
     */
    EXPORT int core_stop(int const contextHandle);
    
    /**
     * Restarts an existing instance.
     *
     * @param contextHandle The handle of the instance to restart.
     *
     * @return 1 if the instance was successfully restarted, -1 otherwise.
     *
     * @note If the handle does not exist, has already been destroyed, or is already stopped, the function does nothing.
     */
    EXPORT int core_restart(int const contextHandle);

    /**
     * @brief Pushes a frame into the specified instance for processing. Remote instances are not supported.
     *
     * @param contextHandle The handle of the instance.
     * @param buffer The buffer containing the frame data (3 channel unsigned char - BGR).
     * @param width The width of the frame.
     * @param height The height of the frame.
     * @param timestampMs The timestamp of the frame in milliseconds.
     *
     * @return Returns 0 if the frame was successfully pushed, -1 if there was an error with the operation, -2 if there was an error due to instance not running.
     */
    EXPORT int core_push_raw_frame(int const contextHandle, void const* buffer, int const width, int const height, unsigned long long int const timestampMs);

    /**
     * @brief Pushes a compressed image frame into the specified instance for processing.
     *
     * @param contextHandle The handle of the instance.
     * @param buffer The buffer containing the frame data.
     * @param dataSize The size of the frame data.
     * @param timestampMs The timestamp of the frame in milliseconds.
     *
     * @return Returns 0 if the frame was successfully pushed, -1 if there was an error with the operation, -2 if there was an error due to instance not running.
     *
     */
    EXPORT int core_push_compressed_frame(int const contextHandle, void const* buffer, unsigned long long int const dataSize, unsigned long long int const timestampMs);
    
    /**
     * @brief Pushes a H.264 video frame into the specified instance for processing.
     *
     * @param contextHandle The handle of the instance.
     * @param buffer The buffer containing the frame data.
     * @param dataSize The size of the frame data.
     * @param timestampMs The timestamp of the frame in milliseconds.
     *
     * @return Returns 0 if the frame was successfully pushed, -1 if there was an error with the operation, -2 if there was an error due to instance not running.
     *
     */
    EXPORT int core_push_h264_frame(int const contextHandle, void const* buffer, unsigned long long int const dataSize, unsigned long long int const timestampMs);
    
    /**
     * @brief Pushes a H.265 video frame into the specified instance for processing.
     *
     * @param contextHandle The handle of the instance.
     * @param buffer The buffer containing the frame data.
     * @param dataSize The size of the frame data.
     * @param timestampMs The timestamp of the frame in milliseconds.
     *
     * @return Returns 0 if the frame was successfully pushed, -1 if there was an error with the operation, -2 if there was an error due to instance not running.
     *
     */
    EXPORT int core_push_h265_frame(int const contextHandle, void const* buffer, unsigned long long int const dataSize, unsigned long long int const timestampMs);

    /**
     * @brief Retrieves the instance information.
     *
     * @param contextHandle The handle of the instance.
     * @param instanceId Pointer to a char* that will be populated with the instance ID. The caller is responsible for freeing the memory allocated for the instance ID.
     * @param instanceName Pointer to a char* that will be populated with the instance name. The caller is responsible for freeing the memory allocated for the instance name.
     * @param group Pointer to a char* that will be populated with the instance group. The caller is responsible for freeing the memory allocated for the instance version.
     * @param solutionId Pointer to a char* that will be populated with the solution id. The caller is responsible for freeing the memory allocated for the instance version.
     * @param solutionName Pointer to a char* that will be populated with the solution name. The caller is responsible for freeing the memory allocated for the instance version.
     * @param loaded Pointer to an int that will be populated with 1 if instance is loaded and 0 otherwise.
     * @param running Pointer to an int that will be populated with 1 if instance is running and 0 otherwise.
     * @param persistent Pointer to an int that will be populated with 1 if instance is persistent and 0 otherwise.
     * @param fps Pointer to a float that will be populated with the current FPS of the instance.
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
     * @param readOnly Pointer to an int that will be populated with 1 if instance is read only and 0 otherwise.
     * @param autoStart Pointer to an int that will be populated with 1 if instance is set to auto start and 0 otherwise.
     * @param autoRestart Pointer to an int that will be populated with 1 if instance is set to auto restart and 0 otherwise.
     * @param systemInstance Pointer to an int that will be populated with 1 if instance is a system instance and 0 otherwise.
     * 
     * @return Returns 0 if the instance handle is invalid, otherwise returns 1.
     */
    EXPORT int core_get_instance(int const contextHandle, char** instanceId, char** instanceName, char** group,
        char** solutionId, char** solutionName, int* loaded, int* running, int* persistent, float* fps, 
        char** instanceVersion, int* detectorMode, int* detectionSensitivity, int* movementSensitivity,
        int* sensorModality, int* frameRateLimit, int* metadataMode, int* statisticsMode,
        int* diagnosticsMode, int* debugMode, int* readOnly, int* autoStart, int* autoRestart, int* systemInstance);

    /**
     * @brief Updates the options of an instance.
     *
     * @param contextHandle The handle of the instance.
     * @param instanceName Display name of the instance.
     * @param group Display name of the instance.
     * @param persistent Persistency of the instance (0 = Not Persistent, 1 = Persistent).
     * @param detectorMode The detector mode to be set (1 = SmartDetection, 2 = Detection).
     * @param detectionSensitivity The detection sensitivity to be set (1 = Low, 2 = Medium, 3 = High).
     * @param movementSensitivity The movement sensitivity to be set (1 = Low, 2 = Medium, 3 = High).
     * @param sensorModality The sensor modality to be set (1 = RGB, 2 = Thermal).
     * @param frameRateLimit The frame rate limit to be set (0 = Unlimited, positive integer = Limit in frames per second).
     * @param metadataMode The metadata mode to be set (0 = Disabled, 1 = Enabled).
     * @param statisticsMode The statistics mode to be set (0 = Disabled, 1 = Enabled).
     * @param diagnosticsMode The diagnostics mode to be set (0 = Disabled, 1 = Enabled).
     * @param debugMode The debug mode to be set (0 = Disabled, 1 = Enabled).
     * @param autoStart The auto-start mode to be set (0 = Disabled, 1 = Enabled).
     * @param autoRestart The auto-restart mode to be set (0 = Disabled, 1 = Enabled).
     *
     * @return Returns 1 if the options were successfully updated, 0 otherwise.
     */
    EXPORT int core_update_instance_options(int const contextHandle, 
        char const* instanceName, char const* group, int const persistent,
        int const detectorMode, int const detectionSensitivity,
        int const movementSensitivity, int const sensorModality,
        int const frameRateLimit, int const metadataMode, int const statisticsMode,
        int const diagnosticsMode, int const debugMode, int const autoStart, int const autoRestart);

    /**
     * Sets the input source of an instance to an RTSP stream.
     *
     * @param contextHandle The handle of the instance.
     * @param rtspUrl The URL of the RTSP stream.
     *
     * @return Returns 1 if the input source was successfully set, -1 otherwise.
     */
    EXPORT int core_set_input_to_rtsp(int const contextHandle, char const* rtspUrl);

    /**
    * Sets the input source of an instance to manual mode.
    *
    * @param contextHandle The handle of the instance.
    *
    * @return Returns 1 if the input source was successfully set, -1 otherwise.
    */
    EXPORT int core_set_input_to_manual(int const contextHandle);

    /**
	 * Set the input orientation so that the image gets properly rotated.
	 *
	 * @param contextHandle The handle of the instance.
	 * @param orientation The orientation of the input image (0, 90, 180, 270).
	 *
	 * @return Returns 1 if the input orientation was successfully set, -1 otherwise.
	 */
	EXPORT int core_set_input_orientation(int const contextHandle, int const orientation);

    /**
     * Enables HLS output for an instance.
     *
     * @param contextHandle The handle of the instance.
     *
     * @return Returns a string with the HLS output URL if successful, NULL otherwise. The caller is responsible for freeing the memory allocated for the string.
     */
    EXPORT char* core_enable_hls_output(int const contextHandle);

    /**
     * Disables HLS output for an instance.
     *
     * @param contextHandle The handle of the instance.
     *
     * @return Returns 1 if the HLS output was successfully disabled, -1 otherwise.
     */
    EXPORT int core_disable_hls_output(int const contextHandle);

    /**
     * Enables RTSP output for an instance.
     *
     * @param contextHandle The handle of the instance.
     * @param rtspUrl The URL of the RTSP stream.
     *
     * @return Returns 1 if the RTSP output was successfully enabled, -1 otherwise.
     */
    EXPORT int core_enable_rtsp_output(int const contextHandle, char const* rtspUrl);

    /**
     * Disables RTSP output for an instance.
     *
     * @param contextHandle The handle of the instance.
     * @param rtspUrl The URL of the RTSP stream.
     *
     * @return Returns 1 if the RTSP output was successfully disabled, -1 otherwise.
     */
    EXPORT int core_disable_rtsp_output(int const contextHandle, char const* rtspUrl);

    /**
     * @brief Returns events from the specified instance.
     *
     * @param contextHandle The handle of the instance.
     * @param outJson Pointer to a char* containing the JSON array of events serialized as a string; must be freed by the caller.
     *
     * @return Returns 1 if there is an event, 0 otherwise and -1 if an error occurred.
    */
    EXPORT int core_consume_events(int const contextHandle, char** outJson);

    /**
     * @brief Returns the instance ID of the instance.
     *
     * @param contextHandle The handle of the instance.
     *
     * @return Returns a string with the instance ID of the instance. The caller is responsible for freeing the memory allocated for the string.
     */
    EXPORT char* core_get_instance_id(int const contextHandle);

    /**
     * @brief Returns the name of the instance.
     *
     * @param contextHandle The handle of the instance.
     *
     * @return Returns a string with the name of the instance. The caller is responsible for freeing the memory allocated for the string.
     */
    EXPORT char* core_get_instance_name(int const contextHandle);

    /**
     * @brief Returns the version of the solution.
     *
     * @param contextHandle The handle of the instance.
     *
     * @return Returns a string with the version of the solution. The caller is responsible for freeing the memory allocated for the string.
     */
    EXPORT char* core_get_version(int const contextHandle);

    /**
     * @brief Sets the detector sensitivity of the specified instance.
     *
     * @param contextHandle The handle of the instance.
     * @param sensitivity The detection sensitivity to be set (1 = Low, 2 = Medium, 3 = High).
     *
     * @note The default detection sensitivity is Medium.
    */
    EXPORT int core_set_detection_sensitivity(int const contextHandle, int sensitivity);

    /**
     * @brief Sets the movement sensitivity of the specified instance.
     *
     * @param contextHandle The handle of the instance.
     * @param sensitivity The movement sensitivity to be set (1 = Low, 2 = Medium, 3 = High).
     *
     * @note The default movement sensitivity is Medium.
     */
    EXPORT int core_set_movement_sensitivity(int const contextHandle, int sensitivity);

    /**
     * @brief Returns the number of frames that have been processed.
     *
     * @param contextHandle The handle of the instance.
     *
     * @return Number of frames.
     */
    EXPORT long long int core_get_frames_processed(int const contextHandle);

    /**
     * @brief Sets a frame rate limit. This will cause push_frame to return 0 if the rate is exceeded.
     *
     * @param contextHandle The handle of the instance.
     * @param frameRateLimit The frame rate limit to be set.
     */
    EXPORT int core_set_frame_rate_limit(int const contextHandle, int frameRateLimit);

    /**
     * @brief Returns the current frame rate limit.
     *
     * @param contextHandle The handle of the instance.
     *
     * @return Current frame rate limit.
     */
    EXPORT int core_get_frame_rate_limit(int const contextHandle);

	/**
	 *	 * @brief Sets the input pixel limit of the specified instance.
	 *	 *
	 *	 * @param contextHandle The handle of the instance.
	 *	 * @param pixels The input pixel limit to be set.
	 *	 *
	 *	 * @note The default input pixel limit is 0.
	 *
	 */
	EXPORT int core_set_input_pixel_limit(int const contextHandle, int pixels);

	/**
	 *	 * @brief Returns the current input pixel limit.
	 *	 *
	 *	 * @param contextHandle The handle of the instance.
	 *	 *
	 *	 * @return Current input pixel limit.
	 *
	 */
	EXPORT int core_get_input_pixel_limit(int const contextHandle);

    /**
     * @brief Sets the sensor modality of the specified instance.
     *
     * @param contextHandle The handle of the instance.
     * @param modality The sensor modality to be set (1 = RGB, 2 = Thermal).
     *
     * @return1 if the sensor modality was successfully set, -1 otherwise.
     */
    EXPORT int core_set_sensor_modality(int const contextHandle, int modality);

    /**
     * @brief Sets the diagnostics mode of the specified instance.
     *
     * @param contextHandle The handle of the instance.
     * @param diagnostics The diagnostics mode to be set (1 = On, 0 = Off).
     *
     * @return 1 if the diagnostics mode was successfully set, -1 otherwise.
     */
    EXPORT int core_set_diagnostics_mode(int const contextHandle, int diagnostics);

    /**
     * @brief Checks if the instance needs more data or is rate limited. This function is not applicable for encoded frames.
     * Remote instances are not supported.
     *
     * @param contextHandle The handle of the instance.
     * @param currentFrameTime The current frame time in milliseconds.
     *
     * @return Returns 1 if more data is needed, 0 if the instance is rate limited.
     */
    EXPORT int core_need_data(int const contextHandle, long long int const currentFrameTime);

    /**
     * @brief Checks if specified instance is running.
     * 
     * @return Returns 1 if the instance is running, 0 otherwise.
     *
     */
    EXPORT int core_is_instance_running(int const contextHandle);

    /**
     * @brief Sets the blocking readahead queue of the specified instance.
     *
     * @param contextHandle The handle of the instance.
     * @param diagnostics State to be set (1 = On, 0 = Off).
     *
     * @return 1 if the operation was successfully, -1 otherwise.
     */
    EXPORT int core_set_blocking_readahead_queue(int const contextHandle, int const state);

    /**
     * Sets the configuration value for a instance.
     *
     * @param contextHandle The handle of the instance.
     * @param key The key of the configuration value to set.
     * @param value The value of the configuration to set.
     *
     * @return Returns 1 if the configuration value was successfully set, 0 otherwise.
     */
    EXPORT int core_set_config_value(int const contextHandle, char const* key, char const* value);

    /**
     * Sets the statistics mode for a instance.
     *
     * @param contextHandle The handle of the instance.
     * @param statisticsMode The statistics mode to set. Possible values:
     *                       - 0: Disable statistics collection.
     *                       - 1: Enable statistics collection.
     *
     * @return Returns 1 if the statistics mode was successfully set, 0 otherwise.
     */
    EXPORT int core_set_statistics_mode(int const contextHandle, int statisticsMode);

    /**
     * Sets the metadata mode for a instance.
     *
     * @param contextHandle The handle of the instance.
     * @param metadataMode The metadata mode to set. Possible values:
     *                     - 0: Disable metadata collection.
     *                     - 1: Enable metadata collection.
     *
     * @return Returns 1 if the metadata mode was successfully set, 0 otherwise.
     */
    EXPORT int core_set_metadata_mode(int const contextHandle, int metadataMode);

    /**
     * @brief Sets the detector mode of the specified instance.
     *
     * @param contextHandle The handle of the instance.
     * @param mode The detector mode to be set (1 = Smart Detection, 2 = Detection).
     *
     * @note The default detector mode is Smart Detection.
     */
    EXPORT int core_set_detector_mode(int const contextHandle, int mode);

    /**
     * @brief Sets the debug mode of the specified instance.
     *
     * @param contextHandle The handle of the instance.
     * @param debugMode The debug mode to be set (1 = On, 0 = Off).
     *
     * @note The default debug mode is Off.
     */
    EXPORT int core_set_debug_mode(int const contextHandle, int debugMode);

    /**
     * @brief Sets whether to return a track history from before the moment it caused an event
     *
     * @param contextHandle The handle of the instance.
     * @param enabled The enabled state to be set (1 = On, 0 = Off).
     *
     * @note The default state is On.
    */
    EXPORT int core_set_tentative_tracks(int const contextHandle, int enabled);

    /**
     * @brief Sets the render preset of the specified instance.
     *
     * @param contextHandle The handle of the instance.
     * @param preset The name of the render preset to be set.
     * 
     * @return 1 if the operation was successfully, -1 otherwise.
     *
    */
    EXPORT int core_set_render_preset(int const contextHandle, char const* preset);

    /**
     * Sets the auto-restart state for an instance.
     *
     * @param contextHandle The handle of the instance.
     * @param state The auto-restart state to set. Possible values:
     *              - 0: Disable auto-restart.
     *              - 1: Enable auto-restart.
     *
     * @return Returns 1 if the auto-restart state was successfully set, 0 otherwise.
     */
    EXPORT int core_set_auto_restart(int const contextHandle, int const state);

	/**
	 * Sets the auto-start state for an instance.
	 *
	 * @param contextHandle The handle of the instance.
	 * @param state The auto-start state to set. Possible values:
	 *              - 0: Disable auto-start.
	 *              - 1: Enable auto-start.
	 *
	 * @return Returns 1 if the auto-start state was successfully set, 0 otherwise.
	 */
	EXPORT int core_set_auto_start(int const contextHandle, int const state);

    /**
	 * Gets the system preferences of the specified instance.
	 *
	 * @return Returns a string with the system preferences of the instance. The caller is responsible for freeing the memory allocated for the string.
     */
    EXPORT char* core_get_preferences();

    /**
     * @brief Frees a string allocated by the SDK.
     *
     * @param ptr The pointer to the string to be freed.
     *
     */
    EXPORT void core_free_string(void* ptr);

    /**
	 * \internal
	 * This function is used internally and should not be used directly.
	 *
	 * @brief Returns the active number of tracked objects. Does not support remote instances.
	 *
	 * @param contextHandle The handle of the instance.
	 *
	 * @return Number of tracked objects in the instance.
	 *
	 */
    EXPORT int core_get_track_count(int const contextHandle);

#ifdef __cplusplus
}
#endif
