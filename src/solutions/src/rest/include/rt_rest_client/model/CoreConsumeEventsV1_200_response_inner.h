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

/*
 * CoreConsumeEventsV1_200_response_inner.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_CoreConsumeEventsV1_200_response_inner_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_CoreConsumeEventsV1_200_response_inner_H_


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
class EXPORT_CLASS  CoreConsumeEventsV1_200_response_inner
    : public ModelBase
{
public:
    EXPORT CoreConsumeEventsV1_200_response_inner();
    EXPORT virtual ~CoreConsumeEventsV1_200_response_inner();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// CoreConsumeEventsV1_200_response_inner members

    /// <summary>
    /// 
    /// </summary>
    EXPORT utility::string_t getDataType() const;
    EXPORT bool dataTypeIsSet() const;
    EXPORT void unsetdataType();

    EXPORT void setDataType(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT utility::string_t getJsonObject() const;
    EXPORT bool jsonObjectIsSet() const;
    EXPORT void unsetjsonObject();

    EXPORT void setJsonObject(const utility::string_t& value);


protected:
    utility::string_t m_dataType;
    bool m_dataTypeIsSet;
    utility::string_t m_jsonObject;
    bool m_jsonObjectIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_CoreConsumeEventsV1_200_response_inner_H_ */
