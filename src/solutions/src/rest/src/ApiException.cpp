/**
 * CVEDIA-RT
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 2024.2.2
 * Contact: support@cvedia.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

#include "rt_rest_client/ApiException.h"

namespace cvedia {
namespace rt {
namespace rest {
namespace client {

ApiException::ApiException( int errorCode
    , const utility::string_t& message
    , std::shared_ptr<std::istream> content /*= nullptr*/ )
    : web::http::http_exception( errorCode, message )
    , m_Content(content)
{
}
ApiException::ApiException( int errorCode
    , const utility::string_t& message
    , std::map<utility::string_t, utility::string_t>& headers
    , std::shared_ptr<std::istream> content /*= nullptr*/ )
    : web::http::http_exception( errorCode, message )
    , m_Content(content)
    , m_Headers(headers)
{
}

ApiException::~ApiException()
{
}

std::shared_ptr<std::istream> ApiException::getContent() const
{
    return m_Content;
}

std::map<utility::string_t, utility::string_t>& ApiException::getHeaders()
{
    return m_Headers;
}

}
}
}
}
