/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 2024.2.0
 * Contact: support@cvedia.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */



#include "rt_rest_client/model/CoreSetOutputHlsV1_request.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



CoreSetOutputHlsV1_request::CoreSetOutputHlsV1_request()
{
    m_enabled = false;
    m_enabledIsSet = false;
}

CoreSetOutputHlsV1_request::~CoreSetOutputHlsV1_request()
{
}

void CoreSetOutputHlsV1_request::validate()
{
    // TODO: implement validation
}

web::json::value CoreSetOutputHlsV1_request::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_enabledIsSet)
    {
        val[utility::conversions::to_string_t(U("enabled"))] = ModelBase::toJson(m_enabled);
    }

    return val;
}

bool CoreSetOutputHlsV1_request::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("enabled"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("enabled")));
        if(!fieldValue.is_null())
        {
            bool refVal_setEnabled;
            ok &= ModelBase::fromJson(fieldValue, refVal_setEnabled);
            setEnabled(refVal_setEnabled);
        }
    }
    return ok;
}

void CoreSetOutputHlsV1_request::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_enabledIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("enabled")), m_enabled));
    }
}

bool CoreSetOutputHlsV1_request::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("enabled"))))
    {
        bool refVal_setEnabled;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("enabled"))), refVal_setEnabled );
        setEnabled(refVal_setEnabled);
    }
    return ok;
}

bool CoreSetOutputHlsV1_request::isEnabled() const
{
    return m_enabled;
}

void CoreSetOutputHlsV1_request::setEnabled(bool value)
{
    m_enabled = value;
    m_enabledIsSet = true;
}

bool CoreSetOutputHlsV1_request::enabledIsSet() const
{
    return m_enabledIsSet;
}

void CoreSetOutputHlsV1_request::unsetenabled()
{
    m_enabledIsSet = false;
}
}
}
}
}
}


