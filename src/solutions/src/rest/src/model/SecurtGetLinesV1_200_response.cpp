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



#include "securt_rest_client/model/SecurtGetLinesV1_200_response.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



SecurtGetLinesV1_200_response::SecurtGetLinesV1_200_response()
{
    m_crossingLinesIsSet = false;
}

SecurtGetLinesV1_200_response::~SecurtGetLinesV1_200_response()
{
}

void SecurtGetLinesV1_200_response::validate()
{
    // TODO: implement validation
}

web::json::value SecurtGetLinesV1_200_response::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_crossingLinesIsSet)
    {
        val[utility::conversions::to_string_t(U("crossingLines"))] = ModelBase::toJson(m_crossingLines);
    }

    return val;
}

bool SecurtGetLinesV1_200_response::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("crossingLines"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("crossingLines")));
        if(!fieldValue.is_null())
        {
            std::vector<std::shared_ptr<LineCrossingRead>> refVal_setCrossingLines;
            ok &= ModelBase::fromJson(fieldValue, refVal_setCrossingLines);
            setCrossingLines(refVal_setCrossingLines);
        }
    }
    return ok;
}

void SecurtGetLinesV1_200_response::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_crossingLinesIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("crossingLines")), m_crossingLines));
    }
}

bool SecurtGetLinesV1_200_response::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("crossingLines"))))
    {
        std::vector<std::shared_ptr<LineCrossingRead>> refVal_setCrossingLines;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("crossingLines"))), refVal_setCrossingLines );
        setCrossingLines(refVal_setCrossingLines);
    }
    return ok;
}

std::vector<std::shared_ptr<LineCrossingRead>>& SecurtGetLinesV1_200_response::getCrossingLines()
{
    return m_crossingLines;
}

void SecurtGetLinesV1_200_response::setCrossingLines(const std::vector<std::shared_ptr<LineCrossingRead>>& value)
{
    m_crossingLines = value;
    m_crossingLinesIsSet = true;
}

bool SecurtGetLinesV1_200_response::crossingLinesIsSet() const
{
    return m_crossingLinesIsSet;
}

void SecurtGetLinesV1_200_response::unsetcrossingLines()
{
    m_crossingLinesIsSet = false;
}
}
}
}
}
}


