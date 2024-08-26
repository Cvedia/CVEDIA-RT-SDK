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



#include "rt_rest_client/model/AnalyticsType.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {


namespace
{
using EnumUnderlyingType = utility::string_t;

AnalyticsType::eAnalyticsType toEnum(const EnumUnderlyingType& val)
{
    if (val == utility::conversions::to_string_t(U("AreaCrossing")))
        return AnalyticsType::eAnalyticsType::AnalyticsType_AREACROSSING;
    if (val == utility::conversions::to_string_t(U("AreaIntrusion")))
        return AnalyticsType::eAnalyticsType::AnalyticsType_AREAINTRUSION;
    if (val == utility::conversions::to_string_t(U("AreaLoitering")))
        return AnalyticsType::eAnalyticsType::AnalyticsType_AREALOITERING;
    if (val == utility::conversions::to_string_t(U("AreaCrowding")))
        return AnalyticsType::eAnalyticsType::AnalyticsType_AREACROWDING;
    if (val == utility::conversions::to_string_t(U("AreaArmedPerson")))
        return AnalyticsType::eAnalyticsType::AnalyticsType_AREAARMEDPERSON;
    if (val == utility::conversions::to_string_t(U("LineCrossing")))
        return AnalyticsType::eAnalyticsType::AnalyticsType_LINECROSSING;
    return {};
}

EnumUnderlyingType fromEnum(AnalyticsType::eAnalyticsType e)
{
    switch (e)
    {
    case AnalyticsType::eAnalyticsType::AnalyticsType_AREACROSSING:
        return U("AreaCrossing");
    case AnalyticsType::eAnalyticsType::AnalyticsType_AREAINTRUSION:
        return U("AreaIntrusion");
    case AnalyticsType::eAnalyticsType::AnalyticsType_AREALOITERING:
        return U("AreaLoitering");
    case AnalyticsType::eAnalyticsType::AnalyticsType_AREACROWDING:
        return U("AreaCrowding");
    case AnalyticsType::eAnalyticsType::AnalyticsType_AREAARMEDPERSON:
        return U("AreaArmedPerson");
    case AnalyticsType::eAnalyticsType::AnalyticsType_LINECROSSING:
        return U("LineCrossing");
    default:
        break;
    }
    return {};
}
}

AnalyticsType::AnalyticsType()
{
}

AnalyticsType::~AnalyticsType()
{
}

void AnalyticsType::validate()
{
    // TODO: implement validation
}

web::json::value AnalyticsType::toJson() const
{
    auto val = fromEnum(m_value);
    return web::json::value(val);
}

bool AnalyticsType::fromJson(const web::json::value& val)
{
    m_value = toEnum(val.as_string());
    return true;
}

void AnalyticsType::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if (!namePrefix.empty() && namePrefix.back() != U('.'))
    {
        namePrefix.push_back(U('.'));
    }

    auto e = fromEnum(m_value);
    multipart->add(ModelBase::toHttpContent(namePrefix, e));
}

bool AnalyticsType::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
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

AnalyticsType::eAnalyticsType AnalyticsType::getValue() const
{
   return m_value;
}

void AnalyticsType::setValue(AnalyticsType::eAnalyticsType const value)
{
   m_value = value;
}


}
}
}
}
}


