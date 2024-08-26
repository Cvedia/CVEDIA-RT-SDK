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
 * HttpContent.h
 *
 * This class represents a single item of a multipart-formdata request.
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_HttpContent_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_HttpContent_H_


#include "defines.h"
#include <cpprest/details/basic_types.h>

#include <memory>

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {

class EXPORT_CLASS  HttpContent
{
public:
    EXPORT HttpContent();
    EXPORT virtual ~HttpContent();

    EXPORT virtual utility::string_t getContentDisposition() const;
    EXPORT virtual void setContentDisposition( const utility::string_t& value );

    EXPORT virtual utility::string_t getName() const;
    EXPORT virtual void setName( const utility::string_t& value );

    EXPORT virtual utility::string_t getFileName() const;
    EXPORT virtual void setFileName( const utility::string_t& value );

    EXPORT virtual utility::string_t getContentType() const;
    EXPORT virtual void setContentType( const utility::string_t& value );

    EXPORT virtual std::shared_ptr<std::istream> getData() const;
    EXPORT virtual void setData( std::shared_ptr<std::istream> value );

    EXPORT virtual void writeTo( std::ostream& stream );

protected:
    // NOTE: no utility::string_t here because those strings can only contain ascii
    utility::string_t m_ContentDisposition;
    utility::string_t m_Name;
    utility::string_t m_FileName;
    utility::string_t m_ContentType;
    std::shared_ptr<std::istream> m_Data;
};

}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_HttpContent_H_ */
