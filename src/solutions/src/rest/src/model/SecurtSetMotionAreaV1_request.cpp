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



#include "rt_rest_client/model/SecurtSetMotionAreaV1_request.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



SecurtSetMotionAreaV1_request::SecurtSetMotionAreaV1_request()
{
    m_coordinatesIsSet = false;
}

SecurtSetMotionAreaV1_request::~SecurtSetMotionAreaV1_request()
{
}

void SecurtSetMotionAreaV1_request::validate()
{
    // TODO: implement validation
}

web::json::value SecurtSetMotionAreaV1_request::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_coordinatesIsSet)
    {
        val[utility::conversions::to_string_t(U("coordinates"))] = ModelBase::toJson(m_coordinates);
    }

    return val;
}

bool SecurtSetMotionAreaV1_request::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("coordinates"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("coordinates")));
        if(!fieldValue.is_null())
        {
            std::vector<std::shared_ptr<Coordinate>> refVal_setCoordinates;
            ok &= ModelBase::fromJson(fieldValue, refVal_setCoordinates);
            setCoordinates(refVal_setCoordinates);
        }
    }
    return ok;
}

void SecurtSetMotionAreaV1_request::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_coordinatesIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("coordinates")), m_coordinates));
    }
}

bool SecurtSetMotionAreaV1_request::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("coordinates"))))
    {
        std::vector<std::shared_ptr<Coordinate>> refVal_setCoordinates;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("coordinates"))), refVal_setCoordinates );
        setCoordinates(refVal_setCoordinates);
    }
    return ok;
}

std::vector<std::shared_ptr<Coordinate>>& SecurtSetMotionAreaV1_request::getCoordinates()
{
    return m_coordinates;
}

void SecurtSetMotionAreaV1_request::setCoordinates(const std::vector<std::shared_ptr<Coordinate>>& value)
{
    m_coordinates = value;
    m_coordinatesIsSet = true;
}

bool SecurtSetMotionAreaV1_request::coordinatesIsSet() const
{
    return m_coordinatesIsSet;
}

void SecurtSetMotionAreaV1_request::unsetcoordinates()
{
    m_coordinatesIsSet = false;
}
}
}
}
}
}


