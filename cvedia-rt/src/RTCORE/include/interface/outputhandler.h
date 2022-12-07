/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"
#include "builtin/moduleimpl.h"
#include "interface/scriptengine.h"

namespace cvedia {
	namespace rt {
		namespace iface {

			class OutputHandler : public ModuleImpl {
			public:
				OutputHandler(std::string const& moduleName) : ModuleImpl(moduleName) {}

				virtual expected<bool> write(pCValue sink_data = VAL(), std::string dynamicPath = "") = 0;
				virtual void close() = 0;
				virtual std::string getSink() = 0;

				pCValue config_;
				std::string sink_;
				std::string schema_;
				std::unique_ptr<iface::ScriptEngine> scriptEngine_;
			};
		}
	}
}
