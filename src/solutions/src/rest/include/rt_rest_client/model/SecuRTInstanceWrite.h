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
 * SecuRTInstanceWrite.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_SecuRTInstanceWrite_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_SecuRTInstanceWrite_H_


#include "defines.h"
#include "rt_rest_client/ModelBase.h"

#include "rt_rest_client/model/Sensitivity.h"
#include "rt_rest_client/model/SensorModality.h"
#include "rt_rest_client/model/DetectorMode.h"
#include <cpprest/details/basic_types.h>

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {


/// <summary>
/// 
/// </summary>
class EXPORT_CLASS  SecuRTInstanceWrite
    : public ModelBase
{
public:
    EXPORT SecuRTInstanceWrite();
    EXPORT virtual ~SecuRTInstanceWrite();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// SecuRTInstanceWrite members

    /// <summary>
    /// 
    /// </summary>
    EXPORT utility::string_t getName() const;
    EXPORT bool nameIsSet() const;
    EXPORT void unsetname();

    EXPORT void setName(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT std::shared_ptr<DetectorMode> getDetectorMode() const;
    EXPORT bool detectorModeIsSet() const;
    EXPORT void unsetdetectorMode();

    EXPORT void setDetectorMode(const std::shared_ptr<DetectorMode>& value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT std::shared_ptr<Sensitivity> getDetectionSensitivity() const;
    EXPORT bool detectionSensitivityIsSet() const;
    EXPORT void unsetdetectionSensitivity();

    EXPORT void setDetectionSensitivity(const std::shared_ptr<Sensitivity>& value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT std::shared_ptr<Sensitivity> getMovementSensitivity() const;
    EXPORT bool movementSensitivityIsSet() const;
    EXPORT void unsetmovementSensitivity();

    EXPORT void setMovementSensitivity(const std::shared_ptr<Sensitivity>& value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT std::shared_ptr<SensorModality> getSensorModality() const;
    EXPORT bool sensorModalityIsSet() const;
    EXPORT void unsetsensorModality();

    EXPORT void setSensorModality(const std::shared_ptr<SensorModality>& value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT double getFrameRateLimit() const;
    EXPORT bool frameRateLimitIsSet() const;
    EXPORT void unsetframeRateLimit();

    EXPORT void setFrameRateLimit(double value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT bool isMetadataMode() const;
    EXPORT bool metadataModeIsSet() const;
    EXPORT void unsetmetadataMode();

    EXPORT void setMetadataMode(bool value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT bool isStatisticsMode() const;
    EXPORT bool statisticsModeIsSet() const;
    EXPORT void unsetstatisticsMode();

    EXPORT void setStatisticsMode(bool value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT bool isDiagnosticsMode() const;
    EXPORT bool diagnosticsModeIsSet() const;
    EXPORT void unsetdiagnosticsMode();

    EXPORT void setDiagnosticsMode(bool value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT bool isForceRecreate() const;
    EXPORT bool forceRecreateIsSet() const;
    EXPORT void unsetforceRecreate();

    EXPORT void setForceRecreate(bool value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT bool isDebugMode() const;
    EXPORT bool debugModeIsSet() const;
    EXPORT void unsetdebugMode();

    EXPORT void setDebugMode(bool value);


protected:
    utility::string_t m_name;
    bool m_nameIsSet;
    std::shared_ptr<DetectorMode> m_detectorMode;
    bool m_detectorModeIsSet;
    std::shared_ptr<Sensitivity> m_detectionSensitivity;
    bool m_detectionSensitivityIsSet;
    std::shared_ptr<Sensitivity> m_movementSensitivity;
    bool m_movementSensitivityIsSet;
    std::shared_ptr<SensorModality> m_sensorModality;
    bool m_sensorModalityIsSet;
    double m_frameRateLimit;
    bool m_frameRateLimitIsSet;
    bool m_metadataMode;
    bool m_metadataModeIsSet;
    bool m_statisticsMode;
    bool m_statisticsModeIsSet;
    bool m_diagnosticsMode;
    bool m_diagnosticsModeIsSet;
    bool m_forceRecreate;
    bool m_forceRecreateIsSet;
    bool m_debugMode;
    bool m_debugModeIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_SecuRTInstanceWrite_H_ */