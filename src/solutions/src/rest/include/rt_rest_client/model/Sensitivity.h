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

/*
 * Sensitivity.h
 *
 * Enumerated types of sensitivity levels.
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_Sensitivity_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_Sensitivity_H_


#include "defines.h"
#include "rt_rest_client/ModelBase.h"


namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {

class EXPORT_CLASS  Sensitivity
    : public ModelBase
{
public:
    EXPORT Sensitivity();
    EXPORT virtual ~Sensitivity();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    enum class eSensitivity
    {
        INVALID_VALUE_OPENAPI_GENERATED = 0,
        Sensitivity_LOW,
        Sensitivity_MEDIUM,
        Sensitivity_HIGH,
    };

    EXPORT eSensitivity getValue() const;
    EXPORT void setValue(eSensitivity const value);

    protected:
        eSensitivity m_value;
};

}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_Sensitivity_H_ */
