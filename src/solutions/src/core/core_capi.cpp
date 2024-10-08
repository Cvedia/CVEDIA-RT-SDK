#include "rtconfig.h"
#include "rterror.h"

#include <array>
#include <cstring>
#include <nlohmann/json.hpp>

#include "core/core_capi.h"
#include "cvalue.h"
#include "api/rt.h"
#include "api/logging.h"
#include "api/convert.h"
#include "api/remote_discovery.h"
#include "api/instances.h"
#include "api/thread.h"
#include "api/rt.h"
#include "api/solutions.h"
#include "interface/instance_controller.h"
#include "interface/solution_manager.h"
#include "interface/solution.h"
#include "interface/instance.h"

#if WITH_CORE_REST_CLIENT
#include <rt_rest_client/api/RTCoreApi.h>
#endif

//#include <plog/Init.h>

#include "api/capi_base.h"
#include "api/system.h"
#include "common/functions.h"
#include "common/context.h"

using namespace cvedia::rt;

// static std::once_flag loggerInitFlag;

#define GET_CTRL(var) \
	auto var = cvedia::rt::api::instances::getInstanceController(context->instanceUuid).value_or(nullptr); \
	if (var == nullptr) { \
		return -1; \
	}

#define GET_CTRL_STR(var) \
	auto var = cvedia::rt::api::instances::getInstanceController(context->instanceUuid).value_or(nullptr); \
	if (var == nullptr) { \
		return nullptr; \
	}

#define GET_CTRL_AND_MGR(varCtrl, varMgr) \
	auto varCtrl = cvedia::rt::api::instances::getInstanceController(context->instanceUuid).value_or(nullptr); \
	if (varCtrl == nullptr) { \
		return RT_C_ERROR_OPERATIONAL; \
	} \
	RT_C_TRY(auto varMgr, varCtrl->getSolutionManagerOrCreate()); \
	if (varMgr == nullptr) { \
		return RT_C_ERROR_OPERATIONAL; \
	}

#define GET_CTRL_AND_MGR_STR(varCtrl, varMgr) \
	auto varCtrl = cvedia::rt::api::instances::getInstanceController(context->instanceUuid).value_or(nullptr); \
	if (varCtrl == nullptr) { \
		return nullptr; \
	} \
	RT_C_TRY(auto varMgr, varCtrl->getSolutionManagerOrCreate()); \
	if (varMgr == nullptr) { \
		return nullptr; \
	}

#define HANDLE_ERROR_DEFAULT(res) \
	if (res) { \
		return RT_C_SUCCESS; \
	} \
	if (res.error() == RTErrc::OperationFailed) \
	{ \
		return RT_C_ERROR_OPERATIONAL; \
	} \
	if (res.error() == RTErrc::InstanceNotRunning) \
	{ \
		return RT_C_ERROR_INSTANCE_NOT_RUNNING; \
	} \
	return RT_C_ERROR_OPERATIONAL;

#define HANDLE_REMOTE_RESPONSE(onSuccess, onFail) \
	if (status == pplx::task_status::completed) \
	{ \
		return onSuccess; \
	} \
	else \
	{ \
		LOGD << "Remote task failed with status: " << status; \
		return onFail; \
	} \

#define HANDLE_REMOTE_RESPONSE_OLD HANDLE_REMOTE_RESPONSE(1, -1)
#define HANDLE_REMOTE_RESPONSE_DEFAULT HANDLE_REMOTE_RESPONSE(RT_C_SUCCESS, RT_C_ERROR_OPERATIONAL)


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

#if WITH_CORE_REST_CLIENT
std::shared_ptr<cvedia::rt::rest::client::DetectorMode> to_remote_detector_mode(int const detectorMode)
{
	auto coreDetectorMode = std::make_shared<cvedia::rt::rest::client::DetectorMode>();

	coreDetectorMode->setValue(static_cast<cvedia::rt::rest::client::DetectorMode::eDetectorMode>(detectorMode));
	return coreDetectorMode;
}

std::shared_ptr<cvedia::rt::rest::client::Sensitivity> to_remote_sensitivity(int const sensitivity)
{
	auto coreSensitivity = std::make_shared<cvedia::rt::rest::client::Sensitivity>();

	coreSensitivity->setValue(static_cast<cvedia::rt::rest::client::Sensitivity::eSensitivity>(sensitivity));
	return coreSensitivity;
}

std::shared_ptr<cvedia::rt::rest::client::SensorModality> to_remote_sensor_modality(int const sensorModality)
{
	auto coreSensorModality = std::make_shared<cvedia::rt::rest::client::SensorModality>();

	coreSensorModality->setValue(static_cast<cvedia::rt::rest::client::SensorModality::eSensorModality>(sensorModality));
	return coreSensorModality;
}

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

int core_create_context(char const* instanceUuid, char const* remoteIp)
{
	// std::call_once(loggerInitFlag, []() {
	// 		if (api::logging::getLogger()) {
	// 			init(api::logging::getSeverity(), api::logging::getLogger());
	// 		}
	// 	}
	// );

	return cvedia::rt::ContextStore::createContext(instanceUuid, remoteIp);
}

int core_delete_context(int const contextHandle)
{
	return cvedia::rt::ContextStore::deleteContext(contextHandle);
}

void core_reset()
{
	cvedia::rt::ContextStore::reset();
}

int core_is_alive(int const contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_is_alive called for " << context->instanceUuid.toString();

	// is it a local instance?
	if (context->remoteIp.empty())
	{
		return api::isRunning() ? 1 : -1;
	}

#if WITH_CORE_REST_CLIENT

	try
	{
		auto const task = context->connections.apiClient->callApi(utility::conversions::to_string_t("/openapi.yaml"), web::http::methods::GET, {}, {}, {}, {}, {}, {});
		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	LOGE << "Remote mode is not supported in this build";
	(void)contextHandle;
	return -1;
#endif
}

void core_enable_remote_mode()
{
	cvedia::rt::api::remote_discovery::start();
}

void core_disable_remote_mode()
{
	cvedia::rt::api::remote_discovery::stop();
}

char* core_find_remote_server() {
	auto resp = cvedia::rt::api::remote_discovery::getAvailableServer();
	if (!resp)
	{
		return nullptr;
	}

	return copyString(resp.value());
}

int core_create_instance(int const contextHandle, char const* instanceName, char const* instanceGroup, char const* solutionId, int const persistent)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_create_instance called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		//LOGV << "Creating local instance for " << context->instanceUuid.toString() << " / " << instanceName << " / " << instanceGroup << " / " << persistent;

		try {
			auto model = api::instances::InstanceWriteModel::createDefault();
			model.instanceId = context->instanceUuid;
			model.instanceName = instanceName ? instanceName : model.instanceName;
			model.solutionId = solutionId ? solutionId : model.solutionId;
			model.groupName = instanceGroup ? instanceGroup : model.groupName;
			model.persistent = persistent > 0;

			RT_C_TRY(auto ctrl, api::instances::createInstanceController(model));

			return 1;
		}
		catch (std::exception const& ex)
		{
			LOGE << "Remote call failed with error: " << ex.what();
			return -1;
		}
	}
	else
	{
#if WITH_CORE_REST_CLIENT
		// if it's not, create a remote instance using previously established connection
		
		LOGV << "Creating remote instance using: " << context->remoteIp << " for " << context->instanceUuid.toString() << " / " << instanceName << " / " << instanceGroup << " / " << persistent;
		
		try
		{
			LOGV << "core_create_instance/remote";

			auto const req = std::make_shared<cvedia::rt::rest::client::model::InstanceWrite>();
			req->setName(utility::conversions::to_string_t(instanceName));
			LOGV << "core_create_instance/remote: set name";

			req->setGroup(utility::conversions::to_string_t(instanceGroup));
			LOGV << "core_create_instance/remote: set group";

			req->setPersistent(static_cast<bool>(persistent));
			LOGV << "core_create_instance/remote: set persistent";

			if (solutionId) {
				req->setSolution(utility::conversions::to_string_t(solutionId));
				LOGV << "core_create_instance/remote: set solution";
			}

			auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

			auto const task = context->connections.coreApi->coreCreateInstance2V1(remoteInstanceId, req);
			LOGV << "core_create_instance/remote: called coreApi->coreCreateInstance2V1";

			auto const status = task.wait();
			LOGV << "core_create_instance/remote: task returned with status " << status;

			HANDLE_REMOTE_RESPONSE_OLD
		}
		catch (std::exception const& ex)
		{
			LOGE << "Remote call failed with error: " << ex.what();
			return -1;
		}
#else
		LOGE << "Remote mode is not supported in this build";
		return -1;
#endif
	}
}

int core_delete_instance(int const contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_delete_instance called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		return cvedia::rt::api::instances::deleteInstanceController(context->instanceUuid) ? 1 : -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const task = context->connections.coreApi->coreDeleteInstanceV1(remoteInstanceId);
		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_load(int const contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_load called for " << context->instanceUuid.toString();

	// is it a local instance?
	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_CATCH(ctrl->getSolutionManagerOrCreate());
		return (ctrl->loadInstance() ? 1 : -1);
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const task = context->connections.coreApi->coreLoadInstanceV1(remoteInstanceId);
		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_unload(int const contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_unload called for " << context->instanceUuid.toString();

	// is it a local instance?
	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_CATCH(ctrl->getSolutionManagerOrCreate());
		return (ctrl->unloadInstance() ? 1 : -1);
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const task = context->connections.coreApi->coreUnloadInstanceV1(remoteInstanceId);
		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_start(int const contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_start called for " << context->instanceUuid.toString();

	// is it a local instance?
	if (context->remoteIp.empty())
	{
		return api::instances::startInstance(context->instanceUuid) ? 1 : -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const task = context->connections.coreApi->coreStartInstanceV1(remoteInstanceId);
		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_stop(int const contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_stop called for " << context->instanceUuid.toString();

	// is it a local instance?
	if (context->remoteIp.empty())
	{
		static constexpr bool allowRestart = false;
		return api::instances::stopInstance(context->instanceUuid, allowRestart) ? 1 : -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const task = context->connections.coreApi->coreStopInstanceV1(remoteInstanceId);
		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_restart(int const contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_restart called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		return api::instances::restartInstance(context->instanceUuid) ? 1 : -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const task = context->connections.coreApi->coreRestartInstanceV1(remoteInstanceId);
		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	LOGE << "Remote mode is not supported in this build";
	return -1;
#endif
}

int core_push_raw_frame(int const contextHandle, void const* buffer, int const width, int const height, unsigned long long int const timestampMs)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_push_raw_frame called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL_AND_MGR(ctrl, mgr);
		auto const res = mgr->pushRawFrame(buffer, width, height, std::chrono::milliseconds(timestampMs));
		HANDLE_ERROR_DEFAULT(res);
	}

	// remote instance
	// TODO: this won't be implemented as we don't want to pass uncompressed frames to REST API
	return RT_C_ERROR_OPERATIONAL;
}

int core_push_compressed_frame(int const contextHandle, void const* buffer, unsigned long long int const dataSize, unsigned long long int const timestampMs)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_push_compressed_frame called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL_AND_MGR(ctrl, mgr);
		auto const res = mgr->pushCompressedFrame(
			static_cast<unsigned char const*>(buffer), dataSize, std::chrono::milliseconds(timestampMs)
		);
		HANDLE_ERROR_DEFAULT(res);
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const httpContent = std::make_shared<cvedia::rt::rest::client::HttpContent>();
		auto const iStream = std::make_shared<MemoryStream>(static_cast<char const*>(buffer), dataSize);

		httpContent->setData(iStream);
		httpContent->setContentDisposition(utility::conversions::to_string_t("form-data"));

		auto const task = context->connections.coreApi->corePostCompressedFrameV1(remoteInstanceId, httpContent, static_cast<int64_t>(timestampMs));
		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE_DEFAULT
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return RT_C_ERROR_OPERATIONAL;
	}
#else
	return RT_C_ERROR_OPERATIONAL;
#endif
}

int core_push_h264_frame(int const contextHandle, void const* buffer, unsigned long long int const dataSize, unsigned long long int const timestampMs)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_push_h264_frame called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL_AND_MGR(ctrl, mgr);
		auto const res = mgr->pushH264Frame(
			static_cast<unsigned char const*>(buffer), dataSize, std::chrono::milliseconds(timestampMs)
		);
		HANDLE_ERROR_DEFAULT(res);
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const httpContent = std::make_shared<cvedia::rt::rest::client::HttpContent>();
		auto const iStream = std::make_shared<MemoryStream>(static_cast<char const*>(buffer), dataSize);

		httpContent->setData(iStream);
		httpContent->setContentDisposition(utility::conversions::to_string_t("form-data"));

		auto const task = context->connections.coreApi->corePostEncodedFrameV1(remoteInstanceId, utility::conversions::to_string_t("h264"), httpContent, static_cast<int64_t>(timestampMs));
		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE_DEFAULT
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return RT_C_ERROR_OPERATIONAL;
	}
#else
	return RT_C_ERROR_OPERATIONAL;
#endif
}

int core_push_h265_frame(int const contextHandle, void const* buffer, unsigned long long int const dataSize, unsigned long long int const timestampMs)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_push_h265_frame called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL_AND_MGR(ctrl, mgr);
		auto const res = mgr->pushH265Frame(
			static_cast<unsigned char const*>(buffer), dataSize, std::chrono::milliseconds(timestampMs)
		);
		HANDLE_ERROR_DEFAULT(res);
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const httpContent = std::make_shared<cvedia::rt::rest::client::HttpContent>();
		auto const iStream = std::make_shared<MemoryStream>(static_cast<char const*>(buffer), dataSize);

		httpContent->setData(iStream);
		httpContent->setContentDisposition(utility::conversions::to_string_t("form-data"));

		auto const task = context->connections.coreApi->corePostEncodedFrameV1(remoteInstanceId, utility::conversions::to_string_t("h265"), httpContent, static_cast<int64_t>(timestampMs));
		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE_DEFAULT
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return RT_C_ERROR_OPERATIONAL;
	}
#else
	return RT_C_ERROR_OPERATIONAL;
#endif
}

int core_get_instance(int const contextHandle, char** instanceId, char** instanceName, char** group,
	char** solutionId, char** solutionName, int* loaded, int* running, int* persistent, float* fps,
	char** instanceVersion, int* detectorMode, int* detectionSensitivity, int* movementSensitivity,
	int* sensorModality, int* frameRateLimit, int* metadataMode, int* statisticsMode,
	int* diagnosticsMode, int* debugMode, int* readOnly, int* autoStart, int* autoRestart, int* systemInstance)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_get_instance called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		if (mgr == nullptr)
		{
			return -1;
		}

		*instanceId = copyString(ctrl->getInstanceId().toString());
		*instanceName = copyString(ctrl->getDisplayName());
		*group = copyString(ctrl->getGroupName());
		*solutionId = copyString("");
		*solutionName = copyString("");
		*fps = 0.0;
		*loaded = (ctrl->isInstanceLoaded() ? 1 : 0);
		*running = (ctrl->isInstanceRunning() ? 1 : 0);
		*persistent = (ctrl->isPersistent() ? 1 : 0);
		*readOnly = (ctrl->getReadOnly() ? 1 : 0);
		*autoStart = (ctrl->getAutoStart() ? 1 : 0);
		*autoRestart = (ctrl->getAutoRestart() ? 1 : 0);
		*systemInstance = (ctrl->isSystemInstance() ? 1 : 0);
		*instanceVersion = copyString(mgr->getVersion());
		*detectorMode = static_cast<int>(mgr->getDetectorMode());
		*detectionSensitivity = static_cast<int>(mgr->getDetectionSensitivity());
		*movementSensitivity = static_cast<int>(mgr->getMovementSensitivity());
		*sensorModality = static_cast<int>(mgr->getSensorModality());
		*frameRateLimit = mgr->getFrameRateLimit();
		*metadataMode = mgr->isMetadataEnabled() ? 1 : 0;
		*statisticsMode = mgr->isStatisticsEnabled() ? 1 : 0;
		*diagnosticsMode = mgr->isDiagnosticsEnabled() ? 1 : 0;
		*debugMode = mgr->isDebugMode() ? 1 : 0;

		if (auto instSol = ctrl->getSolution()) {
			if (auto const& sol = instSol.value().lock()) {
				*solutionId = copyString(sol->getId());
				*solutionName = copyString(sol->getName());
			}
		}

		if (auto instExp = ctrl->getInstance()) {
			if (auto const& inst = instExp.value().lock()) {
				*fps = inst->getFPS();
			}
		}

		return 1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const task = context->connections.coreApi->coreGetInstanceV1(remoteInstanceId).then(
			[&](std::shared_ptr<cvedia::rt::rest::client::InstanceRead> const& instanceRead)
			{
				*instanceId = copyString(utility::conversions::to_utf8string(instanceRead->getInstanceId()));
				*instanceName = copyString(utility::conversions::to_utf8string(instanceRead->getDisplayName()));
				*group = copyString(utility::conversions::to_utf8string(instanceRead->getGroup()));
				*solutionId = copyString(utility::conversions::to_utf8string(instanceRead->getSolutionId()));
				*solutionName = copyString(utility::conversions::to_utf8string(instanceRead->getSolutionName()));
				*fps = instanceRead->getFps();
				*loaded = instanceRead->isLoaded() ? 1 : 0;
				*running = instanceRead->isRunning() ? 1 : 0;
				*persistent = instanceRead->isPersistent() ? 1 : 0;
				*readOnly = instanceRead->isReadOnly() ? 1 : 0;
				*autoStart = instanceRead->isAutoStart() ? 1 : 0;
				*autoRestart = instanceRead->isAutoRestart() ? 1 : 0;
				*systemInstance = instanceRead->isSystemInstance() ? 1 : 0;
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
		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_update_instance_options(int const contextHandle,
	char const* instanceName, char const* group, int const persistent,
	int const detectorMode, int const detectionSensitivity,
	int const movementSensitivity, int const sensorModality,
	int const frameRateLimit, int const metadataMode, int const statisticsMode,
	int const diagnosticsMode, int const debugMode, int const autoStart, int const autoRestart)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_update_instance_options called for " << context->instanceUuid.toString();

	if (!validateBool(persistent) || !validateMode(detectorMode) || !validateSensitivity(detectionSensitivity) || !validateSensitivity(movementSensitivity) || 
		!validateModality(sensorModality) || !validateBool(metadataMode) || !validateBool(statisticsMode) || !validateBool(diagnosticsMode) || !validateBool(debugMode) ||
		!validateBool(autoStart) || !validateBool(autoRestart))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		if (mgr == nullptr)
		{
			return -1;
		}

		auto model = api::instances::InstanceWriteModel{};

		if (ctrl->getDisplayName() != instanceName) {
			model.instanceName = instanceName;
		}
		if (ctrl->getGroupName() != group) {
			model.groupName = group;
		}
		if (ctrl->isPersistent() != static_cast<bool>(persistent)) {
			model.persistent = static_cast<bool>(persistent);
		}
		if (mgr->getDetectorMode() != static_cast<cvedia::rt::types::DetectorMode>(detectorMode)) {
			model.detectorMode = static_cast<cvedia::rt::types::DetectorMode>(detectorMode);
		}
		if (mgr->getDetectionSensitivity() != static_cast<cvedia::rt::types::Sensitivity>(detectionSensitivity)) {
			model.detectionSensitivity = static_cast<cvedia::rt::types::Sensitivity>(detectionSensitivity);
		}
		if (mgr->getMovementSensitivity() != static_cast<cvedia::rt::types::Sensitivity>(movementSensitivity)) {
			model.movementSensitivity = static_cast<cvedia::rt::types::Sensitivity>(movementSensitivity);
		}
		if (mgr->getSensorModality() != static_cast<cvedia::rt::types::Modality>(sensorModality)) {
			model.sensorModality = static_cast<cvedia::rt::types::Modality>(sensorModality);
		}
		if (mgr->getFrameRateLimit() != frameRateLimit) {
			model.frameRateLimit = frameRateLimit;
		}

		model.metadataMode = static_cast<bool>(metadataMode);
		model.statisticsMode = static_cast<bool>(statisticsMode);
		model.diagnosticsMode = static_cast<bool>(diagnosticsMode);
		model.debugMode = static_cast<bool>(debugMode);
		model.autoStart = static_cast<bool>(autoStart);
		model.autoRestart = static_cast<bool>(autoRestart);

		RT_C_CATCH(api::instances::updateInstanceController(ctrl, model));

		return 1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const instanceWrite = std::make_shared<cvedia::rt::rest::client::model::InstanceWrite>();
		instanceWrite->setName(utility::conversions::to_string_t(instanceName));
		instanceWrite->setGroup(utility::conversions::to_string_t(group));
		instanceWrite->setPersistent(static_cast<bool>(persistent));
		instanceWrite->setDetectorMode(to_remote_detector_mode(detectorMode));
		instanceWrite->setDetectionSensitivity(to_remote_sensitivity(detectionSensitivity));
		instanceWrite->setMovementSensitivity(to_remote_sensitivity(movementSensitivity));
		instanceWrite->setSensorModality(to_remote_sensor_modality(sensorModality));
		instanceWrite->setFrameRateLimit(static_cast<double>(frameRateLimit));
		instanceWrite->setDiagnosticsMode(static_cast<bool>(diagnosticsMode));
		instanceWrite->setMetadataMode(static_cast<bool>(metadataMode));
		instanceWrite->setStatisticsMode(static_cast<bool>(statisticsMode));
		instanceWrite->setDebugMode(static_cast<bool>(debugMode));
		instanceWrite->setAutoStart(static_cast<bool>(autoStart));
		instanceWrite->setAutoRestart(static_cast<bool>(autoRestart));

		auto const task = context->connections.coreApi->corePatchInstanceV1(remoteInstanceId, instanceWrite);
		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

EXPORT int core_set_input_to_rtsp(int const contextHandle, char const* rtspUrl)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_set_input_to_rtsp called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());
		if (mgr)
		{
			return mgr->setInputToRtsp(rtspUrl) ? 1 : -1;
		}
		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const req = std::make_shared<cvedia::rt::rest::client::model::CoreSetInputV1_request>();
		auto inputType = std::make_shared<cvedia::rt::rest::client::model::Input>();
		inputType->setValue(cvedia::rt::rest::client::model::Input::eInput::Input_RTSP);
		req->setType(inputType);
		req->setUri(utility::conversions::to_string_t(rtspUrl));

		auto const task = context->connections.coreApi->coreSetInputV1(remoteInstanceId, req);
		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

EXPORT int core_set_input_to_manual(int const contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_set_input_to_manual called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());
		if (mgr)
		{
			return mgr->setInputToManual() ? 1 : -1;
		}
		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const req = std::make_shared<cvedia::rt::rest::client::model::CoreSetInputV1_request>();
		auto inputType = std::make_shared<cvedia::rt::rest::client::model::Input>();
		inputType->setValue(cvedia::rt::rest::client::model::Input::eInput::Input_MANUAL);
		req->setType(inputType);

		auto const task = context->connections.coreApi->coreSetInputV1(remoteInstanceId, req);
		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

EXPORT int core_set_input_orientation(int const contextHandle, int const orientation)
{
	GET_CONTEXT(context, contextHandle);

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());
		if (mgr)
		{
			return mgr->setInputOrientation(orientation) ? 1 : -1;
		}
		return -1;
	}

#if WITH_CORE_REST_CLIENT
	return -1;
#else
	return -1;
#endif
}

EXPORT char* core_enable_hls_output(int const contextHandle)
{
	GET_CONTEXT_STR(context, contextHandle);

	LOGV << "core_enable_hls_output called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL_STR(ctrl);
		RT_C_TRY_STR(auto mgr, ctrl->getSolutionManagerOrCreate());
		return mgr ? copyString(mgr->enableHlsOutput()) : nullptr;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const requestModel = std::make_shared<cvedia::rt::rest::client::model::CoreSetOutputHlsV1_request>();
		requestModel->setEnabled(true);

		char* uri;

		auto const task = context->connections.coreApi->coreSetOutputHlsV1(remoteInstanceId, requestModel).then(
			[&](std::shared_ptr<cvedia::rt::rest::client::CoreSetOutputHlsV1_200_response> const& response)
			{
				uri = copyString(utility::conversions::to_utf8string(response->getUri()));
			}
		);

		auto const status = task.wait();
		return status == pplx::task_status::completed ? uri : nullptr;
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return nullptr;
	}
#endif
	return nullptr;
}

EXPORT int core_disable_hls_output(int const contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_disable_hls_output called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());
		if (mgr)
		{
			return mgr->disableHlsOutput() ? 1 : -1;
		}
		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const requestModel = std::make_shared<cvedia::rt::rest::client::model::CoreSetOutputHlsV1_request>();
		requestModel->setEnabled(false);

		auto const task = context->connections.coreApi->coreSetOutputHlsV1(remoteInstanceId, requestModel);
		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#endif
	return -1;
}

EXPORT int core_enable_rtsp_output(int const contextHandle, char const* rtspUrl)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_enable_rtsp_output called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());
		if (mgr)
		{
			return mgr->enableRtspOutput(rtspUrl) ? 1 : -1;
		}
		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const requestModel = std::make_shared<cvedia::rt::rest::client::model::CoreSetOutputRtspV1_request>();
		requestModel->setUri(utility::conversions::to_string_t(rtspUrl));
		requestModel->setEnabled(true);

		auto const task = context->connections.coreApi->coreSetOutputRtspV1(remoteInstanceId, requestModel);
		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#endif
	return -1;
}

EXPORT int core_disable_rtsp_output(int const contextHandle, char const* rtspUrl)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_disable_rtsp_output called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());
		if (mgr)
		{
			return mgr->disableRtspOutput(rtspUrl) ? 1 : -1;
		}
		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const requestModel = std::make_shared<cvedia::rt::rest::client::model::CoreSetOutputRtspV1_request>();
		requestModel->setUri(utility::conversions::to_string_t(rtspUrl));
		requestModel->setEnabled(false);

		auto const task = context->connections.coreApi->coreSetOutputRtspV1(remoteInstanceId, requestModel);
		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#endif
	return -1;
}

int core_consume_events(int const contextHandle, char** outJson)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_consume_events called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		auto const eventsV = mgr->consumeAllEvents();
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

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());
		auto const data = context->connections.coreSseApi->coreConsumeEventsSse(context->instanceUuid.toString());

		if (!data.empty())
		{
			*outJson = copyString(data);
			return 1;
		}

		return 0;
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

char* core_get_instance_id(int contextHandle)
{
	GET_CONTEXT_STR(context, contextHandle);

	LOGV << "core_get_instance_id called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL_STR(ctrl);
		(void)ctrl->getSolutionManagerOrCreate();
		return copyString(ctrl->getInstanceId().toString());
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		char* instanceId;

		auto const task = context->connections.coreApi->coreGetInstanceV1(remoteInstanceId).then(
			[&](std::shared_ptr<cvedia::rt::rest::client::InstanceRead> const& instanceRead)
			{
				instanceId = copyString(utility::conversions::to_utf8string(instanceRead->getInstanceId()));
			});

		auto const status = task.wait();

		HANDLE_REMOTE_RESPONSE(instanceId, nullptr)
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return nullptr;
	}
#else
	return nullptr;
#endif
}

char* core_get_instance_name(int contextHandle)
{
	GET_CONTEXT_STR(context, contextHandle);

	LOGV << "core_get_instance_name called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL_STR(ctrl);
		(void)ctrl->getSolutionManagerOrCreate();
		return copyString(ctrl->getDisplayName());
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		char* instanceName;

		auto const task = context->connections.coreApi->coreGetInstanceV1(remoteInstanceId).then(
			[&](std::shared_ptr<cvedia::rt::rest::client::InstanceRead> const& instanceRead)
			{
				instanceName = copyString(utility::conversions::to_utf8string(instanceRead->getDisplayName()));
			});

		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE(instanceName, nullptr)
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return nullptr;
	}
#else
	return nullptr;
#endif
}

char* core_get_version(int contextHandle)
{
	GET_CONTEXT_STR(context, contextHandle);

	LOGV << "core_get_version called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL_STR(ctrl);
		RT_C_TRY_STR(auto mgr, ctrl->getSolutionManagerOrCreate());
		return mgr ? copyString(mgr->getVersion()) : nullptr;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		char* version;

		auto const task = context->connections.coreApi->coreGetInstanceV1(remoteInstanceId).then(
			[&](std::shared_ptr<cvedia::rt::rest::client::InstanceRead> const& instanceRead)
			{
				version = copyString(utility::conversions::to_utf8string(instanceRead->getVersion()));
			});

		auto const status = task.wait();
		HANDLE_REMOTE_RESPONSE(version, nullptr)
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return nullptr;
	}
#else
	return nullptr;
#endif
}

long long int core_get_frames_processed(int contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_get_frames_processed called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());
		return mgr ? static_cast<long long int>(mgr->getFramesProcessed()) : -1;
	}

	// remote instance
	// TODO: can't be implemented as there's no REST API for this and won't be. use get_instance_stats instead
	return -1;
}

int core_set_input_pixel_limit(int const contextHandle, int pixels)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_set_input_pixel_limit called for " << context->instanceUuid.toString();

	if (pixels < 0)
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		if (mgr)
		{
			mgr->setInputPixelLimit(pixels);
			return 0;
		}

		return -1;

	}

	return -1;
}

int core_get_input_pixel_limit(int const contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_get_input_pixel_limit called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());
		return mgr ? mgr->getInputPixelLimit() : -1;
	}

	return -1;
}

int core_set_frame_rate_limit(int contextHandle, int frameRateLimit)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_set_frame_rate_limit called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		if (mgr)
		{
			mgr->setFrameRateLimit(frameRateLimit);
			return 1;
		}
		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const coreInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::InstanceWrite>();

		coreInstanceWrite->setFrameRateLimit(static_cast<double>(frameRateLimit));

		auto const task = context->connections.coreApi->corePatchInstanceV1(remoteInstanceId, coreInstanceWrite);
		auto const status = task.wait();

		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_get_frame_rate_limit(int contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_get_frame_rate_limit called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());
		return mgr ? mgr->getFrameRateLimit() : -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		int frameRateLimit = -1;

		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const task = context->connections.coreApi->coreGetInstanceV1(remoteInstanceId).then(
			[&](std::shared_ptr<cvedia::rt::rest::client::InstanceRead> const& instanceRead)
			{
				frameRateLimit = static_cast<int>(instanceRead->getFrameRateLimit());
			});

		auto const status = task.wait();

		HANDLE_REMOTE_RESPONSE(frameRateLimit, -1)
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_need_data(int contextHandle, long long int const currentFrameTime)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_need_data called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());
		return mgr ? mgr->needData(std::chrono::milliseconds(currentFrameTime)): -1;
	}

	// remote instance
	// TODO: can't be implemented as there's no REST API for this and won't be. not applicable for h264/h265 frames
	return -1;
}

int core_set_sensor_modality(int const contextHandle, int modality)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_set_sensor_modality called for " << context->instanceUuid.toString();

	if (!validateModality(modality))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		if (mgr)
		{
			mgr->setSensorModality(static_cast<cvedia::rt::types::Modality>(modality));
			return 1;
		}

		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const coreInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::InstanceWrite>();
		coreInstanceWrite->setSensorModality(to_remote_sensor_modality(modality));

		auto const task = context->connections.coreApi->corePatchInstanceV1(remoteInstanceId, coreInstanceWrite);
		auto const status = task.wait();

		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_set_diagnostics_mode(int const contextHandle, int diagnostics)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_set_diagnostics_mode called for " << context->instanceUuid.toString();

	if (!validateBool(diagnostics))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		if (mgr)
		{
			mgr->setDiagnosticsMode(static_cast<bool>(diagnostics));
			return 1;
		}
		
		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const coreInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::InstanceWrite>();
		coreInstanceWrite->setDiagnosticsMode(static_cast<bool>(diagnostics));

		auto const task = context->connections.coreApi->corePatchInstanceV1(remoteInstanceId, coreInstanceWrite);
		auto const status = task.wait();

		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_set_debug_mode(int const contextHandle, int debugMode)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_set_debug_mode called for " << context->instanceUuid.toString();

	if (!validateBool(debugMode))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		if (mgr)
		{
			mgr->setDebugMode(static_cast<bool>(debugMode));
			return 1;
		}

		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const coreInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::InstanceWrite>();
		coreInstanceWrite->setDebugMode(static_cast<bool>(debugMode));

		auto const task = context->connections.coreApi->corePatchInstanceV1(remoteInstanceId, coreInstanceWrite);
		auto const status = task.wait();

		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_set_statistics_mode(int const contextHandle, int statisticsMode)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_set_statistics_mode called for " << context->instanceUuid.toString();

	if (!validateBool(statisticsMode))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		if (mgr)
		{
			mgr->setStatisticsMode(static_cast<bool>(statisticsMode));
			return 1;
		}

		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const coreInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::InstanceWrite>();

		coreInstanceWrite->setStatisticsMode(static_cast<bool>(statisticsMode));

		auto const task = context->connections.coreApi->corePatchInstanceV1(remoteInstanceId, coreInstanceWrite);
		auto const status = task.wait();

		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_set_metadata_mode(int const contextHandle, int metadataMode)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_set_metadata_mode called for " << context->instanceUuid.toString();

	if (!validateBool(metadataMode))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		if (mgr)
		{
			mgr->setMetadataMode(static_cast<bool>(metadataMode));
			return 1;
		}

		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const coreInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::InstanceWrite>();

		coreInstanceWrite->setMetadataMode(static_cast<bool>(metadataMode));

		auto const task = context->connections.coreApi->corePatchInstanceV1(remoteInstanceId, coreInstanceWrite);
		auto const status = task.wait();

		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_set_tentative_tracks(int const contextHandle, int enabled)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_set_tentative_tracks called for " << context->instanceUuid.toString();

	if (!validateBool(enabled))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		if (mgr)
		{
			mgr->setTentativeTracks(static_cast<bool>(enabled));
			return 1;
		}

		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const req = std::make_shared<cvedia::rt::rest::client::model::CoreSetTentativeTracksV1_request>();

		req->setEnabled(static_cast<bool>(enabled));

		auto const task = context->connections.coreApi->coreSetTentativeTracksV1(remoteInstanceId, req);
		auto const status = task.wait();

		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_is_instance_running(int const contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_is_instance_running called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		return api::instances::isRunning(context->instanceUuid) ? 1 : 0;
	}

	// remote instance	
	return -1;
}

int core_set_blocking_readahead_queue(int const handle, int const state)
{
	GET_CONTEXT(context, handle);

	LOGV << "core_set_blocking_readahead_queue called for " << context->instanceUuid.toString();

	if (!validateBool(state))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		if (mgr)
		{
			mgr->setBlockingReadaheadQueue(static_cast<bool>(state));
			return 1;
		}

		return -1;
	}

	return 1;
}

int core_set_config_value(int const handle, char const* key, char const* value)
{
	GET_CONTEXT(context, handle);

	LOGV << "core_set_config_value called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		if (mgr)
		{
			// parse value into nlohmann
			nlohmann::json json;

			auto cv = api::convert::cvalueFromJsonString(value).value_or(nullptr);
			if (!cv)
			{
				return -1;
			}
			
			return (mgr->setConfigValue(key, std::move(*cv)) ? 1 : -1);
		}

		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const req = std::make_shared<cvedia::rt::rest::client::model::CorePostInstanceConfigV1_request>();

		req->setPath(utility::conversions::to_string_t(key));
		req->setJsonValue(utility::conversions::to_string_t(value));

		auto const task = context->connections.coreApi->corePostInstanceConfigV1(remoteInstanceId, req);
		auto const status = task.wait();

		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_set_render_preset(int const contextHandle, char const* preset)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_set_render_preset called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		if (mgr)
		{
			return (mgr->setRenderPreset(preset) ? 1 : -1);
		}

		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const req = std::make_shared<cvedia::rt::rest::client::model::CoreSetRenderPresetV1_request>();

		req->setPresetName(utility::conversions::to_string_t(preset));

		auto const task = context->connections.coreApi->coreSetRenderPresetV1(remoteInstanceId, req);
		auto const status = task.wait();

		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_set_auto_restart(int const contextHandle, int const state)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_set_auto_restart called for " << context->instanceUuid.toString();

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

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const coreInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::InstanceWrite>();

		coreInstanceWrite->setAutoRestart(static_cast<bool>(state));

		auto const task = context->connections.coreApi->corePatchInstanceV1(remoteInstanceId, coreInstanceWrite);
		auto const status = task.wait();

		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_set_auto_start(int const contextHandle, int const state)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_set_auto_start called for " << context->instanceUuid.toString();

	if (!validateBool(state))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		auto const ctrl = api::instances::getInstanceController(context->instanceUuid);
		if (ctrl)
		{
			return ctrl.value()->setAutoStart(static_cast<bool>(state)) ? 1 : -1;
		}
		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const coreInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::InstanceWrite>();

		coreInstanceWrite->setAutoStart(static_cast<bool>(state));

		auto const task = context->connections.coreApi->corePatchInstanceV1(remoteInstanceId, coreInstanceWrite);
		auto const status = task.wait();

		HANDLE_REMOTE_RESPONSE_OLD
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

int core_set_detection_sensitivity(int const contextHandle, int const sensitivity)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_set_detection_sensitivity called for " << context->instanceUuid.toString();

	if (!validateSensitivity(sensitivity))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		if (mgr)
		{
			mgr->setDetectionSensitivity(static_cast<cvedia::rt::types::Sensitivity>(sensitivity));
			return 1;
		}

		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const coreInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::InstanceWrite>();

		coreInstanceWrite->setDetectionSensitivity(to_remote_sensitivity(sensitivity));

		auto const task = context->connections.coreApi->corePatchInstanceV1(remoteInstanceId, coreInstanceWrite);
		auto const status = task.wait();

		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_set_movement_sensitivity(int const contextHandle, int const sensitivity)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_set_movement_sensitivity called for " << context->instanceUuid.toString();

	if (!validateSensitivity(sensitivity))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		if (mgr)
		{
			mgr->setMovementSensitivity(static_cast<cvedia::rt::types::Sensitivity>(sensitivity));
			return 1;
		}
		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const coreInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::InstanceWrite>();

		coreInstanceWrite->setMovementSensitivity(to_remote_sensitivity(sensitivity));

		auto const task = context->connections.coreApi->corePatchInstanceV1(remoteInstanceId, coreInstanceWrite);
		auto const status = task.wait();

		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

int core_set_detector_mode(int const contextHandle, int const mode)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_set_detector_mode called for " << context->instanceUuid.toString();

	if (!validateMode(mode))
	{
		return -1;
	}

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());

		if (mgr)
		{
			mgr->setDetectorMode(static_cast<cvedia::rt::types::DetectorMode>(mode));
			return 1;
		}

		return -1;
	}

#if WITH_CORE_REST_CLIENT
	// remote instance
	try
	{
		auto const remoteInstanceId = utility::conversions::to_string_t(context->instanceUuid.toString());

		auto const coreInstanceWrite = std::make_shared<cvedia::rt::rest::client::model::InstanceWrite>();

		coreInstanceWrite->setDetectorMode(to_remote_detector_mode(mode));

		auto const task = context->connections.coreApi->corePatchInstanceV1(remoteInstanceId, coreInstanceWrite);
		auto const status = task.wait();

		HANDLE_REMOTE_RESPONSE_OLD
	}
	catch (std::exception const& ex)
	{
		LOGE << "Remote call failed with error: " << ex.what();
		return -1;
	}
#else
	return -1;
#endif
}

void core_free_string(void* ptr)
{
	if (ptr != nullptr)
	{
		free(ptr);
	}
}

int core_get_track_count(int contextHandle)
{
	GET_CONTEXT(context, contextHandle);

	LOGV << "core_get_track_count called for " << context->instanceUuid.toString();

	if (context->remoteIp.empty())
	{
		GET_CTRL(ctrl);
		RT_C_TRY(auto mgr, ctrl->getSolutionManagerOrCreate());
		return mgr ? mgr->getTrackCount() : -1;
	}

	// remote instance
	// TODO: can't be implemented as there's no REST API for this and won't be. use get_instance_stats instead
	return -1;
}

char* core_get_preferences()
{
	std::string pref = cvedia::rt::api::system::getPreferences();

	return copyString(pref);
}