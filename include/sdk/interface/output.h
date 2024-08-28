/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <string>

#include "cvalue.h"
#include "defines.h"
#include "builtin/moduleimpl.h"

namespace cvedia {
	namespace rt {
		class ConfigCheckResults;
	}
}

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS Output : public ModuleImpl
			{
			public:
				Output(std::string const& moduleName, std::weak_ptr<cvedia::rt::iface::Instance> const&) : ModuleImpl(moduleName) {}

				virtual expected<bool> loadHandlersFromConfig() = 0;
				virtual expected<std::shared_ptr<iface::OutputHandler>> addHandler(std::string const& name, std::string const& source, std::string const& sink = "output", pCValue config = VAL()) = 0;
				virtual expected<void> removeHandler(std::string const& name) = 0;
				virtual expected<bool> hasHandler(std::string const& name) = 0;

				void verifyConfig(pCValue instanceConfig, ConfigCheckResults& configCheckResults) override = 0;

				virtual expected<bool> write() = 0;
				virtual void close() = 0;
			};
		}
	}
}
