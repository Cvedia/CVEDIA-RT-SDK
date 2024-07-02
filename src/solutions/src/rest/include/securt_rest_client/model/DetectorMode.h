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
 * DetectorMode.h
 *
 * Enumerated types of detector modes.
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_DetectorMode_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_DetectorMode_H_


#include "defines.h"
#include "securt_rest_client/ModelBase.h"


namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {

class EXPORT_CLASS  DetectorMode
    : public ModelBase
{
public:
    EXPORT DetectorMode();
    EXPORT virtual ~DetectorMode();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    enum class eDetectorMode
    {
        INVALID_VALUE_OPENAPI_GENERATED = 0,
        DetectorMode_SMARTDETECTION,
        DetectorMode_DETECTION,
    };

    EXPORT eDetectorMode getValue() const;
    EXPORT void setValue(eDetectorMode const value);

    protected:
        eDetectorMode m_value;
};

}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_DetectorMode_H_ */