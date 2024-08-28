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



#include "rt_rest_client/model/AreaCrowdingWrite.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



AreaCrowdingWrite::AreaCrowdingWrite()
{
    m_name = utility::conversions::to_string_t("");
    m_nameIsSet = false;
    m_coordinatesIsSet = false;
    m_classesIsSet = false;
    m_objectCount = 0;
    m_objectCountIsSet = false;
    m_colorIsSet = false;
}

AreaCrowdingWrite::~AreaCrowdingWrite()
{
}

void AreaCrowdingWrite::validate()
{
    // TODO: implement validation
}

web::json::value AreaCrowdingWrite::toJson() const
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
    if(m_objectCountIsSet)
    {
        val[utility::conversions::to_string_t(U("objectCount"))] = ModelBase::toJson(m_objectCount);
    }
    if(m_colorIsSet)
    {
        val[utility::conversions::to_string_t(U("color"))] = ModelBase::toJson(m_color);
    }

    return val;
}

bool AreaCrowdingWrite::fromJson(const web::json::value& val)
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
    if(val.has_field(utility::conversions::to_string_t(U("objectCount"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("objectCount")));
        if(!fieldValue.is_null())
        {
            int32_t refVal_setObjectCount;
            ok &= ModelBase::fromJson(fieldValue, refVal_setObjectCount);
            setObjectCount(refVal_setObjectCount);
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

void AreaCrowdingWrite::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
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
    if(m_objectCountIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("objectCount")), m_objectCount));
    }
    if(m_colorIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("color")), m_color));
    }
}

bool AreaCrowdingWrite::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
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
    if(multipart->hasContent(utility::conversions::to_string_t(U("objectCount"))))
    {
        int32_t refVal_setObjectCount;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("objectCount"))), refVal_setObjectCount );
        setObjectCount(refVal_setObjectCount);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("color"))))
    {
        std::vector<double> refVal_setColor;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("color"))), refVal_setColor );
        setColor(refVal_setColor);
    }
    return ok;
}

utility::string_t AreaCrowdingWrite::getName() const
{
    return m_name;
}

void AreaCrowdingWrite::setName(const utility::string_t& value)
{
    m_name = value;
    m_nameIsSet = true;
}

bool AreaCrowdingWrite::nameIsSet() const
{
    return m_nameIsSet;
}

void AreaCrowdingWrite::unsetname()
{
    m_nameIsSet = false;
}
std::vector<std::shared_ptr<Coordinate>>& AreaCrowdingWrite::getCoordinates()
{
    return m_coordinates;
}

void AreaCrowdingWrite::setCoordinates(const std::vector<std::shared_ptr<Coordinate>>& value)
{
    m_coordinates = value;
    m_coordinatesIsSet = true;
}

bool AreaCrowdingWrite::coordinatesIsSet() const
{
    return m_coordinatesIsSet;
}

void AreaCrowdingWrite::unsetcoordinates()
{
    m_coordinatesIsSet = false;
}
std::vector<std::shared_ptr<Class>>& AreaCrowdingWrite::getClasses()
{
    return m_classes;
}

void AreaCrowdingWrite::setClasses(const std::vector<std::shared_ptr<Class>>& value)
{
    m_classes = value;
    m_classesIsSet = true;
}

bool AreaCrowdingWrite::classesIsSet() const
{
    return m_classesIsSet;
}

void AreaCrowdingWrite::unsetclasses()
{
    m_classesIsSet = false;
}
int32_t AreaCrowdingWrite::getObjectCount() const
{
    return m_objectCount;
}

void AreaCrowdingWrite::setObjectCount(int32_t value)
{
    m_objectCount = value;
    m_objectCountIsSet = true;
}

bool AreaCrowdingWrite::objectCountIsSet() const
{
    return m_objectCountIsSet;
}

void AreaCrowdingWrite::unsetobjectCount()
{
    m_objectCountIsSet = false;
}
std::vector<double>& AreaCrowdingWrite::getColor()
{
    return m_color;
}

void AreaCrowdingWrite::setColor(std::vector<double> value)
{
    m_color = value;
    m_colorIsSet = true;
}

bool AreaCrowdingWrite::colorIsSet() const
{
    return m_colorIsSet;
}

void AreaCrowdingWrite::unsetcolor()
{
    m_colorIsSet = false;
}
}
}
}
}
}


