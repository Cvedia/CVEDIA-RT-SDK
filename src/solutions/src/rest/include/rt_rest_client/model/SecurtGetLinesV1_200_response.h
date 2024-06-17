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
 * SecurtGetLinesV1_200_response.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_SecurtGetLinesV1_200_response_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_SecurtGetLinesV1_200_response_H_


#include "defines.h"
#include "rt_rest_client/ModelBase.h"

#include "rt_rest_client/model/LineCrossingRead.h"
#include <vector>

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {

class LineCrossingRead;

/// <summary>
/// 
/// </summary>
class EXPORT_CLASS  SecurtGetLinesV1_200_response
    : public ModelBase
{
public:
    EXPORT SecurtGetLinesV1_200_response();
    EXPORT virtual ~SecurtGetLinesV1_200_response();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// SecurtGetLinesV1_200_response members

    /// <summary>
    /// 
    /// </summary>
    EXPORT std::vector<std::shared_ptr<LineCrossingRead>>& getCrossingLines();
    EXPORT bool crossingLinesIsSet() const;
    EXPORT void unsetcrossingLines();

    EXPORT void setCrossingLines(const std::vector<std::shared_ptr<LineCrossingRead>>& value);


protected:
    std::vector<std::shared_ptr<LineCrossingRead>> m_crossingLines;
    bool m_crossingLinesIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_SecurtGetLinesV1_200_response_H_ */
