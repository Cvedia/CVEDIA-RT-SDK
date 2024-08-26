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
 * IHttpBody.h
 *
 * This is the interface for contents that can be sent to a remote HTTP server.
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_IHttpBody_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_IHttpBody_H_


#include "defines.h"
#include <iostream>

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {

class EXPORT_CLASS  IHttpBody
{
public:
    EXPORT virtual ~IHttpBody() { }

    EXPORT virtual void writeTo( std::ostream& stream ) = 0;
};

}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_IHttpBody_H_ */
