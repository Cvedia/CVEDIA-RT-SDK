/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "cvdict.h"
#include "defines.h"
#include "builtin/objectregistry.h"

namespace cvedia {
	namespace rt {
		namespace ui {
			using configMenuHandler = std::function<pCValue(std::string const& name, std::string const& type, std::string const& key)>;
		}
		namespace iface {
			class Module;
			class Instance;
			class InstanceDescriptor;
			class UiCallback;

			struct configEntry
			{
				std::string name;
				std::string type;
				std::string key;
				ui::configMenuHandler handler;
			};

			class Solution : public std::enable_shared_from_this<Solution>
			{
			public:
				Solution() = default;
				virtual ~Solution() = default;
				Solution(std::string const& solutionPath);

				Solution(Solution const&) = delete;
				Solution& operator =(Solution const&) = delete;

				// TODO: @AJ add setBaseConfig / setScriptFile

				// renameTo()

				virtual expected<void> initialize() = 0;

				virtual expected<std::shared_ptr<InstanceDescriptor>> addInstanceDescriptor(std::string const& configJson) = 0;
				NODISCARD virtual expected<std::shared_ptr<InstanceDescriptor>> getInstanceDescriptor(std::string const& instanceName) = 0;
				NODISCARD virtual std::vector<std::shared_ptr<InstanceDescriptor>> getInstanceDescriptors() = 0;
				virtual expected<void> removeInstanceDescriptor(std::string const& instanceName) = 0;

				NODISCARD virtual expected<std::shared_ptr<iface::Instance>> createInstance(std::string const& instanceName) = 0;
				NODISCARD virtual expected<std::shared_ptr<iface::Instance>> createInstance(std::string const& instanceName, pCValue const& instanceConfig) = 0;
				virtual expected<void> destroyInstance(std::string const& instanceName) = 0;
				virtual expected<void> deleteInstance(std::string const& instanceName) = 0;
				virtual expected<void> renameInstance(std::string const& name, std::string const& newName) = 0;

				// Registry encapsulation
				NODISCARD virtual expected<std::shared_ptr<iface::Instance>> getInstance(std::string const& instanceName) = 0;
				NODISCARD virtual std::vector<std::shared_ptr<iface::Instance>> getInstances() = 0;
				NODISCARD virtual bool hasInstance(std::string const& instanceName) = 0;
				NODISCARD virtual std::vector<std::string> getInstanceNames() = 0;
				virtual void clearInstances() = 0;

				virtual std::string const& getName() = 0;
				virtual std::string getSolutionPath() = 0;
				virtual std::string getScriptFilePath() = 0;

				virtual expected<void> hasValidSolutionPath() = 0;
				virtual expected<void> hasValidBaseConfig() = 0;

				virtual expected<int> discoverInstances() = 0;

				virtual uCVDictRoot config() = 0;
				virtual uCVDictRoot config(std::string const&) = 0;
				virtual uCVDictRoot state() = 0;
				virtual uCVDictRoot state(std::string const&) = 0;
				virtual ObjectRegistry<iface::Module>& modules() = 0;

				virtual bool addConfigWindow(std::string name, std::string type, std::string key, ui::configMenuHandler handler) = 0;
				virtual bool removeConfigWindow(std::string name) = 0;
				virtual std::vector<configEntry> getConfigWindows() = 0;
				virtual expected<iface::configEntry> getConfigWindow(std::string name) = 0;

				virtual iface::UiCallback & uiCallback() = 0;
			};
		}
	}
}
