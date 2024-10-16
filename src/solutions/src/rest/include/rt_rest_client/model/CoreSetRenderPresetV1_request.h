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

/*
 * CoreSetRenderPresetV1_request.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_CoreSetRenderPresetV1_request_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_CoreSetRenderPresetV1_request_H_


#include "defines.h"
#include "rt_rest_client/ModelBase.h"

#include <cpprest/details/basic_types.h>

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {


/// <summary>
/// 
/// </summary>
class EXPORT_CLASS  CoreSetRenderPresetV1_request
    : public ModelBase
{
public:
    EXPORT CoreSetRenderPresetV1_request();
    EXPORT virtual ~CoreSetRenderPresetV1_request();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// CoreSetRenderPresetV1_request members

    /// <summary>
    /// 
    /// </summary>
    EXPORT utility::string_t getPresetName() const;
    EXPORT bool presetNameIsSet() const;
    EXPORT void unsetpresetName();

    EXPORT void setPresetName(const utility::string_t& value);


protected:
    utility::string_t m_presetName;
    bool m_presetNameIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_CoreSetRenderPresetV1_request_H_ */
