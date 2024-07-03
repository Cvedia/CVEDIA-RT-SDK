/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 0.1
 * Contact: support@cvedia.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */



#include "rt_rest_client/model/SecurtGetAnalyticsEntitiesV1_200_response.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



SecurtGetAnalyticsEntitiesV1_200_response::SecurtGetAnalyticsEntitiesV1_200_response()
{
    m_crossingAreasIsSet = false;
    m_intrusionAreasIsSet = false;
    m_loiteringAreasIsSet = false;
    m_crowdingAreasIsSet = false;
    m_crossingLinesIsSet = false;
}

SecurtGetAnalyticsEntitiesV1_200_response::~SecurtGetAnalyticsEntitiesV1_200_response()
{
}

void SecurtGetAnalyticsEntitiesV1_200_response::validate()
{
    // TODO: implement validation
}

web::json::value SecurtGetAnalyticsEntitiesV1_200_response::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_crossingAreasIsSet)
    {
        val[utility::conversions::to_string_t(U("crossingAreas"))] = ModelBase::toJson(m_crossingAreas);
    }
    if(m_intrusionAreasIsSet)
    {
        val[utility::conversions::to_string_t(U("intrusionAreas"))] = ModelBase::toJson(m_intrusionAreas);
    }
    if(m_loiteringAreasIsSet)
    {
        val[utility::conversions::to_string_t(U("loiteringAreas"))] = ModelBase::toJson(m_loiteringAreas);
    }
    if(m_crowdingAreasIsSet)
    {
        val[utility::conversions::to_string_t(U("crowdingAreas"))] = ModelBase::toJson(m_crowdingAreas);
    }
    if(m_crossingLinesIsSet)
    {
        val[utility::conversions::to_string_t(U("crossingLines"))] = ModelBase::toJson(m_crossingLines);
    }

    return val;
}

bool SecurtGetAnalyticsEntitiesV1_200_response::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("crossingAreas"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("crossingAreas")));
        if(!fieldValue.is_null())
        {
            std::vector<std::shared_ptr<AreaCrossingRead>> refVal_setCrossingAreas;
            ok &= ModelBase::fromJson(fieldValue, refVal_setCrossingAreas);
            setCrossingAreas(refVal_setCrossingAreas);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("intrusionAreas"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("intrusionAreas")));
        if(!fieldValue.is_null())
        {
            std::vector<std::shared_ptr<AreaIntrusionRead>> refVal_setIntrusionAreas;
            ok &= ModelBase::fromJson(fieldValue, refVal_setIntrusionAreas);
            setIntrusionAreas(refVal_setIntrusionAreas);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("loiteringAreas"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("loiteringAreas")));
        if(!fieldValue.is_null())
        {
            std::vector<std::shared_ptr<AreaIntrusionRead>> refVal_setLoiteringAreas;
            ok &= ModelBase::fromJson(fieldValue, refVal_setLoiteringAreas);
            setLoiteringAreas(refVal_setLoiteringAreas);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("crowdingAreas"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("crowdingAreas")));
        if(!fieldValue.is_null())
        {
            std::vector<std::shared_ptr<AreaCrowdingRead>> refVal_setCrowdingAreas;
            ok &= ModelBase::fromJson(fieldValue, refVal_setCrowdingAreas);
            setCrowdingAreas(refVal_setCrowdingAreas);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("crossingLines"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("crossingLines")));
        if(!fieldValue.is_null())
        {
            std::vector<std::shared_ptr<LineCrossingRead>> refVal_setCrossingLines;
            ok &= ModelBase::fromJson(fieldValue, refVal_setCrossingLines);
            setCrossingLines(refVal_setCrossingLines);
        }
    }
    return ok;
}

void SecurtGetAnalyticsEntitiesV1_200_response::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_crossingAreasIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("crossingAreas")), m_crossingAreas));
    }
    if(m_intrusionAreasIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("intrusionAreas")), m_intrusionAreas));
    }
    if(m_loiteringAreasIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("loiteringAreas")), m_loiteringAreas));
    }
    if(m_crowdingAreasIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("crowdingAreas")), m_crowdingAreas));
    }
    if(m_crossingLinesIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("crossingLines")), m_crossingLines));
    }
}

bool SecurtGetAnalyticsEntitiesV1_200_response::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("crossingAreas"))))
    {
        std::vector<std::shared_ptr<AreaCrossingRead>> refVal_setCrossingAreas;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("crossingAreas"))), refVal_setCrossingAreas );
        setCrossingAreas(refVal_setCrossingAreas);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("intrusionAreas"))))
    {
        std::vector<std::shared_ptr<AreaIntrusionRead>> refVal_setIntrusionAreas;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("intrusionAreas"))), refVal_setIntrusionAreas );
        setIntrusionAreas(refVal_setIntrusionAreas);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("loiteringAreas"))))
    {
        std::vector<std::shared_ptr<AreaIntrusionRead>> refVal_setLoiteringAreas;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("loiteringAreas"))), refVal_setLoiteringAreas );
        setLoiteringAreas(refVal_setLoiteringAreas);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("crowdingAreas"))))
    {
        std::vector<std::shared_ptr<AreaCrowdingRead>> refVal_setCrowdingAreas;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("crowdingAreas"))), refVal_setCrowdingAreas );
        setCrowdingAreas(refVal_setCrowdingAreas);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("crossingLines"))))
    {
        std::vector<std::shared_ptr<LineCrossingRead>> refVal_setCrossingLines;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("crossingLines"))), refVal_setCrossingLines );
        setCrossingLines(refVal_setCrossingLines);
    }
    return ok;
}

std::vector<std::shared_ptr<AreaCrossingRead>>& SecurtGetAnalyticsEntitiesV1_200_response::getCrossingAreas()
{
    return m_crossingAreas;
}

void SecurtGetAnalyticsEntitiesV1_200_response::setCrossingAreas(const std::vector<std::shared_ptr<AreaCrossingRead>>& value)
{
    m_crossingAreas = value;
    m_crossingAreasIsSet = true;
}

bool SecurtGetAnalyticsEntitiesV1_200_response::crossingAreasIsSet() const
{
    return m_crossingAreasIsSet;
}

void SecurtGetAnalyticsEntitiesV1_200_response::unsetcrossingAreas()
{
    m_crossingAreasIsSet = false;
}
std::vector<std::shared_ptr<AreaIntrusionRead>>& SecurtGetAnalyticsEntitiesV1_200_response::getIntrusionAreas()
{
    return m_intrusionAreas;
}

void SecurtGetAnalyticsEntitiesV1_200_response::setIntrusionAreas(const std::vector<std::shared_ptr<AreaIntrusionRead>>& value)
{
    m_intrusionAreas = value;
    m_intrusionAreasIsSet = true;
}

bool SecurtGetAnalyticsEntitiesV1_200_response::intrusionAreasIsSet() const
{
    return m_intrusionAreasIsSet;
}

void SecurtGetAnalyticsEntitiesV1_200_response::unsetintrusionAreas()
{
    m_intrusionAreasIsSet = false;
}
std::vector<std::shared_ptr<AreaIntrusionRead>>& SecurtGetAnalyticsEntitiesV1_200_response::getLoiteringAreas()
{
    return m_loiteringAreas;
}

void SecurtGetAnalyticsEntitiesV1_200_response::setLoiteringAreas(const std::vector<std::shared_ptr<AreaIntrusionRead>>& value)
{
    m_loiteringAreas = value;
    m_loiteringAreasIsSet = true;
}

bool SecurtGetAnalyticsEntitiesV1_200_response::loiteringAreasIsSet() const
{
    return m_loiteringAreasIsSet;
}

void SecurtGetAnalyticsEntitiesV1_200_response::unsetloiteringAreas()
{
    m_loiteringAreasIsSet = false;
}
std::vector<std::shared_ptr<AreaCrowdingRead>>& SecurtGetAnalyticsEntitiesV1_200_response::getCrowdingAreas()
{
    return m_crowdingAreas;
}

void SecurtGetAnalyticsEntitiesV1_200_response::setCrowdingAreas(const std::vector<std::shared_ptr<AreaCrowdingRead>>& value)
{
    m_crowdingAreas = value;
    m_crowdingAreasIsSet = true;
}

bool SecurtGetAnalyticsEntitiesV1_200_response::crowdingAreasIsSet() const
{
    return m_crowdingAreasIsSet;
}

void SecurtGetAnalyticsEntitiesV1_200_response::unsetcrowdingAreas()
{
    m_crowdingAreasIsSet = false;
}
std::vector<std::shared_ptr<LineCrossingRead>>& SecurtGetAnalyticsEntitiesV1_200_response::getCrossingLines()
{
    return m_crossingLines;
}

void SecurtGetAnalyticsEntitiesV1_200_response::setCrossingLines(const std::vector<std::shared_ptr<LineCrossingRead>>& value)
{
    m_crossingLines = value;
    m_crossingLinesIsSet = true;
}

bool SecurtGetAnalyticsEntitiesV1_200_response::crossingLinesIsSet() const
{
    return m_crossingLinesIsSet;
}

void SecurtGetAnalyticsEntitiesV1_200_response::unsetcrossingLines()
{
    m_crossingLinesIsSet = false;
}
}
}
}
}
}


