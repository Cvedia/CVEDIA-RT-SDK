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
 * ApiClient.h
 *
 * This is an API client responsible for stating the HTTP calls
 */

#ifndef CVEDIA_RT_REST_CLIENT_ApiClient_H_
#define CVEDIA_RT_REST_CLIENT_ApiClient_H_


#include "defines.h"
#include "rt_rest_client/ApiConfiguration.h"
#include "rt_rest_client/ApiException.h"
#include "rt_rest_client/IHttpBody.h"
#include "rt_rest_client/HttpContent.h"

#if  defined (_WIN32) || defined (_WIN64)
#undef U
#endif

#include <cpprest/details/basic_types.h>
#include <cpprest/http_client.h>

#include <memory>
#include <vector>
#include <functional>

namespace cvedia {
namespace rt {
namespace rest {
namespace client {

using namespace cvedia::rt::rest::client::model;

class EXPORT_CLASS  ApiClient
{
public:
    EXPORT ApiClient( std::shared_ptr<const ApiConfiguration> configuration = nullptr );
    EXPORT virtual ~ApiClient();

    typedef std::function<void(web::http::status_code, const web::http::http_headers&)> ResponseHandlerType;

    EXPORT const ResponseHandlerType& getResponseHandler() const;
    EXPORT void setResponseHandler(const ResponseHandlerType& responseHandler);

    EXPORT std::shared_ptr<const ApiConfiguration> getConfiguration() const;
    EXPORT void setConfiguration(std::shared_ptr<const ApiConfiguration> configuration);

    EXPORT static utility::string_t parameterToString(utility::string_t value);
    EXPORT static utility::string_t parameterToString(int32_t value);
    EXPORT static utility::string_t parameterToString(int64_t value);
    EXPORT static utility::string_t parameterToString(float value);
    EXPORT static utility::string_t parameterToString(double value);
    EXPORT static utility::string_t parameterToString(const utility::datetime &value);
    EXPORT static utility::string_t parameterToString(bool value);
    template<class T>
    EXPORT static utility::string_t parameterToString(const std::vector<T>& value);
    template<class T>
    EXPORT static utility::string_t parameterToString(const std::shared_ptr<T>& value);

    EXPORT pplx::task<web::http::http_response> callApi(
        const utility::string_t& path,
        const utility::string_t& method,
        const std::map<utility::string_t, utility::string_t>& queryParams,
        const std::shared_ptr<IHttpBody> postBody,
        const std::map<utility::string_t, utility::string_t>& headerParams,
        const std::map<utility::string_t, utility::string_t>& formParams,
        const std::map<utility::string_t, std::shared_ptr<HttpContent>>& fileParams,
        const utility::string_t& contentType
    ) const;

protected:

    ResponseHandlerType m_ResponseHandler;
    std::shared_ptr<const ApiConfiguration> m_Configuration;
};

template<class T>
utility::string_t ApiClient::parameterToString(const std::vector<T>& value)
{
    utility::stringstream_t ss;

    for( size_t i = 0; i < value.size(); i++)
    {
        if( i > 0) ss << utility::conversions::to_string_t(", ");
        ss << ApiClient::parameterToString(value[i]);
    }

    return ss.str();
}

template<class T>
utility::string_t ApiClient::parameterToString(const std::shared_ptr<T>& value)
{
    return parameterToString(*value.get());
}


}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_ApiClient_H_ */
