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
 * CorePostInstanceConfigV1_request.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_CorePostInstanceConfigV1_request_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_CorePostInstanceConfigV1_request_H_


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
class EXPORT_CLASS  CorePostInstanceConfigV1_request
    : public ModelBase
{
public:
    EXPORT CorePostInstanceConfigV1_request();
    EXPORT virtual ~CorePostInstanceConfigV1_request();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// CorePostInstanceConfigV1_request members

    /// <summary>
    /// 
    /// </summary>
    EXPORT utility::string_t getPath() const;
    EXPORT bool pathIsSet() const;
    EXPORT void unsetpath();

    EXPORT void setPath(const utility::string_t& value);

    /// <summary>
    /// This must be a string containing a JSON value or object.        #### String value    For a simple string value you must include and escape the double quotes:    &#x60;&#x60;&#x60;json  {      \&quot;path\&quot;: \&quot;my/nested/path\&quot;,      \&quot;jsonValue\&quot;: \&quot;\\\&quot;my string\\\&quot;\&quot;  }  &#x60;&#x60;&#x60;    #### Object value    &#x60;&#x60;&#x60;json  {      \&quot;path\&quot;: \&quot;my/nested/path\&quot;,      \&quot;jsonValue\&quot;: \&quot;{\\\&quot;myKey\\\&quot;: 123}\&quot;  }  &#x60;&#x60;&#x60;
    /// </summary>
    EXPORT utility::string_t getJsonValue() const;
    EXPORT bool jsonValueIsSet() const;
    EXPORT void unsetjsonValue();

    EXPORT void setJsonValue(const utility::string_t& value);


protected:
    utility::string_t m_path;
    bool m_pathIsSet;
    utility::string_t m_jsonValue;
    bool m_jsonValueIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_CorePostInstanceConfigV1_request_H_ */
