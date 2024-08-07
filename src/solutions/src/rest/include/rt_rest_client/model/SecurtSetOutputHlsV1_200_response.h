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
 * SecurtSetOutputHlsV1_200_response.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_SecurtSetOutputHlsV1_200_response_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_SecurtSetOutputHlsV1_200_response_H_


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
class EXPORT_CLASS  SecurtSetOutputHlsV1_200_response
    : public ModelBase
{
public:
    EXPORT SecurtSetOutputHlsV1_200_response();
    EXPORT virtual ~SecurtSetOutputHlsV1_200_response();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// SecurtSetOutputHlsV1_200_response members

    /// <summary>
    /// 
    /// </summary>
    EXPORT utility::string_t getUri() const;
    EXPORT bool uriIsSet() const;
    EXPORT void unseturi();

    EXPORT void setUri(const utility::string_t& value);


protected:
    utility::string_t m_uri;
    bool m_uriIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_SecurtSetOutputHlsV1_200_response_H_ */
