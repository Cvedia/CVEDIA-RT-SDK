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
 * SensorModality.h
 *
 * Enumerated types of sensor modality.
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_SensorModality_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_SensorModality_H_


#include "defines.h"
#include "rt_rest_client/ModelBase.h"


namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {

class EXPORT_CLASS  SensorModality
    : public ModelBase
{
public:
    EXPORT SensorModality();
    EXPORT virtual ~SensorModality();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    enum class eSensorModality
    {
        INVALID_VALUE_OPENAPI_GENERATED = 0,
        SensorModality_RGB,
        SensorModality_THERMAL,
    };

    EXPORT eSensorModality getValue() const;
    EXPORT void setValue(eSensorModality const value);

    protected:
        eSensorModality m_value;
};

}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_SensorModality_H_ */
