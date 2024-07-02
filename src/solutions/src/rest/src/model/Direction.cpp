/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 0.1
 * Contact: support@cvedia.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.2.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */



#include "securt_rest_client/model/Direction.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {


namespace
{
using EnumUnderlyingType = utility::string_t;

Direction::eDirection toEnum(const EnumUnderlyingType& val)
{
    if (val == utility::conversions::to_string_t(U("Up")))
        return Direction::eDirection::Direction_UP;
    if (val == utility::conversions::to_string_t(U("Down")))
        return Direction::eDirection::Direction_DOWN;
    if (val == utility::conversions::to_string_t(U("Both")))
        return Direction::eDirection::Direction_BOTH;
    return {};
}

EnumUnderlyingType fromEnum(Direction::eDirection e)
{
    switch (e)
    {
    case Direction::eDirection::Direction_UP:
        return U("Up");
    case Direction::eDirection::Direction_DOWN:
        return U("Down");
    case Direction::eDirection::Direction_BOTH:
        return U("Both");
    default:
        break;
    }
    return {};
}
}

Direction::Direction()
{
}

Direction::~Direction()
{
}

void Direction::validate()
{
    // TODO: implement validation
}

web::json::value Direction::toJson() const
{
    auto val = fromEnum(m_value);
    return web::json::value(val);
}

bool Direction::fromJson(const web::json::value& val)
{
    m_value = toEnum(val.as_string());
    return true;
}

void Direction::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if (!namePrefix.empty() && namePrefix.back() != U('.'))
    {
        namePrefix.push_back(U('.'));
    }

    auto e = fromEnum(m_value);
    multipart->add(ModelBase::toHttpContent(namePrefix, e));
}

bool Direction::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
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

Direction::eDirection Direction::getValue() const
{
   return m_value;
}

void Direction::setValue(Direction::eDirection const value)
{
   m_value = value;
}


}
}
}
}
}

