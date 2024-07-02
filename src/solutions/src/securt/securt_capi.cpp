
#include "securt/securt_capi.h"
#include "securt/securt_sse_api.h"
#include "common/analytics_manager.h"
#include "securt/securt.h"
#include "common/discovery.h"
#include "api/logging.h"
#include "api/instances.h"

#include <httplib.h>

#if WITH_SECURT_REST_CLIENT
#include <securt_rest_client/ApiConfiguration.h>
#include <securt_rest_client/ApiClient.h>
#include <securt_rest_client/api/SecuRTApi.h>
#include <securt_rest_client/api/SecuRTAreasApi.h>
#include <securt_rest_client/api/SecuRTLinesApi.h>
#endif

#include <array>
#include <cstring>
#include <nlohmann/json.hpp>

#include "cvalue.h"
#include "rtconfig.h"
#include "api/convert.h"
#include "api/thread.h"
#include "builtin/functions.h"
#include "interface/instance.h"
#include "interface/instance_controller.h"
#include "interface/solution.h"

using ConnectionData = struct
{
	std::string ip;

#if WITH_SECURT_REST_CLIENT
	std::shared_ptr<cvedia::rt::rest::client::ApiConfiguration> apiConfig;
	std::shared_ptr<cvedia::rt::rest::client::ApiClient> apiClient;
	std::shared_ptr<cvedia::rt::rest::client::SecuRTApi> secuRTApi;
	std::shared_ptr<cvedia::rt::rest::client::SecuRTAreasApi> secuRTAreasApi;
	std::shared_ptr<cvedia::rt::rest::client::SecuRTLinesApi> secuRTLinesApi;
	std::unique_ptr<cvedia::rt::SecuRtSseApi> secuRtSseApi;
#endif
};

using SecuRtContext = struct
{
	cvedia::rt::Uuid instanceUuid;
	std::string remoteIp;

	ConnectionData connections;
};

std::map<int, std::unique_ptr<SecuRtContext>> contextData_;
std::mutex securtMutex;

int incContextId = 1;

char* copyString(std::string const& str) {
#ifdef _WIN32
	return _strdup(str.c_str());
#else
	return strdup(str.c_str());
#endif
}

#define RT_C_TRY(var, expr) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		return -1; \
	} \
	var = std::move(UNIQUE_NAME(tmp).value())

#define RT_C_CATCH(expr) \
	do { \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		return -1; \
	} \
	} while (0) 

#define GET_CONTEXT(var, handle) \
	if (contextData_.find(handle) == contextData_.end()) { \
		return -1; \
	} \
	auto const& var = contextData_[handle]

#define GET_CONTEXT_STR(var, handle) \
	if (contextData_.find(handle) == contextData_.end()) { \
		return nullptr; \
	} \
	auto const& var = contextData_[handle]

#define GET_SECURT(var) \
	auto var = getSecuRt(context->instanceUuid); \
	if (!var) \
		return -1

#define GET_SECURT_STR(var) \
	auto var = getSecuRt(context->instanceUuid); \
	if (!var) \
		return nullptr 

// TODO: this is a quick & dirty implementation of a std::istream that reads from a memory buffer
// TODO: it's used to pass frame data via CPPRestSDK. We might want to find a better way to do this?
// TODO: there's apparently a boost implementation of this (bufferstream ?)

class MemoryBuffer : public std::streambuf
{
public:
	MemoryBuffer(char const* data, size_t size)
	{
		buffer_.resize(size);
		std::copy_n(data, size, buffer_.begin());
		this->setg(buffer_.data(), buffer_.data(), buffer_.data() + size);
	}

	pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode /*which*/) override
	{
		if (dir == std::ios_base::cur)
			gbump(static_cast<int>(off));
		else if (dir == std::ios_base::end)
			setg(eback(), egptr() + off, egptr());
		else if (dir == std::ios_base::beg)
			setg(eback(), eback() + off, egptr());
		return gptr() - eback();
	}

	pos_type seekpos(pos_type sp, std::ios_base::openmode which) override
	{
		return seekoff(sp - pos_type(static_cast<off_type>(0)), std::ios_base::beg, which);
	}

private:
	std::vector<char> buffer_;
};

class MemoryStream : public std::istream
{
public:
	MemoryStream(char const* data, size_t size) : std::istream(&buffer_), buffer_(data, size)
	{
		rdbuf(&buffer_);
	}

private:
	MemoryBuffer buffer_;
};

std::shared_ptr<cvedia::rt::solution::SecuRT> getSecuRt(cvedia::rt::Uuid const& instanceId) {
	if (auto const& ctrlResp = cvedia::rt::api::instances::getInstanceController(instanceId))
	{
		auto const& ctrl = ctrlResp.value();

		if (auto const& mgrResp = ctrl->getSolutionManager())
		{
			auto const& mgr = mgrResp.value();

			if (auto const& securt = std::dynamic_pointer_cast<cvedia::rt::solution::SecuRT>(mgr))
			{
				return securt;
			}
		}
		else
		{
			// there is no SolutionManager yet, so we create one
			if (auto const& solResp = ctrl->getSolution())
			{
				auto solWeak = solResp.value();
				if (auto const sol = solWeak.lock())
				{
					if (sol->getId() == "securt")
					{
						auto securt = std::make_shared<cvedia::rt::solution::SecuRT>();
						// dont initialize because the instance is already running
						securt->attachToInstanceController(ctrl, false);
						return securt;
					}
				}
			}
		}
	}

	return nullptr;
}

int securt_create_context(char const* instanceUuid, char const* remoteIp)
{
	std::unique_lock<std::mutex> lock(securtMutex);

	auto context = std::make_unique<SecuRtContext>();

	if (instanceUuid == nullptr || instanceUuid[0] == '\0')
	{
		context->instanceUuid = cvedia::rt::Uuid::randomUuid();
	}
	else
	{
		context->instanceUuid = cvedia::rt::Uuid(instanceUuid);
		if (!context->instanceUuid.isValid())
		{
			LOGE << "Invalid instance UUID";
			return -1;
		}
	}

#if WITH_SECURT_REST_CLIENT
	if (remoteIp && remoteIp[0] != '\0')
	{
		std::string remoteIpStr(remoteIp);
		int port = 3546;
		if (strchr(remoteIp, ':'))
		{
			auto const spl = split_str(remoteIp, ':');

			port = atoi(spl[1].c_str());
			remoteIpStr = spl[0];
		}

		context->connections.ip = std::string(remoteIpStr);

		context->connections.apiConfig = std::make_shared<cvedia::rt::rest::client::ApiConfiguration>();
		context->connections.apiConfig->setBaseUrl(utility::conversions::to_string_t("http://" + context->connections.ip + ":" + std::to_string(port)));

		context->connections.apiClient = std::make_shared<cvedia::rt::rest::client::ApiClient>(context->connections.apiConfig);
		context->connections.secuRTApi = std::make_shared<cvedia::rt::rest::client::SecuRTApi>(context->connections.apiClient);
		context->connections.secuRTAreasApi = std::make_shared<cvedia::rt::rest::client::SecuRTAreasApi>(context->connections.apiClient);
		context->connections.secuRTLinesApi = std::make_shared<cvedia::rt::rest::client::SecuRTLinesApi>(context->connections.apiClient);
		context->connections.secuRtSseApi = std::make_unique<cvedia::rt::SecuRtSseApi>("http://" + context->connections.ip + ":" + std::to_string(port));

		context->remoteIp = remoteIpStr;
	}
#else
	(void)remoteIp;
#endif

	contextData_.insert({ incContextId, std::move(context) });

	return incContextId++;
}

int securt_delete_context(int const contextHandle)
{
	std::unique_lock<std::mutex> lock(securtMutex);

	if (contextData_.find(contextHandle) == contextData_.end())
		return 0;

	contextData_.erase(contextHandle);
	return 1;
}

auto succeed = [](int a) { (void)a; return -1; };

int securt_create_instance(int const contextHandle, char const* instanceName)
{
	std::unique_lock<std::mutex> lock(securtMutex);

	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		try {
			(void)cvedia::rt::api::thread::registerThread(cvedia::rt::ThreadType::Worker);

			RT_C_TRY(auto const& ctrl, cvedia::rt::api::instances::createInstanceControllerFromMemory(context->instanceUuid, "securt"));

			ctrl->setDisplayName(instanceName);
			ctrl->loadInstance();

			auto const securt = std::make_shared<cvedia::rt::solution::SecuRT>();

			RT_C_CATCH(securt->attachToInstanceController(ctrl, true));

			return 1;
		}
		catch (std::exception const&)
		{
			return -1;
		}
	}
	else
	{
#if WITH_SECURT_REST_CLIENT
		// if it's not, create a remote instance using previously established connection

		try
		{
			auto const remoteInstanceName = utility::conversions::to_string_t(instanceName);			
			auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

			auto const securtPostInstanceV1Request = std::make_shared<cvedia::rt::rest::client::model::SecurtPostInstanceV1_request>();
			securtPostInstanceV1Request->setName(remoteInstanceName);
			securtPostInstanceV1Request->setInstanceId(remoteInstanceId);

			auto const task = context->connections.secuRTApi->securtPostInstanceV1(securtPostInstanceV1Request);
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
		LOGE << "Remote mode is not supported in this build";
		return -1;
#endif
	}
}

int securt_destroy_instance(int const contextHandle)
{
	std::unique_lock<std::mutex> lock(securtMutex);

	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		if (cvedia::rt::api::instances::deleteInstanceController(context->instanceUuid))
		{
			return 1;
		}

		return -1;
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const task = context->connections.secuRTApi->securtDeleteInstanceV1(remoteInstanceId);
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
	catch (std::exception const&e)
	{
		std::string const str = e.what();
		LOGE << str;
		return -1;
	}
#else
	return -1;
#endif
}

int securt_start(int const contextHandle)
{
	std::unique_lock<std::mutex> lock(securtMutex);

	GET_CONTEXT(context, contextHandle);

	// is it a local instance?
	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);
		return (securt->start() ? 1 : -1);
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const task = context->connections.secuRTApi->securtStartInstanceV1(remoteInstanceId);
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

int securt_stop(int const contextHandle)
{
	std::unique_lock<std::mutex> lock(securtMutex);

	GET_CONTEXT(context, contextHandle);

	// is it a local instance?
	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);
//		std::cout << securt->getConfigurationFile();

		return (securt->stop() ? 1 : -1);
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const task = context->connections.secuRTApi->securtStopInstanceV1(remoteInstanceId);
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

std::unordered_set<cvedia::rt::solution::SecuRT::Classes> classes_to_set(int const* classes, int const classesSize)
{
	std::unordered_set<cvedia::rt::solution::SecuRT::Classes> classesSet;
	for (int i = 0; i < classesSize; ++i)
	{
		classesSet.insert(static_cast<cvedia::rt::solution::SecuRT::Classes>(classes[i]));
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

std::vector<std::shared_ptr<cvedia::rt::rest::client::model::Class>> to_remote_classes(std::unordered_set<cvedia::rt::solution::SecuRT::Classes> const& classes)
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

	if (numPoints % 2 != 0)
	{
		LOGE << "Number of points must be a multiple of 2";
		return -1;
	}

	// is it a local instance?
	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const area = coords_to_points(coords, numPoints);
		auto const classesSet = classes_to_set(classes, classesSize);
		auto const colorArray = to_color_array(color);

		return (securt->createCrossingArea(areaId, name, area, classesSet, ignoreStationaryObjects, static_cast<cvedia::rt::solution::SecuRT::AreaEvent>(areaType), colorArray) ? 1 : -1);
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

	if (numPoints % 2 != 0)
	{
		LOGE << "Number of points must be a multiple of 2";
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

	if (numPoints % 2 != 0)
	{
		LOGE << "Number of points must be a multiple of 2";
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

	if (numPoints % 2 != 0)
	{
		LOGE << "Number of points must be a multiple of 2";
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

	if (numPoints % 2 != 0)
	{
		LOGE << "Number of points must be a multiple of 2";
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const area = coords_to_points(coords, numPoints);
		auto const classesSet = classes_to_set(classes, classesSize);
		auto const colorArray = to_color_array(color);

		return (securt->createLineCrossing(areaId, name, area, classesSet, static_cast<cvedia::rt::solution::SecuRT::Direction>(direction), colorArray) ? 1 : -1);
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

	if (numPoints % 2 != 0)
	{
		LOGE << "Number of points must be a multiple of 2";
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
	// TODO: implement REST API for this first
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

int securt_set_motion_area(int const contextHandle, float const* coords, int const numPoints)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const area = coords_to_points(coords, numPoints);
		return securt->setMotionArea(area);
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
		return securt->setExclusionAreas(areas);
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		// TODO: implement REST API for this first

		//auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());


		//auto const securtSetMotionAreaV1Request = std::make_shared<cvedia::rt::rest::client::model::SecurtSetMotionAreaV1_request>();
		//securtSetMotionAreaV1Request->setCoordinates(to_remote_coordinates(coords_to_points(coords, numPoints)));

		//auto const task = context->connections.secuRTApi->securtSetMotionAreaV1(remoteInstanceId, securtSetMotionAreaV1Request);
		//auto const status = task.wait();
		//if (status == pplx::task_status::completed)
		//{
		//	return 1;
		//}
		//else
		//{
		//	return -1;
		//}

		return -1;
	}
	catch (std::exception const&)
	{
		return -1;
	}
#else
	return -1;
#endif
}

int securt_push_frame(int const contextHandle, void const* buffer, int const width, int const height, unsigned long long int const timestampMs)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->pushFrame(buffer, width, height, std::chrono::milliseconds(timestampMs));
	}

	// remote instance
	// TODO: this won't be implemented as we don't want to pass uncompressed frames to REST API
	return -1;
}

int securt_push_h264_frame(int const contextHandle, void const* buffer, unsigned long long int const dataSize, unsigned long long int const timestampMs)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->pushH264VideoFrame(static_cast<const unsigned char*>(buffer), dataSize, std::chrono::milliseconds(timestampMs));
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());


		auto const httpContent = std::make_shared<cvedia::rt::rest::client::HttpContent>();		
		auto const iStream = std::make_shared<MemoryStream>(static_cast<char const*>(buffer), dataSize);				

		httpContent->setData(iStream);
		httpContent->setContentDisposition(utility::conversions::to_string_t("form-data"));

		auto const task = context->connections.secuRTApi->securtPushFrameV1(remoteInstanceId, utility::conversions::to_string_t("h264"), httpContent, static_cast<int64_t>(timestampMs));
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

int securt_push_h265_frame(int const contextHandle, void const* buffer, unsigned long long int const dataSize, unsigned long long int const timestampMs)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->pushH265VideoFrame(static_cast<const unsigned char*>(buffer), dataSize, std::chrono::milliseconds(timestampMs));
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());


		auto const httpContent = std::make_shared<cvedia::rt::rest::client::HttpContent>();
		auto const iStream = std::make_shared<MemoryStream>(static_cast<char const*>(buffer), dataSize);

		httpContent->setData(iStream);
		httpContent->setContentDisposition(utility::conversions::to_string_t("form-data"));

		auto const task = context->connections.secuRTApi->securtPushFrameV1(remoteInstanceId, utility::conversions::to_string_t("h265"), httpContent, static_cast<int64_t>(timestampMs));
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

int securt_consume_events(int const contextHandle, char** outJson)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		auto const eventsV = securt->consumeAllEvents();
		if (eventsV.empty())		
		{
			*outJson = nullptr;		
			return 0;
		}

		nlohmann::json json = nlohmann::json::array();

		for (auto const& md : eventsV)
		{
			nlohmann::json obj;
			obj["type"] = md->dataType;
			obj["object"] = md->jsonObject;			
			json.push_back(obj);
		}

		*outJson = copyString(json.dump(4));		
		return 1;
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());


		auto const data = context->connections.secuRtSseApi->securtConsumeEventsSse(context->instanceUuid.toString());

		if (!data.empty())
		{
			*outJson = copyString(data);
			return 1;
		}

		//auto const task = context->connections.secuRTApi->securtConsumeMetadataV1(remoteInstanceId).then(
		//	[&](std::vector<std::shared_ptr<cvedia::rt::rest::client::SecurtConsumeMetadataV1_200_response_inner>>& response)
		//{
		//	if (response.empty())
		//	{				
		//		*outJson = nullptr;
		//	}
		//	else
		//	{
		//		nlohmann::json json = nlohmann::json::array();

		//		for (auto const& md : response)
		//		{
		//			nlohmann::json obj;
		//			obj["type"] = utility::conversions::to_utf8string(md->getDataType());
		//			obj["metadata"] = utility::conversions::to_utf8string(md->getJsonObject());
		//			json.push_back(obj);
		//		}

		//		*outJson = copyString(json.dump(4));
		//	}
		//});		

		//auto const status = task.wait();
		//if (status == pplx::task_status::completed)
		//{
		//	return 1;
		//}
		//else
		//{
		//	return -1;
		//}

		return 0;
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

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		if (securt->getDetectorMode() != static_cast<cvedia::rt::solution::SecuRT::Mode>(detectorMode)) {
			securt->setDetectorMode(static_cast<cvedia::rt::solution::SecuRT::Mode>(detectorMode));
		}
		if (securt->getDetectionSensitivity() != static_cast<cvedia::rt::solution::SecuRT::Sensitivity>(detectionSensitivity)) {
			securt->setDetectionSensitivity(static_cast<cvedia::rt::solution::SecuRT::Sensitivity>(detectionSensitivity));
		}
		if (securt->getMovementSensitivity() != static_cast<cvedia::rt::solution::SecuRT::Sensitivity>(movementSensitivity)) {
			securt->setMovementSensitivity(static_cast<cvedia::rt::solution::SecuRT::Sensitivity>(movementSensitivity));
		}
		if (securt->getSensorModality() != static_cast<cvedia::rt::solution::SecuRT::Modality>(sensorModality)) {
			securt->setSensorModality(static_cast<cvedia::rt::solution::SecuRT::Modality>(sensorModality));
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

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->setDetectorMode(static_cast<cvedia::rt::solution::SecuRT::Mode>(mode));
		return 0;
	}

	// remote instance
	// TODO: can't be implemented as there's no REST API for this and won't be. use update_instance_options instead
	return -1;
}

int securt_set_detection_sensitivity(int const contextHandle, int const sensitivity)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->setDetectionSensitivity(static_cast<cvedia::rt::solution::SecuRT::Sensitivity>(sensitivity));
		return 0;
	}

	// remote instance
	// TODO: can't be implemented as there's no REST API for this and won't be. use update_instance_options instead
	return -1;
}

int securt_set_movement_sensitivity(int const contextHandle, int const sensitivity)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->setMovementSensitivity(static_cast<cvedia::rt::solution::SecuRT::Sensitivity>(sensitivity));
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

void securt_free_string(void* ptr)
{
	if (ptr != nullptr)
	{
		free(ptr);
	}
}

int securt_set_input_to_rtsp(int const contextHandle, char const* rtspUrl)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->setInputToRtsp(rtspUrl);
	}

	// remote instance
	// TODO: implement this
	return -1;
}

int securt_set_input_to_manual(int const contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->setInputToManual();
	}

	// remote instance
	// TODO: implement this
	return -1;
}

char* securt_enable_hls_output(int const contextHandle)
{
	GET_CONTEXT_STR(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT_STR(securt);

		return copyString(securt->enableHlsOutput());
	}

	// remote instance
	// TODO: implement this
	return nullptr;
}

int securt_disable_hls_output(int const contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->disableHlsOutput();
	}

	// remote instance
	// TODO: implement this
	return -1;
}

int securt_enable_rtsp_output(int const contextHandle, char const* rtspUri)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->enableRtspOutput(rtspUri);
	}

	// remote instance
	// TODO: implement this
	return -1;
}

int securt_disable_rtsp_output(int const contextHandle, char const* rtspUri)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		return securt->disableRtspOutput(rtspUri);
	}

	// remote instance
	// TODO: implement this
	return -1;
}

int securt_set_sensor_modality(int const contextHandle, int modality)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		// confirm that the modality is supported
		switch (modality)
		{
			case cvedia::rt::solution::SecuRT::Modality::RGB:
			case cvedia::rt::solution::SecuRT::Modality::Thermal:
				break;
		default:
			return -1;
		}

		securt->setSensorModality(static_cast<cvedia::rt::solution::SecuRT::Modality>(modality));
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

	if (diagnostics < 0 || diagnostics > 1)
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

	if (debugMode < 0 || debugMode > 1)
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

	if (statisticsMode < 0 || statisticsMode > 1)
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

	if (metadataMode < 0 || metadataMode > 1)
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
		GET_SECURT(securt);

		return securt->isRunning();
	}

	// remote instance	
	return -1;
}

int securt_restart(int const contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->restart();
		return 1;
	}

#if WITH_SECURT_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const task = context->connections.secuRTApi->securtRestartInstanceV1(remoteInstanceId);
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
	LOGE << "Remote mode is not supported in this build";
	return -1;
#endif
}

/**************************
* Remote connection API
***************************/

int securt_is_alive(int const contextHandle)
{
#if WITH_SECURT_REST_CLIENT
	GET_CONTEXT(context, contextHandle);

	try
	{
		auto const task = context->connections.apiClient->callApi(utility::conversions::to_string_t("/openapi.yaml"), web::http::methods::GET, {}, {}, {}, {}, {}, {});
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
		std::string const t = ex.what();
		LOGE << t;
		return -1;
	}
#else
	LOGE << "Remote mode is not supported in this build";
	(void)contextHandle;
	return -1;
#endif
}

void securt_enable_remote_mode()
{
	cvedia::rt::solution::SecuRT::remoteDiscovery_.start();
}

void securt_disable_remote_mode()
{
	cvedia::rt::solution::SecuRT::remoteDiscovery_.stop();
}

char* securt_find_remote_server() {
	auto resp = cvedia::rt::solution::SecuRT::remoteDiscovery_.getAvailableServer();
	if (!resp)
	{
		return nullptr;
	}

	return copyString(resp.value());
}

void securt_reset()
{
	std::unique_lock<std::mutex> lock(securtMutex);
	contextData_.clear();

	incContextId = 1;
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

int securt_set_blocking_readahead_queue(int const handle, int const state)
{
	GET_CONTEXT(context, handle);

	if (context->remoteIp.empty())
	{
		GET_SECURT(securt);

		securt->setBlockingReadaheadQueue(state);
	}

	return 1;
}