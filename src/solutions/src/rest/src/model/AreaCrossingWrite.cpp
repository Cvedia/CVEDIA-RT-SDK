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



#include "rt_rest_client/model/AreaCrossingWrite.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



AreaCrossingWrite::AreaCrossingWrite()
{
    m_name = utility::conversions::to_string_t("");
    m_nameIsSet = false;
    m_coordinatesIsSet = false;
    m_classesIsSet = false;
    m_ignoreStationaryObjects = false;
    m_ignoreStationaryObjectsIsSet = false;
    m_areaEventIsSet = false;
    m_colorIsSet = false;
}

AreaCrossingWrite::~AreaCrossingWrite()
{
}

void AreaCrossingWrite::validate()
{
    // TODO: implement validation
}

web::json::value AreaCrossingWrite::toJson() const
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
    if(m_ignoreStationaryObjectsIsSet)
    {
        val[utility::conversions::to_string_t(U("ignoreStationaryObjects"))] = ModelBase::toJson(m_ignoreStationaryObjects);
    }
    if(m_areaEventIsSet)
    {
        val[utility::conversions::to_string_t(U("areaEvent"))] = ModelBase::toJson(m_areaEvent);
    }
    if(m_colorIsSet)
    {
        val[utility::conversions::to_string_t(U("color"))] = ModelBase::toJson(m_color);
    }

    return val;
}

bool AreaCrossingWrite::fromJson(const web::json::value& val)
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
    if(val.has_field(utility::conversions::to_string_t(U("ignoreStationaryObjects"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("ignoreStationaryObjects")));
        if(!fieldValue.is_null())
        {
            bool refVal_setIgnoreStationaryObjects;
            ok &= ModelBase::fromJson(fieldValue, refVal_setIgnoreStationaryObjects);
            setIgnoreStationaryObjects(refVal_setIgnoreStationaryObjects);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("areaEvent"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("areaEvent")));
        if(!fieldValue.is_null())
        {
            std::shared_ptr<AreaEvent> refVal_setAreaEvent;
            ok &= ModelBase::fromJson(fieldValue, refVal_setAreaEvent);
            setAreaEvent(refVal_setAreaEvent);
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

void AreaCrossingWrite::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
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
    if(m_ignoreStationaryObjectsIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("ignoreStationaryObjects")), m_ignoreStationaryObjects));
    }
    if(m_areaEventIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("areaEvent")), m_areaEvent));
    }
    if(m_colorIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("color")), m_color));
    }
}

bool AreaCrossingWrite::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
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
    if(multipart->hasContent(utility::conversions::to_string_t(U("ignoreStationaryObjects"))))
    {
        bool refVal_setIgnoreStationaryObjects;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("ignoreStationaryObjects"))), refVal_setIgnoreStationaryObjects );
        setIgnoreStationaryObjects(refVal_setIgnoreStationaryObjects);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("areaEvent"))))
    {
        std::shared_ptr<AreaEvent> refVal_setAreaEvent;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("areaEvent"))), refVal_setAreaEvent );
        setAreaEvent(refVal_setAreaEvent);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("color"))))
    {
        std::vector<double> refVal_setColor;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("color"))), refVal_setColor );
        setColor(refVal_setColor);
    }
    return ok;
}

utility::string_t AreaCrossingWrite::getName() const
{
    return m_name;
}

void AreaCrossingWrite::setName(const utility::string_t& value)
{
    m_name = value;
    m_nameIsSet = true;
}

bool AreaCrossingWrite::nameIsSet() const
{
    return m_nameIsSet;
}

void AreaCrossingWrite::unsetname()
{
    m_nameIsSet = false;
}
std::vector<std::shared_ptr<Coordinate>>& AreaCrossingWrite::getCoordinates()
{
    return m_coordinates;
}

void AreaCrossingWrite::setCoordinates(const std::vector<std::shared_ptr<Coordinate>>& value)
{
    m_coordinates = value;
    m_coordinatesIsSet = true;
}

bool AreaCrossingWrite::coordinatesIsSet() const
{
    return m_coordinatesIsSet;
}

void AreaCrossingWrite::unsetcoordinates()
{
    m_coordinatesIsSet = false;
}
std::vector<std::shared_ptr<Class>>& AreaCrossingWrite::getClasses()
{
    return m_classes;
}

void AreaCrossingWrite::setClasses(const std::vector<std::shared_ptr<Class>>& value)
{
    m_classes = value;
    m_classesIsSet = true;
}

bool AreaCrossingWrite::classesIsSet() const
{
    return m_classesIsSet;
}

void AreaCrossingWrite::unsetclasses()
{
    m_classesIsSet = false;
}
bool AreaCrossingWrite::isIgnoreStationaryObjects() const
{
    return m_ignoreStationaryObjects;
}

void AreaCrossingWrite::setIgnoreStationaryObjects(bool value)
{
    m_ignoreStationaryObjects = value;
    m_ignoreStationaryObjectsIsSet = true;
}

bool AreaCrossingWrite::ignoreStationaryObjectsIsSet() const
{
    return m_ignoreStationaryObjectsIsSet;
}

void AreaCrossingWrite::unsetignoreStationaryObjects()
{
    m_ignoreStationaryObjectsIsSet = false;
}
std::shared_ptr<AreaEvent> AreaCrossingWrite::getAreaEvent() const
{
    return m_areaEvent;
}

void AreaCrossingWrite::setAreaEvent(const std::shared_ptr<AreaEvent>& value)
{
    m_areaEvent = value;
    m_areaEventIsSet = true;
}

bool AreaCrossingWrite::areaEventIsSet() const
{
    return m_areaEventIsSet;
}

void AreaCrossingWrite::unsetareaEvent()
{
    m_areaEventIsSet = false;
}
std::vector<double>& AreaCrossingWrite::getColor()
{
    return m_color;
}

void AreaCrossingWrite::setColor(std::vector<double> value)
{
    m_color = value;
    m_colorIsSet = true;
}

bool AreaCrossingWrite::colorIsSet() const
{
    return m_colorIsSet;
}

void AreaCrossingWrite::unsetcolor()
{
    m_colorIsSet = false;
}
}
}
}
}
}


