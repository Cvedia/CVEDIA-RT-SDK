/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 2024.2.3
 * Contact: 
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

/*
 * RTCoreApi.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_RTCoreApi_H_
#define CVEDIA_RT_REST_CLIENT_RTCoreApi_H_


#include "defines.h"

#include "rt_rest_client/ApiClient.h"

#include "rt_rest_client/model/CoreConsumeEventsV1_200_response_inner.h"
#include "rt_rest_client/model/CoreCreateInstanceV1_201_response.h"
#include "rt_rest_client/model/CoreGetFrameV1_200_response.h"
#include "rt_rest_client/model/CoreGetInstancesV1_200_response.h"
#include "rt_rest_client/model/CoreGetLicensesV1_200_response.h"
#include "rt_rest_client/model/CoreGetLogV1_200_response.h"
#include "rt_rest_client/model/CoreGetSolutionsV1_200_response.h"
#include "rt_rest_client/model/CoreGetSystemConfigV1_200_response.h"
#include "rt_rest_client/model/CoreGetSystemInfoV1_200_response.h"
#include "rt_rest_client/model/CoreGetVersionV1_200_response.h"
#include "rt_rest_client/model/CorePostInstanceConfigV1_request.h"
#include "rt_rest_client/model/CorePostInstanceStateV1_request.h"
#include "rt_rest_client/model/CorePutSystemConfigV1_request.h"
#include "rt_rest_client/model/CoreSetInputV1_request.h"
#include "rt_rest_client/model/CoreSetOutputHlsV1_200_response.h"
#include "rt_rest_client/model/CoreSetOutputHlsV1_request.h"
#include "rt_rest_client/model/CoreSetOutputRtspV1_request.h"
#include "rt_rest_client/model/CoreSetRenderPresetV1_request.h"
#include "rt_rest_client/model/CoreSetTentativeTracksV1_request.h"
#include "rt_rest_client/HttpContent.h"
#include "rt_rest_client/model/InstanceRead.h"
#include "rt_rest_client/model/InstanceWrite.h"
#include "rt_rest_client/Object.h"
#include <vector>
#include <cpprest/details/basic_types.h>
#include <boost/optional.hpp>

namespace cvedia {
namespace rt {
namespace rest {
namespace client {

using namespace cvedia::rt::rest::client::model;



class  RTCoreApi 
{
public:

    EXPORT explicit RTCoreApi( std::shared_ptr<const ApiClient> apiClient );

    EXPORT virtual ~RTCoreApi();

    /// <summary>
    /// Get events
    /// </summary>
    /// <remarks>
    /// This endpoint returns the events for an instance.     The schemas for events can be found at https://bin.cvedia.com/schema/
    /// </remarks>
    /// <param name="instanceId"></param>
    EXPORT pplx::task<std::vector<std::shared_ptr<CoreConsumeEventsV1_200_response_inner>>> coreConsumeEventsV1(
        utility::string_t instanceId
    ) const;
    /// <summary>
    /// Create a new instance
    /// </summary>
    /// <remarks>
    /// This endpoint has a similar behaviour as the &#x60;POST /v1/core/instance&#x60; endpoint except it allows providing the instance id. The client must provide a unique id for the new instance to be created.
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="instanceWrite"> (optional)</param>
    EXPORT pplx::task<void> coreCreateInstance2V1(
        utility::string_t instanceId,
        boost::optional<std::shared_ptr<InstanceWrite>> instanceWrite
    ) const;
    /// <summary>
    /// Create a new instance
    /// </summary>
    /// <remarks>
    /// Creates and registers a new instance within an optional group and solution. This methods returns a new random UUID that can be used to control the instance.  The &#x60;group&#x60; field can be left empty if not required. &#x60;solution&#x60; is also optional and if used needs to match one of the available solution ID&#39;s.   If the instance is created with persistency enabled there will be a &#x60;json&#x60; file on disk in the &#x60;instances&#x60; folder.
    /// </remarks>
    /// <param name="instanceWrite"> (optional)</param>
    EXPORT pplx::task<std::shared_ptr<CoreCreateInstanceV1_201_response>> coreCreateInstanceV1(
        boost::optional<std::shared_ptr<InstanceWrite>> instanceWrite
    ) const;
    /// <summary>
    /// Delete an instance
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    EXPORT pplx::task<void> coreDeleteInstanceV1(
        utility::string_t instanceId
    ) const;
    /// <summary>
    /// Get the last frame from the instance
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    EXPORT pplx::task<std::shared_ptr<CoreGetFrameV1_200_response>> coreGetFrameV1(
        utility::string_t instanceId
    ) const;
    /// <summary>
    /// Get config
    /// </summary>
    /// <remarks>
    /// This endpoint provides the instance configuration, where various settings of the instance can be seen. The difference between the config and the state is the latter is for runtime only settings.
    /// </remarks>
    /// <param name="instanceId"></param>
    EXPORT pplx::task<std::shared_ptr<Object>> coreGetInstanceConfigV1(
        utility::string_t instanceId
    ) const;
    /// <summary>
    /// Get state
    /// </summary>
    /// <remarks>
    /// This endpoint provides the instance state, where various settings of the instance can be seen. The difference between the config and the state is the latter is for runtime only settings.    The state is the dictionary for runtime settings of various components. It is not the \&quot;status\&quot; information such as whether the instance is loaded/running and so on.    Since the state stores runtime related settings, it requires the instance being loaded for this endpoint to return data.
    /// </remarks>
    /// <param name="instanceId"></param>
    EXPORT pplx::task<std::shared_ptr<Object>> coreGetInstanceStateV1(
        utility::string_t instanceId
    ) const;
    /// <summary>
    /// Get an instance
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    EXPORT pplx::task<std::shared_ptr<InstanceRead>> coreGetInstanceV1(
        utility::string_t instanceId
    ) const;
    /// <summary>
    /// Get all instances
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    EXPORT pplx::task<std::shared_ptr<CoreGetInstancesV1_200_response>> coreGetInstancesV1(
    ) const;
    /// <summary>
    /// Get the information of the existing licenses on the system
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    EXPORT pplx::task<std::shared_ptr<CoreGetLicensesV1_200_response>> coreGetLicensesV1(
    ) const;
    /// <summary>
    /// Get log
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="count"> (optional, default to 0.0)</param>
    EXPORT pplx::task<std::shared_ptr<CoreGetLogV1_200_response>> coreGetLogV1(
        boost::optional<double> count
    ) const;
    /// <summary>
    /// Get all solutions
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    EXPORT pplx::task<std::shared_ptr<CoreGetSolutionsV1_200_response>> coreGetSolutionsV1(
    ) const;
    /// <summary>
    /// Get system config
    /// </summary>
    /// <remarks>
    /// Returns a dict of entities which provides information about the system such as webserver details.
    /// </remarks>
    EXPORT pplx::task<std::shared_ptr<CoreGetSystemConfigV1_200_response>> coreGetSystemConfigV1(
    ) const;
    /// <summary>
    /// Get system information
    /// </summary>
    /// <remarks>
    /// Returns a list of entities which provides information about the system such as hardware details.
    /// </remarks>
    EXPORT pplx::task<std::shared_ptr<CoreGetSystemInfoV1_200_response>> coreGetSystemInfoV1(
    ) const;
    /// <summary>
    /// Get version
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    EXPORT pplx::task<std::shared_ptr<CoreGetVersionV1_200_response>> coreGetVersionV1(
    ) const;
    /// <summary>
    /// Load an instance
    /// </summary>
    /// <remarks>
    /// Loads an instance into memory. If an instance is bound to a solution this will cause resources to be consumed. 
    /// </remarks>
    /// <param name="instanceId"></param>
    EXPORT pplx::task<void> coreLoadInstanceV1(
        utility::string_t instanceId
    ) const;
    /// <summary>
    /// Update an instance
    /// </summary>
    /// <remarks>
    /// This endpoint allows clients to update a SecuRT instance.
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="instanceWrite"> (optional)</param>
    EXPORT pplx::task<void> corePatchInstanceV1(
        utility::string_t instanceId,
        boost::optional<std::shared_ptr<InstanceWrite>> instanceWrite
    ) const;
    /// <summary>
    /// Push a compressed frame
    /// </summary>
    /// <remarks>
    /// Push a frame with a standard OpenCV decode supported format eg. JPEG, BMP, PNG and so on.
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="frame"> (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    /// <param name="timestamp"> (optional, default to 0L)</param>
    EXPORT pplx::task<void> corePostCompressedFrameV1(
        utility::string_t instanceId,
        boost::optional<std::shared_ptr<HttpContent>> frame,
        boost::optional<int64_t> timestamp
    ) const;
    /// <summary>
    /// Push an encoded frame
    /// </summary>
    /// <remarks>
    /// Push a frame with a provided codec, e.g. h264/h265
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="codecId"></param>
    /// <param name="frame"> (optional, default to utility::conversions::to_string_t(&quot;&quot;))</param>
    /// <param name="timestamp"> (optional, default to 0L)</param>
    EXPORT pplx::task<void> corePostEncodedFrameV1(
        utility::string_t instanceId,
        utility::string_t codecId,
        boost::optional<std::shared_ptr<HttpContent>> frame,
        boost::optional<int64_t> timestamp
    ) const;
    /// <summary>
    /// Set config
    /// </summary>
    /// <remarks>
    /// This endpoint sets the config value at the provided path. The path is the key in the config, and for nested structures it will be the nested keys separated by forward slashed \&quot;/\&quot;.    It is important to note that this operation will override the value at the given path.
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="corePostInstanceConfigV1Request"> (optional)</param>
    EXPORT pplx::task<void> corePostInstanceConfigV1(
        utility::string_t instanceId,
        boost::optional<std::shared_ptr<CorePostInstanceConfigV1_request>> corePostInstanceConfigV1Request
    ) const;
    /// <summary>
    /// Set state
    /// </summary>
    /// <remarks>
    /// To set a value in the instance runtime state, the instance must be in a loaded or running state.
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="corePostInstanceStateV1Request"> (optional)</param>
    EXPORT pplx::task<void> corePostInstanceStateV1(
        utility::string_t instanceId,
        boost::optional<std::shared_ptr<CorePostInstanceStateV1_request>> corePostInstanceStateV1Request
    ) const;
    /// <summary>
    /// Stop the system
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    EXPORT pplx::task<void> corePostShutdownV1(
    ) const;
    /// <summary>
    /// Update system config
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="corePutSystemConfigV1Request"> (optional)</param>
    EXPORT pplx::task<void> corePutSystemConfigV1(
        boost::optional<std::shared_ptr<CorePutSystemConfigV1_request>> corePutSystemConfigV1Request
    ) const;
    /// <summary>
    /// Restart an instance
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    EXPORT pplx::task<void> coreRestartInstanceV1(
        utility::string_t instanceId
    ) const;
    /// <summary>
    /// Set input source
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="coreSetInputV1Request"> (optional)</param>
    EXPORT pplx::task<void> coreSetInputV1(
        utility::string_t instanceId,
        boost::optional<std::shared_ptr<CoreSetInputV1_request>> coreSetInputV1Request
    ) const;
    /// <summary>
    /// Configure HLS output
    /// </summary>
    /// <remarks>
    /// This endpoint enables HLS output for an instance. Upon success it returns a URI for the output HLS stream.
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="coreSetOutputHlsV1Request"> (optional)</param>
    EXPORT pplx::task<std::shared_ptr<CoreSetOutputHlsV1_200_response>> coreSetOutputHlsV1(
        utility::string_t instanceId,
        boost::optional<std::shared_ptr<CoreSetOutputHlsV1_request>> coreSetOutputHlsV1Request
    ) const;
    /// <summary>
    /// Configure RTSP output
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="coreSetOutputRtspV1Request">This endpoint enables RTSP output for an instance at the requested URI. (optional)</param>
    EXPORT pplx::task<void> coreSetOutputRtspV1(
        utility::string_t instanceId,
        boost::optional<std::shared_ptr<CoreSetOutputRtspV1_request>> coreSetOutputRtspV1Request
    ) const;
    /// <summary>
    /// Set render preset
    /// </summary>
    /// <remarks>
    /// This endpoint allows setting the preset name to be used for rendering the output of the instance. Render presets can be created using the RT Studio.
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="coreSetRenderPresetV1Request"> (optional)</param>
    EXPORT pplx::task<void> coreSetRenderPresetV1(
        utility::string_t instanceId,
        boost::optional<std::shared_ptr<CoreSetRenderPresetV1_request>> coreSetRenderPresetV1Request
    ) const;
    /// <summary>
    /// Set tentative tracks
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="coreSetTentativeTracksV1Request"> (optional)</param>
    EXPORT pplx::task<void> coreSetTentativeTracksV1(
        utility::string_t instanceId,
        boost::optional<std::shared_ptr<CoreSetTentativeTracksV1_request>> coreSetTentativeTracksV1Request
    ) const;
    /// <summary>
    /// Start an instance
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    EXPORT pplx::task<void> coreStartInstanceV1(
        utility::string_t instanceId
    ) const;
    /// <summary>
    /// Stop an instance
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    EXPORT pplx::task<void> coreStopInstanceV1(
        utility::string_t instanceId
    ) const;
    /// <summary>
    /// Unload an instance
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    EXPORT pplx::task<void> coreUnloadInstanceV1(
        utility::string_t instanceId
    ) const;

protected:
    std::shared_ptr<const ApiClient> m_ApiClient;
};

}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_RTCoreApi_H_ */

