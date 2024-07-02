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
 * CoreGetLicensesV1_200_response.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_CoreGetLicensesV1_200_response_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_CoreGetLicensesV1_200_response_H_


#include "defines.h"
#include "rt_rest_client/ModelBase.h"

#include <vector>
#include "rt_rest_client/model/LicenseInfo.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {

class LicenseInfo;

/// <summary>
/// 
/// </summary>
class EXPORT_CLASS  CoreGetLicensesV1_200_response
    : public ModelBase
{
public:
    EXPORT CoreGetLicensesV1_200_response();
    EXPORT virtual ~CoreGetLicensesV1_200_response();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// CoreGetLicensesV1_200_response members

    /// <summary>
    /// 
    /// </summary>
    EXPORT std::vector<std::shared_ptr<LicenseInfo>>& getLicenses();
    EXPORT bool licensesIsSet() const;
    EXPORT void unsetlicenses();

    EXPORT void setLicenses(const std::vector<std::shared_ptr<LicenseInfo>>& value);


protected:
    std::vector<std::shared_ptr<LicenseInfo>> m_licenses;
    bool m_licensesIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_CoreGetLicensesV1_200_response_H_ */