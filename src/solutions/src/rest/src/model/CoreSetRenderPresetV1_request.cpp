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



#include "rt_rest_client/model/CoreSetRenderPresetV1_request.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



CoreSetRenderPresetV1_request::CoreSetRenderPresetV1_request()
{
    m_presetName = utility::conversions::to_string_t("");
    m_presetNameIsSet = false;
}

CoreSetRenderPresetV1_request::~CoreSetRenderPresetV1_request()
{
}

void CoreSetRenderPresetV1_request::validate()
{
    // TODO: implement validation
}

web::json::value CoreSetRenderPresetV1_request::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_presetNameIsSet)
    {
        val[utility::conversions::to_string_t(U("presetName"))] = ModelBase::toJson(m_presetName);
    }

    return val;
}

bool CoreSetRenderPresetV1_request::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("presetName"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("presetName")));
        if(!fieldValue.is_null())
        {
            utility::string_t refVal_setPresetName;
            ok &= ModelBase::fromJson(fieldValue, refVal_setPresetName);
            setPresetName(refVal_setPresetName);
        }
    }
    return ok;
}

void CoreSetRenderPresetV1_request::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_presetNameIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("presetName")), m_presetName));
    }
}

bool CoreSetRenderPresetV1_request::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("presetName"))))
    {
        utility::string_t refVal_setPresetName;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("presetName"))), refVal_setPresetName );
        setPresetName(refVal_setPresetName);
    }
    return ok;
}

utility::string_t CoreSetRenderPresetV1_request::getPresetName() const
{
    return m_presetName;
}

void CoreSetRenderPresetV1_request::setPresetName(const utility::string_t& value)
{
    m_presetName = value;
    m_presetNameIsSet = true;
}

bool CoreSetRenderPresetV1_request::presetNameIsSet() const
{
    return m_presetNameIsSet;
}

void CoreSetRenderPresetV1_request::unsetpresetName()
{
    m_presetNameIsSet = false;
}
}
}
}
}
}


