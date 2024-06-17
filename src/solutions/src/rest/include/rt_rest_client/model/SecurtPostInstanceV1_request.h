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
 * SecurtPostInstanceV1_request.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_SecurtPostInstanceV1_request_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_SecurtPostInstanceV1_request_H_


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
class EXPORT_CLASS  SecurtPostInstanceV1_request
    : public ModelBase
{
public:
    EXPORT SecurtPostInstanceV1_request();
    EXPORT virtual ~SecurtPostInstanceV1_request();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// SecurtPostInstanceV1_request members

    /// <summary>
    /// (optional) Instance ID to create
    /// </summary>
    EXPORT utility::string_t getInstanceId() const;
    EXPORT bool instanceIdIsSet() const;
    EXPORT void unsetinstanceId();

    EXPORT void setInstanceId(const utility::string_t& value);

    /// <summary>
    /// Name of the Securt instance to be created.
    /// </summary>
    EXPORT utility::string_t getName() const;
    EXPORT bool nameIsSet() const;
    EXPORT void unsetname();

    EXPORT void setName(const utility::string_t& value);


protected:
    utility::string_t m_instanceId;
    bool m_instanceIdIsSet;
    utility::string_t m_name;
    bool m_nameIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_SecurtPostInstanceV1_request_H_ */
