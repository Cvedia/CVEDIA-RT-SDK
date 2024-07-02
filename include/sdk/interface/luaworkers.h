/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <thread>

#include "builtin/moduleimpl.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS LuaWorkers
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

				virtual expected<std::thread::id> createWorker(std::string const& oninit, std::string const& onrun) =0;
				virtual expected<void> deleteWorker(std::thread::id) =0;

				virtual float getOnRunLatency() const =0;
			};
		}
	}
}
