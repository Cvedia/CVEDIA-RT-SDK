/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "defines.h"
#include "rterror.h"
#include "cvaluefwd.h"
#include "uuid.h"

namespace cvedia {
	namespace rt {

		namespace iface {
			class Solution;
			class Instance;
			class SolutionManager;
			

			class EXPORT_CLASS InstanceController {
			public:
				virtual ~InstanceController() = default;

				virtual expected<std::shared_ptr<iface::Instance>> loadInstance() = 0;
				virtual expected<void> loadInstanceAsync() = 0;
				virtual expected<void> unloadInstance() = 0;

				virtual bool isInstanceLoaded() = 0;
				virtual bool isInstanceRunning() = 0;
				virtual bool isInstancePaused() = 0;
				
				virtual expected<void> startInstance() = 0;
				virtual expected<void> stopInstance(bool allowRestart) = 0;
				virtual expected<void> restartInstance() = 0;
				
				NODISCARD virtual expected<void> reloadConfigFromDisk() = 0;

				virtual uCVDictRoot config() = 0;
				NODISCARD virtual expected<void> setConfig(pCValue && config) = 0;
				NODISCARD virtual expected<pCValue> getConfigChanges() const = 0;				
				virtual expected<pCValue> getConfig() = 0;

				NODISCARD virtual expected<void> initializeFromExternalFile(std::string const& configFile) = 0;
				NODISCARD virtual expected<void> initializeFromFile(std::string const& configFile) = 0;
				NODISCARD virtual expected<void> initializeFromMemory(pCValue config) = 0;

				virtual Uuid const& getInstanceId() = 0;
				virtual expected<void> setInstanceUuid(Uuid const& id) = 0;
				virtual std::string getDisplayName() const = 0;
				virtual std::string const& getGroupName() = 0;
				virtual std::string const& getConfigFile() = 0;
				virtual expected<void> setDisplayName(std::string const& name) = 0;
				virtual expected<void> setGroupName(std::string const& name) = 0;
//				virtual void setConfigFile(std::string const& configFile) = 0;
				virtual expected<std::weak_ptr<iface::Solution>> getSolution() = 0;
				virtual expected<std::weak_ptr<Instance>> getInstance() = 0;
				virtual bool getAutoStart() const = 0;
				virtual bool getAutoRestart() const = 0;
				NODISCARD virtual expected<void> setAutoStart(bool autoStart) = 0;
				NODISCARD virtual expected<void> setAutoRestart(bool autoRestart) = 0;
				virtual bool getReadOnly() const = 0;
				NODISCARD virtual expected<void> setReadOnly(bool readOnly) = 0;
				virtual bool isSystemInstance() const = 0;
				virtual void setSystemInstance(bool systemInstance) = 0;
				virtual expected<void> setSolution(std::weak_ptr<iface::Solution> const& solution) = 0;
				virtual bool isPersistent() const = 0;
				virtual expected<void> setPersistent(bool persistent) = 0;

				virtual expected<void> enableSolutionManager(bool fallbackToDefault = true) = 0;
				virtual expected<std::shared_ptr<iface::SolutionManager>> getSolutionManager() const = 0;
				virtual expected<std::shared_ptr<iface::SolutionManager>> getSolutionManagerOrCreate(bool fallbackToDefault = true) = 0;

				NODISCARD virtual expected<void> save() = 0;
			};
		}
	}
}
