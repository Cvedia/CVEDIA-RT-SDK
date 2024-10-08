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
 * SecuRTAreasApi.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_SecuRTAreasApi_H_
#define CVEDIA_RT_REST_CLIENT_SecuRTAreasApi_H_


#include "defines.h"

#include "rt_rest_client/ApiClient.h"

#include "rt_rest_client/model/AreaAlprWrite.h"
#include "rt_rest_client/model/AreaArmedPersonWrite.h"
#include "rt_rest_client/model/AreaCrossingWrite.h"
#include "rt_rest_client/model/AreaCrowdingWrite.h"
#include "rt_rest_client/model/AreaFallenPersonWrite.h"
#include "rt_rest_client/model/AreaIntrusionWrite.h"
#include "rt_rest_client/model/AreaLoiteringWrite.h"
#include "rt_rest_client/model/AreaObjectLeftRemovedWrite.h"
#include "rt_rest_client/model/SecurtCreateArmedPersonAreaV1_201_response.h"
#include "rt_rest_client/model/SecurtCreateCrossingAreaV1_201_response.h"
#include "rt_rest_client/model/SecurtCreateCrowdingAreaV1_201_response.h"
#include "rt_rest_client/model/SecurtCreateIntrusionAreaV1_201_response.h"
#include "rt_rest_client/model/SecurtCreateLoiteringAreaV1_201_response.h"
#include "rt_rest_client/model/SecurtGetAreasV1_200_response.h"
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
    /// Create an ALPR area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaAlprWrite"> (optional)</param>
    EXPORT pplx::task<void> securtCreateAlprAreaV1(
        utility::string_t instanceId,
        boost::optional<std::shared_ptr<AreaAlprWrite>> areaAlprWrite
    ) const;
    /// <summary>
    /// Create an ALPR area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaId"></param>
    /// <param name="areaAlprWrite"> (optional)</param>
    EXPORT pplx::task<void> securtCreateAlprAreaWithIdV1(
        utility::string_t instanceId,
        utility::string_t areaId,
        boost::optional<std::shared_ptr<AreaAlprWrite>> areaAlprWrite
    ) const;
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
    /// Create a fallen person area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaFallenPersonWrite"> (optional)</param>
    EXPORT pplx::task<void> securtCreateFallenPersonAreaV1(
        utility::string_t instanceId,
        boost::optional<std::shared_ptr<AreaFallenPersonWrite>> areaFallenPersonWrite
    ) const;
    /// <summary>
    /// Create a fallen person area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaId"></param>
    /// <param name="areaFallenPersonWrite"> (optional)</param>
    EXPORT pplx::task<void> securtCreateFallenPersonAreaWithIdV1(
        utility::string_t instanceId,
        utility::string_t areaId,
        boost::optional<std::shared_ptr<AreaFallenPersonWrite>> areaFallenPersonWrite
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
    /// Create an object left area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaObjectLeftRemovedWrite"> (optional)</param>
    EXPORT pplx::task<void> securtCreateObjectLeftAreaV1(
        utility::string_t instanceId,
        boost::optional<std::shared_ptr<AreaObjectLeftRemovedWrite>> areaObjectLeftRemovedWrite
    ) const;
    /// <summary>
    /// Create an object left area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaId"></param>
    /// <param name="areaObjectLeftRemovedWrite"> (optional)</param>
    EXPORT pplx::task<void> securtCreateObjectLeftAreaWithIdV1(
        utility::string_t instanceId,
        utility::string_t areaId,
        boost::optional<std::shared_ptr<AreaObjectLeftRemovedWrite>> areaObjectLeftRemovedWrite
    ) const;
    /// <summary>
    /// Creat an object removed area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaObjectLeftRemovedWrite"> (optional)</param>
    EXPORT pplx::task<void> securtCreateObjectRemovedAreaV1(
        utility::string_t instanceId,
        boost::optional<std::shared_ptr<AreaObjectLeftRemovedWrite>> areaObjectLeftRemovedWrite
    ) const;
    /// <summary>
    /// Create an object removed area
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="instanceId"></param>
    /// <param name="areaId"></param>
    /// <param name="areaObjectLeftRemovedWrite"> (optional)</param>
    EXPORT pplx::task<void> securtCreateObjectRemovedAreaWithIdV1(
        utility::string_t instanceId,
        utility::string_t areaId,
        boost::optional<std::shared_ptr<AreaObjectLeftRemovedWrite>> areaObjectLeftRemovedWrite
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

