/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <string>

#include "defines.h"
#include <sol.hpp>


namespace cvedia {
	namespace rt {
		class PluginLua;
	}
}

namespace cvedia {
	namespace rt {
		namespace iface {
			class Module;

			class ScriptEngine
			{
			public:
				ScriptEngine() = default;
				virtual ~ScriptEngine() = default;
				ScriptEngine(ScriptEngine const& other) = default;
				ScriptEngine& operator=(ScriptEngine const& other) = default;

				ScriptEngine(ScriptEngine&& other) = default;
				ScriptEngine& operator=(ScriptEngine&& other) = default;

				virtual sol::state* getLua() = 0;
				virtual std::vector<pCValue> callLogic(pCValue ctxData, std::string method) = 0;
				virtual expected<std::vector<pCValue>> executeScript(std::vector<pCValue> const& stateData, std::string const& luaMethod, int numResults, std::string const& pluginName, std::string const& className, Module* self = nullptr) = 0;
				virtual std::vector<pCValue> executeString(std::string const& luaCode) = 0;
				virtual expected<void> loadScript(std::string scriptName) = 0;
				virtual bool isScriptLoaded(std::string const scriptName) const = 0;
				virtual void clearLoadedFilesList() = 0;
				virtual bool scriptLoadedOk() const = 0;
				virtual void setAutoReload(bool state) = 0;
				virtual void collectGarbage() = 0;
			};
		}
	}
}
