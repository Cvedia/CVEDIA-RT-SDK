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
 * SecurtSetMotionAreaV1_request.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_SecurtSetMotionAreaV1_request_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_SecurtSetMotionAreaV1_request_H_


#include "defines.h"
#include "rt_rest_client/ModelBase.h"

#include "rt_rest_client/model/Coordinate.h"
#include <vector>

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {

class Coordinate;

/// <summary>
/// 
/// </summary>
class EXPORT_CLASS  SecurtSetMotionAreaV1_request
    : public ModelBase
{
public:
    EXPORT SecurtSetMotionAreaV1_request();
    EXPORT virtual ~SecurtSetMotionAreaV1_request();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// SecurtSetMotionAreaV1_request members

    /// <summary>
    /// 
    /// </summary>
    EXPORT std::vector<std::shared_ptr<Coordinate>>& getCoordinates();
    EXPORT bool coordinatesIsSet() const;
    EXPORT void unsetcoordinates();

    EXPORT void setCoordinates(const std::vector<std::shared_ptr<Coordinate>>& value);


protected:
    std::vector<std::shared_ptr<Coordinate>> m_coordinates;
    bool m_coordinatesIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_SecurtSetMotionAreaV1_request_H_ */
