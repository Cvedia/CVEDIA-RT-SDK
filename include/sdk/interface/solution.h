/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <unordered_set>

#include "cvdict.h"
#include "defines.h"
#include "uuid.h"
#include "builtin/objectregistry.h"

namespace cvedia {
	namespace rt {
		namespace ui {
			using configMenuHandler = std::function<pCValue(std::string const& name, std::string const& type, std::string const& key)>;
		}
		namespace iface {
			class Module;
			class Instance;
			class UiCallback;
			class Group;

			struct configEntry
			{
				std::string name;
				std::string type;
				std::string key;
				ui::configMenuHandler handler;
			};

			class EXPORT_CLASS Solution : public std::enable_shared_from_this<Solution>
			{
			public:
				Solution() = default;
				virtual ~Solution() = default;
				Solution(std::string const& solutionPath);

				Solution(Solution const&) = delete;
				Solution& operator =(Solution const&) = delete;

				// TODO: @AJ add setBaseConfig / setScriptFile

				// renameTo()

				NODISCARD virtual expected<void> initialize() = 0;

				NODISCARD virtual expected<void> linkInstance(std::shared_ptr<iface::Instance> instance) = 0;
				NODISCARD virtual expected<void> unlinkInstance(std::shared_ptr<iface::Instance> instance) = 0;
				NODISCARD virtual std::unordered_set<Uuid> getLinkedInstances() = 0;
				virtual void clearLinkedInstances() = 0;

				NODISCARD virtual std::string getVersion() = 0;

				NODISCARD virtual expected<void> runOnInitCallback() = 0;
				NODISCARD virtual expected<void> runOnInstanceCreationCallback() = 0;

				virtual std::string const& getName() = 0;
				virtual std::string const& getFileName() = 0;
				virtual std::string const& getId() = 0;
				virtual std::string getSolutionPath() = 0;
				virtual std::string getScriptFilePath() = 0;
				virtual std::string getInstancePath() = 0;

				NODISCARD virtual expected<void> hasValidSolutionPath() = 0;
				NODISCARD virtual expected<void> hasValidBaseConfig() = 0;
				
				virtual expected<pCValue> getBaseConfig() const = 0;
				virtual uCVDictRoot config() = 0;
				virtual uCVDictRoot config(std::string const&) = 0;
				virtual uCVDictRoot state() = 0;
				virtual uCVDictRoot state(std::string const&) = 0;
				virtual ObjectRegistry<iface::Module>& modules() = 0;

				virtual bool addConfigWindow(std::string name, std::string type, std::string key, ui::configMenuHandler handler) = 0;
				virtual bool removeConfigWindow(std::string name) = 0;
				virtual std::vector<configEntry> getConfigWindows() = 0;
				NODISCARD virtual expected<iface::configEntry> getConfigWindow(std::string name) = 0;

				virtual bool registerExportConfigCallback(std::function<void(void)> onExportConfig) = 0;
				virtual bool unregisterExportConfigCallback() = 0;
				virtual std::function<void(void)> getExportConfigCallback() = 0;

				virtual iface::UiCallback & uiCallback() = 0;

				NODISCARD virtual expected<cmap> getPresetConfig(std::string const& presetName) = 0;
			};
		}
	}
}
