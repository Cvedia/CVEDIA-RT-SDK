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

/*
 * CoreGetSolutionsV1_200_response.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_CoreGetSolutionsV1_200_response_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_CoreGetSolutionsV1_200_response_H_


#include "defines.h"
#include "rt_rest_client/ModelBase.h"

#include "rt_rest_client/model/CoreGetSolutionsV1_200_response_solutions.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {

class CoreGetSolutionsV1_200_response_solutions;

/// <summary>
/// 
/// </summary>
class EXPORT_CLASS  CoreGetSolutionsV1_200_response
    : public ModelBase
{
public:
    EXPORT CoreGetSolutionsV1_200_response();
    EXPORT virtual ~CoreGetSolutionsV1_200_response();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// CoreGetSolutionsV1_200_response members

    /// <summary>
    /// 
    /// </summary>
    EXPORT std::shared_ptr<CoreGetSolutionsV1_200_response_solutions> getSolutions() const;
    EXPORT bool solutionsIsSet() const;
    EXPORT void unsetsolutions();

    EXPORT void setSolutions(const std::shared_ptr<CoreGetSolutionsV1_200_response_solutions>& value);


protected:
    std::shared_ptr<CoreGetSolutionsV1_200_response_solutions> m_solutions;
    bool m_solutionsIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_CoreGetSolutionsV1_200_response_H_ */
