/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 2024.2.0
 * Contact: support@cvedia.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */



#include "rt_rest_client/model/CoreGetSolutionsV1_200_response.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



CoreGetSolutionsV1_200_response::CoreGetSolutionsV1_200_response()
{
    m_solutionsIsSet = false;
}

CoreGetSolutionsV1_200_response::~CoreGetSolutionsV1_200_response()
{
}

void CoreGetSolutionsV1_200_response::validate()
{
    // TODO: implement validation
}

web::json::value CoreGetSolutionsV1_200_response::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_solutionsIsSet)
    {
        val[utility::conversions::to_string_t(U("solutions"))] = ModelBase::toJson(m_solutions);
    }

    return val;
}

bool CoreGetSolutionsV1_200_response::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("solutions"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("solutions")));
        if(!fieldValue.is_null())
        {
            std::shared_ptr<CoreGetSolutionsV1_200_response_solutions> refVal_setSolutions;
            ok &= ModelBase::fromJson(fieldValue, refVal_setSolutions);
            setSolutions(refVal_setSolutions);
        }
    }
    return ok;
}

void CoreGetSolutionsV1_200_response::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_solutionsIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("solutions")), m_solutions));
    }
}

bool CoreGetSolutionsV1_200_response::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("solutions"))))
    {
        std::shared_ptr<CoreGetSolutionsV1_200_response_solutions> refVal_setSolutions;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("solutions"))), refVal_setSolutions );
        setSolutions(refVal_setSolutions);
    }
    return ok;
}

std::shared_ptr<CoreGetSolutionsV1_200_response_solutions> CoreGetSolutionsV1_200_response::getSolutions() const
{
    return m_solutions;
}

void CoreGetSolutionsV1_200_response::setSolutions(const std::shared_ptr<CoreGetSolutionsV1_200_response_solutions>& value)
{
    m_solutions = value;
    m_solutionsIsSet = true;
}

bool CoreGetSolutionsV1_200_response::solutionsIsSet() const
{
    return m_solutionsIsSet;
}

void CoreGetSolutionsV1_200_response::unsetsolutions()
{
    m_solutionsIsSet = false;
}
}
}
}
}
}


