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
 * ApiException.h
 *
 * This is the exception being thrown in case the api call was not successful
 */

#ifndef CVEDIA_RT_REST_CLIENT_ApiException_H_
#define CVEDIA_RT_REST_CLIENT_ApiException_H_


#include "defines.h"
#include <cpprest/details/basic_types.h>
#include <cpprest/http_msg.h>

#include <memory>
#include <map>

namespace cvedia {
namespace rt {
namespace rest {
namespace client {

class EXPORT_CLASS  ApiException
    : public web::http::http_exception
{
public:
    EXPORT ApiException( int errorCode
        , const utility::string_t& message
        , std::shared_ptr<std::istream> content = nullptr );
    EXPORT ApiException( int errorCode
        , const utility::string_t& message
        , std::map<utility::string_t, utility::string_t>& headers
        , std::shared_ptr<std::istream> content = nullptr );
    EXPORT virtual ~ApiException();

    EXPORT std::map<utility::string_t, utility::string_t>& getHeaders();
    EXPORT std::shared_ptr<std::istream> getContent() const;

protected:
    std::shared_ptr<std::istream> m_Content;
    std::map<utility::string_t, utility::string_t> m_Headers;
};

}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_ApiBase_H_ */