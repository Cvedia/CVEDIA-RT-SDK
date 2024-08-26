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



#include "rt_rest_client/model/CoreGetSystemConfigV1_200_response.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



CoreGetSystemConfigV1_200_response::CoreGetSystemConfigV1_200_response()
{
    m_systemConfigIsSet = false;
}

CoreGetSystemConfigV1_200_response::~CoreGetSystemConfigV1_200_response()
{
}

void CoreGetSystemConfigV1_200_response::validate()
{
    // TODO: implement validation
}

web::json::value CoreGetSystemConfigV1_200_response::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_systemConfigIsSet)
    {
        val[utility::conversions::to_string_t(U("systemConfig"))] = ModelBase::toJson(m_systemConfig);
    }

    return val;
}

bool CoreGetSystemConfigV1_200_response::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("systemConfig"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("systemConfig")));
        if(!fieldValue.is_null())
        {
            std::vector<std::shared_ptr<SystemConfigEntity>> refVal_setSystemConfig;
            ok &= ModelBase::fromJson(fieldValue, refVal_setSystemConfig);
            setSystemConfig(refVal_setSystemConfig);
        }
    }
    return ok;
}

void CoreGetSystemConfigV1_200_response::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_systemConfigIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("systemConfig")), m_systemConfig));
    }
}

bool CoreGetSystemConfigV1_200_response::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("systemConfig"))))
    {
        std::vector<std::shared_ptr<SystemConfigEntity>> refVal_setSystemConfig;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("systemConfig"))), refVal_setSystemConfig );
        setSystemConfig(refVal_setSystemConfig);
    }
    return ok;
}

std::vector<std::shared_ptr<SystemConfigEntity>>& CoreGetSystemConfigV1_200_response::getSystemConfig()
{
    return m_systemConfig;
}

void CoreGetSystemConfigV1_200_response::setSystemConfig(const std::vector<std::shared_ptr<SystemConfigEntity>>& value)
{
    m_systemConfig = value;
    m_systemConfigIsSet = true;
}

bool CoreGetSystemConfigV1_200_response::systemConfigIsSet() const
{
    return m_systemConfigIsSet;
}

void CoreGetSystemConfigV1_200_response::unsetsystemConfig()
{
    m_systemConfigIsSet = false;
}
}
}
}
}
}


