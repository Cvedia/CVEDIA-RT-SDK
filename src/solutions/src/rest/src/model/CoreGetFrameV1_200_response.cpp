/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 2024.2.3
 * Contact: 
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */



#include "rt_rest_client/model/CoreGetFrameV1_200_response.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



CoreGetFrameV1_200_response::CoreGetFrameV1_200_response()
{
    m_frame = utility::conversions::to_string_t("");
    m_frameIsSet = false;
    m_running = false;
    m_runningIsSet = false;
}

CoreGetFrameV1_200_response::~CoreGetFrameV1_200_response()
{
}

void CoreGetFrameV1_200_response::validate()
{
    // TODO: implement validation
}

web::json::value CoreGetFrameV1_200_response::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_frameIsSet)
    {
        val[utility::conversions::to_string_t(U("frame"))] = ModelBase::toJson(m_frame);
    }
    if(m_runningIsSet)
    {
        val[utility::conversions::to_string_t(U("running"))] = ModelBase::toJson(m_running);
    }

    return val;
}

bool CoreGetFrameV1_200_response::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("frame"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("frame")));
        if(!fieldValue.is_null())
        {
            utility::string_t refVal_setFrame;
            ok &= ModelBase::fromJson(fieldValue, refVal_setFrame);
            setFrame(refVal_setFrame);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("running"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("running")));
        if(!fieldValue.is_null())
        {
            bool refVal_setRunning;
            ok &= ModelBase::fromJson(fieldValue, refVal_setRunning);
            setRunning(refVal_setRunning);
        }
    }
    return ok;
}

void CoreGetFrameV1_200_response::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_frameIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("frame")), m_frame));
    }
    if(m_runningIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("running")), m_running));
    }
}

bool CoreGetFrameV1_200_response::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("frame"))))
    {
        utility::string_t refVal_setFrame;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("frame"))), refVal_setFrame );
        setFrame(refVal_setFrame);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("running"))))
    {
        bool refVal_setRunning;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("running"))), refVal_setRunning );
        setRunning(refVal_setRunning);
    }
    return ok;
}

utility::string_t CoreGetFrameV1_200_response::getFrame() const
{
    return m_frame;
}

void CoreGetFrameV1_200_response::setFrame(const utility::string_t& value)
{
    m_frame = value;
    m_frameIsSet = true;
}

bool CoreGetFrameV1_200_response::frameIsSet() const
{
    return m_frameIsSet;
}

void CoreGetFrameV1_200_response::unsetframe()
{
    m_frameIsSet = false;
}
bool CoreGetFrameV1_200_response::isRunning() const
{
    return m_running;
}

void CoreGetFrameV1_200_response::setRunning(bool value)
{
    m_running = value;
    m_runningIsSet = true;
}

bool CoreGetFrameV1_200_response::runningIsSet() const
{
    return m_runningIsSet;
}

void CoreGetFrameV1_200_response::unsetrunning()
{
    m_runningIsSet = false;
}
}
}
}
}
}


