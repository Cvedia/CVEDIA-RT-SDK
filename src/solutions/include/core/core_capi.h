#pragma once

#include "api/capi_base.h"

#ifdef __cplusplus
extern "C" {
#endif

    EXPORT int core_create_context(char const* instanceUuid, char const* remoteIp);
    EXPORT int core_delete_context(int const contextHandle);
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
     * @return Returns a string with the IP address of the remote CVEDIA-RT instance.
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
     * @return
     */
    EXPORT int core_load(int const contextHandle);

    /**
     * Unloads an existing instance that is loaded.
     *
     * @param contextHandle The handle of the instance to unload.
     *
     * @return
     */
    EXPORT int core_unload(int const contextHandle);
    
    /**
     * Starts an instance.
     *
     * @param contextHandle The handle of the instance to start.
     *
     * @return int
     *
     * @note If the handle does not exist, has already been destroyed, or is already running, the function does nothing.
     */
    EXPORT int core_start(int const contextHandle);

    /**
     * Stops an existing instance.
     *
     * @param contextHandle The handle of the instance to stop.
     *
     * @return int
     *
     * @note If the handle does not exist, has already been destroyed, or is already stopped, the function does nothing.
     */
    EXPORT int core_stop(int const contextHandle);
    
    /**
     * Restarts an existing instance.
     *
     * @param contextHandle The handle of the instance to restart.
     *
     * @return int
     *
     * @note If the handle does not exist, has already been destroyed, or is already stopped, the function does nothing.
     */
    EXPORT int core_restart(int const contextHandle);

    /**
     * @brief Pushes a frame into the specified instance for processing.
     *
     * @param contextHandle The handle of the instance.
     * @param buffer The buffer containing the frame data.
     * @param width The width of the frame.
     * @param height The height of the frame.
     * @param timestampMs The timestamp of the frame in milliseconds.
     *
     * @return Returns 1 if the frame was successfully pushed, 0 is the queue was full.
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
     * @return Returns 1 if the frame was successfully pushed, 0 is the queue was full.
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
     * @return Returns 1 if the frame was successfully pushed, 0 is the queue was full.
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
     * @return Returns 1 if the frame was successfully pushed, 0 is the queue was full.
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
     * 
     * @return Returns 0 if the instance handle is invalid, otherwise returns 1.
     */
    EXPORT int core_get_instance(int const contextHandle, char** instanceId, char** instanceName, char** group,
        char** solutionId, char** solutionName, int* loaded, int* running, int* persistent, float* fps);

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
    * @return Returns 1 if the input source was successfully set, 0 otherwise.
    */
    EXPORT int core_set_input_to_manual(int const contextHandle);

    /**
     * Enables HLS output for an instance.
     *
     * @param contextHandle The handle of the instance.
     *
     * @return Returns a string with the HLS output URL if successful, NULL otherwise.
     */
    EXPORT char* core_enable_hls_output(int const contextHandle);

    /**
     * Disables HLS output for an instance.
     *
     * @param contextHandle The handle of the instance.
     *
     * @return Returns 1 if the HLS output was successfully disabled, 0 otherwise.
     */
    EXPORT int core_disable_hls_output(int const contextHandle);

    /**
     * Enables RTSP output for an instance.
     *
     * @param contextHandle The handle of the instance.
     * @param rtspUrl The URL of the RTSP stream.
     *
     * @return Returns 1 if the RTSP output was successfully enabled, 0 otherwise.
     */
    EXPORT int core_enable_rtsp_output(int const contextHandle, char const* rtspUrl);

    /**
     * Disables RTSP output for an instance.
     *
     * @param contextHandle The handle of the instance.
     * @param rtspUrl The URL of the RTSP stream.
     *
     * @return Returns 1 if the RTSP output was successfully disabled, 0 otherwise.
     */
    EXPORT int core_disable_rtsp_output(int const contextHandle, char const* rtspUrl);

    /**
     * @brief Returns events from the specified instance.
     *
     * @param contextHandle The handle of the instance.
     * @param outJson Pointer to a char* containing the JSON array of events serialized as a string; must be freed by the caller.
     *
     * @return Returns 1 if there is an event, 0 otherwise
    */
    EXPORT int core_consume_events(int const contextHandle, char** outJson);

    /**
     * @brief Frees a string allocated by the SDK.
     *
     * @param ptr The pointer to the string to be freed.
     *
     */
    EXPORT void core_free_string(void* ptr);

#ifdef __cplusplus
}
#endif
