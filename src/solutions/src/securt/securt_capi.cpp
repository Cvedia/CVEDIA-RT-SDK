#include "rtconfig.h"

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

#include "cvalue.h"
#include "api/convert.h"
#include "api/thread.h"
#include "builtin/functions.h"
#include "core/core_capi.h"
#include "interface/instance.h"
#include "interface/instance_controller.h"
#include "interface/solution.h"

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

//auto succeed = [](int a) { (void)a; return -1; };

int securt_create_instance(int const contextHandle, char const* instanceName)
{
	return core_create_instance(contextHandle, instanceName, "default", "securt", 0);
}

int securt_destroy_instance(int const contextHandle)
{
	return core_delete_instance(contextHandle);
}

bool validateBool(int value)
{
	if (value != 0 && value != 1)
	{
		LOGE << "Invalid boolean value " << value;
		return false;
	}
	return true;
}

bool validateEnum(int value, int min, int max)
{
	return value >= min && value <= max;
}

bool validateDirection(int direction)
{
	if (!validateEnum(direction, 1, 3))
	{
		LOGE << "Invalid direction value " << direction;
		return false;
	}
	return true;
}

bool validateNumberOfPoints(int numPoints)
{
	if (numPoints % 2 != 0)
	{
		LOGE << "Number of points must be a multiple of 2";
		return false;
	}
	return true;
}

bool validateClasses(int const* classes, int const classesSize)
{
	for (int i = 0; i < classesSize; ++i)
	{
		if (!validateEnum(classes[i], 1, 5))
		{
			LOGE << "Invalid class value " << classes[i];
			return false;
		}
	}
	return true;
}

bool validateMode(int mode)
{
	if (!validateEnum(mode, 1, 2))
	{
		LOGE << "Invalid mode value " << mode;
		return false;
	}
	return true;
}

bool validateAreaEvent(int areaEvent)
{
	if (!validateEnum(areaEvent, 1, 3))
	{
		LOGE << "Invalid area event value " << areaEvent;
		return false;
	}
	return true;
}

bool validateSensitivity(int sensitivity)
{
	if (!validateEnum(sensitivity, 1, 3))
	{
		LOGE << "Invalid sensitivity value " << sensitivity;
		return false;
	}
	return true;
}

bool validateModality(int modality)
{
	if (!validateEnum(modality, 1, 2))
	{
		LOGE << "Invalid modality value " << modality;
		return false;
	}
	return true;
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

int secure_create_license_plate_access_control_area(int const handle, char const* areaId, char const* name, float const* coords,
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

#if WITH_SECURT_REST_CLIENT
std::shared_ptr<cvedia::rt::rest::client::DetectorMode> to_remote_detector_mode(int const detectorMode)
{
	auto securtDetectorMode = std::make_shared<cvedia::rt::rest::client::DetectorMode>();

	securtDetectorMode->setValue(static_cast<cvedia::rt::rest::client::DetectorMode::eDetectorMode>(detectorMode));
	return securtDetectorMode;
}

std::shared_ptr<cvedia::rt::rest::client::Sensitivity> to_remote_sensitivity(int const sensitivity)
{
	auto securtSensitivity = std::make_shared<cvedia::rt::rest::client::Sensitivity>();

	securtSensitivity->setValue(static_cast<cvedia::rt::rest::client::Sensitivity::eSensitivity>(sensitivity));
	return securtSensitivity;
}

std::shared_ptr<cvedia::rt::rest::client::SensorModality> to_remote_sensor_modality(int const sensorModality)
{
	auto securtSensorModality = std::make_shared<cvedia::rt::rest::client::SensorModality>();

	securtSensorModality->setValue(static_cast<cvedia::rt::rest::client::SensorModality::eSensorModality>(sensorModality));
	return securtSensorModality;
}
#endif

int securt_update_instance_options(int const contextHandle, int const detectorMode, int const detectionSensitivity, int const movementSensitivity, int const sensorModality, int const frameRateLimit, int const metadataMode, int const statisticsMode, int const diagnosticsMode, int const debugMode)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateMode(detectorMode) || !validateSensitivity(detectionSensitivity) || !validateSensitivity(movementSensitivity) || !validateModality(sensorModality) ||
		!validateBool(metadataMode) || !validateBool(statisticsMode) || !validateBool(diagnosticsMode) || !validateBool(debugMode))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		if (securt->getDetectorMode() != static_cast<cvedia::rt::iface::SecuRT::Mode>(detectorMode)) {
			securt->setDetectorMode(static_cast<cvedia::rt::iface::SecuRT::Mode>(detectorMode));
		}
		if (securt->getDetectionSensitivity() != static_cast<cvedia::rt::iface::SecuRT::Sensitivity>(detectionSensitivity)) {
			securt->setDetectionSensitivity(static_cast<cvedia::rt::iface::SecuRT::Sensitivity>(detectionSensitivity));
		}
		if (securt->getMovementSensitivity() != static_cast<cvedia::rt::iface::SecuRT::Sensitivity>(movementSensitivity)) {
			securt->setMovementSensitivity(static_cast<cvedia::rt::iface::SecuRT::Sensitivity>(movementSensitivity));
		}
		if (securt->getSensorModality() != static_cast<cvedia::rt::iface::SecuRT::Modality>(sensorModality)) {
			securt->setSensorModality(static_cast<cvedia::rt::iface::SecuRT::Modality>(sensorModality));
		}
		if (securt->getFrameRateLimit() != frameRateLimit) {
			securt->setFrameRateLimit(frameRateLimit);
		}
		securt->setMetadataMode(static_cast<bool>(metadataMode));
		securt->setStatisticsMode(static_cast<bool>(statisticsMode));
		securt->setDiagnosticsMode(static_cast<bool>(diagnosticsMode));
		securt->setDebugMode(static_cast<bool>(debugMode));
		return 1;
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());


		auto const securtInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::SecuRTInstanceWrite>();
		securtInstanceWrite->setDetectorMode(to_remote_detector_mode(detectorMode));
		securtInstanceWrite->setDetectionSensitivity(to_remote_sensitivity(detectionSensitivity));
		securtInstanceWrite->setMovementSensitivity(to_remote_sensitivity(movementSensitivity));
		securtInstanceWrite->setSensorModality(to_remote_sensor_modality(sensorModality));
		securtInstanceWrite->setFrameRateLimit(static_cast<double>(frameRateLimit));
		securtInstanceWrite->setDiagnosticsMode(static_cast<bool>(diagnosticsMode));
		securtInstanceWrite->setMetadataMode(static_cast<bool>(metadataMode));
		securtInstanceWrite->setStatisticsMode(static_cast<bool>(statisticsMode));
		securtInstanceWrite->setDebugMode(static_cast<bool>(debugMode));

		auto const task = context->connections.secuRTApi->securtPatchInstanceV1(remoteInstanceId, securtInstanceWrite);
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
	catch (std::exception const& ex)
	{
		std::string const str = ex.what();
		LOGE << str;
		return -1;
	}
#else
	return -1;
#endif
}

int securt_set_detector_mode(int const contextHandle, int const mode)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateMode(mode))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->setDetectorMode(static_cast<cvedia::rt::iface::SecuRT::Mode>(mode));
		return 0;
	}

	// remote instance
	// TODO: can't be implemented as there's no REST API for this and won't be. use update_instance_options instead
	return -1;
}

int securt_set_detection_sensitivity(int const contextHandle, int const sensitivity)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateSensitivity(sensitivity))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->setDetectionSensitivity(static_cast<cvedia::rt::iface::SecuRT::Sensitivity>(sensitivity));
		return 0;
	}

	// remote instance
	// TODO: can't be implemented as there's no REST API for this and won't be. use update_instance_options instead
	return -1;
}

int securt_set_movement_sensitivity(int const contextHandle, int const sensitivity)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateSensitivity(sensitivity))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->setMovementSensitivity(static_cast<cvedia::rt::iface::SecuRT::Sensitivity>(sensitivity));
		return 0;
	}

	// remote instance
	// TODO: can't be implemented as there's no REST API for this and won't be. use update_instance_options instead
	return -1;
}

#if WITH_SECURT_REST_CLIENT
int from_remote_detector_mode(std::shared_ptr<cvedia::rt::rest::client::DetectorMode> const& detectorMode)
{
	return static_cast<int>(detectorMode->getValue());
}

int from_remote_sensitivity(std::shared_ptr<cvedia::rt::rest::client::Sensitivity> const& sensitivity)
{
	return static_cast<int>(sensitivity->getValue());
}

int from_remote_sensor_modality(std::shared_ptr<cvedia::rt::rest::client::SensorModality> const& sensorModality)
{
	return static_cast<int>(sensorModality->getValue());
}
#endif

int securt_get_instance(int const contextHandle, char** instanceId, char** instanceName, char** instanceVersion, int* detectorMode, int* detectionSensitivity, int* movementSensitivity,
	int* sensorModality, int* frameRateLimit, int* metadataMode, int* statisticsMode, int* diagnosticsMode, int* debugMode)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		*instanceId = copyString(securt->getInstanceUuid().toString());
		*instanceName = copyString(securt->getInstanceName());
		*instanceVersion = copyString(securt->getVersion());

		*detectorMode = static_cast<int>(securt->getDetectorMode());
		*detectionSensitivity = static_cast<int>(securt->getDetectionSensitivity());
		*movementSensitivity = static_cast<int>(securt->getMovementSensitivity());
		*sensorModality = static_cast<int>(securt->getSensorModality());
		*frameRateLimit = securt->getFrameRateLimit();
		*metadataMode = securt->isMetadataEnabled() ? 1 : 0;
		*statisticsMode = securt->isStatisticsEnabled() ? 1 : 0;
		*diagnosticsMode = securt->isDiagnosticsEnabled() ? 1 : 0;
		*debugMode = securt->isDebugMode() ? 1 : 0;		

		return 1;
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());


		auto const task = context->connections.secuRTApi->securtGetInstanceV1(remoteInstanceId).then(
		[&](const std::shared_ptr<cvedia::rt::rest::client::SecuRTInstanceRead>& instanceRead)
		{
			*instanceId = copyString(utility::conversions::to_utf8string(instanceRead->getId()));
			*instanceName = copyString(utility::conversions::to_utf8string(instanceRead->getName()));
			*instanceVersion = copyString(utility::conversions::to_utf8string(instanceRead->getVersion()));

			*detectorMode = from_remote_detector_mode(instanceRead->getDetectorMode());
			*detectionSensitivity = from_remote_sensitivity(instanceRead->getDetectionSensitivity());
			*movementSensitivity = from_remote_sensitivity(instanceRead->getMovementSensitivity());
			*sensorModality = from_remote_sensor_modality(instanceRead->getSensorModality());
			*frameRateLimit = static_cast<int>(instanceRead->getFrameRateLimit());
			*metadataMode = instanceRead->isMetadataMode() ? 1 : 0;
			*statisticsMode = instanceRead->isStatisticsMode() ? 1 : 0;
			*diagnosticsMode = instanceRead->isDiagnosticsMode() ? 1 : 0;
			*debugMode = instanceRead->isDebugMode() ? 1 : 0;
		});

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

char* securt_get_instance_id(int contextHandle)
{
	GET_CONTEXT_STR(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT_STR(securt);

		return copyString(securt->getInstanceUuid().toString());
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		char* instanceId;

		auto const task = context->connections.secuRTApi->securtGetInstanceV1(remoteInstanceId).then(
			[&](const std::shared_ptr<cvedia::rt::rest::client::SecuRTInstanceRead>& instanceRead)
			{
				instanceId = copyString(utility::conversions::to_utf8string(instanceRead->getId()));
			});

		auto const status = task.wait();
		if (status == pplx::task_status::completed)
		{
			return instanceId;
		}
		else
		{
			return nullptr;
		}
	}
	catch (std::exception const&)
	{
		return nullptr;
	}
#else
	return nullptr;
#endif
}

char* securt_get_instance_name(int contextHandle)
{
	GET_CONTEXT_STR(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT_STR(securt);

		return copyString(securt->getInstanceName());
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		char* instanceName;

		auto const task = context->connections.secuRTApi->securtGetInstanceV1(remoteInstanceId).then(
			[&](const std::shared_ptr<cvedia::rt::rest::client::SecuRTInstanceRead>& instanceRead)
			{
				instanceName = copyString(utility::conversions::to_utf8string(instanceRead->getName()));
			});

		auto const status = task.wait();
		if (status == pplx::task_status::completed)
		{
			return instanceName;
		}
		else
		{
			return nullptr;
		}
	}
	catch (std::exception const&)
	{
		return nullptr;
	}
#else
	return nullptr;
#endif
}

char* securt_get_version(int contextHandle)
{
	GET_CONTEXT_STR(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT_STR(securt);

		return copyString(securt->getVersion());
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		char* version;

		auto const task = context->connections.secuRTApi->securtGetInstanceV1(remoteInstanceId).then(
			[&](const std::shared_ptr<cvedia::rt::rest::client::SecuRTInstanceRead>& instanceRead)
			{
				version = copyString(utility::conversions::to_utf8string(instanceRead->getVersion()));
			});

		auto const status = task.wait();
		if (status == pplx::task_status::completed)
		{
			return version;
		}
		else
		{
			return nullptr;
		}
	}
	catch (std::exception const&)
	{
		return nullptr;
	}
#else
	return nullptr;
#endif
}


int securt_get_track_count(int contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->getTrackCount();
	}

	// remote instance
	// TODO: can't be implemented as there's no REST API for this and won't be. use get_instance_stats instead
	return -1;
}

long long int securt_get_frames_processed(int contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->getFramesProcessed();
	}

	// remote instance
	// TODO: can't be implemented as there's no REST API for this and won't be. use get_instance_stats instead
	return -1;
}

float securt_get_latency(int contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->getLatency();
	}

	// remote instance
	// TODO: can't be implemented as there's no REST API for this and won't be. use get_instance_stats instead
	return -1;
}

float securt_get_current_frame_rate(int contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->getCurrentFrameRate();
	}

	// remote instance
	// TODO: can't be implemented as there's no REST API for this and won't be. use get_instance_stats instead
	return -1;
}

int securt_set_frame_rate_limit(int contextHandle, int frameRateLimit)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->setFrameRateLimit(frameRateLimit);
		return 0;
	}

	// remote instance
	// TODO: can't be implemented as there's no REST API for this and won't be. use update_instance_options instead
	return -1;
}

int securt_get_frame_rate_limit(int contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->getFrameRateLimit();
	}

	// remote instance
	// TODO: can't be implemented as there's no REST API for this and won't be. use get_instance instead
	return -1;
}

int securt_get_recommended_frame_rate(int contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->getRecommendedFrameRate();
	}

	// remote instance
	// TODO: can't be implemented as there's no REST API for this and won't be. use get_instance instead
	return -1;
}

int securt_need_data(int contextHandle, long long int const currentFrameTime)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->needData(std::chrono::milliseconds(currentFrameTime));
	}

	// remote instance
	// TODO: can't be implemented as there's no REST API for this and won't be. not applicable for h264/h265 frames
	return -1;
}

int securt_set_sensor_modality(int const contextHandle, int modality)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateModality(modality))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->setSensorModality(static_cast<cvedia::rt::iface::SecuRT::Modality>(modality));
		return 1;
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const securtInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::SecuRTInstanceWrite>();
		securtInstanceWrite->setSensorModality(to_remote_sensor_modality(modality));

		auto const task = context->connections.secuRTApi->securtPatchInstanceV1(remoteInstanceId, securtInstanceWrite);
		auto const status = task.wait();

		return status == pplx::task_status::completed ? 1 : -1;
	}
	catch (std::exception const& ex)
	{
		std::string const str = ex.what();
		LOGE << str;
		return -1;
	}
#else
	return -1;
#endif
}

int securt_set_diagnostics_mode(int const contextHandle, int diagnostics)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateBool(diagnostics))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->setDiagnosticsMode(static_cast<bool>(diagnostics));
		return 1;
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const securtInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::SecuRTInstanceWrite>();
		securtInstanceWrite->setDiagnosticsMode(static_cast<bool>(diagnostics));

		auto const task = context->connections.secuRTApi->securtPatchInstanceV1(remoteInstanceId, securtInstanceWrite);
		auto const status = task.wait();

		return status == pplx::task_status::completed ? 1 : -1;
	}
	catch (std::exception const& ex)
	{
		std::string const str = ex.what();
		LOGE << str;
		return -1;
	}
#else
	return -1;
#endif
}

int securt_set_debug_mode(int const contextHandle, int debugMode)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateBool(debugMode))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->setDebugMode(static_cast<bool>(debugMode));
		return 1;
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const securtInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::SecuRTInstanceWrite>();
		securtInstanceWrite->setDebugMode(static_cast<bool>(debugMode));

		auto const task = context->connections.secuRTApi->securtPatchInstanceV1(remoteInstanceId, securtInstanceWrite);
		auto const status = task.wait();

		return status == pplx::task_status::completed ? 1 : -1;
	}
	catch (std::exception const& ex)
	{
		std::string const str = ex.what();
		LOGE << str;
		return -1;
	}
#else
	return -1;
#endif
}

int securt_set_statistics_mode(int const contextHandle, int statisticsMode)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateBool(statisticsMode))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->setStatisticsMode(static_cast<bool>(statisticsMode));
		return 1;
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const securtInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::SecuRTInstanceWrite>();

		securtInstanceWrite->setStatisticsMode(static_cast<bool>(statisticsMode));

		auto const task = context->connections.secuRTApi->securtPatchInstanceV1(remoteInstanceId, securtInstanceWrite);
		auto const status = task.wait();

		return status == pplx::task_status::completed ? 1 : -1;
	}
	catch (std::exception const& ex)
	{
		std::string const str = ex.what();
		LOGE << str;
		return -1;
	}
#else
	return -1;
#endif
}

int securt_set_metadata_mode(int const contextHandle, int metadataMode)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateBool(metadataMode))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->setMetadataMode(static_cast<bool>(metadataMode));
		return 1;
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const securtInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::SecuRTInstanceWrite>();
		
		securtInstanceWrite->setMetadataMode(static_cast<bool>(metadataMode));

		auto const task = context->connections.secuRTApi->securtPatchInstanceV1(remoteInstanceId, securtInstanceWrite);
		auto const status = task.wait();

		return status == pplx::task_status::completed ? 1 : -1;
	}
	catch (std::exception const& ex)
	{
		std::string const str = ex.what();
		LOGE << str;
		return -1;
	}
#else
	return -1;
#endif
}

int securt_set_tentative_tracks(int const contextHandle, int enabled)
{
	GET_CONTEXT(context, contextHandle);

	if (!validateBool(enabled))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->setTentativeTracks(static_cast<bool>(enabled));
		return 1;
	}

	// remote instance
	// TODO: implement this - why is this not in REST API? is it still used?
	return -1;
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

int securt_is_instance_running(int const contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		return api::instances::isRunning(context->instanceUuid) ? 1 : 0;
	}

	// remote instance	
	return -1;
}

int securt_set_config_value(int const handle, char const* key, char const* value)
{
	GET_CONTEXT(context, handle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		// parse value into nlohmann
		nlohmann::json json;

		auto cv = api::convert::cvalueFromJsonString(value).value_or(nullptr);
		if (!cv)
		{
			return -1;
		}

		return (securt->setConfigValue(key, std::move(*cv)) ? 1 : -1);
	}

	// remote instance
#if WITH_SECURT_REST_CLIENT
	try
	{
		//auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		//auto const securtConfigValueWrite = std::make_shared<cvedia::rt::rest::client::model::SecuRTConfigValueWrite>();
		//securtConfigValueWrite->setKey(utility::conversions::to_string_t(key));
		//securtConfigValueWrite->setValue(utility::conversions::to_string_t(value));

		//auto const task = context->connections.secuRTApi->securtPatchConfigValueV1(remoteInstanceId, utility::conversions::to_string_t(key), securtConfigValueWrite);
		//auto const status = task.wait();
		//if (status == pplx::task_status::completed)
		//{
		//	return 1;
		//}
		//else
		//{
		//	return -1;
		//}

		// TODO : DISCUSSION WITH MAURIZIO AND EMRE

		return -1;
	}
	catch (std::exception const&)
	{
		return -1;
	}
#else
	LOGE << "Remote mode is not supported in this build";
	return -1;
#endif
}

int securt_set_render_preset(int const contextHandle, char const* preset)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return (securt->setRenderPreset(preset) ? 1 : -1);
	}

	// remote instance
	// TODO: implement this
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

int securt_set_blocking_readahead_queue(int const handle, int const state)
{
	GET_CONTEXT(context, handle);

	if (!validateBool(state))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->setBlockingReadaheadQueue(static_cast<bool>(state));
	}

	return 1;
}
