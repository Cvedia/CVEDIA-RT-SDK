/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <unordered_set>

#include "cvdict.h"
#include "defines.h"
#include "uuid.h"
#include "builtin/types.h"
#include "builtin/analytics_manager.h"
#include "builtin/objectregistry.h"

namespace cvedia {
	namespace rt {
		class InstanceControllerImpl;

		namespace iface {
			class Input;
			class InstanceController;
			class InputHandler;
			class AppSrc;
			class Solution;
			class Instance;

			struct DiagnosticEvent
			{
				DiagnosticEvent(types::Severity severity, std::string const& description, std::string const& message) {
					this->severity = severity;
					this->description = description;
					this->message = message;
				}

				types::Severity severity;
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
				virtual ~SolutionManager() = default;

				// Instance Identification
				virtual Uuid getInstanceUuid() const = 0;
				virtual std::string getInstanceName() const = 0;

				// Solution Information
				virtual std::string getVersion() const = 0;
				virtual int64_t getStartTime() const = 0;
				virtual int getTrackCount() const = 0;
				virtual int getInputQueueSize() const = 0;
				virtual uint64_t getFramesProcessed() const = 0;
				virtual int getDroppedFramesCount() const = 0;
				virtual float getLatency() const = 0;
				virtual float getCurrentFrameRate() const = 0;

				// Frame and Rate Information
				virtual std::string getInputResolution() const = 0;
				virtual void setFrameRateLimit(int frameRateLimit) = 0;
				virtual int getFrameRateLimit() const = 0;
				virtual int getRecommendedFrameRate() const = 0;
				virtual bool needData(std::chrono::milliseconds const currentFrameTime) = 0;

				virtual bool pushRawFrame(const rt::cbuffer& image) = 0;
				virtual bool pushRawFrame(void const* buffer, int width, int height, std::chrono::milliseconds timestamp) = 0;
				virtual bool pushCompressedFrame(unsigned char const* data, size_t dataSize, std::chrono::milliseconds timestamp) = 0;
				virtual bool pushH264Frame(unsigned char const* data, size_t dataSize, std::chrono::milliseconds timestamp) = 0;
				virtual bool pushH265Frame(unsigned char const* data, size_t dataSize, std::chrono::milliseconds timestamp) = 0;

				virtual std::unique_ptr<solution::Event> consumeEvents() = 0;
				virtual std::vector<std::unique_ptr<solution::Event>> consumeAllEvents() = 0;

				// Diagnostics
				virtual void setMetadataMode(bool enable) = 0;
				virtual void setStatisticsMode(bool enable) = 0;
				virtual void setDiagnosticsMode(bool enable) = 0;
				virtual void setDebugMode(bool enableDebug) = 0;
				virtual bool getMetadataMode() const = 0;
				virtual bool getStatisticsMode() const = 0;
				virtual bool getDiagnosticsMode() const = 0;
				virtual bool getDebugMode() const = 0;

				virtual void setTentativeTracks(bool enable) const = 0;
				virtual bool getTentativeTracks() const = 0;

				virtual bool isDebugMode() const = 0;
				virtual bool isMetadataEnabled() const = 0;
				virtual bool isStatisticsEnabled() const = 0;
				virtual bool isDiagnosticsEnabled() const = 0;

				virtual DiagnosticEvent consumeDiagnosticEvent() const = 0;
				virtual std::vector<DiagnosticEvent> consumeAllDiagnosticEvents() const = 0;
				virtual std::string getConfigurationFile() const = 0;

				virtual bool setInputToManual() = 0;
				virtual bool setInputToRtsp(std::string const& rtspUri) = 0;

				virtual bool setInputOrientation(int orientation) = 0;

				virtual std::string enableHlsOutput() = 0;
				virtual bool disableHlsOutput() = 0;

				virtual bool enableRtspOutput(std::string const& uri) = 0;
				virtual bool disableRtspOutput(std::string const& uri) = 0;

				virtual bool setRenderPreset(std::string const& preset) = 0;

				virtual void setBlockingReadaheadQueue(bool enable) = 0;

				virtual expected<void> attachToInstanceController(iface::InstanceController* const instanceController) = 0;

				virtual void setupCallbacks() = 0;
				virtual void removeCallbacks() = 0;

				virtual pCValue getConfigValue(std::string const& path) const = 0;
				virtual bool setStateValue(std::string const& path, CValue value) const = 0;
				virtual expected<bool> setConfigValue(std::string const& path, CValue value) const = 0;
				virtual expected<bool> setConfigValues(std::vector<std::pair<std::string, CValue>> const& pathsToValues) const = 0;
				virtual bool deleteStateValue(std::string const& path) const = 0;
				virtual expected<bool> deleteConfigValue(std::string const& path) const = 0;
				virtual expected<bool> deleteConfigValues(std::vector<std::string> const& paths) const = 0;

				virtual types::DetectorMode getDetectorMode() const = 0;
				virtual void setDetectorMode(types::DetectorMode mode) const = 0;
				virtual types::Modality getSensorModality() const = 0;
				virtual void setSensorModality(types::Modality const modality) const = 0;
				virtual types::Sensitivity getDetectionSensitivity() const = 0;
				virtual void setDetectionSensitivity(types::Sensitivity mode) const = 0;
				virtual types::Sensitivity getMovementSensitivity() const = 0;
				virtual void setMovementSensitivity(types::Sensitivity mode) const = 0;

				virtual nlohmann::json extractInfo() const = 0;

			protected:
				// Operations
				virtual bool load() = 0;
				virtual bool unload() = 0;

				virtual void pushDiagnosticEvent(types::Severity severity, std::string const& title, std::string const& message) const = 0;
				virtual void setRecommendedFrameRate(int fps) const = 0;

				virtual void statisticsThread() = 0;
				virtual bool _loadInstanceCallback() = 0;
				virtual bool _unloadInstanceCallback() = 0;

				virtual iface::InstanceController* getInstanceController() const = 0;
				virtual iface::InstanceController* getInstanceController() = 0;
				virtual std::shared_ptr<iface::Instance> getInstance() const = 0;
				virtual std::shared_ptr<iface::Instance> getInstance() = 0;
				virtual std::shared_ptr<iface::Solution> getSolution() const = 0;
				virtual std::shared_ptr<iface::Solution> getSolution() = 0;
				virtual void setInstanceController(iface::InstanceController* const instanceController) = 0;

				friend class ::cvedia::rt::InstanceControllerImpl;
			};
		}
	}
}
