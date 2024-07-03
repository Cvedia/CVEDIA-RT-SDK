/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 2024.2.0
 * Contact: support@cvedia.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */



#include "rt_rest_client/model/InstanceWrite.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {



InstanceWrite::InstanceWrite()
{
    m_name = utility::conversions::to_string_t("");
    m_nameIsSet = false;
    m_group = utility::conversions::to_string_t("");
    m_groupIsSet = false;
    m_solution = utility::conversions::to_string_t("");
    m_solutionIsSet = false;
    m_persistent = false;
    m_persistentIsSet = false;
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
    m_detectorModeIsSet = false;
    m_detectionSensitivityIsSet = false;
    m_movementSensitivityIsSet = false;
    m_sensorModalityIsSet = false;
}

InstanceWrite::~InstanceWrite()
{
}

void InstanceWrite::validate()
{
    // TODO: implement validation
}

web::json::value InstanceWrite::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_nameIsSet)
    {
        val[utility::conversions::to_string_t(U("name"))] = ModelBase::toJson(m_name);
    }
    if(m_groupIsSet)
    {
        val[utility::conversions::to_string_t(U("group"))] = ModelBase::toJson(m_group);
    }
    if(m_solutionIsSet)
    {
        val[utility::conversions::to_string_t(U("solution"))] = ModelBase::toJson(m_solution);
    }
    if(m_persistentIsSet)
    {
        val[utility::conversions::to_string_t(U("persistent"))] = ModelBase::toJson(m_persistent);
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

    return val;
}

bool InstanceWrite::fromJson(const web::json::value& val)
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
    if(val.has_field(utility::conversions::to_string_t(U("group"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("group")));
        if(!fieldValue.is_null())
        {
            utility::string_t refVal_setGroup;
            ok &= ModelBase::fromJson(fieldValue, refVal_setGroup);
            setGroup(refVal_setGroup);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("solution"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("solution")));
        if(!fieldValue.is_null())
        {
            utility::string_t refVal_setSolution;
            ok &= ModelBase::fromJson(fieldValue, refVal_setSolution);
            setSolution(refVal_setSolution);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("persistent"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("persistent")));
        if(!fieldValue.is_null())
        {
            bool refVal_setPersistent;
            ok &= ModelBase::fromJson(fieldValue, refVal_setPersistent);
            setPersistent(refVal_setPersistent);
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
    return ok;
}

void InstanceWrite::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
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
    if(m_groupIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("group")), m_group));
    }
    if(m_solutionIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("solution")), m_solution));
    }
    if(m_persistentIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("persistent")), m_persistent));
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
}

bool InstanceWrite::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
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
    if(multipart->hasContent(utility::conversions::to_string_t(U("group"))))
    {
        utility::string_t refVal_setGroup;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("group"))), refVal_setGroup );
        setGroup(refVal_setGroup);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("solution"))))
    {
        utility::string_t refVal_setSolution;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("solution"))), refVal_setSolution );
        setSolution(refVal_setSolution);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("persistent"))))
    {
        bool refVal_setPersistent;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("persistent"))), refVal_setPersistent );
        setPersistent(refVal_setPersistent);
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
    return ok;
}

utility::string_t InstanceWrite::getName() const
{
    return m_name;
}

void InstanceWrite::setName(const utility::string_t& value)
{
    m_name = value;
    m_nameIsSet = true;
}

bool InstanceWrite::nameIsSet() const
{
    return m_nameIsSet;
}

void InstanceWrite::unsetname()
{
    m_nameIsSet = false;
}
utility::string_t InstanceWrite::getGroup() const
{
    return m_group;
}

void InstanceWrite::setGroup(const utility::string_t& value)
{
    m_group = value;
    m_groupIsSet = true;
}

bool InstanceWrite::groupIsSet() const
{
    return m_groupIsSet;
}

void InstanceWrite::unsetgroup()
{
    m_groupIsSet = false;
}
utility::string_t InstanceWrite::getSolution() const
{
    return m_solution;
}

void InstanceWrite::setSolution(const utility::string_t& value)
{
    m_solution = value;
    m_solutionIsSet = true;
}

bool InstanceWrite::solutionIsSet() const
{
    return m_solutionIsSet;
}

void InstanceWrite::unsetsolution()
{
    m_solutionIsSet = false;
}
bool InstanceWrite::isPersistent() const
{
    return m_persistent;
}

void InstanceWrite::setPersistent(bool value)
{
    m_persistent = value;
    m_persistentIsSet = true;
}

bool InstanceWrite::persistentIsSet() const
{
    return m_persistentIsSet;
}

void InstanceWrite::unsetpersistent()
{
    m_persistentIsSet = false;
}
double InstanceWrite::getFrameRateLimit() const
{
    return m_frameRateLimit;
}

void InstanceWrite::setFrameRateLimit(double value)
{
    m_frameRateLimit = value;
    m_frameRateLimitIsSet = true;
}

bool InstanceWrite::frameRateLimitIsSet() const
{
    return m_frameRateLimitIsSet;
}

void InstanceWrite::unsetframeRateLimit()
{
    m_frameRateLimitIsSet = false;
}
bool InstanceWrite::isMetadataMode() const
{
    return m_metadataMode;
}

void InstanceWrite::setMetadataMode(bool value)
{
    m_metadataMode = value;
    m_metadataModeIsSet = true;
}

bool InstanceWrite::metadataModeIsSet() const
{
    return m_metadataModeIsSet;
}

void InstanceWrite::unsetmetadataMode()
{
    m_metadataModeIsSet = false;
}
bool InstanceWrite::isStatisticsMode() const
{
    return m_statisticsMode;
}

void InstanceWrite::setStatisticsMode(bool value)
{
    m_statisticsMode = value;
    m_statisticsModeIsSet = true;
}

bool InstanceWrite::statisticsModeIsSet() const
{
    return m_statisticsModeIsSet;
}

void InstanceWrite::unsetstatisticsMode()
{
    m_statisticsModeIsSet = false;
}
bool InstanceWrite::isDiagnosticsMode() const
{
    return m_diagnosticsMode;
}

void InstanceWrite::setDiagnosticsMode(bool value)
{
    m_diagnosticsMode = value;
    m_diagnosticsModeIsSet = true;
}

bool InstanceWrite::diagnosticsModeIsSet() const
{
    return m_diagnosticsModeIsSet;
}

void InstanceWrite::unsetdiagnosticsMode()
{
    m_diagnosticsModeIsSet = false;
}
bool InstanceWrite::isDebugMode() const
{
    return m_debugMode;
}

void InstanceWrite::setDebugMode(bool value)
{
    m_debugMode = value;
    m_debugModeIsSet = true;
}

bool InstanceWrite::debugModeIsSet() const
{
    return m_debugModeIsSet;
}

void InstanceWrite::unsetdebugMode()
{
    m_debugModeIsSet = false;
}
std::shared_ptr<DetectorMode> InstanceWrite::getDetectorMode() const
{
    return m_detectorMode;
}

void InstanceWrite::setDetectorMode(const std::shared_ptr<DetectorMode>& value)
{
    m_detectorMode = value;
    m_detectorModeIsSet = true;
}

bool InstanceWrite::detectorModeIsSet() const
{
    return m_detectorModeIsSet;
}

void InstanceWrite::unsetdetectorMode()
{
    m_detectorModeIsSet = false;
}
std::shared_ptr<Sensitivity> InstanceWrite::getDetectionSensitivity() const
{
    return m_detectionSensitivity;
}

void InstanceWrite::setDetectionSensitivity(const std::shared_ptr<Sensitivity>& value)
{
    m_detectionSensitivity = value;
    m_detectionSensitivityIsSet = true;
}

bool InstanceWrite::detectionSensitivityIsSet() const
{
    return m_detectionSensitivityIsSet;
}

void InstanceWrite::unsetdetectionSensitivity()
{
    m_detectionSensitivityIsSet = false;
}
std::shared_ptr<Sensitivity> InstanceWrite::getMovementSensitivity() const
{
    return m_movementSensitivity;
}

void InstanceWrite::setMovementSensitivity(const std::shared_ptr<Sensitivity>& value)
{
    m_movementSensitivity = value;
    m_movementSensitivityIsSet = true;
}

bool InstanceWrite::movementSensitivityIsSet() const
{
    return m_movementSensitivityIsSet;
}

void InstanceWrite::unsetmovementSensitivity()
{
    m_movementSensitivityIsSet = false;
}
std::shared_ptr<SensorModality> InstanceWrite::getSensorModality() const
{
    return m_sensorModality;
}

void InstanceWrite::setSensorModality(const std::shared_ptr<SensorModality>& value)
{
    m_sensorModality = value;
    m_sensorModalityIsSet = true;
}

bool InstanceWrite::sensorModalityIsSet() const
{
    return m_sensorModalityIsSet;
}

void InstanceWrite::unsetsensorModality()
{
    m_sensorModalityIsSet = false;
}
}
}
}
}
}


