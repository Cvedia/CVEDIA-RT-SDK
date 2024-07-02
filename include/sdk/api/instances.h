/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <memory>
#include <unordered_set>

#include "defines.h"
#include "rterror.h"
#include "uuid.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class Instance;
			class InstanceController;
			class Module;
		}

		namespace api {
			namespace instances {
			    NODISCARD EXPORT expected<std::shared_ptr<iface::InstanceController>> createInstanceController();
				NODISCARD EXPORT expected<std::shared_ptr<iface::InstanceController>> createInstanceController(Uuid const& instanceId);
				NODISCARD EXPORT expected<std::shared_ptr<iface::InstanceController>> createInstanceControllerFromMemory(Uuid const& instanceId, std::string const& solutionId);
				NODISCARD EXPORT expected<std::shared_ptr<iface::InstanceController>> createInstanceControllerFromMemory(std::string const& solutionId);

				NODISCARD EXPORT expected<std::shared_ptr<iface::Instance>> createInstance(Uuid const& instanceId, std::string const& instanceName);
				NODISCARD EXPORT expected<std::shared_ptr<iface::Instance>> createInstance(std::string const& instanceName);

				NODISCARD EXPORT expected<void> deleteInstanceController(Uuid const& instanceId);

				NODISCARD EXPORT expected<std::shared_ptr<iface::InstanceController>> getInstanceController(Uuid const& uuid);
				NODISCARD EXPORT expected<std::weak_ptr<iface::Instance>> getInstanceById(Uuid const& uuid);
				NODISCARD EXPORT expected<void> unloadInstance(Uuid const& uuid);
				NODISCARD EXPORT std::unordered_set<std::shared_ptr<iface::InstanceController>> getInstanceControllers();
				NODISCARD EXPORT expected<void> discoverInstances();
				NODISCARD EXPORT expected<std::shared_ptr<iface::Module>> getModuleByName(std::string const& moduleName);
			};
		}
	}
}
