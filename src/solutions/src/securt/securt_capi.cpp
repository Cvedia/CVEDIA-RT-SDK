#include "rtconfig.h"

#include "cvalue.h"
#include "api/convert.h"
#include "api/thread.h"
#include "builtin/functions.h"
#include "core/core_capi.h"
#include "interface/instance.h"
#include "interface/instance_controller.h"
#include "interface/solution.h"

#include "common/context.h"
#include "common/functions.h"
#include "core/core_capi.h"

#include "securt/securt_capi.h"
#include "interface/securt.h"
#include "builtin/discovery.h"
#include "api/logging.h"
#include "api/instances.h"
#include "builtin/analytics_manager.h"

#include <httplib.h>

#include <array>
#include <nlohmann/json.hpp>

#if WITH_SECURT_REST_CLIENT
#include <rt_rest_client/ApiConfiguration.h>
#include <rt_rest_client/ApiClient.h>
#include <rt_rest_client/api/SecuRTApi.h>
#include <rt_rest_client/api/SecuRTAreasApi.h>
#include <rt_rest_client/api/SecuRTLinesApi.h>
#endif

#include <plog/Init.h>

#define GET_SECURT(var) \
	auto var = getSecuRt(context->instanceUuid); \
	if (!var) \
		return -1

#define GET_SECURT_STR(var) \
	auto var = getSecuRt(context->instanceUuid); \
	if (!var) \
		return nullptr 

std::shared_ptr<cvedia::rt::iface::SecuRT> getSecuRt(cvedia::rt::Uuid const& instanceId) {
	if (auto const& ctrlResp = cvedia::rt::api::instances::getInstanceController(instanceId))
	{
		auto const& ctrl = ctrlResp.value();
		if (auto const& mgrResp = ctrl->getSolutionManagerOrCreate())
		{
			auto const& mgr = mgrResp.value();
			if (auto const& securt = std::dynamic_pointer_cast<cvedia::rt::iface::SecuRT>(mgr))
			{
				return securt;
			}
		}
	}

	return nullptr;
}

std::array<double, 4> to_color_array(double const* color)
{
	std::array<double, 4> colorArray;
	for (int i = 0; i < 4; ++i)
	{
		colorArray[i] = color[i];
	}
	return colorArray;
}

std::vector<double> to_color_vector(double const* color)
{
	std::vector<double> colorVec;
	for (int i = 0; i < 4; ++i)
	{
		colorVec.push_back(color[i]);
	}
	return colorVec;
}

std::vector<cvedia::rt::Point2f> coords_to_points(float const* coords, int const numPoints)
{
	std::vector<cvedia::rt::Point2f> points;
	for (int i = 0; i < numPoints; i += 2)
	{
		points.push_back({ coords[i], coords[i + 1] });
	}

	return points;
}

// coords_to_points but with a vector of vector, separating shapes if the coords are (-1, -1)
std::vector<std::vector<cvedia::rt::Point2f>> coords_to_points_v2(float const* coords, int const numPoints)
{
	std::vector<std::vector<cvedia::rt::Point2f>> points;
	std::vector<cvedia::rt::Point2f> currentShape;
	for (int i = 0; i < numPoints; i += 2)
	{
		if (static_cast<int>(coords[i]) == -1 && static_cast<int>(coords[i + 1]) == -1)
		{
			points.push_back(currentShape);
			currentShape.clear();
		}
		else
		{
			currentShape.push_back({ coords[i], coords[i + 1] });
		}
	}

	if (!currentShape.empty())
	{
		points.push_back(currentShape);
	}

	return points;
}

std::unordered_set<cvedia::rt::iface::SecuRT::Classes> classes_to_set(int const* classes, int const classesSize)
{
	std::unordered_set<cvedia::rt::iface::SecuRT::Classes> classesSet;
	for (int i = 0; i < classesSize; ++i)
	{
		classesSet.insert(static_cast<cvedia::rt::iface::SecuRT::Classes>(classes[i]));
	}
	return classesSet;
}

#if WITH_SECURT_REST_CLIENT
std::shared_ptr<cvedia::rt::rest::client::model::AreaEvent> to_remote_area_event(int const areaType)
{
	auto areaEvent = std::make_shared<cvedia::rt::rest::client::AreaEvent>();

	switch(areaType)
	{
		case 1:
			areaEvent->setValue(cvedia::rt::rest::client::AreaEvent::eAreaEvent::AreaEvent_ENTER);
			break;
		case 2:
			areaEvent->setValue(cvedia::rt::rest::client::AreaEvent::eAreaEvent::AreaEvent_EXIT);
			break;
		case 3:
			areaEvent->setValue(cvedia::rt::rest::client::AreaEvent::eAreaEvent::AreaEvent_BOTH);
			break;
		default:
			areaEvent->setValue(cvedia::rt::rest::client::AreaEvent::eAreaEvent::AreaEvent_BOTH);
			break;
	}

	return areaEvent;
}

std::vector<std::shared_ptr<cvedia::rt::rest::client::model::Coordinate>> to_remote_coordinates(std::vector<cvedia::rt::Point2f> const& points)
{
	std::vector<std::shared_ptr<cvedia::rt::rest::client::model::Coordinate>> coordinates;
	for (auto const& point : points)
	{
		auto const coordinate = std::make_shared<cvedia::rt::rest::client::Coordinate>();
		coordinate->setX(point.x);
		coordinate->setY(point.y);
		coordinates.push_back(coordinate);
	}

	return coordinates;
}

std::vector<std::vector<std::shared_ptr<cvedia::rt::rest::client::model::Coordinate>>> to_remote_coordinates_v2(std::vector<std::vector<cvedia::rt::Point2f>> const& points)
{
	std::vector<std::vector<std::shared_ptr<cvedia::rt::rest::client::model::Coordinate>>> coordinates;
	for (auto const& p : points)
	{
		coordinates.push_back(to_remote_coordinates(p));
	}

	return coordinates;
}


std::vector<std::shared_ptr<cvedia::rt::rest::client::model::Class>> to_remote_classes(std::unordered_set<cvedia::rt::iface::SecuRT::Classes> const& classes)
{
	std::vector<std::shared_ptr<cvedia::rt::rest::client::model::Class>> classesV;
	for (auto const& c : classes)
	{
		auto const cls = std::make_shared<cvedia::rt::rest::client::Class>();
		cls->setValue(static_cast<cvedia::rt::rest::client::Class::eClass>(c));
		classesV.push_back(cls);
	}

	return classesV;
}

std::shared_ptr<cvedia::rt::rest::client::model::Direction> to_remote_direction(int const direction)
{
	auto dir = std::make_shared<cvedia::rt::rest::client::Direction>();
	dir->setValue(static_cast<cvedia::rt::rest::client::Direction::eDirection>(direction));
	return dir;
}
#endif

int securt_create_instance(int const contextHandle, char const* instanceName)
{
	return core_create_instance(contextHandle, instanceName, "default", "securt", 0);
}

int securt_destroy_instance(int const contextHandle)
{
	return core_delete_instance(contextHandle);
}

int securt_create_crossing_area(int const contextHandle, char const* areaId, char const* name, float const* coords, int const numPoints, int const* classes, int const classesSize, int const ignoreStationaryObjects, int areaType, double const* color)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateNumberOfPoints(numPoints) || !validateClasses(classes, classesSize) || !validateAreaEvent(areaType))
	{
		return -1;
	}

	// is it a local instance?
	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const area = coords_to_points(coords, numPoints);
		auto const classesSet = classes_to_set(classes, classesSize);
		auto const colorArray = to_color_array(color);

		return (securt->createCrossingArea(areaId, name, area, classesSet, ignoreStationaryObjects, static_cast<cvedia::rt::iface::SecuRT::AreaEvent>(areaType), colorArray) ? 1 : -1);
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const remoteAreaId = utility::conversions::to_string_t(areaId);

		auto const areaCrossingWriteModel = std::make_shared<cvedia::rt::rest::client::model::AreaCrossingWrite>();
		areaCrossingWriteModel->setName(utility::conversions::to_string_t(name));
		areaCrossingWriteModel->setIgnoreStationaryObjects(ignoreStationaryObjects);
		areaCrossingWriteModel->setAreaEvent({ to_remote_area_event(areaType) });
		areaCrossingWriteModel->setCoordinates(to_remote_coordinates(coords_to_points(coords, numPoints)));
		areaCrossingWriteModel->setClasses(to_remote_classes(classes_to_set(classes, classesSize)));
		areaCrossingWriteModel->setColor(to_color_vector(color));

		auto const task = context->connections.secuRTAreasApi->securtCreateCrossingAreaWithIdV1(remoteInstanceId, remoteAreaId, areaCrossingWriteModel);
		auto const status = task.wait();
		if (status == pplx::task_status::completed)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	catch (std::exception const&ex)
	{
		std::string const str = ex.what();
		LOGE << str;
		return -1;
	}
#else
	return -1;
#endif
}

int securt_create_intrusion_area(int const contextHandle, char const* areaId, char const* name, float const* coords, int const numPoints, int const* classes, int const classesSize, double const* color)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateNumberOfPoints(numPoints) || !validateClasses(classes, classesSize))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const area = coords_to_points(coords, numPoints);
		auto const classesSet = classes_to_set(classes, classesSize);
		auto const colorArray = to_color_array(color);

		return (securt->createIntrusionArea(areaId, name, area, classesSet, colorArray) ? 1 : -1);
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const remoteAreaId = utility::conversions::to_string_t(areaId);

		auto const areaIntrusionWriteModel = std::make_shared<cvedia::rt::rest::client::model::AreaIntrusionWrite>();		
		areaIntrusionWriteModel->setName(utility::conversions::to_string_t(name));
		areaIntrusionWriteModel->setCoordinates(to_remote_coordinates(coords_to_points(coords, numPoints)));
		areaIntrusionWriteModel->setClasses(to_remote_classes(classes_to_set(classes, classesSize)));
		areaIntrusionWriteModel->setColor(to_color_vector(color));

		auto const task = context->connections.secuRTAreasApi->securtCreateIntrusionAreaWithIdV1(remoteInstanceId, remoteAreaId, areaIntrusionWriteModel);
		auto const status = task.wait();
		if (status == pplx::task_status::completed)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	catch (std::exception const&)
	{
		return -1;
	}
#else
	return -1;
#endif
}

int securt_create_loitering_area(int const contextHandle, char const* areaId, char const* name, float const* coords, int const numPoints, int const* classes, int const classesSize, int const seconds, double const* color)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateNumberOfPoints(numPoints) || !validateClasses(classes, classesSize))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const area = coords_to_points(coords, numPoints);
		auto const classesSet = classes_to_set(classes, classesSize);
		auto const colorArray = to_color_array(color);

		return (securt->createLoiteringArea(areaId, name, area, classesSet, seconds, colorArray) ? 1 : -1);
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const remoteAreaId = utility::conversions::to_string_t(areaId);

		auto const areaLoiteringWriteModel = std::make_shared<cvedia::rt::rest::client::model::AreaLoiteringWrite>();
		areaLoiteringWriteModel->setName(utility::conversions::to_string_t(name));
		areaLoiteringWriteModel->setCoordinates(to_remote_coordinates(coords_to_points(coords, numPoints)));
		areaLoiteringWriteModel->setClasses(to_remote_classes(classes_to_set(classes, classesSize)));
		areaLoiteringWriteModel->setSeconds(seconds);
		areaLoiteringWriteModel->setColor(to_color_vector(color));

		auto const task = context->connections.secuRTAreasApi->securtCreateLoiteringAreaWithIdV1(remoteInstanceId, remoteAreaId, areaLoiteringWriteModel);
		auto const status = task.wait();
		if (status == pplx::task_status::completed)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	catch (std::exception const&)
	{
		return -1;
	}
#else
	return -1;
#endif
}

int securt_create_crowding_area(int const contextHandle, char const* areaId, char const* name, float const* coords, int const numPoints, int const* classes, int const classesSize, int const objectCnt, double const* color)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateNumberOfPoints(numPoints) || !validateClasses(classes, classesSize))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const area = coords_to_points(coords, numPoints);
		auto const classesSet = classes_to_set(classes, classesSize);
		auto const colorArray = to_color_array(color);

		return (securt->createCrowdingArea(areaId, name, area, classesSet, objectCnt, colorArray) ? 1 : -1);
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const remoteAreaId = utility::conversions::to_string_t(areaId);

		auto const areaCrowdingWriteModel = std::make_shared<cvedia::rt::rest::client::model::AreaCrowdingWrite>();
		areaCrowdingWriteModel->setName(utility::conversions::to_string_t(name));
		areaCrowdingWriteModel->setCoordinates(to_remote_coordinates(coords_to_points(coords, numPoints)));
		areaCrowdingWriteModel->setClasses(to_remote_classes(classes_to_set(classes, classesSize)));
		areaCrowdingWriteModel->setObjectCount(objectCnt);
		areaCrowdingWriteModel->setColor(to_color_vector(color));

		auto const task = context->connections.secuRTAreasApi->securtCreateCrowdingAreaWithIdV1(remoteInstanceId, remoteAreaId, areaCrowdingWriteModel);
		auto const status = task.wait();
		if (status == pplx::task_status::completed)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	catch (std::exception const& e)
	{
		std::string const str = e.what();
		LOGE << str;
		return -1;
	}
#else
	return -1;
#endif
}

int securt_create_line_crossing(int const contextHandle, char const* areaId, char const* name, float const* coords, int const numPoints, int const* classes, int const classesSize, int direction, double const* color)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateNumberOfPoints(numPoints) || !validateClasses(classes, classesSize) || !validateDirection(direction))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const area = coords_to_points(coords, numPoints);
		auto const classesSet = classes_to_set(classes, classesSize);
		auto const colorArray = to_color_array(color);

		return (securt->createLineCrossing(areaId, name, area, classesSet, static_cast<cvedia::rt::iface::SecuRT::Direction>(direction), colorArray) ? 1 : -1);
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const remoteAreaId = utility::conversions::to_string_t(areaId);

		auto const lineCrossingWriteModel = std::make_shared<cvedia::rt::rest::client::model::LineCrossingWrite>();
		lineCrossingWriteModel->setName(utility::conversions::to_string_t(name));
		lineCrossingWriteModel->setCoordinates(to_remote_coordinates(coords_to_points(coords, numPoints)));
		lineCrossingWriteModel->setClasses(to_remote_classes(classes_to_set(classes, classesSize)));
		lineCrossingWriteModel->setDirection(to_remote_direction(direction));
		lineCrossingWriteModel->setColor(to_color_vector(color));

		auto const task = context->connections.secuRTLinesApi->securtCreateLineCrossingWithIdV1(remoteInstanceId, remoteAreaId, lineCrossingWriteModel);
		auto const status = task.wait();
		if (status == pplx::task_status::completed)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	catch (std::exception const&)
	{
		return -1;
	}
#else
	return -1;
#endif
}

int securt_create_armed_person_area(int const contextHandle, char const* areaId, char const* name, float const* coords, int numPoints, double const* color)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateNumberOfPoints(numPoints))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const area = coords_to_points(coords, numPoints);
		auto const colorArray = to_color_array(color);

		return (securt->createArmedPersonArea(areaId, name, area, colorArray) ? 1 : -1);
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const remoteAreaId = utility::conversions::to_string_t(areaId);

		auto const areaArmedPersonWriteModel = std::make_shared<cvedia::rt::rest::client::model::AreaArmedPersonWrite>();
		areaArmedPersonWriteModel->setName(utility::conversions::to_string_t(name));
		areaArmedPersonWriteModel->setCoordinates(to_remote_coordinates(coords_to_points(coords, numPoints)));
		areaArmedPersonWriteModel->setColor(to_color_vector(color));

		auto const task = context->connections.secuRTAreasApi->securtCreateArmedPersonAreaWithIdV1(remoteInstanceId, remoteAreaId, areaArmedPersonWriteModel);
		auto const status = task.wait();
		if (status == pplx::task_status::completed)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	catch (std::exception const&)
	{
		return -1;
	}
#else
	return -1;
#endif
}

int securt_create_object_left_area(int const handle, char const* areaId, char const* name, float const* coords,
	int numPoints, int minDuration, double const* color)
{
	GET_CONTEXT(context, handle);

	if (!validateNumberOfPoints(numPoints))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const area = coords_to_points(coords, numPoints);
		auto const colorArray = to_color_array(color);

		return (securt->createObjectLeftArea(areaId, name, area, minDuration, colorArray) ? 1 : -1);
	}

#if WITH_SECURT_REST_CLIENT
	// TODO
	return -1;
#else
	return -1;
#endif
}

int securt_create_object_removed_area(int const handle, char const* areaId, char const* name, float const* coords,
	int numPoints, int minDuration, double const* color)
{
	GET_CONTEXT(context, handle);

	if (!validateNumberOfPoints(numPoints))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const area = coords_to_points(coords, numPoints);
		auto const colorArray = to_color_array(color);

		return (securt->createObjectRemovedArea(areaId, name, area, minDuration, colorArray) ? 1 : -1);
	}

#if WITH_SECURT_REST_CLIENT
	// TODO
	return -1;
#else
	return -1;
#endif
}

int securt_create_tailgating_line(int const handle, char const* lineId, char const* name, float const* coords,
	int numPoints, int const* classes, int classesSize, int maxDuration, int direction, double const* color)
{
	GET_CONTEXT(context, handle);

	if (!validateNumberOfPoints(numPoints) || !validateClasses(classes, classesSize))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const area = coords_to_points(coords, numPoints);
		auto const classesSet = classes_to_set(classes, classesSize);
		auto const colorArray = to_color_array(color);

		return (securt->createTailgatingLine(lineId, name, area, classesSet, maxDuration, static_cast<cvedia::rt::iface::SecuRT::Direction>(direction), colorArray) ? 1 : -1);
	}

#if WITH_SECURT_REST_CLIENT
	// TODO
	return -1;
#else
	return -1;
#endif
}

int securt_create_fallen_person_area(int const handle, char const* areaId, char const* name, float const* coords,
	int numPoints, double const* color)
{
	GET_CONTEXT(context, handle);

	if (!validateNumberOfPoints(numPoints))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const area = coords_to_points(coords, numPoints);
		auto const colorArray = to_color_array(color);

		return (securt->createFallenPersonArea(areaId, name, area, colorArray) ? 1 : -1);
	}

	#if WITH_SECURT_REST_CLIENT
	// TODO
	return -1;
#else
	return -1;
#endif
}

int securt_create_license_control_area(int const handle, char const* areaId, char const* name, float const* coords,
	int numPoints, double const* color)
{
	GET_CONTEXT(context, handle);

	if (!validateNumberOfPoints(numPoints))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const area = coords_to_points(coords, numPoints);
		auto const colorArray = to_color_array(color);

		return (securt->createLicensePlateAccessControlArea(areaId, name, area, colorArray) ? 1 : -1);
	}

#if WITH_SECURT_REST_CLIENT
	// TODO
	return -1;
#else
	return -1;
#endif
}

int securt_delete_area(int const contextHandle, char const* areaId)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->deleteArea(areaId) ? 1 : -1;
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const remoteAreaId = utility::conversions::to_string_t(areaId);

		auto const task = context->connections.secuRTAreasApi->securtDeleteAreaV1(remoteInstanceId, remoteAreaId);
		auto const status = task.wait();
		if (status == pplx::task_status::completed)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	catch (std::exception const&)
	{
		return -1;
	}
#else
	return -1;
#endif
}

int securt_delete_line(int const contextHandle, char const* lineId)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->deleteLine(lineId) ? 1 : -1;
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const remoteLineId = utility::conversions::to_string_t(lineId);

		auto const task = context->connections.secuRTLinesApi->securtDeleteLineV1(remoteInstanceId, remoteLineId);
		auto const status = task.wait();
		if (status == pplx::task_status::completed)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	catch (std::exception const&)
	{
		return -1;
	}
#else
	return -1;
#endif
}

int securt_set_appearance_search(int const handle, int const mode)
{
	GET_CONTEXT(context, handle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->setAppearanceSearchMode(static_cast<cvedia::rt::iface::SecuRT::AppearanceMode>(mode)) ? 1 : -1;
	}

#if WITH_SECURT_REST_CLIENT
	// TODO: implement REST API
	return -1;
#else
	return -1;
#endif
}

int securt_set_motion_area(int const contextHandle, float const* coords, int const numPoints)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const area = coords_to_points(coords, numPoints);
		return securt->setMotionArea(area) ? 1 : -1;
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());


		auto const securtSetMotionAreaV1Request = std::make_shared<cvedia::rt::rest::client::model::SecurtSetMotionAreaV1_request>();
		securtSetMotionAreaV1Request->setCoordinates(to_remote_coordinates(coords_to_points(coords, numPoints)));

		auto const task = context->connections.secuRTApi->securtSetMotionAreaV1(remoteInstanceId, securtSetMotionAreaV1Request);
		auto const status = task.wait();
		if (status == pplx::task_status::completed)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	catch (std::exception const&)
	{
		return -1;
	}
#else
	return -1;
#endif
}

int securt_set_exclusion_areas(int const contextHandle, float const* coords, int const numPoints)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const areas = coords_to_points_v2(coords, numPoints);
		return securt->setExclusionAreas(areas) ? 1 : -1;
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const request = std::make_shared<cvedia::rt::rest::client::model::SecurtSetExclusionAreasV1_request>();
		request->setAreas(to_remote_coordinates_v2(coords_to_points_v2(coords, numPoints)));

		auto const task = context->connections.secuRTApi->securtSetExclusionAreasV1(remoteInstanceId, request);
		auto const status = task.wait();
		if (status == pplx::task_status::completed)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
	catch (std::exception const&)
	{
		return -1;
	}
#else
	return -1;
#endif
}

// TODO: this is now baked into various area/line operations, so we don't need this anymore?
int securt_calculate_motion_mask(int const contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->calculateMotionMask();
		return 1;
	}

	// remote instance	
	return -1;
}

int securt_set_auto_restart(int const contextHandle, int const state)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateBool(state))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		auto const ctrl = api::instances::getInstanceController(context->instanceUuid);
		if (ctrl)
		{
			return ctrl.value()->setAutoRestart(static_cast<bool>(state)) ? 1 : -1;
		}
		return -1;
	}

	// remote instance
	// TODO: implement this
	return -1;
}
