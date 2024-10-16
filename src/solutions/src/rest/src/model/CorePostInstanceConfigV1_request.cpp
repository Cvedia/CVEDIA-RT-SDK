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



#include "rt_rest_client/model/CorePostInstanceConfigV1_request.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



CorePostInstanceConfigV1_request::CorePostInstanceConfigV1_request()
{
    m_path = utility::conversions::to_string_t("");
    m_pathIsSet = false;
    m_jsonValue = utility::conversions::to_string_t("");
    m_jsonValueIsSet = false;
}

CorePostInstanceConfigV1_request::~CorePostInstanceConfigV1_request()
{
}

void CorePostInstanceConfigV1_request::validate()
{
    // TODO: implement validation
}

web::json::value CorePostInstanceConfigV1_request::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_pathIsSet)
    {
        val[utility::conversions::to_string_t(U("path"))] = ModelBase::toJson(m_path);
    }
    if(m_jsonValueIsSet)
    {
        val[utility::conversions::to_string_t(U("jsonValue"))] = ModelBase::toJson(m_jsonValue);
    }

    return val;
}

bool CorePostInstanceConfigV1_request::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("path"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("path")));
        if(!fieldValue.is_null())
        {
            utility::string_t refVal_setPath;
            ok &= ModelBase::fromJson(fieldValue, refVal_setPath);
            setPath(refVal_setPath);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("jsonValue"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("jsonValue")));
        if(!fieldValue.is_null())
        {
            utility::string_t refVal_setJsonValue;
            ok &= ModelBase::fromJson(fieldValue, refVal_setJsonValue);
            setJsonValue(refVal_setJsonValue);
        }
    }
    return ok;
}

void CorePostInstanceConfigV1_request::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_pathIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("path")), m_path));
    }
    if(m_jsonValueIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("jsonValue")), m_jsonValue));
    }
}

bool CorePostInstanceConfigV1_request::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("path"))))
    {
        utility::string_t refVal_setPath;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("path"))), refVal_setPath );
        setPath(refVal_setPath);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("jsonValue"))))
    {
        utility::string_t refVal_setJsonValue;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("jsonValue"))), refVal_setJsonValue );
        setJsonValue(refVal_setJsonValue);
    }
    return ok;
}

utility::string_t CorePostInstanceConfigV1_request::getPath() const
{
    return m_path;
}

void CorePostInstanceConfigV1_request::setPath(const utility::string_t& value)
{
    m_path = value;
    m_pathIsSet = true;
}

bool CorePostInstanceConfigV1_request::pathIsSet() const
{
    return m_pathIsSet;
}

void CorePostInstanceConfigV1_request::unsetpath()
{
    m_pathIsSet = false;
}
utility::string_t CorePostInstanceConfigV1_request::getJsonValue() const
{
    return m_jsonValue;
}

void CorePostInstanceConfigV1_request::setJsonValue(const utility::string_t& value)
{
    m_jsonValue = value;
    m_jsonValueIsSet = true;
}

bool CorePostInstanceConfigV1_request::jsonValueIsSet() const
{
    return m_jsonValueIsSet;
}

void CorePostInstanceConfigV1_request::unsetjsonValue()
{
    m_jsonValueIsSet = false;
}
}
}
}
}
}


