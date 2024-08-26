/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#include "defines.h"

#include "interface/cvdict.h"
#include "interface/module.h"
#include "rterror.h"

#define getConfigWithReadLock(varname) \
	RT_TRY(uCVDictRoot UNIQUE_NAME(cvdict), getConfigDict()); \
	auto UNIQUE_NAME(lck) = UNIQUE_NAME(cvdict)->getReadLock(); \
	/*{ auto & t = *UNIQUE_NAME(lck).mutex(); LockMark(t); }*/ \
	auto const varname = UNIQUE_NAME(cvdict)->getNodeUnsafe() \

#define getConfigWithWriteLock(varname) \
	RT_TRY(uCVDictRoot UNIQUE_NAME(cvdict), getConfigDict()); \
	auto UNIQUE_NAME(lck) = UNIQUE_NAME(cvdict)->getWriteLock(); \
	/*{ auto & t = *UNIQUE_NAME(lck).mutex(); LockMark(t); }*/ \
	auto const varname = UNIQUE_NAME(cvdict)->getNodeUnsafe() \

#define getStateWithReadLock(varname) \
	RT_TRY(uCVDictRoot UNIQUE_NAME(cvdict), getStateDict()); \
	auto UNIQUE_NAME(lck) = UNIQUE_NAME(cvdict)->getReadLock(); \
	/*{ auto & t = *UNIQUE_NAME(lck).mutex(); LockMark(t); }*/ \
	auto const varname = UNIQUE_NAME(cvdict)->getNodeUnsafe() \

#define getStateWithWriteLock(varname) \
	RT_TRY(uCVDictRoot UNIQUE_NAME(cvdict), getStateDict()); \
	auto UNIQUE_NAME(lck) = UNIQUE_NAME(cvdict)->getWriteLock(); \
	/*{ auto & t = *UNIQUE_NAME(lck).mutex(); LockMark(t); }*/ \
	auto const varname = UNIQUE_NAME(cvdict)->getNodeUnsafe() \

// Get the State and Config dictionaries and lock them respectively for Write, Write
#define getStateConfigWithWWLock(state, config) \
	RT_TRY(uCVDictRoot UNIQUE_NAME(cvstate), getStateDict()); \
	RT_TRY2(uCVDictRoot UNIQUE_NAME(cvconf), getConfigDict()); \
	auto UNIQUE_NAME(lck1) = UNIQUE_NAME(cvstate)->getWriteLockDefer(); \
	auto UNIQUE_NAME(lck2) = UNIQUE_NAME(cvconf)->getWriteLockDefer(); \
	std::lock(UNIQUE_NAME(lck1), UNIQUE_NAME(lck2)); \
	/*{ auto & t = *UNIQUE_NAME(lck1).mutex(); LockMark(t); } \
	{ auto & t = *UNIQUE_NAME(lck2).mutex(); LockMark(t); } */ \
	auto const state = UNIQUE_NAME(cvstate)->getNodeUnsafe(); \
	auto const config = UNIQUE_NAME(cvconf)->getNodeUnsafe() \

// Get the State and Config dictionaries and lock them respectively for Read, Write
#define getStateConfigWithRWLock(state, config) \
	RT_TRY(uCVDictRoot UNIQUE_NAME(cvstate), getStateDict()); \
	RT_TRY2(uCVDictRoot UNIQUE_NAME(cvconf), getConfigDict()); \
	auto UNIQUE_NAME(lck1) = UNIQUE_NAME(cvstate)->getReadLockDefer(); \
	auto UNIQUE_NAME(lck2) = UNIQUE_NAME(cvconf)->getWriteLockDefer(); \
	std::lock(UNIQUE_NAME(lck1), UNIQUE_NAME(lck2)); \
	/*{ auto & t = *UNIQUE_NAME(lck1).mutex(); LockMark(t); } \
	{ auto & t = *UNIQUE_NAME(lck2).mutex(); LockMark(t); } */ \
	auto const state = UNIQUE_NAME(cvstate)->getNodeUnsafe(); \
	auto const config = UNIQUE_NAME(cvconf)->getNodeUnsafe() \

// Get the State and Config dictionaries and lock them respectively for Write, Read
#define getStateConfigWithWRLock(state, config) \
	RT_TRY(uCVDictRoot UNIQUE_NAME(cvstate), getStateDict()); \
	RT_TRY2(uCVDictRoot UNIQUE_NAME(cvconf), getConfigDict()); \
	auto UNIQUE_NAME(lck1) = UNIQUE_NAME(cvstate)->getWriteLockDefer(); \
	auto UNIQUE_NAME(lck2) = UNIQUE_NAME(cvconf)->getReadLockDefer(); \
	std::lock(UNIQUE_NAME(lck1), UNIQUE_NAME(lck2)); \
	/*{ auto & t = *UNIQUE_NAME(lck1).mutex(); LockMark(t); } \
	{ auto & t = *UNIQUE_NAME(lck2).mutex(); LockMark(t); } */ \
	auto const state = UNIQUE_NAME(cvstate)->getNodeUnsafe(); \
	auto const config = UNIQUE_NAME(cvconf)->getNodeUnsafe() \

// Get the State and Config dictionaries and lock them respectively for Read, Read
#define getStateConfigWithRRLock(state, config) \
	RT_TRY(uCVDictRoot UNIQUE_NAME(cvstate), getStateDict()); \
	RT_TRY2(uCVDictRoot UNIQUE_NAME(cvconf), getConfigDict()); \
	auto UNIQUE_NAME(lck1) = UNIQUE_NAME(cvstate)->getReadLockDefer(); \
	auto UNIQUE_NAME(lck2) = UNIQUE_NAME(cvconf)->getReadLockDefer(); \
	std::lock(UNIQUE_NAME(lck1), UNIQUE_NAME(lck2)); \
	/*{ auto & t = *UNIQUE_NAME(lck1).mutex(); LockMark(t); } \
	{ auto & t = *UNIQUE_NAME(lck2).mutex(); LockMark(t); } */ \
	auto const state = UNIQUE_NAME(cvstate)->getNodeUnsafe(); \
	auto const config = UNIQUE_NAME(cvconf)->getNodeUnsafe() \


namespace cvedia {
	namespace rt {
		namespace iface {
			class VideoDecoder;
			class OutputHandler;
			class EventActionHandler;
			class InputHandler;
			class CVDict;
			class InferenceHandler;
			class Instance;
			class Module;
			class SolutionManager;
		}

		class PluginState;
		class PluginLua;
		enum Codec : size_t;

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
			typedef void(__stdcall* registerHandler)();
			typedef void(__stdcall* _Instantiate)(PluginInit* init);
			typedef void(__stdcall* _InstantiateLua)(PluginLuaInit* init);
#else
			typedef const char* (*_PluginName)();
			typedef void(*registerHandler)();
			typedef void(*_Instantiate)(PluginInit* init);
			typedef void(*_InstantiateLua)(PluginLuaInit* init);
#endif

			using createInferenceHandler = std::function<std::shared_ptr<iface::InferenceHandler>(std::string const& moduleName)>;
			using createInputHandler = std::function<std::unique_ptr<iface::InputHandler>(std::string const& moduleName)>;
			using createOutputHandler = std::function<std::shared_ptr<iface::OutputHandler>(std::string const& moduleName, std::string const& schema, std::string const& sink, std::string const& path, pCValue config)>;
			using createVideoDecoderHandler = std::function<std::unique_ptr<iface::VideoDecoder>(std::string const& deviceId, Codec const& codec)>;
			using createSolutionManagerHandler = std::function<std::shared_ptr<iface::SolutionManager>()>;

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

		class EXPORT_CLASS ModuleImpl : public iface::Module
		{
		public:
			EXPORT explicit ModuleImpl(std::string moduleName);

			EXPORT void* getPtr() override;
			
			EXPORT void verifyConfig(pCValue instanceConfig, ConfigCheckResults& configCheckResults) override;
			EXPORT pCValue getConfigSpecs() const override;

			EXPORT std::string const& getName() const override;
			EXPORT void setName(std::string const& name) override;

			EXPORT void setParent(iface::Module*) override;
			EXPORT iface::Module* getParent() const override;

			EXPORT expected<pCValue> getValue(std::string const& key, int dictType) const override;
			EXPORT expected<cvec> getValue(std::vector<std::string> const& keys, int dictType) const override;
			EXPORT expected<void> setValue(std::string const& key, pCValue const& value, int dictType) override;
			EXPORT expected<void> setValue(cmap const& values, int dictType) override;
			EXPORT expected<void> unsetKey(std::string const& key, int const dictType);

			EXPORT expected<std::unique_ptr<iface::CVDictRoot>> getStateDict() const override;
			EXPORT expected<std::unique_ptr<iface::CVDictRoot>> getConfigDict() const override;

			EXPORT expected<void> setConfig(pCValue const& conf) const override;
			EXPORT std::string getRootNode() const override;

			std::shared_mutex pluginMut_;
		private:
			std::string moduleName_;

			iface::Module* parent_;

			int curCtxVersion_ = -999;
		};
	}
}