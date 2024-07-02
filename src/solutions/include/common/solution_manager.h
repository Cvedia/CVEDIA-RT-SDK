/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>

#include "cvaluefwd.h"
#include "defines.h"
#include "rterror.h"
#include "uuid.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class Input;
			class InstanceController;
			class InputHandler;
			class AppSrc;
			class Solution;
			class Instance;
		}
	}
}

namespace cvedia {
	namespace rt {
		namespace solution {
			class AnalyticsManager;
			class Event;

			enum InputMode
			{
				AppSrc,
				Rtsp
			};

			enum Severity
			{
				Empty = 1,
				Debug = 2,
				Info = 3,
				Warning = 4,
				Error = 5,
				Critical = 6
			};

			struct DiagnosticEvent
			{
				DiagnosticEvent(Severity severity, std::string const& description, std::string const& message) {
					this->severity = severity;
					this->description = description;
					this->message = message;
				}

				Severity severity;
				std::string description;
				std::string message;
			};

			class EXPORT_CLASS SolutionManager : public std::enable_shared_from_this<SolutionManager>
			{
			public:
				enum Codec
				{
					H264 = 1,
					H265 = 2
				};

				// Constructors and Destructors
				SolutionManager();
				EXPORT virtual ~SolutionManager();

				EXPORT expected<void> initialize();

				// Instance Identification
				EXPORT Uuid getInstanceUuid() const;
				EXPORT std::string getInstanceName() const;

				// Solution Information
				EXPORT std::string getVersion() const;
				EXPORT int64_t getStartTime() const;
				EXPORT int getTrackCount() const;
				EXPORT int getInputQueueSize() const;
				EXPORT int getDroppedFramesCount() const;
				EXPORT uint64_t getFramesProcessed() const;
				EXPORT float getLatency() const;
				EXPORT float getCurrentFrameRate() const;

				// Frame and Rate Information
				EXPORT void setFrameRateLimit(int frameRateLimit);
				EXPORT int getFrameRateLimit() const;
				EXPORT int getRecommendedFrameRate() const;
				EXPORT bool needData(std::chrono::milliseconds const currentFrameTime);

				// Operations
				EXPORT bool load();
				EXPORT bool start();
				EXPORT bool stop();
				EXPORT bool isRunning() const;

				EXPORT bool pushFrame(const rt::cbuffer& image);
				EXPORT bool pushFrame(void const* buffer, int width, int height, std::chrono::milliseconds timestamp);
				EXPORT bool pushH264VideoFrame(unsigned char const* data, size_t dataSize, std::chrono::milliseconds timestamp);
				EXPORT bool pushH265VideoFrame(unsigned char const* data, size_t dataSize, std::chrono::milliseconds timestamp);

				EXPORT std::unique_ptr<Event> consumeEvents();
				EXPORT std::vector<std::unique_ptr<Event>> consumeAllEvents();

				// Diagnostics
				EXPORT void setMetadataMode(bool enable);
				EXPORT void setStatisticsMode(bool enable);
				EXPORT void setDiagnosticsMode(bool enable);
				EXPORT void setDebugMode(bool enableDebug);
				EXPORT bool getMetadataMode() const;
				EXPORT bool getStatisticsMode() const;
				EXPORT bool getDiagnosticsMode() const;
				EXPORT bool getDebugMode() const;

				EXPORT void setTentativeTracks(bool enable) const;
				EXPORT bool getTentativeTracks() const;

				EXPORT bool isDebugMode() const;
				EXPORT bool isMetadataEnabled() const;
				EXPORT bool isStatisticsEnabled() const;
				EXPORT bool isDiagnosticsEnabled() const;

				EXPORT DiagnosticEvent consumeDiagnosticEvent() const;
				EXPORT std::vector<DiagnosticEvent> consumeAllDiagnosticEvents() const;
				EXPORT std::string getConfigurationFile() const;

				EXPORT bool setInputToManual();
				EXPORT bool setInputToRtsp(std::string const& rtspUri);

				EXPORT std::string enableHlsOutput();
				EXPORT bool disableHlsOutput();

				EXPORT bool enableRtspOutput(std::string const& uri);
				EXPORT bool disableRtspOutput(std::string const& uri);

				EXPORT bool setRenderPreset(std::string const& preset);

				EXPORT void setBlockingReadaheadQueue(bool enable);

				EXPORT void setAutoRestart(bool enable);
				EXPORT bool getAutoRestart() const;

				EXPORT void restart();

				EXPORT void destroy();

				EXPORT pCValue getConfigValue(std::string const& path) const;
				EXPORT bool setStateValue(std::string const& path, CValue value) const;
				EXPORT expected<bool> setConfigValue(std::string const& path, CValue value) const;
				EXPORT expected<bool> setConfigValues(std::vector<std::pair<std::string, CValue>> const& pathsToValues) const;
				EXPORT bool deleteStateValue(std::string const& path) const;
				EXPORT expected<bool> deleteConfigValue(std::string const& path) const;
				EXPORT expected<bool> deleteConfigValues(std::vector<std::string> const& paths) const;

			protected:
				void pushDiagnosticEvent(Severity severity, std::string const& title, std::string const& message) const;

				void setRecommendedFrameRate(int fps) const;

				virtual void setupCallbacks() = 0;
				virtual expected<void> attachToInstanceController(std::weak_ptr<iface::InstanceController> instanceController, bool initialize) = 0;
				virtual void statisticsThread() = 0;

				virtual bool _loadInstanceCallback() = 0;

				std::shared_ptr<iface::InstanceController> getInstanceController() const;
				std::shared_ptr<iface::InstanceController> getInstanceController();

				std::shared_ptr<iface::Instance> getInstance() const;
				std::shared_ptr<iface::Instance> getInstance();

				std::shared_ptr<iface::Solution> getSolution() const;
				std::shared_ptr<iface::Solution> getSolution();

				void setInstanceController(std::weak_ptr<iface::InstanceController> const& instanceController);

				std::unique_ptr<AnalyticsManager> analytics_;

				std::deque<std::unique_ptr<Event>> eventsQueue_;
				std::mutex eventsQueueMutex_;
				bool shouldRun_ = false;
				bool isStatisticsRunning_ = false;
			private:
				void _startInstance();
				void _restartInstance();
				void _handleInstanceStartFailureDueToLicensing(std::error_code const err);

				bool isInstanceStarted_ = false;
				bool isSignaledRestart_ = false;
				bool isInstanceStartFailedDueToLicensing_ = false;

				bool _setInputUri(std::string const& uri);

				void _processInstance();
				void _disableFreeRunMode() const;

				void _initialize(std::string const& solution);
				rt::iface::AppSrc* _getAppSrc();
				void _handleAppSrcFailure() const;
				void _calculateThruput(std::chrono::milliseconds currentFrameTime, bool checkOnly);

				mutable std::deque<DiagnosticEvent> diagnosticEvents_;
				mutable std::mutex diagnosticEventsMutex_;

				bool needData_ = true;

				bool autoRestart_ = true;

				bool enableDebug_ = false;
				bool sendDiagnostics_ = false;
				bool sendMetadata_ = false;

				std::mutex appSrcMutex_;
				rt::iface::AppSrc *appSrc_ = nullptr;
				std::shared_ptr<iface::InputHandler> inputHandler_;
				iface::Input* input_;

				std::thread instanceThread_;
				std::thread statisticsThread_;

				std::mutex cvediartMutex_;

				bool isRunning_ = false;
				expected<void>  instanceStartError = {};

				int frameRateLimit_ = 0;
				mutable std::chrono::milliseconds lastFrameTime_;
				mutable std::deque<std::chrono::milliseconds> avgFrameTimes_;

				mutable float curSolutionFrameRate_ = 0;
				int activeTrackCount_ = 0;

				uint64_t framesProcessed_ = 0;

				InputMode inputMode_ = InputMode::AppSrc;
				std::string inputUri_;

				std::weak_ptr<cvedia::rt::iface::InstanceController> instanceController_;
			};
		}
	}
}
