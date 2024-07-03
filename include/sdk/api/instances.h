/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <memory>
#include <optional>
#include <unordered_set>

#include "defines.h"
#include "rterror.h"
#include "uuid.h"
#include "builtin/types.h"
#include "builtin/defaults.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class Instance;
			class InstanceController;
			class Module;
		}

		namespace api {
			namespace instances {

				struct EXPORT_CLASS InstanceWriteModel {
					std::optional<Uuid> instanceId;
					std::optional<std::string> instanceName;
					std::optional<std::string> solutionId;
					std::optional<std::string> groupName;
					std::optional<bool> persistent;
					std::optional<int> frameRateLimit;
					std::optional<bool> metadataMode;
					std::optional<bool> statisticsMode;
					std::optional<bool> diagnosticsMode;
					std::optional<bool> debugMode;
					std::optional<types::DetectorMode> detectorMode;
					std::optional<types::Sensitivity> detectionSensitivity;
					std::optional<types::Sensitivity> movementSensitivity;
					std::optional<types::Modality> sensorModality;

					std::optional<bool> loadAfterCreation;
					std::optional<bool> enableSolutionManager;

					static InstanceWriteModel createDefault() {
						
						// Minor note: This would be a good place to use designated initializers if we ever move to C++20

						auto const uuid = Uuid::randomUuid();
						auto const instanceName = uuid.toString();
						auto const solutionId = "";
						auto const groupName = "default";
						auto const persistent = defaults::solution_manager::persistent;
						auto const frameRateLimit = defaults::solution_manager::frameRateLimit;
						auto const metadataMode = defaults::solution_manager::metadataMode;
						auto const statisticsMode = defaults::solution_manager::statisticsMode;
						auto const diagnosticsMode = defaults::solution_manager::diagnosticsMode;
						auto const debugMode = defaults::solution_manager::debugMode;
						auto const detectorMode = types::getDefault<types::DetectorMode>();
						auto const detectionSensitivity = types::getDefault<types::Sensitivity>();
						auto const movementSensitivity = types::getDefault<types::Sensitivity>();
						auto const sensorModality = types::getDefault<types::Modality>();
						auto const loadAfterCreation = true;
						auto const enableSolutionManager = true;

						return { 
							uuid, instanceName, solutionId, groupName, persistent, frameRateLimit, metadataMode, statisticsMode, 
							diagnosticsMode, debugMode, detectorMode, detectionSensitivity, movementSensitivity, sensorModality, 
							loadAfterCreation, enableSolutionManager
						};
					}
				};

				NODISCARD EXPORT expected<std::shared_ptr<iface::InstanceController>> createInstanceController(InstanceWriteModel const& model);

				NODISCARD EXPORT expected<std::shared_ptr<iface::InstanceController>> createInstanceController();
				NODISCARD EXPORT expected<std::shared_ptr<iface::InstanceController>> createInstanceController(Uuid const& instanceId);
				NODISCARD EXPORT expected<std::shared_ptr<iface::InstanceController>> createInstanceControllerFromMemory(Uuid const& instanceId, std::string const& solutionId);
				NODISCARD EXPORT expected<std::shared_ptr<iface::InstanceController>> createInstanceControllerFromMemory(std::string const& solutionId);
				NODISCARD EXPORT expected<std::shared_ptr<iface::InstanceController>> createInstanceControllerFromFile(std::string const& configFile);

				NODISCARD EXPORT expected<void> updateInstanceController(std::shared_ptr<iface::InstanceController> ctrl, InstanceWriteModel const& model);

				NODISCARD EXPORT expected<std::shared_ptr<iface::Instance>> createInstance(Uuid const& instanceId, std::string const& instanceName);
				NODISCARD EXPORT expected<std::shared_ptr<iface::Instance>> createInstance(std::string const& instanceName);

				NODISCARD EXPORT expected<void> deleteInstanceController(Uuid const& instanceId);
				NODISCARD EXPORT expected<void> deleteAllInstanceControllers();

				NODISCARD EXPORT expected<std::shared_ptr<iface::InstanceController>> getInstanceController(Uuid const& uuid);
				NODISCARD EXPORT expected<std::weak_ptr<iface::Instance>> getInstanceById(Uuid const& uuid);
				NODISCARD EXPORT bool isLoaded(Uuid const& uuid);
				NODISCARD EXPORT bool isRunning(Uuid const& uuid);

				NODISCARD EXPORT expected<void> unloadInstance(Uuid const& uuid);
				NODISCARD EXPORT expected<void> startInstance(Uuid const& uuid);
				NODISCARD EXPORT expected<void> stopInstance(Uuid const& uuid);
				NODISCARD EXPORT expected<void> restartInstance(Uuid const& uuid);
				NODISCARD EXPORT std::unordered_set<std::shared_ptr<iface::InstanceController>> getInstanceControllers();
				NODISCARD EXPORT expected<void> discoverInstances();
				NODISCARD EXPORT expected<std::shared_ptr<iface::Module>> getModuleByName(std::string const& moduleName);
			};
		}
	}
}
