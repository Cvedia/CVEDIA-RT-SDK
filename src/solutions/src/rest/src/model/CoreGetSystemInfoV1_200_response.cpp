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



#include "rt_rest_client/model/CoreGetSystemInfoV1_200_response.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



CoreGetSystemInfoV1_200_response::CoreGetSystemInfoV1_200_response()
{
    m_systemInfoIsSet = false;
}

CoreGetSystemInfoV1_200_response::~CoreGetSystemInfoV1_200_response()
{
}

void CoreGetSystemInfoV1_200_response::validate()
{
    // TODO: implement validation
}

web::json::value CoreGetSystemInfoV1_200_response::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_systemInfoIsSet)
    {
        val[utility::conversions::to_string_t(U("systemInfo"))] = ModelBase::toJson(m_systemInfo);
    }

    return val;
}

bool CoreGetSystemInfoV1_200_response::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("systemInfo"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("systemInfo")));
        if(!fieldValue.is_null())
        {
            std::vector<std::shared_ptr<DisplayableEntity>> refVal_setSystemInfo;
            ok &= ModelBase::fromJson(fieldValue, refVal_setSystemInfo);
            setSystemInfo(refVal_setSystemInfo);
        }
    }
    return ok;
}

void CoreGetSystemInfoV1_200_response::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_systemInfoIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("systemInfo")), m_systemInfo));
    }
}

bool CoreGetSystemInfoV1_200_response::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("systemInfo"))))
    {
        std::vector<std::shared_ptr<DisplayableEntity>> refVal_setSystemInfo;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("systemInfo"))), refVal_setSystemInfo );
        setSystemInfo(refVal_setSystemInfo);
    }
    return ok;
}

std::vector<std::shared_ptr<DisplayableEntity>>& CoreGetSystemInfoV1_200_response::getSystemInfo()
{
    return m_systemInfo;
}

void CoreGetSystemInfoV1_200_response::setSystemInfo(const std::vector<std::shared_ptr<DisplayableEntity>>& value)
{
    m_systemInfo = value;
    m_systemInfoIsSet = true;
}

bool CoreGetSystemInfoV1_200_response::systemInfoIsSet() const
{
    return m_systemInfoIsSet;
}

void CoreGetSystemInfoV1_200_response::unsetsystemInfo()
{
    m_systemInfoIsSet = false;
}
}
}
}
}
}


