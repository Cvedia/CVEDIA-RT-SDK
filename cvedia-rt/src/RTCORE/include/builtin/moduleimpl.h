/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once

#include <vector>
#include <string>

#include "defines.h"
#include "interface/cvdict.h"
#include "interface/eventstorage.h"
#include "interface/module.h"

#define getConfigWithReadLock(varname) \
	RT_TRY(uCVDictRoot UNIQUE_NAME(cvdict), getConfigDict()); \
	auto UNIQUE_NAME(lck) = UNIQUE_NAME(cvdict)->getReadLock(); \
	auto const varname = UNIQUE_NAME(cvdict)->getNodeUnsafe() \

#define getConfigWithWriteLock(varname) \
	RT_TRY(uCVDictRoot UNIQUE_NAME(cvdict), getConfigDict()); \
	auto UNIQUE_NAME(lck) = UNIQUE_NAME(cvdict)->getWriteLock(); \
	auto const varname = UNIQUE_NAME(cvdict)->getNodeUnsafe() \

#define getStateWithReadLock(varname) \
	RT_TRY(uCVDictRoot UNIQUE_NAME(cvdict), getStateDict()); \
	auto UNIQUE_NAME(lck) = UNIQUE_NAME(cvdict)->getReadLock(); \
	auto const varname = UNIQUE_NAME(cvdict)->getNodeUnsafe() \

#define getStateWithWriteLock(varname) \
	RT_TRY(uCVDictRoot UNIQUE_NAME(cvdict), getStateDict()); \
	auto UNIQUE_NAME(lck) = UNIQUE_NAME(cvdict)->getWriteLock(); \
	auto const varname = UNIQUE_NAME(cvdict)->getNodeUnsafe() \

namespace cvedia {
	namespace rt {
		namespace iface {
			class OutputHandler;
			class EventActionHandler;
			class InputHandler;
			class CVDict;
			class InferenceHandler;
			class Instance;
			class Module;
		}

		class PluginState;
		class PluginLua;

		namespace internal {
			class PerformanceBase;
			class ConfigCheckResults;
		}

		namespace module {
			struct PluginInit {
				std::string pluginName;
				std::string parentName;
				std::shared_ptr<iface::Module> pluginInst;
			};

			struct PluginLuaInit {
				std::string pluginName;
				std::string parentName;
				std::shared_ptr<PluginLua> pluginInst;
			};

#ifdef _WIN32
			typedef const char* (__stdcall* _PluginName)();
			typedef void(__stdcall* _RegisterHandler)();
			typedef void(__stdcall* _Instantiate)(PluginInit* init);
			typedef void(__stdcall* _InstantiateLua)(PluginLuaInit* init);
#else
			typedef const char* (*_PluginName)();
			typedef void(*_RegisterHandler)();
			typedef void(*_Instantiate)(PluginInit* init);
			typedef void(*_InstantiateLua)(PluginLuaInit* init);
#endif

			using createInferenceHandler = std::function<std::shared_ptr<iface::InferenceHandler>(std::string const& moduleName)>;
			using createEventActionHandler = std::function<std::shared_ptr<iface::EventActionHandler>(std::string const& moduleName, iface::EventActionTypeEnum actionType, pCValue config)>;
			using createInputHandler = std::function<std::shared_ptr<iface::InputHandler>(std::string const& moduleName)>;
			using createOutputHandler = std::function<std::shared_ptr<iface::OutputHandler>(std::string const& moduleName, std::string schema, std::string sink, pCValue config)>;

			using uiHandler = std::function<pCValue(std::string const& eventName, pCValue const& input)>;

			struct PluginTpl {
				void* hMod = nullptr;
				std::string pluginName;
				std::vector<std::string> interfaces;
				_Instantiate _InstantiatePtr = nullptr;
				_InstantiateLua _InstantiateLuaPtr = nullptr;
			};

			struct PluginInst {
				_Instantiate _InstantiatePtr;
				_InstantiateLua _InstantiateLuaPtr;
				std::shared_ptr<iface::Module> ptr;
				std::shared_ptr<PluginLua> ptrLua;
				std::string pluginName;
				std::string className;
				std::string instanceName;
				std::string type;
			};
		}

		class ModuleImpl : public iface::Module
		{
		public:
			EXPORT ModuleImpl(std::string moduleName);

			EXPORT void* getPtr() override;
			
			EXPORT void verifyConfig(pCValue instanceConfig, ConfigCheckResults& configCheckResults) override;
			EXPORT pCValue getConfigSpecs() override;

			EXPORT std::string getName() override;

			EXPORT void setParent(iface::Module*) override;
			EXPORT iface::Module* getParent() override;

			EXPORT expected<std::unique_ptr<iface::CVDictRoot>> getStateDict() const override;
			EXPORT expected<std::unique_ptr<iface::CVDictRoot>> getConfigDict() const override;

			EXPORT expected<void> setConfig(pCValue conf) const override;
			EXPORT std::string getRootNode() const override;
			
			mutable __shared_mutex_class pluginMut_;
		private:
			std::string moduleName_;

			iface::Module* parent_;

			int curCtxVersion_ = -999;
		};
	}
}