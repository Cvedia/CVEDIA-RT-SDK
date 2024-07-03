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

/*
 * DisplayableEntity.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_DisplayableEntity_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_DisplayableEntity_H_


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
class EXPORT_CLASS  DisplayableEntity
    : public ModelBase
{
public:
    EXPORT DisplayableEntity();
    EXPORT virtual ~DisplayableEntity();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// DisplayableEntity members

    /// <summary>
    /// 
    /// </summary>
    EXPORT utility::string_t getDisplayName() const;
    EXPORT bool displayNameIsSet() const;
    EXPORT void unsetdisplayName();

    EXPORT void setDisplayName(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT utility::string_t getValue() const;
    EXPORT bool valueIsSet() const;
    EXPORT void unsetvalue();

    EXPORT void setValue(const utility::string_t& value);


protected:
    utility::string_t m_displayName;
    bool m_displayNameIsSet;
    utility::string_t m_value;
    bool m_valueIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_DisplayableEntity_H_ */
