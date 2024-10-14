/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 2024.2.3
 * Contact: support@cvedia.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

/*
 * SecuRTLinesApi.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_SecuRTLinesApi_H_
#define CVEDIA_RT_REST_CLIENT_SecuRTLinesApi_H_


#include "defines.h"

#include "rt_rest_client/ApiClient.h"

#include "rt_rest_client/model/LineCrossingWrite.h"
#include "rt_rest_client/model/LineTailgatingWrite.h"
#include "rt_rest_client/model/SecurtCreateLineCrossingV1_201_response.h"
#include "rt_rest_client/model/SecurtGetLinesV1_200_response.h"
#include <cpprest/details/basic_types.h>
#include <boost/optional.hpp>

namespace cvedia {
namespace rt {
namespace rest {
namespace client {

using namespace cvedia::rt::rest::client::model;



class  SecuRTLinesApi 
{
public:

    EXPORT explicit SecuRTLinesApi( std::shared_ptr<const ApiClient> apiClient );

    EXPORT virtual ~SecuRTLinesApi();

    /// <summary>
    /// Create a line crossing
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="lineCrossingWrite"></param>
    EXPORT pplx::task<std::shared_ptr<SecurtCreateLineCrossingV1_201_response>> securtCreateLineCrossingV1(
        utility::string_t instanceId,
        std::shared_ptr<LineCrossingWrite> lineCrossingWrite
    ) const;
    /// <summary>
    /// Create a line crossing
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="lineId"></param>
    /// <param name="lineCrossingWrite"> (optional)</param>
    EXPORT pplx::task<void> securtCreateLineCrossingWithIdV1(
        utility::string_t instanceId,
        utility::string_t lineId,
        boost::optional<std::shared_ptr<LineCrossingWrite>> lineCrossingWrite
    ) const;
    /// <summary>
    /// Create a tailgating line
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="lineTailgatingWrite"> (optional)</param>
    EXPORT pplx::task<void> securtCreateLineTailgatingV1(
        utility::string_t instanceId,
        boost::optional<std::shared_ptr<LineTailgatingWrite>> lineTailgatingWrite
    ) const;
    /// <summary>
    /// Create a tailgating line
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="lineId"></param>
    /// <param name="lineTailgatingWrite"> (optional)</param>
    EXPORT pplx::task<void> securtCreateLineTailgatingWithIdV1(
        utility::string_t instanceId,
        utility::string_t lineId,
        boost::optional<std::shared_ptr<LineTailgatingWrite>> lineTailgatingWrite
    ) const;
    /// <summary>
    /// Delete all lines
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    EXPORT pplx::task<void> securtDeleteAllLinesV1(
        utility::string_t instanceId
    ) const;
    /// <summary>
    /// Delete a line
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="lineId"></param>
    EXPORT pplx::task<void> securtDeleteLineV1(
        utility::string_t instanceId,
        utility::string_t lineId
    ) const;
    /// <summary>
    /// Get all lines
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    EXPORT pplx::task<std::shared_ptr<SecurtGetLinesV1_200_response>> securtGetLinesV1(
        utility::string_t instanceId
    ) const;

protected:
    std::shared_ptr<const ApiClient> m_ApiClient;
};

}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_SecuRTLinesApi_H_ */

