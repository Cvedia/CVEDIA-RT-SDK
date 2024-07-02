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



#include "securt_rest_client/model/AreaCrowdingRead.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



AreaCrowdingRead::AreaCrowdingRead()
{
    m_id = utility::conversions::to_string_t("");
    m_idIsSet = false;
    m_name = utility::conversions::to_string_t("");
    m_nameIsSet = false;
    m_coordinatesIsSet = false;
    m_classesIsSet = false;
    m_objectCount = 0;
    m_objectCountIsSet = false;
    m_colorIsSet = false;
}

AreaCrowdingRead::~AreaCrowdingRead()
{
}

void AreaCrowdingRead::validate()
{
    // TODO: implement validation
}

web::json::value AreaCrowdingRead::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_idIsSet)
    {
        val[utility::conversions::to_string_t(U("id"))] = ModelBase::toJson(m_id);
    }
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

bool AreaCrowdingRead::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("id"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("id")));
        if(!fieldValue.is_null())
        {
            utility::string_t refVal_setId;
            ok &= ModelBase::fromJson(fieldValue, refVal_setId);
            setId(refVal_setId);
        }
    }
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

void AreaCrowdingRead::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_idIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("id")), m_id));
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

bool AreaCrowdingRead::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("id"))))
    {
        utility::string_t refVal_setId;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("id"))), refVal_setId );
        setId(refVal_setId);
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

utility::string_t AreaCrowdingRead::getId() const
{
    return m_id;
}

void AreaCrowdingRead::setId(const utility::string_t& value)
{
    m_id = value;
    m_idIsSet = true;
}

bool AreaCrowdingRead::idIsSet() const
{
    return m_idIsSet;
}

void AreaCrowdingRead::unsetid()
{
    m_idIsSet = false;
}
utility::string_t AreaCrowdingRead::getName() const
{
    return m_name;
}

void AreaCrowdingRead::setName(const utility::string_t& value)
{
    m_name = value;
    m_nameIsSet = true;
}

bool AreaCrowdingRead::nameIsSet() const
{
    return m_nameIsSet;
}

void AreaCrowdingRead::unsetname()
{
    m_nameIsSet = false;
}
std::vector<std::shared_ptr<Coordinate>>& AreaCrowdingRead::getCoordinates()
{
    return m_coordinates;
}

void AreaCrowdingRead::setCoordinates(const std::vector<std::shared_ptr<Coordinate>>& value)
{
    m_coordinates = value;
    m_coordinatesIsSet = true;
}

bool AreaCrowdingRead::coordinatesIsSet() const
{
    return m_coordinatesIsSet;
}

void AreaCrowdingRead::unsetcoordinates()
{
    m_coordinatesIsSet = false;
}
std::vector<std::shared_ptr<Class>>& AreaCrowdingRead::getClasses()
{
    return m_classes;
}

void AreaCrowdingRead::setClasses(const std::vector<std::shared_ptr<Class>>& value)
{
    m_classes = value;
    m_classesIsSet = true;
}

bool AreaCrowdingRead::classesIsSet() const
{
    return m_classesIsSet;
}

void AreaCrowdingRead::unsetclasses()
{
    m_classesIsSet = false;
}
int32_t AreaCrowdingRead::getObjectCount() const
{
    return m_objectCount;
}

void AreaCrowdingRead::setObjectCount(int32_t value)
{
    m_objectCount = value;
    m_objectCountIsSet = true;
}

bool AreaCrowdingRead::objectCountIsSet() const
{
    return m_objectCountIsSet;
}

void AreaCrowdingRead::unsetobjectCount()
{
    m_objectCountIsSet = false;
}
std::vector<double>& AreaCrowdingRead::getColor()
{
    return m_color;
}

void AreaCrowdingRead::setColor(std::vector<double> value)
{
    m_color = value;
    m_colorIsSet = true;
}

bool AreaCrowdingRead::colorIsSet() const
{
    return m_colorIsSet;
}

void AreaCrowdingRead::unsetcolor()
{
    m_colorIsSet = false;
}
}
}
}
}
}

