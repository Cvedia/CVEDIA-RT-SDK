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
#include "interface/solution_manager.h"

namespace cvedia::rt::iface
{
	class Module;
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

			class EXPORT_CLASS SolutionManagerImpl : virtual public cvedia::rt::iface::SolutionManager
			{
			public:
				enum Codec
				{
					H264 = 1,
					H265 = 2
				};

				// Constructors and Destructors
				EXPORT SolutionManagerImpl();
				EXPORT ~SolutionManagerImpl() override;

				// Instance Identification
				EXPORT Uuid getInstanceUuid() const override;
				EXPORT std::string getInstanceName() const override;

				// Solution Information
				EXPORT std::string getVersion() const override;
				EXPORT int64_t getStartTime() const override;
				EXPORT int getTrackCount() const override;
				EXPORT int getInputQueueSize() const override;
				EXPORT int getDroppedFramesCount() const override;
				EXPORT std::string getInputResolution() const override;
				EXPORT uint64_t getFramesProcessed() const override;
				EXPORT float getLatency() const override;
				EXPORT float getCurrentFrameRate() const override;

				// Frame and Rate Information
				EXPORT void setFrameRateLimit(int frameRateLimit) override;
				EXPORT int getFrameRateLimit() const override;
				EXPORT int getRecommendedFrameRate() const override;
				EXPORT bool needData(std::chrono::milliseconds const currentFrameTime) override;

				EXPORT bool pushRawFrame(const rt::cbuffer& image) override;
				EXPORT bool pushRawFrame(void const* buffer, int width, int height, std::chrono::milliseconds timestamp) override;
				EXPORT bool pushCompressedFrame(unsigned char const* data, size_t dataSize, std::chrono::milliseconds timestamp) override;
				EXPORT bool pushH264Frame(unsigned char const* data, size_t dataSize, std::chrono::milliseconds timestamp) override;
				EXPORT bool pushH265Frame(unsigned char const* data, size_t dataSize, std::chrono::milliseconds timestamp) override;

				EXPORT std::unique_ptr<solution::Event> consumeEvents() override;
				EXPORT std::vector<std::unique_ptr<solution::Event>> consumeAllEvents() override;

				// Diagnostics
				EXPORT void setMetadataMode(bool enable) override;
				EXPORT void setStatisticsMode(bool enable) override;
				EXPORT void setDiagnosticsMode(bool enable) override;
				EXPORT void setDebugMode(bool enableDebug) override;
				EXPORT bool getMetadataMode() const override;
				EXPORT bool getStatisticsMode() const override;
				EXPORT bool getDiagnosticsMode() const override;
				EXPORT bool getDebugMode() const override;

				EXPORT void setTentativeTracks(bool enable) const override;
				EXPORT bool getTentativeTracks() const override;

				EXPORT bool isDebugMode() const override;
				EXPORT bool isMetadataEnabled() const override;
				EXPORT bool isStatisticsEnabled() const override;
				EXPORT bool isDiagnosticsEnabled() const override;

				EXPORT types::DetectorMode getDetectorMode() const override;
				EXPORT void setDetectorMode(types::DetectorMode mode) const override;
				EXPORT types::Modality getSensorModality() const override;
				EXPORT void setSensorModality(types::Modality const modality) const override;
				EXPORT types::Sensitivity getDetectionSensitivity() const override;
				EXPORT void setDetectionSensitivity(types::Sensitivity mode) const override;
				EXPORT types::Sensitivity getMovementSensitivity() const override;
				EXPORT void setMovementSensitivity(types::Sensitivity mode) const override;

				EXPORT cvedia::rt::iface::DiagnosticEvent consumeDiagnosticEvent() const override;
				EXPORT std::vector<cvedia::rt::iface::DiagnosticEvent> consumeAllDiagnosticEvents() const override;
				EXPORT std::string getConfigurationFile() const override;

				EXPORT bool setInputToManual() override;
				EXPORT bool setInputToRtsp(std::string const& rtspUri) override;

				EXPORT bool setInputOrientation(int orientation) override;

				EXPORT std::string enableHlsOutput() override;
				EXPORT bool disableHlsOutput() override;

				EXPORT bool enableRtspOutput(std::string const& uri) override;
				EXPORT bool disableRtspOutput(std::string const& uri) override;

				EXPORT bool setRenderPreset(std::string const& preset) override;

				EXPORT void setBlockingReadaheadQueue(bool enable) override;

				EXPORT NODISCARD expected<void> attachToInstanceController(iface::InstanceController* const instanceController) override;

				EXPORT pCValue getConfigValue(std::string const& path) const override;
				EXPORT bool setStateValue(std::string const& path, CValue value) const override;
				EXPORT expected<bool> setConfigValue(std::string const& path, CValue value) const override;
				EXPORT expected<bool> setConfigValues(std::vector<std::pair<std::string, CValue>> const& pathsToValues) const override;
				EXPORT bool deleteStateValue(std::string const& path) const override;
				EXPORT expected<bool> deleteConfigValue(std::string const& path) const override;
				EXPORT expected<bool> deleteConfigValues(std::vector<std::string> const& paths) const override;

				EXPORT nlohmann::json extractInfo() const override;

			protected:
				// Operations
				EXPORT bool load() override;
				EXPORT bool unload() override;

				EXPORT void pushDiagnosticEvent(types::Severity severity, std::string const& title, std::string const& message) const override;
				EXPORT void setRecommendedFrameRate(int fps) const override;
				EXPORT void setupCallbacks() override;
				EXPORT void removeCallbacks() override;
				EXPORT void statisticsThread() override;

				EXPORT iface::InstanceController* getInstanceController() const override;
				EXPORT iface::InstanceController* getInstanceController() override;
				
				EXPORT std::shared_ptr<iface::Instance> getInstance() const override;
				EXPORT std::shared_ptr<iface::Instance> getInstance() override;
				EXPORT std::shared_ptr<iface::Solution> getSolution() const override;
				EXPORT std::shared_ptr<iface::Solution> getSolution() override;
				
				EXPORT void setInstanceController(iface::InstanceController* const instanceController) override;

				std::unique_ptr<solution::AnalyticsManager> analytics_;

				std::deque<std::unique_ptr<Event>> eventsQueue_;
				std::mutex eventsQueueMutex_;
				bool isStatisticsRunning_ = false;
			private:
				void _runStatisticsThread();
				expected<std::string> _getResolution() const;

				expected<std::pair<std::shared_ptr<iface::Module>, iface::Input*>> _getInputManaged() const;
				bool _setInputUri(std::string const& uri);

				void _disableFreeRunMode() const;

				rt::iface::AppSrc* _getAppSrc();
				void _handleAppSrcFailure() const;
				void _calculateThruput(std::chrono::milliseconds currentFrameTime, bool checkOnly);

				mutable std::deque<cvedia::rt::iface::DiagnosticEvent> diagnosticEvents_;
				mutable std::mutex diagnosticEventsMutex_;

				bool needData_ = true;

				bool enableDebug_ = false;
				bool sendDiagnostics_ = false;
				bool sendMetadata_ = false;

				std::mutex appSrcMutex_;
				std::shared_ptr<iface::Module> inputModule_ = nullptr;
				rt::iface::AppSrc* appSrc_ = nullptr;
				iface::InputHandler* inputHandler_ = nullptr;

				std::thread statisticsThread_;

				std::mutex solutionManMut_;

				expected<void>  instanceStartError = {};

				int frameRateLimit_ = 0;
				mutable std::chrono::milliseconds lastFrameTime_;
				mutable std::deque<std::chrono::milliseconds> avgFrameTimes_;

				mutable float curSolutionFrameRate_ = 0;

				uint64_t framesProcessed_ = 0;

				InputMode inputMode_ = InputMode::AppSrc;
				std::string inputUri_;

				cvedia::rt::iface::InstanceController* instanceController_ = nullptr;
			};

			class EXPORT_CLASS DefaultSolutionManager : public SolutionManagerImpl
			{
			public:
				EXPORT static std::shared_ptr<iface::SolutionManager> create()
				{
					return std::make_shared<DefaultSolutionManager>();
				}

			protected:
				bool _loadInstanceCallback() override
				{
					return true;
				}

				bool _unloadInstanceCallback() override
				{
					return true;
				}
			};

		}
	}
}
