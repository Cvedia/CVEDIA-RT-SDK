/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <thread>
#include "defines.h"
#include "builtin/moduleimpl.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class LuaWorkers
			{
			public:
				virtual ~LuaWorkers() = default;

				virtual expected<void> loadLuaScript(std::string const& luaScript) =0;

				virtual expected<void> start() =0;
				virtual bool stop() =0;
				virtual void runStep() =0;
				virtual bool isRunningStep() = 0;

				virtual void mainThread() =0;
				virtual void workerThread(std::string const& oninit, std::string const& onrun) =0;

				// Lua interactions
				virtual expected<void> loadLuaFile(std::string const& luaFilePath) = 0;
				virtual cvec runLuaFunction(std::string const& luaMethod, std::vector<pCValue> const& stateData) const =0;
				virtual bool hasFunction(std::string const& functionName) const =0;
				virtual bool renameFunction(std::string const& oldFunctionName, std::string const& newFunctionName) = 0;
				virtual std::string runLuaCode(std::string const& luaCode) const =0;

				virtual expected<std::thread::id> createWorker(std::string const& oninit, std::string const& onrun) =0;
				virtual expected<void> deleteWorker(std::thread::id) =0;
			};
		}
	}
}
