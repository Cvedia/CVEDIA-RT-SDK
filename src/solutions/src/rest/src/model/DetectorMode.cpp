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



#include "securt_rest_client/model/DetectorMode.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {


namespace
{
using EnumUnderlyingType = utility::string_t;

DetectorMode::eDetectorMode toEnum(const EnumUnderlyingType& val)
{
    if (val == utility::conversions::to_string_t(U("SmartDetection")))
        return DetectorMode::eDetectorMode::DetectorMode_SMARTDETECTION;
    if (val == utility::conversions::to_string_t(U("Detection")))
        return DetectorMode::eDetectorMode::DetectorMode_DETECTION;
    return {};
}

EnumUnderlyingType fromEnum(DetectorMode::eDetectorMode e)
{
    switch (e)
    {
    case DetectorMode::eDetectorMode::DetectorMode_SMARTDETECTION:
        return U("SmartDetection");
    case DetectorMode::eDetectorMode::DetectorMode_DETECTION:
        return U("Detection");
    default:
        break;
    }
    return {};
}
}

DetectorMode::DetectorMode()
{
}

DetectorMode::~DetectorMode()
{
}

void DetectorMode::validate()
{
    // TODO: implement validation
}

web::json::value DetectorMode::toJson() const
{
    auto val = fromEnum(m_value);
    return web::json::value(val);
}

bool DetectorMode::fromJson(const web::json::value& val)
{
    m_value = toEnum(val.as_string());
    return true;
}

void DetectorMode::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if (!namePrefix.empty() && namePrefix.back() != U('.'))
    {
        namePrefix.push_back(U('.'));
    }

    auto e = fromEnum(m_value);
    multipart->add(ModelBase::toHttpContent(namePrefix, e));
}

bool DetectorMode::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
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

DetectorMode::eDetectorMode DetectorMode::getValue() const
{
   return m_value;
}

void DetectorMode::setValue(DetectorMode::eDetectorMode const value)
{
   m_value = value;
}


}
}
}
}
}


