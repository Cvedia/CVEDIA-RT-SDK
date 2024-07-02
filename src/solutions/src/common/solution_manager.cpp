/*
    SPDX-FileCopyrightText: 2023 CVEDIA LTD

    SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#include <iterator>
#include <numeric>
#include "api/logging.h"

#include "common/solution_manager.h"

#include "common/analytics_manager.h"
#include "common/solution_config.h"
#include "securt/securt.h"
#include "interface/instance.h"

#include "cvalue.h"
#include "api/factory.h"
#include "api/solutions.h"
#include "api/system.h"
#include "api/thread.h"
#include "api/logging.h"

#include "builtin/objectregistry.h"
#include "interface/appsrc.h"
#include "interface/buffer.h"
#include "interface/cvdict.h"
#include "interface/input.h"
#include "interface/output.h"
#include "interface/instance_controller.h"
#include "interface/luaworkers.h"
#include "interface/solution.h"

#include "builtin/functions.h"
#include <uri.h>

#include "api/convert.h"
#include "api/instances.h"
#include "api/rt.h"
#include "interface/outputhandler.h"

//#include "tracy/Tracy.hpp"

using namespace cvedia::rt;
using namespace solution;

SolutionManager::SolutionManager() : lastFrameTime_()
{
	analytics_ = std::make_unique<AnalyticsManager>();
	input_ = nullptr;

	//	_initialize(solution);
}

expected<void> SolutionManager::initialize()
{
	// AJ: Disabling free_run_mode causes the video decoders to build up frames and eat all memory
	// keeping it enabled will keep the queues in check
//	_disableFreeRunMode();

	setInputToManual();

	return {};
}

void SolutionManager::_disableFreeRunMode() const
{
	setConfigValue("/Input/free_run_mode", CValue(false));
}

void SolutionManager::_initialize(std::string const& /*solution*/)
{
	//(void)rt::api::thread::registerThread(rt::ThreadType::Worker);

	//auto ctrlExp = api::instances::createInstanceControllerFromMemory(solution);
	//if (!ctrlExp)
	//{
	//	LOGE << "Failed to create instance controller: " << ctrlExp.error().message();
	//	throw std::runtime_error("Failed to create instance controller");
	//}

	//instanceController_ = ctrlExp.value();
	//auto const ctrl = getInstanceController();
	//(void)ctrl->setInstanceUuid(instanceUuid_);

	//auto instance = ctrl->loadInstance().value_or(nullptr);
	//if (!instance)
	//{
	//	throw std::runtime_error("Failed to load instance");
	//}

	//instance_ = instance;

	//	solution_ = ctrl->getSolution().value();
}

float SolutionManager::getLatency() const
{
	if (auto const& instance = getInstance())
	{
		return instance->lua()->getOnRunLatency();
	}

	return 0;
}

std::string SolutionManager::getVersion() const
{
	if (auto const& sol = getSolution())
	{
		return sol->getVersion();
	}
	return "";
}

int SolutionManager::getInputQueueSize() const
{
	if (input_)
	{
		return input_->getReadAheadQueueSize();
	} else
	{
		return 0;
	}
}

int SolutionManager::getDroppedFramesCount() const
{
	if (input_)
	{
		return input_->getDroppedFramesCount();
	}
	else
	{
		return 0;
	}
}

uint64_t SolutionManager::getFramesProcessed() const
{
	return framesProcessed_;
}

int64_t SolutionManager::getStartTime() const
{
	if (auto const& instance = getInstance())
	{
		return instance->getStartTime();
	}
	return 0;
}

int SolutionManager::getTrackCount() const
{
	return activeTrackCount_;
}

/**
 * @brief Returns the current frame rate of the solution based on the speed of frames being supplied, the frame rate limit and processing limitations.
 * @return Achieved frame rate in frames per second
 */
float SolutionManager::getCurrentFrameRate() const
{
	return curSolutionFrameRate_;
}

/**
 * @brief Returns the user specified frame rate processing limit.
 * @return Frame rate in frames per second
 */
int SolutionManager::getFrameRateLimit() const
{
	if (auto const val = getConfigValue("/SolutionManager/frame_rate_limit"))
	{
		if (auto ret = val->getValue<int>())
		{
			return ret.value();
		}
	}
	return 0;
}

void SolutionManager::setFrameRateLimit(int const frameRateLimit)
{
	frameRateLimit_ = frameRateLimit;

	if (input_)
	{
		input_->setFps(static_cast<float>(frameRateLimit_));
	}

	setConfigValue("/SolutionManager/frame_rate_limit", CValue(frameRateLimit));

	if (inputMode_ == InputMode::Rtsp)
	{
		// recreate stream with new frame rate limit
		setInputToRtsp(inputUri_);
	}
}

/**
 * @brief Returns the recommended frame rate as specified by the solution for best results
 * @return Frame rate in frames per second
 */
int SolutionManager::getRecommendedFrameRate() const
{
	if (auto const val = getConfigValue("/SolutionManager/recommended_frame_rate"))
	{
		if (auto ret = val->getValue<int>())
		{
			return ret.value();
		}
	}
	return 10;
}

void SolutionManager::setRecommendedFrameRate(int const fps) const
{
	setConfigValue("/SolutionManager/recommended_frame_rate", CValue(fps));
}

/**
 * @brief Returns whether the solution is currently throttling the frame rate and data should be dropped
 * @return Returns true if more data should be fed to the solution
 */
bool SolutionManager::needData(std::chrono::milliseconds const currentFrameTime)
{
	_calculateThruput(currentFrameTime, true);

	return needData_;
}

bool SolutionManager::isRunning() const
{
	return isRunning_;
}

std::unique_ptr<Event> SolutionManager::consumeEvents()
{
	std::unique_lock<std::mutex> lock(eventsQueueMutex_);

	if (eventsQueue_.empty())
	{
		return nullptr;
	}

	auto events = std::move(eventsQueue_.front());
	eventsQueue_.pop_front();

	return events;
}

std::vector<std::unique_ptr<Event>> SolutionManager::consumeAllEvents()
{
	std::unique_lock<std::mutex> lock(eventsQueueMutex_);

	if (eventsQueue_.empty())
	{
		return {};
	}

	auto eventsVector = std::vector<std::unique_ptr<Event>>();
	while (!eventsQueue_.empty())
	{
		eventsVector.emplace_back(std::move(eventsQueue_.front()));
		eventsQueue_.pop_front();
	}

	return eventsVector;
}

void SolutionManager::_calculateThruput(std::chrono::milliseconds const currentFrameTime, bool checkOnly)
{
	if (lastFrameTime_.count() <= 0)
	{
		lastFrameTime_ = currentFrameTime;
		return;
	}

	if (avgFrameTimes_.size() > 3 && !checkOnly)
	{
		avgFrameTimes_.pop_front();
	}

	auto const timeSinceLastFrame = currentFrameTime - lastFrameTime_;

	// slow down not only on the current frame time but also on the average of the past 3
	// this gave a smoother result much closer to the desired fps
	// most likely due to the fact we're taking timestamps from the camera feed and not just the system time
	auto const curAvgFrameTime = (std::accumulate(avgFrameTimes_.begin(), avgFrameTimes_.end(),
	                                              std::chrono::milliseconds(0)) + timeSinceLastFrame) / (avgFrameTimes_.
		size() + 1);

	auto curFps = 0.0f;

	if (curAvgFrameTime.count() > 0)
	{
		curFps = static_cast<float>(1000 / curAvgFrameTime.count());
	}

	if (!checkOnly)
	{
		curSolutionFrameRate_ = curFps;
	}

	if (frameRateLimit_ > 0)
	{
		auto const frameDurationMs = std::chrono::milliseconds(1000 / frameRateLimit_);
		if (curAvgFrameTime < frameDurationMs)
		{
			needData_ = false;
			return;
		}

		// CRTP-3166: Removing the warning for now, it will be improved in future release by CRTP-3417

		/*
		if (curFps < frameRateLimit_ * SOLUTION_THROTTLE_WARNING_BANDWIDTH)
		{
			if (TIME_IN_S - startTime_ > SOLUTION_THROTTLE_WARNING_WARMUP && TIME_IN_S - lastSolutionFrameRateWarning_ > SOLUTION_THROTTLE_WARNING_INTERVAL) {
				pushDiagnosticEvent(Severity::Warning, "Solution frame-rate is too low", "Solution on " + instanceUuid_.toString() + "(" + instanceName_ + ") is not able to keep up with the requested frame-rate");

				lastSolutionFrameRateWarning_ = TIME_IN_S;
			}
		}
		*/
	}

	if (!checkOnly)
	{
		avgFrameTimes_.push_back(timeSinceLastFrame);
		lastFrameTime_ = currentFrameTime;
	}

	needData_ = true;
}

std::shared_ptr<iface::InstanceController> SolutionManager::getInstanceController() const
{
	if (auto ctrl = instanceController_.lock())
	{
		return ctrl;
	}
	return nullptr;
}

std::shared_ptr<iface::InstanceController> SolutionManager::getInstanceController()
{
	if (auto ctrl = instanceController_.lock())
	{
		return ctrl;
	}
	return nullptr;
}

std::shared_ptr<iface::Instance> SolutionManager::getInstance() const
{
	if (auto const ctrl = getInstanceController())
	{
		if (auto const instResp = ctrl->getInstance())
		{
			if (auto inst = instResp.value().lock())
			{
				return inst;
			}
		}
	}
	return nullptr;
}

std::shared_ptr<iface::Instance> SolutionManager::getInstance()
{
	if (auto const ctrl = getInstanceController())
	{
		if (auto const instResp = ctrl->getInstance())
		{
			if (auto inst = instResp.value().lock())
			{
				return inst;
			}
		}
	}
	return nullptr;
}

std::shared_ptr<iface::Solution> SolutionManager::getSolution() const
{
	if (auto const& inst = getInstance())
	{
		if (auto const& sol = inst->getSolution())
		{
			return sol;
		}
	}
	return nullptr;
}

std::shared_ptr<iface::Solution> SolutionManager::getSolution()
{
	if (auto const& inst = getInstance())
	{
		if (auto sol = inst->getSolution())
		{
			return sol;
		}
	}
	return nullptr;
}

pCValue SolutionManager::getConfigValue(std::string const& path) const
{
	if (auto const instance = getInstance())
	{
		auto const config = instance->config();
		auto lck = config->getReadLock();

		if (auto resp = config->getNodeIfSetUnsafe(path))
		{
			return resp.value();
		}
	}
	else if (auto const ctrl = getInstanceController())
	{
		if (auto configResp = ctrl->getConfig())
		{
			if (auto node = configResp.value()->findChild(path))
			{
				return node.value();
			}
		}
	}
	return nullptr;
}

bool SolutionManager::setStateValue(std::string const& path, CValue value) const
{
	if (auto const instance = getInstance())
	{
		auto const state = instance->state();
		auto lck = state->getWriteLock();

		if (!state->setNode(path, value))
		{
			LOGE << "Unable to set state entity '" << path << "' to '" << value.str() << "' for instance " << instance->
getInstanceId().toString();
			return false;
		}
	}
	return true;
}

bool SolutionManager::deleteStateValue(std::string const& path) const
{
	if (auto const instance = getInstance())
	{
		auto const state = instance->state();
		auto lck = state->getReadLock();

		if (!state->isNodeSet(path) || !state->deleteNode(path))
		{
			return false;
		}
	}
	return true;
}

expected<bool> SolutionManager::setConfigValue(std::string const& path, CValue value) const
{
	std::vector<std::pair<std::string, CValue>> pathsToValues;
	pathsToValues.emplace_back(path, value);
	return setConfigValues(pathsToValues);
}

expected<bool> SolutionManager::setConfigValues(std::vector<std::pair<std::string, CValue>> const& pathsToValues) const
{
	bool success = true;

	auto const& ctrl = getInstanceController();
	if (!ctrl)
	{
		return false;
	}

	auto configRoot = ctrl->config();
	{
		auto lock = configRoot->getWriteLock();

		for (auto const& [path, value] : pathsToValues)
		{
			if (auto res = configRoot->setNode(path, value)) {
				success = true;
			}
			else
			{
				LOGE << "Unable to set config entity '" << path << "' to '" << value.str() << "' for instance " << ctrl->getInstanceId().toString() << " due to " << res.error().message();
				success = false;
			}
		}
	}

	return ctrl->save() && success;
}

expected<bool> SolutionManager::deleteConfigValue(std::string const& path) const
{
	std::vector<std::string> paths;
	paths.emplace_back(path);
	return deleteConfigValues(paths);
}

expected<bool> SolutionManager::deleteConfigValues(std::vector<std::string> const& paths) const
{
	auto const ctrl = getInstanceController();
	if (!ctrl)
	{
		return false;
	}

	for (auto const& path : paths)
	{
		if (!ctrl->config()->deleteNode(path))
		{
			LOGE << "Unable to delete config entity '" << path << "' for instance " << ctrl->getInstanceId().toString();
			return false;
		}
	}

	return true;
}

void SolutionManager::setInstanceController(std::weak_ptr<iface::InstanceController> const& instanceController)
{
	instanceController_ = instanceController;
}

Uuid SolutionManager::getInstanceUuid() const
{
	if (auto const ctrl = getInstanceController())
	{
		return ctrl->getInstanceId();
	}
	return {};
}

std::string SolutionManager::getInstanceName() const
{
	if (auto const& ctrl = getInstanceController())
	{
		return ctrl->getDisplayName();
	}
	return "";
}

std::string SolutionManager::getConfigurationFile() const
{
	if (auto const& instance = getInstance())
	{
		return instance->config()->asJsonString(4);
	}
	else if (auto const& ctrl = getInstanceController())
	{
		if (auto const& configResp = ctrl->getConfig())
		{
			return api::convert::cvalueToJsonString(*(configResp.value()), false, false, false, 4);
		}
	}
	return "";
}

void SolutionManager::pushDiagnosticEvent(Severity severity, std::string const& title, std::string const& message) const
{
	std::lock_guard<std::mutex> lock(diagnosticEventsMutex_);

	if (diagnosticEvents_.size() > DIAGNOSTICS_QUEUE_LIMIT)
	{
		diagnosticEvents_.pop_front();
	}
	diagnosticEvents_.emplace_back(severity, message, title);
}

DiagnosticEvent SolutionManager::consumeDiagnosticEvent() const
{
	std::lock_guard<std::mutex> lock(diagnosticEventsMutex_);
	if (diagnosticEvents_.empty())
	{
		return DiagnosticEvent(Severity::Empty, "", "");
	}

	auto const event = diagnosticEvents_.front();
	diagnosticEvents_.pop_front();
	return event;
}

std::vector<DiagnosticEvent> SolutionManager::consumeAllDiagnosticEvents() const
{
	std::lock_guard<std::mutex> lock(diagnosticEventsMutex_);

	if (diagnosticEvents_.empty())
	{
		return {};
	}

	auto events = std::vector<DiagnosticEvent>();
	while (!diagnosticEvents_.empty())
	{
		events.emplace_back(diagnosticEvents_.front());
		diagnosticEvents_.pop_front();
	}

	return events;
}

iface::AppSrc* SolutionManager::_getAppSrc()
{
	if (auto const& instance = getInstance())
	{
		(void)api::thread::registerThread(ThreadType::Worker, -1);
		(void)api::thread::setActiveInstance(instance);

		if (auto inputObj = instance->modules().getObject("Input"))
		{
			auto const input = static_cast<iface::Input*>(inputObj.value().get());
			if (auto ret = input->getInputHandler())
			{
				inputHandler_ = ret.value();

				try
				{
					(void)api::thread::unregisterThread();
					return dynamic_cast<iface::AppSrc*>(ret.value().get());
				}
				catch (std::bad_cast const& /*e*/)
				{
					inputHandler_ = nullptr;
				}
			}
		}
		(void)api::thread::unregisterThread();
	}

	return nullptr;
}

bool SolutionManager::load()
{
	if (auto const ctrl = getInstanceController())
	{
		if (!ctrl->isInstanceLoaded())
		{
			if (!ctrl->loadInstance())
			{
				return false;
			}

			if (!this->_loadInstanceCallback())
			{
				ctrl->unloadInstance();
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool SolutionManager::start()
{
	if (shouldRun_ == true)
	{
		return false;
	}

	if (!load())
	{
		return false;
	}

	shouldRun_ = true;
	instanceThread_ = std::thread(&SolutionManager::_processInstance, this);

	// wait for max 5 seconds for isRunning_ to become true
	for (int i = 0; i < 50; i++)
	{
		if (isRunning_)
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return isRunning_;
}

bool SolutionManager::stop()
{
	if (auto const& instance = getInstance())
	{
		shouldRun_ = false;

		if (instanceThread_.joinable())
		{
			instanceThread_.join();
		}

		bool const ret = instance->stop();
		instance->reset();

		if (auto const ctrl = getInstanceController())
		{
			ctrl->unloadInstance();
		}

		return ret;
	}
	else
	{
		return false;
	}
}

void SolutionManager::_handleAppSrcFailure() const
{
	if (auto const& instance = getInstance())
	{
		pushDiagnosticEvent(Severity::Error, "Failed to start feed", "Failed to get AppSrc input plugin");
		instance->stop();
	}
}

void SolutionManager::setAutoRestart(bool enable)
{
	autoRestart_ = enable;
	setConfigValue("/SolutionManager/auto_restart", CValue(enable));
}

bool SolutionManager::getAutoRestart() const
{
	if (auto const val = getConfigValue("/SolutionManager/auto_restart"))
	{
		if (auto ret = val->getValue<bool>())
		{
			return ret.value();
		}
	}
	return true;
}

void SolutionManager::restart()
{
	if (auto const& instance = getInstance())
	{
		isSignaledRestart_ = true;
		instance->stop();
	}
}

void SolutionManager::_startInstance()
{
	if (auto const& instance = getInstance())
	{
		auto canStart = instance->start();
		if (canStart.has_value())
		{
			isInstanceStarted_ = true;
			isRunning_ = true;
			isInstanceStartFailedDueToLicensing_ = false;
		}
		else
		{
			_handleInstanceStartFailureDueToLicensing(canStart.error());
		}

		if (isInstanceStartFailedDueToLicensing_)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		}
	}
}

void SolutionManager::_restartInstance()
{
	if (auto const& instance = getInstance())
	{
		isRunning_ = false;

		if (!isSignaledRestart_)
		{
			LOGD << "Instance terminated unexpectedly: " << instance->getName();
			pushDiagnosticEvent(Severity::Error, "Restarting instance",
			                    "Instance terminated unexpectedly: " + instance->getName());
		}

		instance->stop();
		LOGD << "Restarting instance; instance stop called: " << instance->getName();

		auto canStart = instance->start();
		if (canStart.has_value())
		{
			LOGD << "Restarting instance successful: " << instance->getName();
			isRunning_ = true;
			isInstanceStartFailedDueToLicensing_ = false;
		}
		else
		{
			_handleInstanceStartFailureDueToLicensing(canStart.error());
		}

		//	if (isInstanceStartFailedDueToLicensing_) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		//	}
	}
}

void SolutionManager::_handleInstanceStartFailureDueToLicensing(std::error_code const err)
{
	if (err == RTErrc::NoLicensesLoaded)
	{
		isInstanceStartFailedDueToLicensing_ = true;
		if (!isSignaledRestart_)
		{
			pushDiagnosticEvent(Severity::Error, "No licenses are loaded", err.message());
		}
	}
	else if (err == RTErrc::MaxNumberOfStreamsReached)
	{
		isInstanceStartFailedDueToLicensing_ = true;
		pushDiagnosticEvent(Severity::Error, "Stream limit reached", err.message());
	}
}

bool SolutionManager::_setInputUri(std::string const& uri)
{
	if (!setConfigValue("/Input/uri", CValue(uri)))
	{
		return false;
	}

	if (auto const instance = getInstance())
	{
		(void)api::thread::registerThread(ThreadType::Worker, -1);
		(void)api::thread::setActiveInstance(instance);
		if (auto inputObj = instance->modules().getObject("Input"))
		{
			auto const input = static_cast<iface::Input*>(inputObj.value().get());

			input_ = input;
			input_->setFps(static_cast<float>(frameRateLimit_));

			if (auto const ret = input->setSource(uri); ret && ret.value())
			{
				(void)api::thread::unregisterThread();
				return true;
			}
		}
		(void)api::thread::unregisterThread();
		return false;
	}
	return false;
}

bool SolutionManager::setInputToManual()
{
	inputUri_ = "appsrc://";
	inputMode_ = InputMode::AppSrc;

	bool const ret = _setInputUri(inputUri_);

	if (ret)
	{
		std::lock_guard<std::mutex> lock(appSrcMutex_);
		appSrc_ = _getAppSrc();
	}

	return ret;
}


static std::string preprocessURI(std::string uriIn)
{
	parser::uri uri(uriIn);
	auto protocol = uri.get_scheme();
	std::string username = "";
	std::string password = "";
	std::string ip = "";
	std::string port = "";
	std::string path = "";

	bool pwdOK = false;
	// FIXME: This will not work if the password also have / or : after any of the @
	while (!pwdOK)
	{
		if (uri.get_host().empty())
		{
			LOGE << "No host in uri!";
			return "";
		}

		ip = urlDecode(uri.get_host());

		if (ip.find('@') != std::string::npos)
		{
			pwdOK = false;
			std::string newUri = "";

			for (size_t idx = 0; idx < uriIn.find('@'); ++idx)
			{
				newUri += uriIn[idx];
			}

			newUri += "%40";

			for (size_t idx = uriIn.find('@') + 1; idx < uriIn.size(); ++idx)
			{
				newUri += uriIn[idx];
			}

			uriIn = newUri;
			uri = parser::uri(newUri);
		}
		else
		{
			pwdOK = true;
		}
	}

	if (!uri.get_username().empty())
	{
		username = urlDecode(uri.get_username());
	}

	if (!uri.get_password().empty())
	{
		password = urlDecode(uri.get_password());
	}

	if (!uri.get_host().empty())
	{
		ip = urlDecode(uri.get_host());
	}

	if (uri.get_port())
	{
		port = std::to_string(uri.get_port());
	}

	if (!uri.get_path().empty())
	{
		std::string fullPath = uri.get_path();
		if (!uri.get_query().empty())
		{
			fullPath += "?" + urlDecode(uri.get_query());
		}
		if (!uri.get_fragment().empty())
		{
			fullPath += "#" + urlDecode(uri.get_fragment());
		}

		path = fullPath;
	}

	std::string addr = "";

	if (!protocol.empty())
	{
		addr += protocol + "://";
	}

	if (!username.empty() && !password.empty())
	{
		addr += urlEncode(username) + ":" + urlEncode(password) + "@";
	}

	if (!ip.empty())
	{
		addr += ip;

		if (!port.empty())
		{
			addr += ":" + port;
		}

		if (!path.empty())
		{
			addr += "/" + path;
		}
	}

	return addr;
}

bool SolutionManager::setInputToRtsp(std::string const& rtspUri)
{
	std::string const pipelinePrefix = "gstreamer:///urisourcebin uri=";
	std::string const pipelineSuffix = " ! video/x-raw, format=NV12 ! appsink drop=true name=cvdsink";

	auto const addr = preprocessURI(rtspUri);

	if (addr.empty())
	{
		return false;
	}

	inputUri_ = addr;

	inputMode_ = InputMode::Rtsp;

	int frameRateLimit = getFrameRateLimit();
	if (frameRateLimit == 0)
	{
		frameRateLimit = 10;
	}

	std::string const uri_full = pipelinePrefix + addr + " ! decodebin ! videoconvert ! videorate max-rate=" +
		std::to_string(frameRateLimit) + pipelineSuffix;

	return _setInputUri(uri_full);
}

bool SolutionManager::pushFrame(void const* buffer, int width, int height, std::chrono::milliseconds timestamp)
{
	std::lock_guard<std::mutex> lock(appSrcMutex_);

	if (!isRunning_)
	{
		return false;
	}

	if (inputMode_ != InputMode::AppSrc)
	{
		return false;
	}

	if (!appSrc_)
	{
		return false;
	}

	if (appSrc_->getQueueSize() >= 2)
	{
		return false;
	}

	if (timestamp.count() == 0)
	{
		auto const duration = std::chrono::system_clock::now().time_since_epoch();
		// time in ms since epoch
		timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	}

	_calculateThruput(timestamp, false);

	appSrc_->writeData(buffer, width, height, timestamp, cmap());
	framesProcessed_++;

	return true;
}

bool SolutionManager::pushFrame(const rt::cbuffer& image)
{
	std::lock_guard<std::mutex> lock(appSrcMutex_);

	if (!isRunning_)
	{
		return false;
	}

	if (inputMode_ != InputMode::AppSrc)
	{
		return false;
	}

	if (!appSrc_)
	{
		return false;
	}

	if (appSrc_->getQueueSize() >= 2)
	{
		return false;
	}

	if (image->timestamp() == 0)
	{
		auto const duration = std::chrono::system_clock::now().time_since_epoch();
		// time in ms since epoch
		image->timestamp(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 1000.0);
	}

	_calculateThruput(std::chrono::milliseconds(static_cast<uint64_t>(image->timestamp() * 1000.0)), false);

	appSrc_->writeData(image, cmap());
	framesProcessed_++;

	return true;
}

bool SolutionManager::pushH264VideoFrame(unsigned char const* data, size_t dataSize,
                                         std::chrono::milliseconds timestamp)
{
	if (getInstance())
	{
		if (auto const appsrc = _getAppSrc())
		{
			_calculateThruput(timestamp, false);
			return appsrc->writeH264VideoFrame(data, dataSize, timestamp).has_value();
		}
	}
	return false;
}

bool SolutionManager::pushH265VideoFrame(unsigned char const* data, size_t dataSize,
                                         std::chrono::milliseconds timestamp)
{
	if (getInstance())
	{
		if (auto const appsrc = _getAppSrc())
		{
			_calculateThruput(timestamp, false);
			return appsrc->writeH265VideoFrame(data, dataSize, timestamp).has_value(); // segfaults
		}
	}
	return false;
}

void SolutionManager::setDebugMode(bool enableDebug)
{
	enableDebug_ = enableDebug;
	analytics_->setDebugMode(enableDebug);
	setConfigValues({
		{"/Global/Debug/capture_extra_track_info", CValue(enableDebug_)},
		{"/SolutionManager/enable_debug", CValue(enableDebug)}
	});
}

bool SolutionManager::getDebugMode() const
{
	if (auto const val = getConfigValue("/SolutionManager/enable_debug"))
	{
		if (auto ret = val->getValue<bool>())
		{
			return ret.value();
		}
	}
	return false;
}

void SolutionManager::setDiagnosticsMode(bool enable)
{
	sendDiagnostics_ = enable;
	analytics_->setDiagnostics(enable);
	setConfigValue("/SolutionManager/send_diagnostics", CValue(enable));
}

bool SolutionManager::getDiagnosticsMode() const
{
	if (auto const val = getConfigValue("/SolutionManager/send_diagnostics"))
	{
		if (auto ret = val->getValue<bool>())
		{
			return ret.value();
		}
	}
	return false;
}

void SolutionManager::setStatisticsMode(bool enable)
{
	if (enable)
	{
		if (!isStatisticsRunning_)
		{
			isStatisticsRunning_ = true;
			statisticsThread_ = std::thread(&SolutionManager::statisticsThread, this);
		}
	}
	else if (isStatisticsRunning_)
	{
		isStatisticsRunning_ = false;
		if (statisticsThread_.joinable())
		{
			statisticsThread_.join();
		}
	}
	setConfigValue("/SolutionManager/run_statistics", CValue(enable));
}

bool SolutionManager::getStatisticsMode() const
{
	if (auto const val = getConfigValue("/SolutionManager/run_statistics"))
	{
		if (auto ret = val->getValue<bool>())
		{
			return ret.value();
		}
	}
	return false;
}

void SolutionManager::setMetadataMode(bool enable)
{
	sendMetadata_ = enable;
	setConfigValue("/SolutionManager/send_metadata", CValue(enable));
}

bool SolutionManager::getMetadataMode() const
{
	if (auto const val = getConfigValue("/SolutionManager/send_metadata"))
	{
		if (auto ret = val->getValue<bool>())
		{
			return ret.value();
		}
	}
	return false;
}

bool SolutionManager::getTentativeTracks() const
{
	if (auto const val = getConfigValue("/SolutionManager/tentative_tracks"))
	{
		if (auto ret = val->getValue<bool>())
		{
			return ret.value();
		}
	}
	return false;
}

void SolutionManager::setTentativeTracks(bool enable) const
{
	analytics_->setTentativeTracks(enable);
}

bool SolutionManager::isDebugMode() const
{
	return enableDebug_;
}

bool SolutionManager::isStatisticsEnabled() const
{
	return isStatisticsRunning_;
}

bool SolutionManager::isMetadataEnabled() const
{
	return sendMetadata_;
}

bool SolutionManager::isDiagnosticsEnabled() const
{
	return sendDiagnostics_;
}

void SolutionManager::_processInstance()
{
	if (auto instance = getInstance())
	{
		(void)api::thread::registerThread(ThreadType::Instance, -1);
		(void)api::thread::setActiveInstance(instance);

		// Leaking small amount of memory on purpose to keep
		// string alive for Tracy
		auto const workerId = new char[128];
		snprintf(workerId, 128, "%s", instance->getName().c_str());

		isRunning_ = false;
		isInstanceStarted_ = false;

		while (shouldRun_ && api::isRunning())
		{
			if (!isInstanceStarted_)
			{
				_startInstance();
			}
			else if (!instance->isRunning() || instance->isPaused())
			{
				if (autoRestart_)
				{
					if (api::isRunning())
					{
						_restartInstance();
					}
				}
				else
				{
					continue;
				}
			}
			else
			{
				isSignaledRestart_ = false;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		isRunning_ = false;

		(void)api::thread::unregisterThread();
	}
}

std::string SolutionManager::enableHlsOutput()
{
	std::string const sink = "output-image";
	std::string const name = "HLS";
	std::string const path = "/Output/handlers/" + name;
	std::string uri = "hls://";
	cmap const handlerConfig = {
		{"config", VAL(cmap())}, {"enabled", VAL(true)}, {"sink", VAL(sink)}, {"uri", VAL(uri)}
	};

	bool success = setConfigValue(path, CValue(handlerConfig)).value_or(false);

	if (auto const instance = getInstance())
	{
		(void)api::thread::registerThread(ThreadType::Instance, -1);
		(void)api::thread::setActiveInstance(instance);

		if (auto outputObj = instance->modules().getObject("Output"))
		{
			auto const output = static_cast<iface::Output*>(outputObj.value().get());

			auto const& resHasHdl = output->hasHandler(name);

			if (resHasHdl.has_value() && resHasHdl.value()) {
				LOGV << "Handler already exists - will be replaced";
				auto const& resRemHdl = output->removeHandler(name);
				if (!resRemHdl.has_value()) {
					LOGE << "Failed to remove existing handler";
				}
			}

			if (auto const handler = output->addHandler(name, uri, sink, VAL()).value_or(nullptr)) {
				uri = handler->uri_;
			}
			else
			{
				LOGE << "Failed to add HLS output handler";
				success = false;
			}
		}
		else
		{
			LOGE << "Failed to get Output module";
			success = false;
		}

		(void)api::thread::unregisterThread();
	}

	return success ? uri : "";
}

bool SolutionManager::disableHlsOutput() {
	std::string const sink = "output-image";
	std::string const name = "HLS";
	std::string const path = "/Output/handlers/" + name;
	std::string uri = "hls://";
	
	auto phandlerConfig = getConfigValue(path);
	cmap handlerConfig{};
	if (phandlerConfig != nullptr && phandlerConfig->getType() == CValueType::MAP) {
		handlerConfig = phandlerConfig->getValueOr<cmap>(cmap{});
		if (handlerConfig.count("enabled") == 1) {
			handlerConfig["enabled"] = VAL(false);
		}
	}
	if (handlerConfig.empty()) {
		handlerConfig = cmap{ {"config", VAL(cmap())}, {"enabled", VAL(false)}, {"sink", VAL(sink)}, {"uri", VAL(uri)} };
	}

	bool success = setConfigValue(path, CValue(handlerConfig)).value_or(false);

	if (auto const instance = getInstance())
	{
		if (auto outputObj = instance->modules().getObject("Output"))
		{
			auto const output = static_cast<iface::Output*>(outputObj.value().get());

			if (!output->removeHandler(name))
			{
				success = false;
			}
		}
		else
		{
			success = false;
		}
	}

	return success;
}

namespace
{
	std::string _uriToOutputName(std::string uri)
	{
		str_replace(uri, "/", "-");
		return uri;
	}
}

bool SolutionManager::enableRtspOutput(std::string const& uri)
{
	std::string const sink = "output-image";
	std::string const name = _uriToOutputName(uri);
	std::string const path = "/Output/handlers/" + name;
	cmap const handlerConfig = {{"config", VAL(cmap())}, {"enabled", VAL(true)}, {"sink", VAL(sink)}, {"uri", VAL(uri)}};

	bool success = setConfigValue(path, CValue(handlerConfig)).value_or(false);

	if (auto const instance = getInstance())
	{
		if (auto outputObj = instance->modules().getObject("Output"))
		{
			auto const output = static_cast<iface::Output*>(outputObj.value().get());

			auto const& resHasHdl = output->hasHandler(name);

			if (resHasHdl.has_value() && resHasHdl.value()) {
				LOGV << "Handler already exists - will be replaced";
				auto const& resRemHdl = output->removeHandler(name);
				if (!resRemHdl.has_value()) {
					LOGE << "Failed to remove existing handler";
				}
			}

			auto const handler = output->addHandler(name, uri, sink, VAL());
			if (!handler)
			{
				success = false;
			}
		}
		else
		{
			success = false;
		}
	}
	return success;
}

bool SolutionManager::disableRtspOutput(std::string const& uri) {
	std::string const sink = "output-image";
	std::string const name = _uriToOutputName(uri);
	std::string const path = "/Output/handlers/" + name;

	auto phandlerConfig = getConfigValue(path);
	cmap handlerConfig{};
	if (phandlerConfig != nullptr && phandlerConfig->getType() == CValueType::MAP) {
		handlerConfig = phandlerConfig->getValueOr<cmap>(cmap{});
		if (handlerConfig.count("enabled") == 1) {
			handlerConfig["enabled"] = VAL(false);
		}
	}
	if (handlerConfig.empty()) {
		handlerConfig = cmap{{"config", VAL(cmap())}, {"enabled", VAL(false)}, {"sink", VAL(sink)}, {"uri", VAL(uri)}};
	}

	bool success = setConfigValue(path, CValue(handlerConfig)).value_or(false);

	if (auto const instance = getInstance())
	{
		if (auto outputObj = instance->modules().getObject("Output"))
		{
			auto const output = static_cast<iface::Output*>(outputObj.value().get());

			if (!output->removeHandler(name))
			{
				success = false;
			}
		}
		else
		{
			success = false;
		}
	}
	return success;
}

bool SolutionManager::setRenderPreset(std::string const& preset)
{
	bool const success = setConfigValue("/Output/render_preset", CValue(preset)).value_or(false);

	return success;
}

void SolutionManager::setBlockingReadaheadQueue(bool enable)
{
	setConfigValue("/Input/free_run_mode", CValue(!enable));
}

void SolutionManager::destroy()
{
	SolutionManager::stop();

	if (auto const ctrl = getInstanceController())
	{
		(void)api::instances::deleteInstanceController(ctrl->getInstanceId());
	}
}


SolutionManager::~SolutionManager()
{
	shouldRun_ = false;
	isStatisticsRunning_ = false;

	if (instanceThread_.joinable())
	{
		instanceThread_.join();
	}

	if (statisticsThread_.joinable())
	{
		statisticsThread_.join();
	}
}
