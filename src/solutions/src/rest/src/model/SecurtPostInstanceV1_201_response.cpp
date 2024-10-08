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



#include "rt_rest_client/model/SecurtPostInstanceV1_201_response.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



SecurtPostInstanceV1_201_response::SecurtPostInstanceV1_201_response()
{
    m_instanceId = utility::conversions::to_string_t("");
    m_instanceIdIsSet = false;
}

SecurtPostInstanceV1_201_response::~SecurtPostInstanceV1_201_response()
{
}

void SecurtPostInstanceV1_201_response::validate()
{
    // TODO: implement validation
}

web::json::value SecurtPostInstanceV1_201_response::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_instanceIdIsSet)
    {
        val[utility::conversions::to_string_t(U("instanceId"))] = ModelBase::toJson(m_instanceId);
    }

    return val;
}

bool SecurtPostInstanceV1_201_response::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("instanceId"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("instanceId")));
        if(!fieldValue.is_null())
        {
            utility::string_t refVal_setInstanceId;
            ok &= ModelBase::fromJson(fieldValue, refVal_setInstanceId);
            setInstanceId(refVal_setInstanceId);
        }
    }
    return ok;
}

void SecurtPostInstanceV1_201_response::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_instanceIdIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("instanceId")), m_instanceId));
    }
}

bool SecurtPostInstanceV1_201_response::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("instanceId"))))
    {
        utility::string_t refVal_setInstanceId;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("instanceId"))), refVal_setInstanceId );
        setInstanceId(refVal_setInstanceId);
    }
    return ok;
}

utility::string_t SecurtPostInstanceV1_201_response::getInstanceId() const
{
    return m_instanceId;
}

void SecurtPostInstanceV1_201_response::setInstanceId(const utility::string_t& value)
{
    m_instanceId = value;
    m_instanceIdIsSet = true;
}

bool SecurtPostInstanceV1_201_response::instanceIdIsSet() const
{
    return m_instanceIdIsSet;
}

void SecurtPostInstanceV1_201_response::unsetinstanceId()
{
    m_instanceIdIsSet = false;
}
}
}
}
}
}


