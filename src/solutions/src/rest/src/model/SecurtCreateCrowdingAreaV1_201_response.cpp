/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 2024.2.1
 * Contact: support@cvedia.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */



#include "rt_rest_client/model/SecurtCreateCrowdingAreaV1_201_response.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



SecurtCreateCrowdingAreaV1_201_response::SecurtCreateCrowdingAreaV1_201_response()
{
    m_areaId = utility::conversions::to_string_t("");
    m_areaIdIsSet = false;
}

SecurtCreateCrowdingAreaV1_201_response::~SecurtCreateCrowdingAreaV1_201_response()
{
}

void SecurtCreateCrowdingAreaV1_201_response::validate()
{
    // TODO: implement validation
}

web::json::value SecurtCreateCrowdingAreaV1_201_response::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_areaIdIsSet)
    {
        val[utility::conversions::to_string_t(U("areaId"))] = ModelBase::toJson(m_areaId);
    }

    return val;
}

bool SecurtCreateCrowdingAreaV1_201_response::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("areaId"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("areaId")));
        if(!fieldValue.is_null())
        {
            utility::string_t refVal_setAreaId;
            ok &= ModelBase::fromJson(fieldValue, refVal_setAreaId);
            setAreaId(refVal_setAreaId);
        }
    }
    return ok;
}

void SecurtCreateCrowdingAreaV1_201_response::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_areaIdIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("areaId")), m_areaId));
    }
}

bool SecurtCreateCrowdingAreaV1_201_response::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("areaId"))))
    {
        utility::string_t refVal_setAreaId;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("areaId"))), refVal_setAreaId );
        setAreaId(refVal_setAreaId);
    }
    return ok;
}

utility::string_t SecurtCreateCrowdingAreaV1_201_response::getAreaId() const
{
    return m_areaId;
}

void SecurtCreateCrowdingAreaV1_201_response::setAreaId(const utility::string_t& value)
{
    m_areaId = value;
    m_areaIdIsSet = true;
}

bool SecurtCreateCrowdingAreaV1_201_response::areaIdIsSet() const
{
    return m_areaIdIsSet;
}

void SecurtCreateCrowdingAreaV1_201_response::unsetareaId()
{
    m_areaIdIsSet = false;
}
}
}
}
}
}


