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
 * SecuRTInstanceStats.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_SecuRTInstanceStats_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_SecuRTInstanceStats_H_


#include "defines.h"
#include "rt_rest_client/ModelBase.h"


namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {


/// <summary>
/// 
/// </summary>
class EXPORT_CLASS  SecuRTInstanceStats
    : public ModelBase
{
public:
    EXPORT SecuRTInstanceStats();
    EXPORT virtual ~SecuRTInstanceStats();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// SecuRTInstanceStats members

    /// <summary>
    /// 
    /// </summary>
    EXPORT int64_t getStartTime() const;
    EXPORT bool startTimeIsSet() const;
    EXPORT void unsetstartTime();

    EXPORT void setStartTime(int64_t value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT float getFrameRate() const;
    EXPORT bool frameRateIsSet() const;
    EXPORT void unsetframeRate();

    EXPORT void setFrameRate(float value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT float getLatency() const;
    EXPORT bool latencyIsSet() const;
    EXPORT void unsetlatency();

    EXPORT void setLatency(float value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT int32_t getFramesProcessed() const;
    EXPORT bool framesProcessedIsSet() const;
    EXPORT void unsetframesProcessed();

    EXPORT void setFramesProcessed(int32_t value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT int32_t getTrackCount() const;
    EXPORT bool trackCountIsSet() const;
    EXPORT void unsettrackCount();

    EXPORT void setTrackCount(int32_t value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT bool isIsRunning() const;
    EXPORT bool isRunningIsSet() const;
    EXPORT void unsetisRunning();

    EXPORT void setIsRunning(bool value);


protected:
    int64_t m_startTime;
    bool m_startTimeIsSet;
    float m_frameRate;
    bool m_frameRateIsSet;
    float m_latency;
    bool m_latencyIsSet;
    int32_t m_framesProcessed;
    bool m_framesProcessedIsSet;
    int32_t m_trackCount;
    bool m_trackCountIsSet;
    bool m_isRunning;
    bool m_isRunningIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_SecuRTInstanceStats_H_ */
