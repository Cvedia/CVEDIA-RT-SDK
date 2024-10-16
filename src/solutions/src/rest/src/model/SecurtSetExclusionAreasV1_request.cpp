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



#include "rt_rest_client/model/SecurtSetExclusionAreasV1_request.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



SecurtSetExclusionAreasV1_request::SecurtSetExclusionAreasV1_request()
{
    m_areasIsSet = false;
}

SecurtSetExclusionAreasV1_request::~SecurtSetExclusionAreasV1_request()
{
}

void SecurtSetExclusionAreasV1_request::validate()
{
    // TODO: implement validation
}

web::json::value SecurtSetExclusionAreasV1_request::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_areasIsSet)
    {
        val[utility::conversions::to_string_t(U("areas"))] = ModelBase::toJson(m_areas);
    }

    return val;
}

bool SecurtSetExclusionAreasV1_request::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("areas"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("areas")));
        if(!fieldValue.is_null())
        {
            std::vector<std::vector<std::shared_ptr<Coordinate>>> refVal_setAreas;
            ok &= ModelBase::fromJson(fieldValue, refVal_setAreas);
            setAreas(refVal_setAreas);
        }
    }
    return ok;
}

void SecurtSetExclusionAreasV1_request::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_areasIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("areas")), m_areas));
    }
}

bool SecurtSetExclusionAreasV1_request::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("areas"))))
    {
        std::vector<std::vector<std::shared_ptr<Coordinate>>> refVal_setAreas;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("areas"))), refVal_setAreas );
        setAreas(refVal_setAreas);
    }
    return ok;
}

std::vector<std::vector<std::shared_ptr<Coordinate>>>& SecurtSetExclusionAreasV1_request::getAreas()
{
    return m_areas;
}

void SecurtSetExclusionAreasV1_request::setAreas(const std::vector<std::vector<std::shared_ptr<Coordinate>>>& value)
{
    m_areas = value;
    m_areasIsSet = true;
}

bool SecurtSetExclusionAreasV1_request::areasIsSet() const
{
    return m_areasIsSet;
}

void SecurtSetExclusionAreasV1_request::unsetareas()
{
    m_areasIsSet = false;
}
}
}
}
}
}


