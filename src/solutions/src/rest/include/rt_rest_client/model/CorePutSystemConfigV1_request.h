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
 * CorePutSystemConfigV1_request.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_CorePutSystemConfigV1_request_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_CorePutSystemConfigV1_request_H_


#include "defines.h"
#include "rt_rest_client/ModelBase.h"

#include "rt_rest_client/model/CorePutSystemConfigV1_request_systemConfig_inner.h"
#include <vector>

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {

class CorePutSystemConfigV1_request_systemConfig_inner;

/// <summary>
/// 
/// </summary>
class EXPORT_CLASS  CorePutSystemConfigV1_request
    : public ModelBase
{
public:
    EXPORT CorePutSystemConfigV1_request();
    EXPORT virtual ~CorePutSystemConfigV1_request();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// CorePutSystemConfigV1_request members

    /// <summary>
    /// 
    /// </summary>
    EXPORT std::vector<std::shared_ptr<CorePutSystemConfigV1_request_systemConfig_inner>>& getSystemConfig();
    EXPORT bool systemConfigIsSet() const;
    EXPORT void unsetsystemConfig();

    EXPORT void setSystemConfig(const std::vector<std::shared_ptr<CorePutSystemConfigV1_request_systemConfig_inner>>& value);


protected:
    std::vector<std::shared_ptr<CorePutSystemConfigV1_request_systemConfig_inner>> m_systemConfig;
    bool m_systemConfigIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_CorePutSystemConfigV1_request_H_ */
