/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 2024.2.2
 * Contact: support@cvedia.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */



#include "rt_rest_client/model/CorePutSystemConfigV1_request.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



CorePutSystemConfigV1_request::CorePutSystemConfigV1_request()
{
    m_systemConfigIsSet = false;
}

CorePutSystemConfigV1_request::~CorePutSystemConfigV1_request()
{
}

void CorePutSystemConfigV1_request::validate()
{
    // TODO: implement validation
}

web::json::value CorePutSystemConfigV1_request::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_systemConfigIsSet)
    {
        val[utility::conversions::to_string_t(U("systemConfig"))] = ModelBase::toJson(m_systemConfig);
    }

    return val;
}

bool CorePutSystemConfigV1_request::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("systemConfig"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("systemConfig")));
        if(!fieldValue.is_null())
        {
            std::vector<std::shared_ptr<CorePutSystemConfigV1_request_systemConfig_inner>> refVal_setSystemConfig;
            ok &= ModelBase::fromJson(fieldValue, refVal_setSystemConfig);
            setSystemConfig(refVal_setSystemConfig);
        }
    }
    return ok;
}

void CorePutSystemConfigV1_request::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
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

bool CorePutSystemConfigV1_request::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("systemConfig"))))
    {
        std::vector<std::shared_ptr<CorePutSystemConfigV1_request_systemConfig_inner>> refVal_setSystemConfig;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("systemConfig"))), refVal_setSystemConfig );
        setSystemConfig(refVal_setSystemConfig);
    }
    return ok;
}

std::vector<std::shared_ptr<CorePutSystemConfigV1_request_systemConfig_inner>>& CorePutSystemConfigV1_request::getSystemConfig()
{
    return m_systemConfig;
}

void CorePutSystemConfigV1_request::setSystemConfig(const std::vector<std::shared_ptr<CorePutSystemConfigV1_request_systemConfig_inner>>& value)
{
    m_systemConfig = value;
    m_systemConfigIsSet = true;
}

bool CorePutSystemConfigV1_request::systemConfigIsSet() const
{
    return m_systemConfigIsSet;
}

void CorePutSystemConfigV1_request::unsetsystemConfig()
{
    m_systemConfigIsSet = false;
}
}
}
}
}
}


