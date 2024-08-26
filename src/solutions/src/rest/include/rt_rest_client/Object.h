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
 * Object.h
 *
 * This is the implementation of a JSON object.
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_Object_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_Object_H_


#include "defines.h"
#include "rt_rest_client/ModelBase.h"

#include <cpprest/details/basic_types.h>
#include <cpprest/json.h>

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {

class EXPORT_CLASS  Object : public ModelBase
{
public:
    EXPORT Object();
    EXPORT virtual ~Object();

    /////////////////////////////////////////////
    /// ModelBase overrides
    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// Object manipulation
    EXPORT web::json::value getValue(const utility::string_t& key) const;
    EXPORT void setValue(const utility::string_t& key, const web::json::value& value);

private:
    web::json::value m_object;
};

}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_Object_H_ */
