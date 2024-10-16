/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 2024.2.3
 * Contact: 
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

/*
 * LicenseInfo.h
 *
 * Model providing the details about a license on the system
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_LicenseInfo_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_LicenseInfo_H_


#include "defines.h"
#include "rt_rest_client/ModelBase.h"

#include <cpprest/details/basic_types.h>

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {


/// <summary>
/// Model providing the details about a license on the system
/// </summary>
class EXPORT_CLASS  LicenseInfo
    : public ModelBase
{
public:
    EXPORT LicenseInfo();
    EXPORT virtual ~LicenseInfo();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// LicenseInfo members

    /// <summary>
    /// 
    /// </summary>
    EXPORT utility::string_t getLicenseKey() const;
    EXPORT bool licenseKeyIsSet() const;
    EXPORT void unsetlicenseKey();

    EXPORT void setLicenseKey(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT utility::string_t getExpiryDate() const;
    EXPORT bool expiryDateIsSet() const;
    EXPORT void unsetexpiryDate();

    EXPORT void setExpiryDate(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT int32_t getMaxMachines() const;
    EXPORT bool maxMachinesIsSet() const;
    EXPORT void unsetmaxMachines();

    EXPORT void setMaxMachines(int32_t value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT int32_t getMaxStreams() const;
    EXPORT bool maxStreamsIsSet() const;
    EXPORT void unsetmaxStreams();

    EXPORT void setMaxStreams(int32_t value);


protected:
    utility::string_t m_licenseKey;
    bool m_licenseKeyIsSet;
    utility::string_t m_expiryDate;
    bool m_expiryDateIsSet;
    int32_t m_maxMachines;
    bool m_maxMachinesIsSet;
    int32_t m_maxStreams;
    bool m_maxStreamsIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_LicenseInfo_H_ */
