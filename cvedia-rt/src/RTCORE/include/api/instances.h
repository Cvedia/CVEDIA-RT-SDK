/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <memory>

#include "defines.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class Module;
			class Instance;
		}

		namespace api {
			namespace instances {
				EXPORT expected<std::shared_ptr<iface::Instance>> createInstance();
				EXPORT expected<std::shared_ptr<iface::Instance>> createInstance(std::string const& instanceName);
				EXPORT expected<std::shared_ptr<iface::Instance>> getInstanceByName(std::string const& instanceName, std::string const& solutionName = "");
				EXPORT expected<std::shared_ptr<iface::Module>> getModuleByName(std::string const& moduleName);
			};
		}
	}
}
