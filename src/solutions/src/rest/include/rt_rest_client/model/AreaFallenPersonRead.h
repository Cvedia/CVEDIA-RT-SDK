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

/*
 * AreaFallenPersonRead.h
 *
 * 
 */

#ifndef CVEDIA_RT_REST_CLIENT_MODEL_AreaFallenPersonRead_H_
#define CVEDIA_RT_REST_CLIENT_MODEL_AreaFallenPersonRead_H_


#include "defines.h"
#include "rt_rest_client/ModelBase.h"

#include <cpprest/details/basic_types.h>
#include "rt_rest_client/model/Coordinate.h"
#include <vector>

namespace cvedia {
namespace rt {
namespace rest {
namespace client {
namespace model {

class Coordinate;

/// <summary>
/// 
/// </summary>
class EXPORT_CLASS  AreaFallenPersonRead
    : public ModelBase
{
public:
    EXPORT AreaFallenPersonRead();
    EXPORT virtual ~AreaFallenPersonRead();

    /////////////////////////////////////////////
    /// ModelBase overrides

    EXPORT void validate() override;

    EXPORT web::json::value toJson() const override;
    EXPORT bool fromJson(const web::json::value& json) override;

    EXPORT void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    EXPORT bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// AreaFallenPersonRead members

    /// <summary>
    /// 
    /// </summary>
    EXPORT utility::string_t getId() const;
    EXPORT bool idIsSet() const;
    EXPORT void unsetid();

    EXPORT void setId(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT utility::string_t getName() const;
    EXPORT bool nameIsSet() const;
    EXPORT void unsetname();

    EXPORT void setName(const utility::string_t& value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT std::vector<std::shared_ptr<Coordinate>>& getCoordinates();
    EXPORT bool coordinatesIsSet() const;
    EXPORT void unsetcoordinates();

    EXPORT void setCoordinates(const std::vector<std::shared_ptr<Coordinate>>& value);

    /// <summary>
    /// 
    /// </summary>
    EXPORT std::vector<double>& getColor();
    EXPORT bool colorIsSet() const;
    EXPORT void unsetcolor();

    EXPORT void setColor(std::vector<double> value);


protected:
    utility::string_t m_id;
    bool m_idIsSet;
    utility::string_t m_name;
    bool m_nameIsSet;
    std::vector<std::shared_ptr<Coordinate>> m_coordinates;
    bool m_coordinatesIsSet;
    std::vector<double> m_color;
    bool m_colorIsSet;
};


}
}
}
}
}

#endif /* CVEDIA_RT_REST_CLIENT_MODEL_AreaFallenPersonRead_H_ */
