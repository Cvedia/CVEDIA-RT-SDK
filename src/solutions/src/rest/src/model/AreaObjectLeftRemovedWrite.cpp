/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 2024.2.2
 * Contact: support@cvedia.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */



#include "rt_rest_client/model/AreaObjectLeftRemovedWrite.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



AreaObjectLeftRemovedWrite::AreaObjectLeftRemovedWrite()
{
    m_name = utility::conversions::to_string_t("");
    m_nameIsSet = false;
    m_coordinatesIsSet = false;
    m_seconds = 0;
    m_secondsIsSet = false;
    m_colorIsSet = false;
}

AreaObjectLeftRemovedWrite::~AreaObjectLeftRemovedWrite()
{
}

void AreaObjectLeftRemovedWrite::validate()
{
    // TODO: implement validation
}

web::json::value AreaObjectLeftRemovedWrite::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_nameIsSet)
    {
        val[utility::conversions::to_string_t(U("name"))] = ModelBase::toJson(m_name);
    }
    if(m_coordinatesIsSet)
    {
        val[utility::conversions::to_string_t(U("coordinates"))] = ModelBase::toJson(m_coordinates);
    }
    if(m_secondsIsSet)
    {
        val[utility::conversions::to_string_t(U("seconds"))] = ModelBase::toJson(m_seconds);
    }
    if(m_colorIsSet)
    {
        val[utility::conversions::to_string_t(U("color"))] = ModelBase::toJson(m_color);
    }

    return val;
}

bool AreaObjectLeftRemovedWrite::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("name"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("name")));
        if(!fieldValue.is_null())
        {
            utility::string_t refVal_setName;
            ok &= ModelBase::fromJson(fieldValue, refVal_setName);
            setName(refVal_setName);
        }
    }
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
    if(val.has_field(utility::conversions::to_string_t(U("seconds"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("seconds")));
        if(!fieldValue.is_null())
        {
            int32_t refVal_setSeconds;
            ok &= ModelBase::fromJson(fieldValue, refVal_setSeconds);
            setSeconds(refVal_setSeconds);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("color"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("color")));
        if(!fieldValue.is_null())
        {
            std::vector<double> refVal_setColor;
            ok &= ModelBase::fromJson(fieldValue, refVal_setColor);
            setColor(refVal_setColor);
        }
    }
    return ok;
}

void AreaObjectLeftRemovedWrite::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_nameIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("name")), m_name));
    }
    if(m_coordinatesIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("coordinates")), m_coordinates));
    }
    if(m_secondsIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("seconds")), m_seconds));
    }
    if(m_colorIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("color")), m_color));
    }
}

bool AreaObjectLeftRemovedWrite::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("name"))))
    {
        utility::string_t refVal_setName;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("name"))), refVal_setName );
        setName(refVal_setName);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("coordinates"))))
    {
        std::vector<std::shared_ptr<Coordinate>> refVal_setCoordinates;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("coordinates"))), refVal_setCoordinates );
        setCoordinates(refVal_setCoordinates);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("seconds"))))
    {
        int32_t refVal_setSeconds;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("seconds"))), refVal_setSeconds );
        setSeconds(refVal_setSeconds);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("color"))))
    {
        std::vector<double> refVal_setColor;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("color"))), refVal_setColor );
        setColor(refVal_setColor);
    }
    return ok;
}

utility::string_t AreaObjectLeftRemovedWrite::getName() const
{
    return m_name;
}

void AreaObjectLeftRemovedWrite::setName(const utility::string_t& value)
{
    m_name = value;
    m_nameIsSet = true;
}

bool AreaObjectLeftRemovedWrite::nameIsSet() const
{
    return m_nameIsSet;
}

void AreaObjectLeftRemovedWrite::unsetname()
{
    m_nameIsSet = false;
}
std::vector<std::shared_ptr<Coordinate>>& AreaObjectLeftRemovedWrite::getCoordinates()
{
    return m_coordinates;
}

void AreaObjectLeftRemovedWrite::setCoordinates(const std::vector<std::shared_ptr<Coordinate>>& value)
{
    m_coordinates = value;
    m_coordinatesIsSet = true;
}

bool AreaObjectLeftRemovedWrite::coordinatesIsSet() const
{
    return m_coordinatesIsSet;
}

void AreaObjectLeftRemovedWrite::unsetcoordinates()
{
    m_coordinatesIsSet = false;
}
int32_t AreaObjectLeftRemovedWrite::getSeconds() const
{
    return m_seconds;
}

void AreaObjectLeftRemovedWrite::setSeconds(int32_t value)
{
    m_seconds = value;
    m_secondsIsSet = true;
}

bool AreaObjectLeftRemovedWrite::secondsIsSet() const
{
    return m_secondsIsSet;
}

void AreaObjectLeftRemovedWrite::unsetseconds()
{
    m_secondsIsSet = false;
}
std::vector<double>& AreaObjectLeftRemovedWrite::getColor()
{
    return m_color;
}

void AreaObjectLeftRemovedWrite::setColor(std::vector<double> value)
{
    m_color = value;
    m_colorIsSet = true;
}

bool AreaObjectLeftRemovedWrite::colorIsSet() const
{
    return m_colorIsSet;
}

void AreaObjectLeftRemovedWrite::unsetcolor()
{
    m_colorIsSet = false;
}
}
}
}
}
}


