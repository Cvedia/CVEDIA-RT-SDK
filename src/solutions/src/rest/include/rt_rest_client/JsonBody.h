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
 * JsonBody.h
 *
 * This is a JSON http body which can be submitted via http
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_JsonBody_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_JsonBody_H_


#include "defines.h"
#include "rt_rest_client/IHttpBody.h"

#include <cpprest/json.h>

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {

class EXPORT_CLASS  JsonBody
    : public IHttpBody
{
public:
    EXPORT JsonBody( const web::json::value& value );
    EXPORT virtual ~JsonBody();

    EXPORT void writeTo( std::ostream& target ) override;

protected:
    web::json::value m_Json;
};

}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_JsonBody_H_ */
