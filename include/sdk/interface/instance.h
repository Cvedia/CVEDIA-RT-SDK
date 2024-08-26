/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "defines.h"
#include "uuid.h"
#include "cvaluefwd.h"
#include "rterror.h"
#include "builtin/objectregistry.h"

namespace cvedia {
	namespace rt {
		class Sink;
		class PluginLua;

		namespace internal {
			class PerformanceBase;
			class VersionControl;
		}

		namespace module {
			struct PluginInst;
		}

		class KalmanFilter;
		enum BufferDataType: unsigned int;

		namespace internal {
			enum SourceTypeEnum: int;
		}
	}
}

namespace cvedia {
	namespace rt {
		namespace solution
		{
			class SolutionManager;
		}

		enum class InstanceState
		{
			INSTANCE_NONE = 0,
			INSTANCE_STOPPED = 1,
			INSTANCE_STARTING = 2,
			INSTANCE_PAUSED = 3,
			INSTANCE_RUNNING = 4,
			INSTANCE_FAIL = 5
		};

		enum InstanceStepResult
		{
			INSTANCE_STEP_FAIL = 0,
			INSTANCE_STEP_OK = 1,
			INSTANCE_STEP_INVALID = 2
		};
		class UIState;

		namespace iface {
			class LuaWorkers;
			class Module;
			class Solution;
			class CVDict;

			class EXPORT_CLASS Instance: public std::enable_shared_from_this<Instance> {
			public:
				virtual ~Instance() = default;

				virtual Uuid const& getInstanceId() const = 0;

				// Timestep methods
				virtual double getDeltaTime() const = 0;
				virtual void setDeltaTime(double step) = 0;
				virtual void setTimestamp(double timestamp) = 0;
				virtual double getTimestamp() const = 0;
				virtual float getFPS() const = 0;
				virtual void setFPSLabel(float fps_) = 0;
				virtual int64_t getStartTime() const = 0;

				virtual expected<void> initialize() = 0;

				// Base instance configurations
				virtual std::string getName() const = 0;
				virtual void setName(std::string const& name) = 0;
				virtual void clearLog() = 0;
				virtual expected<void> loadConfig(std::string const& configPath) = 0;

				virtual std::string getSolutionPath() const = 0;
				virtual void setSolutionPath(std::string const& path) = 0;

				virtual std::shared_ptr<UIState> getUIState() const = 0;
				virtual void setUIState(std::shared_ptr<UIState> state) = 0;

				virtual std::shared_ptr<KalmanFilter> getKalman() const = 0;

				virtual std::shared_ptr<internal::VersionControl> getVersionControl() const = 0;

				virtual expected<void> stepThread() const = 0;

				// Pipeline methods
				virtual expected<void> waitForEpoch(int epoch) const = 0;

				// Status indicators
				virtual bool isRunning() const = 0;
				virtual bool isPaused() const = 0;
				virtual bool isConfigured() const = 0;
//				virtual void setRunningState(bool state) = 0;

				// RT instance lifecycle management
				virtual expected<void> start() = 0;
				virtual bool stop() = 0;
				virtual expected<void> reset() = 0;
				virtual bool setPause(bool pause_state) = 0;
				virtual InstanceState getState() const = 0;
				virtual void setState(InstanceState state) = 0;
				
				virtual internal::SourceTypeEnum getSourceType() const = 0;

				// Event system interactions
				virtual std::vector<std::string> getBoundEventTypes(std::string const& domain) = 0;
				virtual int bindEvent(std::string const& domain, std::string const& eventType, std::function<void(pCValue, std::string, float)> handler, float callback_id = 0) = 0;
				virtual void unbindEvent(std::string const& domain, int bindId) = 0;
				virtual void postEvent(std::string const& domain, std::string const& eventType, pCValue data) = 0;

				virtual pCValue getLastOutput() = 0;

				virtual expected<cbuffer> getContextBuffer(std::string const& name) = 0;
				virtual void setContextBuffer(std::string const& name, cbuffer const& buffer) = 0;

				virtual void setPerformanceCounterState(bool enabled) = 0;

				virtual std::vector<std::pair<std::string, std::string>> getLogsList() const = 0;

				virtual std::shared_ptr<iface::Solution> getSolution() const = 0;
				virtual expected<void> setSolution(std::weak_ptr<iface::Solution> solution) = 0;

				virtual expected<void> loadLuaScript(std::string const& luaScript) = 0;
				virtual iface::LuaWorkers* lua() = 0;

				virtual uCVDictRoot config() = 0;
				virtual uCVDictRoot config(std::string const&) = 0;
				virtual uCVDictRoot state() = 0;
				virtual uCVDictRoot state(std::string const&) = 0;
				virtual ObjectRegistry<iface::Module>& modules() = 0;

				virtual Sink& inputSink() = 0;
				virtual Sink& outputSink() = 0;
				virtual internal::PerformanceBase& performance() = 0;

				virtual expected<void> loadLicenses() = 0;
				virtual expected<void> unloadLicenses() = 0;

			};
		}
	}
}
