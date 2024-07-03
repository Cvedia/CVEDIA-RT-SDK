/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 0.1
 * Contact: support@cvedia.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

/*
 * SecurtCreateLineCrossingV1_201_response.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_SecurtCreateLineCrossingV1_201_response_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_SecurtCreateLineCrossingV1_201_response_H_


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
class EXPORT_CLASS  SecurtCreateLineCrossingV1_201_response
    : public ModelBase
{
public:
    EXPORT SecurtCreateLineCrossingV1_201_response();
    EXPORT virtual ~SecurtCreateLineCrossingV1_201_response();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// SecurtCreateLineCrossingV1_201_response members

    /// <summary>
    /// 
    /// </summary>
    EXPORT utility::string_t getAreaId() const;
    EXPORT bool areaIdIsSet() const;
    EXPORT void unsetareaId();

    EXPORT void setAreaId(const utility::string_t& value);


protected:
    utility::string_t m_areaId;
    bool m_areaIdIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_SecurtCreateLineCrossingV1_201_response_H_ */
