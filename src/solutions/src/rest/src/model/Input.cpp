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



#include "rt_rest_client/model/Input.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {


namespace
{
using EnumUnderlyingType = utility::string_t;

Input::eInput toEnum(const EnumUnderlyingType& val)
{
    if (val == utility::conversions::to_string_t(U("Manual")))
        return Input::eInput::Input_MANUAL;
    if (val == utility::conversions::to_string_t(U("RTSP")))
        return Input::eInput::Input_RTSP;
    return {};
}

EnumUnderlyingType fromEnum(Input::eInput e)
{
    switch (e)
    {
    case Input::eInput::Input_MANUAL:
        return U("Manual");
    case Input::eInput::Input_RTSP:
        return U("RTSP");
    default:
        break;
    }
    return {};
}
}

Input::Input()
{
}

Input::~Input()
{
}

void Input::validate()
{
    // TODO: implement validation
}

web::json::value Input::toJson() const
{
    auto val = fromEnum(m_value);
    return web::json::value(val);
}

bool Input::fromJson(const web::json::value& val)
{
    m_value = toEnum(val.as_string());
    return true;
}

void Input::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if (!namePrefix.empty() && namePrefix.back() != U('.'))
    {
        namePrefix.push_back(U('.'));
    }

    auto e = fromEnum(m_value);
    multipart->add(ModelBase::toHttpContent(namePrefix, e));
}

bool Input::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if (!namePrefix.empty() && namePrefix.back() != U('.'))
    {
        namePrefix.push_back(U('.'));
    }
    {
        EnumUnderlyingType e;
        ok = ModelBase::fromHttpContent(multipart->getContent(namePrefix), e);
        if (ok)
        {
            auto v = toEnum(e);
            setValue(v);
        }
    }
    return ok;
}

Input::eInput Input::getValue() const
{
   return m_value;
}

void Input::setValue(Input::eInput const value)
{
   m_value = value;
}


}
}
}
}
}


