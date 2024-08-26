/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 2024.2.1
 * Contact: support@cvedia.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */



#include "rt_rest_client/model/SecuRTInstanceWrite.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



SecuRTInstanceWrite::SecuRTInstanceWrite()
{
    m_name = utility::conversions::to_string_t("");
    m_nameIsSet = false;
    m_detectorModeIsSet = false;
    m_detectionSensitivityIsSet = false;
    m_movementSensitivityIsSet = false;
    m_sensorModalityIsSet = false;
    m_frameRateLimit = 0.0;
    m_frameRateLimitIsSet = false;
    m_metadataMode = false;
    m_metadataModeIsSet = false;
    m_statisticsMode = false;
    m_statisticsModeIsSet = false;
    m_diagnosticsMode = false;
    m_diagnosticsModeIsSet = false;
    m_debugMode = false;
    m_debugModeIsSet = false;
}

SecuRTInstanceWrite::~SecuRTInstanceWrite()
{
}

void SecuRTInstanceWrite::validate()
{
    // TODO: implement validation
}

web::json::value SecuRTInstanceWrite::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_nameIsSet)
    {
        val[utility::conversions::to_string_t(U("name"))] = ModelBase::toJson(m_name);
    }
    if(m_detectorModeIsSet)
    {
        val[utility::conversions::to_string_t(U("detectorMode"))] = ModelBase::toJson(m_detectorMode);
    }
    if(m_detectionSensitivityIsSet)
    {
        val[utility::conversions::to_string_t(U("detectionSensitivity"))] = ModelBase::toJson(m_detectionSensitivity);
    }
    if(m_movementSensitivityIsSet)
    {
        val[utility::conversions::to_string_t(U("movementSensitivity"))] = ModelBase::toJson(m_movementSensitivity);
    }
    if(m_sensorModalityIsSet)
    {
        val[utility::conversions::to_string_t(U("sensorModality"))] = ModelBase::toJson(m_sensorModality);
    }
    if(m_frameRateLimitIsSet)
    {
        val[utility::conversions::to_string_t(U("frameRateLimit"))] = ModelBase::toJson(m_frameRateLimit);
    }
    if(m_metadataModeIsSet)
    {
        val[utility::conversions::to_string_t(U("metadataMode"))] = ModelBase::toJson(m_metadataMode);
    }
    if(m_statisticsModeIsSet)
    {
        val[utility::conversions::to_string_t(U("statisticsMode"))] = ModelBase::toJson(m_statisticsMode);
    }
    if(m_diagnosticsModeIsSet)
    {
        val[utility::conversions::to_string_t(U("diagnosticsMode"))] = ModelBase::toJson(m_diagnosticsMode);
    }
    if(m_debugModeIsSet)
    {
        val[utility::conversions::to_string_t(U("debugMode"))] = ModelBase::toJson(m_debugMode);
    }

    return val;
}

bool SecuRTInstanceWrite::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("name"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("name")));
        if(!fieldValue.is_null())
        {
            utility::string_t refVal_setName;
            ok &= ModelBase::fromJson(fieldValue, refVal_setName);
            setName(refVal_setName);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("detectorMode"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("detectorMode")));
        if(!fieldValue.is_null())
        {
            std::shared_ptr<DetectorMode> refVal_setDetectorMode;
            ok &= ModelBase::fromJson(fieldValue, refVal_setDetectorMode);
            setDetectorMode(refVal_setDetectorMode);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("detectionSensitivity"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("detectionSensitivity")));
        if(!fieldValue.is_null())
        {
            std::shared_ptr<Sensitivity> refVal_setDetectionSensitivity;
            ok &= ModelBase::fromJson(fieldValue, refVal_setDetectionSensitivity);
            setDetectionSensitivity(refVal_setDetectionSensitivity);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("movementSensitivity"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("movementSensitivity")));
        if(!fieldValue.is_null())
        {
            std::shared_ptr<Sensitivity> refVal_setMovementSensitivity;
            ok &= ModelBase::fromJson(fieldValue, refVal_setMovementSensitivity);
            setMovementSensitivity(refVal_setMovementSensitivity);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("sensorModality"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("sensorModality")));
        if(!fieldValue.is_null())
        {
            std::shared_ptr<SensorModality> refVal_setSensorModality;
            ok &= ModelBase::fromJson(fieldValue, refVal_setSensorModality);
            setSensorModality(refVal_setSensorModality);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("frameRateLimit"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("frameRateLimit")));
        if(!fieldValue.is_null())
        {
            double refVal_setFrameRateLimit;
            ok &= ModelBase::fromJson(fieldValue, refVal_setFrameRateLimit);
            setFrameRateLimit(refVal_setFrameRateLimit);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("metadataMode"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("metadataMode")));
        if(!fieldValue.is_null())
        {
            bool refVal_setMetadataMode;
            ok &= ModelBase::fromJson(fieldValue, refVal_setMetadataMode);
            setMetadataMode(refVal_setMetadataMode);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("statisticsMode"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("statisticsMode")));
        if(!fieldValue.is_null())
        {
            bool refVal_setStatisticsMode;
            ok &= ModelBase::fromJson(fieldValue, refVal_setStatisticsMode);
            setStatisticsMode(refVal_setStatisticsMode);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("diagnosticsMode"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("diagnosticsMode")));
        if(!fieldValue.is_null())
        {
            bool refVal_setDiagnosticsMode;
            ok &= ModelBase::fromJson(fieldValue, refVal_setDiagnosticsMode);
            setDiagnosticsMode(refVal_setDiagnosticsMode);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("debugMode"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("debugMode")));
        if(!fieldValue.is_null())
        {
            bool refVal_setDebugMode;
            ok &= ModelBase::fromJson(fieldValue, refVal_setDebugMode);
            setDebugMode(refVal_setDebugMode);
        }
    }
    return ok;
}

void SecuRTInstanceWrite::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_nameIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("name")), m_name));
    }
    if(m_detectorModeIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("detectorMode")), m_detectorMode));
    }
    if(m_detectionSensitivityIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("detectionSensitivity")), m_detectionSensitivity));
    }
    if(m_movementSensitivityIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("movementSensitivity")), m_movementSensitivity));
    }
    if(m_sensorModalityIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("sensorModality")), m_sensorModality));
    }
    if(m_frameRateLimitIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("frameRateLimit")), m_frameRateLimit));
    }
    if(m_metadataModeIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("metadataMode")), m_metadataMode));
    }
    if(m_statisticsModeIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("statisticsMode")), m_statisticsMode));
    }
    if(m_diagnosticsModeIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("diagnosticsMode")), m_diagnosticsMode));
    }
    if(m_debugModeIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("debugMode")), m_debugMode));
    }
}

bool SecuRTInstanceWrite::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("name"))))
    {
        utility::string_t refVal_setName;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("name"))), refVal_setName );
        setName(refVal_setName);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("detectorMode"))))
    {
        std::shared_ptr<DetectorMode> refVal_setDetectorMode;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("detectorMode"))), refVal_setDetectorMode );
        setDetectorMode(refVal_setDetectorMode);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("detectionSensitivity"))))
    {
        std::shared_ptr<Sensitivity> refVal_setDetectionSensitivity;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("detectionSensitivity"))), refVal_setDetectionSensitivity );
        setDetectionSensitivity(refVal_setDetectionSensitivity);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("movementSensitivity"))))
    {
        std::shared_ptr<Sensitivity> refVal_setMovementSensitivity;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("movementSensitivity"))), refVal_setMovementSensitivity );
        setMovementSensitivity(refVal_setMovementSensitivity);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("sensorModality"))))
    {
        std::shared_ptr<SensorModality> refVal_setSensorModality;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("sensorModality"))), refVal_setSensorModality );
        setSensorModality(refVal_setSensorModality);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("frameRateLimit"))))
    {
        double refVal_setFrameRateLimit;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("frameRateLimit"))), refVal_setFrameRateLimit );
        setFrameRateLimit(refVal_setFrameRateLimit);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("metadataMode"))))
    {
        bool refVal_setMetadataMode;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("metadataMode"))), refVal_setMetadataMode );
        setMetadataMode(refVal_setMetadataMode);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("statisticsMode"))))
    {
        bool refVal_setStatisticsMode;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("statisticsMode"))), refVal_setStatisticsMode );
        setStatisticsMode(refVal_setStatisticsMode);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("diagnosticsMode"))))
    {
        bool refVal_setDiagnosticsMode;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("diagnosticsMode"))), refVal_setDiagnosticsMode );
        setDiagnosticsMode(refVal_setDiagnosticsMode);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("debugMode"))))
    {
        bool refVal_setDebugMode;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("debugMode"))), refVal_setDebugMode );
        setDebugMode(refVal_setDebugMode);
    }
    return ok;
}

utility::string_t SecuRTInstanceWrite::getName() const
{
    return m_name;
}

void SecuRTInstanceWrite::setName(const utility::string_t& value)
{
    m_name = value;
    m_nameIsSet = true;
}

bool SecuRTInstanceWrite::nameIsSet() const
{
    return m_nameIsSet;
}

void SecuRTInstanceWrite::unsetname()
{
    m_nameIsSet = false;
}
std::shared_ptr<DetectorMode> SecuRTInstanceWrite::getDetectorMode() const
{
    return m_detectorMode;
}

void SecuRTInstanceWrite::setDetectorMode(const std::shared_ptr<DetectorMode>& value)
{
    m_detectorMode = value;
    m_detectorModeIsSet = true;
}

bool SecuRTInstanceWrite::detectorModeIsSet() const
{
    return m_detectorModeIsSet;
}

void SecuRTInstanceWrite::unsetdetectorMode()
{
    m_detectorModeIsSet = false;
}
std::shared_ptr<Sensitivity> SecuRTInstanceWrite::getDetectionSensitivity() const
{
    return m_detectionSensitivity;
}

void SecuRTInstanceWrite::setDetectionSensitivity(const std::shared_ptr<Sensitivity>& value)
{
    m_detectionSensitivity = value;
    m_detectionSensitivityIsSet = true;
}

bool SecuRTInstanceWrite::detectionSensitivityIsSet() const
{
    return m_detectionSensitivityIsSet;
}

void SecuRTInstanceWrite::unsetdetectionSensitivity()
{
    m_detectionSensitivityIsSet = false;
}
std::shared_ptr<Sensitivity> SecuRTInstanceWrite::getMovementSensitivity() const
{
    return m_movementSensitivity;
}

void SecuRTInstanceWrite::setMovementSensitivity(const std::shared_ptr<Sensitivity>& value)
{
    m_movementSensitivity = value;
    m_movementSensitivityIsSet = true;
}

bool SecuRTInstanceWrite::movementSensitivityIsSet() const
{
    return m_movementSensitivityIsSet;
}

void SecuRTInstanceWrite::unsetmovementSensitivity()
{
    m_movementSensitivityIsSet = false;
}
std::shared_ptr<SensorModality> SecuRTInstanceWrite::getSensorModality() const
{
    return m_sensorModality;
}

void SecuRTInstanceWrite::setSensorModality(const std::shared_ptr<SensorModality>& value)
{
    m_sensorModality = value;
    m_sensorModalityIsSet = true;
}

bool SecuRTInstanceWrite::sensorModalityIsSet() const
{
    return m_sensorModalityIsSet;
}

void SecuRTInstanceWrite::unsetsensorModality()
{
    m_sensorModalityIsSet = false;
}
double SecuRTInstanceWrite::getFrameRateLimit() const
{
    return m_frameRateLimit;
}

void SecuRTInstanceWrite::setFrameRateLimit(double value)
{
    m_frameRateLimit = value;
    m_frameRateLimitIsSet = true;
}

bool SecuRTInstanceWrite::frameRateLimitIsSet() const
{
    return m_frameRateLimitIsSet;
}

void SecuRTInstanceWrite::unsetframeRateLimit()
{
    m_frameRateLimitIsSet = false;
}
bool SecuRTInstanceWrite::isMetadataMode() const
{
    return m_metadataMode;
}

void SecuRTInstanceWrite::setMetadataMode(bool value)
{
    m_metadataMode = value;
    m_metadataModeIsSet = true;
}

bool SecuRTInstanceWrite::metadataModeIsSet() const
{
    return m_metadataModeIsSet;
}

void SecuRTInstanceWrite::unsetmetadataMode()
{
    m_metadataModeIsSet = false;
}
bool SecuRTInstanceWrite::isStatisticsMode() const
{
    return m_statisticsMode;
}

void SecuRTInstanceWrite::setStatisticsMode(bool value)
{
    m_statisticsMode = value;
    m_statisticsModeIsSet = true;
}

bool SecuRTInstanceWrite::statisticsModeIsSet() const
{
    return m_statisticsModeIsSet;
}

void SecuRTInstanceWrite::unsetstatisticsMode()
{
    m_statisticsModeIsSet = false;
}
bool SecuRTInstanceWrite::isDiagnosticsMode() const
{
    return m_diagnosticsMode;
}

void SecuRTInstanceWrite::setDiagnosticsMode(bool value)
{
    m_diagnosticsMode = value;
    m_diagnosticsModeIsSet = true;
}

bool SecuRTInstanceWrite::diagnosticsModeIsSet() const
{
    return m_diagnosticsModeIsSet;
}

void SecuRTInstanceWrite::unsetdiagnosticsMode()
{
    m_diagnosticsModeIsSet = false;
}
bool SecuRTInstanceWrite::isDebugMode() const
{
    return m_debugMode;
}

void SecuRTInstanceWrite::setDebugMode(bool value)
{
    m_debugMode = value;
    m_debugModeIsSet = true;
}

bool SecuRTInstanceWrite::debugModeIsSet() const
{
    return m_debugModeIsSet;
}

void SecuRTInstanceWrite::unsetdebugMode()
{
    m_debugModeIsSet = false;
}
}
}
}
}
}


