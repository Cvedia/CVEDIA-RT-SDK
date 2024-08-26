/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 2024.2.1
 * Contact: support@cvedia.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

/*
 * CorePutSystemConfigV1_request_systemConfig_inner.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_CorePutSystemConfigV1_request_systemConfig_inner_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_CorePutSystemConfigV1_request_systemConfig_inner_H_


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
class EXPORT_CLASS  CorePutSystemConfigV1_request_systemConfig_inner
    : public ModelBase
{
public:
    EXPORT CorePutSystemConfigV1_request_systemConfig_inner();
    EXPORT virtual ~CorePutSystemConfigV1_request_systemConfig_inner();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// CorePutSystemConfigV1_request_systemConfig_inner members

    /// <summary>
    /// 
    /// </summary>
    EXPORT utility::string_t getFieldId() const;
    EXPORT bool fieldIdIsSet() const;
    EXPORT void unsetfieldId();

    EXPORT void setFieldId(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT utility::string_t getValue() const;
    EXPORT bool valueIsSet() const;
    EXPORT void unsetvalue();

    EXPORT void setValue(const utility::string_t& value);


protected:
    utility::string_t m_fieldId;
    bool m_fieldIdIsSet;
    utility::string_t m_value;
    bool m_valueIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_CorePutSystemConfigV1_request_systemConfig_inner_H_ */
