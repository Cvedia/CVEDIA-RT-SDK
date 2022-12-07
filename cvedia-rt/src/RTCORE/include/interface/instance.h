/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "cvdict.h"
#include "defines.h"
#include "builtin/objectregistry.h"
#include "builtin/performancebase.h"

namespace cvedia {
	namespace rt {
		class Sink;
		class PluginLua;

		namespace internal {
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

			class Instance: public std::enable_shared_from_this<Instance> {
			public:
				virtual ~Instance() = default;
				// Timestep methods
				virtual float getTimestep() const = 0;
				virtual void setTimestep(float step) = 0;
				virtual void stepTime() = 0;
				virtual float getTime() const = 0;
				virtual float getFPS() const = 0;
				virtual void setFPSLabel(float fps_) = 0;

				virtual expected<void> initialize() = 0;

				//void load();
				//void unload();

				// Base instance configurations
				virtual std::string getName() const = 0;
				virtual void setName(std::string const& name) = 0;
				virtual void clearLog() = 0;
				virtual expected<void> loadConfig(std::string const& configPath) = 0;
				//virtual bool checkConfig() = 0;
				//virtual expected<pCValue> getConfigSpecs(std::string pluginName) const = 0;
				virtual std::string getSolutionPath() const = 0;
				virtual void setSolutionPath(std::string const& path) = 0;

				virtual std::shared_ptr<UIState> getUIState() const = 0;
				virtual void setUIState(std::shared_ptr<UIState> state) = 0;

				// StateDict interaction
				//virtual void setStateDict(pCValue const& dict) const = 0;
				//virtual void setStateDictEntry(std::string const& path, std::string const& value) const = 0;
				//virtual std::string getStateDictAsJson(std::string const& path) const = 0;
				//virtual pCValue getStateDict() const = 0;
				//virtual pCValue getStateDict(std::string const& path) const = 0;

				// Composite classes
//				virtual void setLuaRoot(std::string const& luaRoot) = 0;
#if WITH_GENERIC_KALMAN
				virtual std::shared_ptr<KalmanFilter> getKalman() const = 0;
#endif
#if WITH_VERSION_CONTROL
				virtual std::shared_ptr<internal::VersionControl> getVersionControl() const = 0;

				virtual expected<void> stepThread() const = 0;


				// Pipeline methods
				virtual expected<void> waitForEpoch(int epoch) const = 0;
#endif

				// Status indicators
				virtual bool isRunning() const = 0;
				virtual bool isPaused() const = 0;
				virtual bool isConfigured() const = 0;
				virtual void setRunningState(bool state) = 0;

				// RT instance lifecycle management
				virtual expected<void> start() = 0;
				virtual bool stop() = 0;
				virtual expected<void> reset() = 0;
				virtual bool setPause(bool pause_state) = 0;
				virtual InstanceState getState() const = 0;
				virtual void setState(InstanceState state) = 0;
				
				virtual internal::SourceTypeEnum getSourceType() const = 0;

				// Event system interactions
				virtual void bindEvent(const std::string& descriptor, std::function<void(pCValue, std::string, float)> handler, float callback_id = 0) = 0;
				virtual void postEvent(pCValue data, const std::string& type = "", bool persist = true, const std::string& source_desc = "", cbuffer img = nullptr, const std::string& timestamp = "") = 0;
				virtual pCValue queryEvents(const std::string& query, int list_size = -1) = 0;
				virtual void processEvents() = 0;

				virtual pCValue getLastOutput() = 0;

//				virtual expected<void> writeStateToDisk() = 0;

				virtual void setPerformanceCounterState(bool enabled) = 0;

				virtual std::vector<std::pair<std::string, std::string>> getLogsList() const = 0;

#if WITH_SOLUTIONS
				virtual std::shared_ptr<iface::Solution> getSolution() const = 0;
				virtual expected<void> setSolution(std::weak_ptr<iface::Solution> solution) = 0;
#endif

				virtual bool reloadConfig() = 0;

#if BUILD_LUA_LIB
				virtual expected<void> loadLuaScript(std::string const& luaScript) = 0;
#endif

#if BUILD_LUA_LIB
				virtual iface::LuaWorkers* lua() = 0;
#endif
				virtual uCVDictRoot config() = 0;
				virtual uCVDictRoot config(std::string const&) = 0;
				virtual uCVDictRoot state() = 0;
				virtual uCVDictRoot state(std::string const&) = 0;
				virtual ObjectRegistry<iface::Module>& modules() = 0;

				virtual Sink& inputSink() = 0;
				virtual Sink& outputSink() = 0;
				virtual internal::PerformanceBase& performance() = 0;

			};
		}
	}
}
