/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 0.1
 * Contact: support@cvedia.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.2.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

/*
 * SecuRTAreasApi.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_SecuRTAreasApi_H_
#define CVEDIA_RT_REST_CLIENT_SecuRTAreasApi_H_


#include "defines.h"

#include "securt_rest_client/ApiClient.h"

#include "securt_rest_client/model/AreaArmedPersonWrite.h"
#include "securt_rest_client/model/AreaCrossingWrite.h"
#include "securt_rest_client/model/AreaCrowdingWrite.h"
#include "securt_rest_client/model/AreaIntrusionWrite.h"
#include "securt_rest_client/model/AreaLoiteringWrite.h"
#include "securt_rest_client/model/SecurtCreateArmedPersonAreaV1_201_response.h"
#include "securt_rest_client/model/SecurtCreateCrossingAreaV1_201_response.h"
#include "securt_rest_client/model/SecurtCreateCrowdingAreaV1_201_response.h"
#include "securt_rest_client/model/SecurtCreateIntrusionAreaV1_201_response.h"
#include "securt_rest_client/model/SecurtCreateLoiteringAreaV1_201_response.h"
#include "securt_rest_client/model/SecurtGetAreasV1_200_response.h"
#include <cpprest/details/basic_types.h>
#include <boost/optional.hpp>

namespace cvedia {
namespace rt {
namespace rest {
namespace client {

using namespace cvedia::rt::rest::client::model;



class  SecuRTAreasApi 
{
public:

    EXPORT explicit SecuRTAreasApi( std::shared_ptr<const ApiClient> apiClient );

    EXPORT virtual ~SecuRTAreasApi();

    /// <summary>
    /// Create an armed person area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaArmedPersonWrite"> (optional)</param>
    EXPORT pplx::task<std::shared_ptr<SecurtCreateArmedPersonAreaV1_201_response>> securtCreateArmedPersonAreaV1(
        utility::string_t instanceId,
        boost::optional<std::shared_ptr<AreaArmedPersonWrite>> areaArmedPersonWrite
    ) const;
    /// <summary>
    /// Create an armed person area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaId"></param>
    /// <param name="areaArmedPersonWrite"> (optional)</param>
    EXPORT pplx::task<void> securtCreateArmedPersonAreaWithIdV1(
        utility::string_t instanceId,
        utility::string_t areaId,
        boost::optional<std::shared_ptr<AreaArmedPersonWrite>> areaArmedPersonWrite
    ) const;
    /// <summary>
    /// Create a crossing area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaCrossingWrite"></param>
    EXPORT pplx::task<std::shared_ptr<SecurtCreateCrossingAreaV1_201_response>> securtCreateCrossingAreaV1(
        utility::string_t instanceId,
        std::shared_ptr<AreaCrossingWrite> areaCrossingWrite
    ) const;
    /// <summary>
    /// Create a crossing area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaId"></param>
    /// <param name="areaCrossingWrite"> (optional)</param>
    EXPORT pplx::task<void> securtCreateCrossingAreaWithIdV1(
        utility::string_t instanceId,
        utility::string_t areaId,
        boost::optional<std::shared_ptr<AreaCrossingWrite>> areaCrossingWrite
    ) const;
    /// <summary>
    /// Create a crowding area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaCrowdingWrite"></param>
    EXPORT pplx::task<std::shared_ptr<SecurtCreateCrowdingAreaV1_201_response>> securtCreateCrowdingAreaV1(
        utility::string_t instanceId,
        std::shared_ptr<AreaCrowdingWrite> areaCrowdingWrite
    ) const;
    /// <summary>
    /// Create a crowding area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaId"></param>
    /// <param name="areaCrowdingWrite"> (optional)</param>
    EXPORT pplx::task<void> securtCreateCrowdingAreaWithIdV1(
        utility::string_t instanceId,
        utility::string_t areaId,
        boost::optional<std::shared_ptr<AreaCrowdingWrite>> areaCrowdingWrite
    ) const;
    /// <summary>
    /// Create an intrusion area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaIntrusionWrite"></param>
    EXPORT pplx::task<std::shared_ptr<SecurtCreateIntrusionAreaV1_201_response>> securtCreateIntrusionAreaV1(
        utility::string_t instanceId,
        std::shared_ptr<AreaIntrusionWrite> areaIntrusionWrite
    ) const;
    /// <summary>
    /// Create an intrusion area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaId"></param>
    /// <param name="areaIntrusionWrite"> (optional)</param>
    EXPORT pplx::task<void> securtCreateIntrusionAreaWithIdV1(
        utility::string_t instanceId,
        utility::string_t areaId,
        boost::optional<std::shared_ptr<AreaIntrusionWrite>> areaIntrusionWrite
    ) const;
    /// <summary>
    /// Create a loitering area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaLoiteringWrite"></param>
    EXPORT pplx::task<std::shared_ptr<SecurtCreateLoiteringAreaV1_201_response>> securtCreateLoiteringAreaV1(
        utility::string_t instanceId,
        std::shared_ptr<AreaLoiteringWrite> areaLoiteringWrite
    ) const;
    /// <summary>
    /// Create a loitering area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaId"></param>
    /// <param name="areaLoiteringWrite"> (optional)</param>
    EXPORT pplx::task<void> securtCreateLoiteringAreaWithIdV1(
        utility::string_t instanceId,
        utility::string_t areaId,
        boost::optional<std::shared_ptr<AreaLoiteringWrite>> areaLoiteringWrite
    ) const;
    /// <summary>
    /// Delete all areas
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    EXPORT pplx::task<void> securtDeleteAllAreasV1(
        utility::string_t instanceId
    ) const;
    /// <summary>
    /// Delete an area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaId"></param>
    EXPORT pplx::task<void> securtDeleteAreaV1(
        utility::string_t instanceId,
        utility::string_t areaId
    ) const;
    /// <summary>
    /// Get all areas
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    EXPORT pplx::task<std::shared_ptr<SecurtGetAreasV1_200_response>> securtGetAreasV1(
        utility::string_t instanceId
    ) const;

protected:
    std::shared_ptr<const ApiClient> m_ApiClient;
};

}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_SecuRTAreasApi_H_ */

