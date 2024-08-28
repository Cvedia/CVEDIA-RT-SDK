/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "rterror.h"
#include <nlohmann/json_fwd.hpp>
#include "interface/module.h"

namespace cvedia {
	namespace rt {
		class ConfigDescriptor;
	}
}

namespace cvedia {
	namespace rt {
		namespace iface {

			typedef std::map<std::string, std::array<int, 10>> SchemaCapabilities;

			class EXPORT_CLASS Exporter {
			public:
				Exporter() = default;
				virtual ~Exporter() = default;

				virtual std::string getName() = 0;

				// Returns the maximum capabilities of the exporter
				virtual SchemaCapabilities getCapabilities() = 0;
				// Returns the capabilities requested by the currently active instance
				virtual expected<iface::SchemaCapabilities> getRequestedCapabilities() = 0;

				virtual expected<void> initialize(std::string const& initFile) = 0;

				virtual std::map<std::string, std::shared_ptr<ConfigDescriptor>> getConfigDescriptors() = 0;

				virtual expected<void> writeData(nlohmann::json const& data) = 0;
				virtual expected<void> writeData(std::string const& data) = 0;
			};
		}
	}
}
