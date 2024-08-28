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



#include "rt_rest_client/model/LineCrossingWrite.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



LineCrossingWrite::LineCrossingWrite()
{
    m_name = utility::conversions::to_string_t("");
    m_nameIsSet = false;
    m_coordinatesIsSet = false;
    m_classesIsSet = false;
    m_directionIsSet = false;
    m_colorIsSet = false;
}

LineCrossingWrite::~LineCrossingWrite()
{
}

void LineCrossingWrite::validate()
{
    // TODO: implement validation
}

web::json::value LineCrossingWrite::toJson() const
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
    if(m_classesIsSet)
    {
        val[utility::conversions::to_string_t(U("classes"))] = ModelBase::toJson(m_classes);
    }
    if(m_directionIsSet)
    {
        val[utility::conversions::to_string_t(U("direction"))] = ModelBase::toJson(m_direction);
    }
    if(m_colorIsSet)
    {
        val[utility::conversions::to_string_t(U("color"))] = ModelBase::toJson(m_color);
    }

    return val;
}

bool LineCrossingWrite::fromJson(const web::json::value& val)
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
    if(val.has_field(utility::conversions::to_string_t(U("classes"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("classes")));
        if(!fieldValue.is_null())
        {
            std::vector<std::shared_ptr<Class>> refVal_setClasses;
            ok &= ModelBase::fromJson(fieldValue, refVal_setClasses);
            setClasses(refVal_setClasses);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("direction"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("direction")));
        if(!fieldValue.is_null())
        {
            std::shared_ptr<Direction> refVal_setDirection;
            ok &= ModelBase::fromJson(fieldValue, refVal_setDirection);
            setDirection(refVal_setDirection);
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

void LineCrossingWrite::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
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
    if(m_classesIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("classes")), m_classes));
    }
    if(m_directionIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("direction")), m_direction));
    }
    if(m_colorIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("color")), m_color));
    }
}

bool LineCrossingWrite::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
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
    if(multipart->hasContent(utility::conversions::to_string_t(U("classes"))))
    {
        std::vector<std::shared_ptr<Class>> refVal_setClasses;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("classes"))), refVal_setClasses );
        setClasses(refVal_setClasses);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("direction"))))
    {
        std::shared_ptr<Direction> refVal_setDirection;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("direction"))), refVal_setDirection );
        setDirection(refVal_setDirection);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("color"))))
    {
        std::vector<double> refVal_setColor;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("color"))), refVal_setColor );
        setColor(refVal_setColor);
    }
    return ok;
}

utility::string_t LineCrossingWrite::getName() const
{
    return m_name;
}

void LineCrossingWrite::setName(const utility::string_t& value)
{
    m_name = value;
    m_nameIsSet = true;
}

bool LineCrossingWrite::nameIsSet() const
{
    return m_nameIsSet;
}

void LineCrossingWrite::unsetname()
{
    m_nameIsSet = false;
}
std::vector<std::shared_ptr<Coordinate>>& LineCrossingWrite::getCoordinates()
{
    return m_coordinates;
}

void LineCrossingWrite::setCoordinates(const std::vector<std::shared_ptr<Coordinate>>& value)
{
    m_coordinates = value;
    m_coordinatesIsSet = true;
}

bool LineCrossingWrite::coordinatesIsSet() const
{
    return m_coordinatesIsSet;
}

void LineCrossingWrite::unsetcoordinates()
{
    m_coordinatesIsSet = false;
}
std::vector<std::shared_ptr<Class>>& LineCrossingWrite::getClasses()
{
    return m_classes;
}

void LineCrossingWrite::setClasses(const std::vector<std::shared_ptr<Class>>& value)
{
    m_classes = value;
    m_classesIsSet = true;
}

bool LineCrossingWrite::classesIsSet() const
{
    return m_classesIsSet;
}

void LineCrossingWrite::unsetclasses()
{
    m_classesIsSet = false;
}
std::shared_ptr<Direction> LineCrossingWrite::getDirection() const
{
    return m_direction;
}

void LineCrossingWrite::setDirection(const std::shared_ptr<Direction>& value)
{
    m_direction = value;
    m_directionIsSet = true;
}

bool LineCrossingWrite::directionIsSet() const
{
    return m_directionIsSet;
}

void LineCrossingWrite::unsetdirection()
{
    m_directionIsSet = false;
}
std::vector<double>& LineCrossingWrite::getColor()
{
    return m_color;
}

void LineCrossingWrite::setColor(std::vector<double> value)
{
    m_color = value;
    m_colorIsSet = true;
}

bool LineCrossingWrite::colorIsSet() const
{
    return m_colorIsSet;
}

void LineCrossingWrite::unsetcolor()
{
    m_colorIsSet = false;
}
}
}
}
}
}


