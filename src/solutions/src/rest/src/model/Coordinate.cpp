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



#include "securt_rest_client/model/Coordinate.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



Coordinate::Coordinate()
{
    m_x = 0.0;
    m_xIsSet = false;
    m_y = 0.0;
    m_yIsSet = false;
}

Coordinate::~Coordinate()
{
}

void Coordinate::validate()
{
    // TODO: implement validation
}

web::json::value Coordinate::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_xIsSet)
    {
        val[utility::conversions::to_string_t(U("x"))] = ModelBase::toJson(m_x);
    }
    if(m_yIsSet)
    {
        val[utility::conversions::to_string_t(U("y"))] = ModelBase::toJson(m_y);
    }

    return val;
}

bool Coordinate::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("x"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("x")));
        if(!fieldValue.is_null())
        {
            double refVal_setX;
            ok &= ModelBase::fromJson(fieldValue, refVal_setX);
            setX(refVal_setX);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("y"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("y")));
        if(!fieldValue.is_null())
        {
            double refVal_setY;
            ok &= ModelBase::fromJson(fieldValue, refVal_setY);
            setY(refVal_setY);
        }
    }
    return ok;
}

void Coordinate::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_xIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("x")), m_x));
    }
    if(m_yIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("y")), m_y));
    }
}

bool Coordinate::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("x"))))
    {
        double refVal_setX;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("x"))), refVal_setX );
        setX(refVal_setX);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("y"))))
    {
        double refVal_setY;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("y"))), refVal_setY );
        setY(refVal_setY);
    }
    return ok;
}

double Coordinate::getX() const
{
    return m_x;
}

void Coordinate::setX(double value)
{
    m_x = value;
    m_xIsSet = true;
}

bool Coordinate::XIsSet() const
{
    return m_xIsSet;
}

void Coordinate::unsetx()
{
    m_xIsSet = false;
}
double Coordinate::getY() const
{
    return m_y;
}

void Coordinate::setY(double value)
{
    m_y = value;
    m_yIsSet = true;
}

bool Coordinate::YIsSet() const
{
    return m_yIsSet;
}

void Coordinate::unsety()
{
    m_yIsSet = false;
}
}
}
}
}
}

