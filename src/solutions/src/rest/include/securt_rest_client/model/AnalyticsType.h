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
 * AnalyticsType.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_AnalyticsType_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_AnalyticsType_H_


#include "defines.h"
#include "securt_rest_client/ModelBase.h"


namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {

class EXPORT_CLASS  AnalyticsType
    : public ModelBase
{
public:
    EXPORT AnalyticsType();
    EXPORT virtual ~AnalyticsType();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    enum class eAnalyticsType
    {
        INVALID_VALUE_OPENAPI_GENERATED = 0,
        AnalyticsType_AREACROSSING,
        AnalyticsType_AREAINTRUSION,
        AnalyticsType_AREALOITERING,
        AnalyticsType_AREACROWDING,
        AnalyticsType_AREAARMEDPERSON,
        AnalyticsType_LINECROSSING,
    };

    EXPORT eAnalyticsType getValue() const;
    EXPORT void setValue(eAnalyticsType const value);

    protected:
        eAnalyticsType m_value;
};

}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_AnalyticsType_H_ */