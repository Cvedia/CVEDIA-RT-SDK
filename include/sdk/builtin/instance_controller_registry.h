/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#include <set>
#include <unordered_set>

#include "defines.h"
#include "objectregistry.h"
#include "interface/instance_controller.h"
#include "filesystem.hpp"

namespace cvedia {
	namespace rt {

		enum class GroupType {
			User,
			System,
			All
		};

		class InstanceControllerRegistry
		{
		public:
			EXPORT static void clear();
			NODISCARD EXPORT static expected<void> discoverInstances();

			NODISCARD EXPORT static expected<std::shared_ptr<iface::InstanceController>> createInstanceControllerFromExternalFile(std::string const& configFile);
			NODISCARD EXPORT static expected<std::shared_ptr<iface::InstanceController>> createInstanceControllerFromFile(std::string const& configFile);
			NODISCARD EXPORT static expected<std::shared_ptr<iface::InstanceController>> createInstanceControllerFromMemory(pCValue const& config);
			NODISCARD EXPORT static expected<std::shared_ptr<iface::InstanceController>> createInstanceControllerFromMemory(std::string const& solutionId, pCValue const& config);
			
			NODISCARD EXPORT static expected<std::shared_ptr<iface::InstanceController>> getInstanceController(Uuid const& instanceId);
			NODISCARD EXPORT static expected<void> deleteInstanceController(Uuid const& instanceId);
			NODISCARD EXPORT static expected<void> remapUuid(Uuid const& oldId, Uuid const& newId);

			NODISCARD EXPORT static std::unordered_set<std::shared_ptr<iface::InstanceController>> getInstanceControllers(GroupType groupType = GroupType::All, std::string const& groupFilter = "*", std::string const& solutionIdFilter = "*");

			template<typename Comparator>
			NODISCARD EXPORT static std::set<std::shared_ptr<iface::InstanceController>, Comparator> getInstanceControllersSorted(GroupType groupType = GroupType::All, std::string const& groupFilter = "*", std::string const& solutionIdFilter = "*") {
				auto unorderedControllers = InstanceControllerRegistry::getInstanceControllers(groupType, groupFilter, solutionIdFilter);
				std::set<std::shared_ptr<iface::InstanceController>, Comparator> orderedcontrollers(unorderedControllers.begin(), unorderedControllers.end());
				return orderedcontrollers;
			}

			NODISCARD EXPORT static std::unordered_set<std::string> getGroupNames(GroupType groupType, std::string const solutionIdFilter = "*");

			NODISCARD EXPORT static expected<void> startAllInstances(std::string const& groupFilter = "*");
			NODISCARD EXPORT static expected<void> stopAllInstances(std::string const& groupFilter = "*");

			EXPORT static bool hasInstanceController(Uuid const& instanceId);
		private:
			static void _discoverInstancesInDir(fs::path const& dir, bool embeddedInstance);
			static ObjectRegistry<iface::InstanceController, Uuid> instanceControllers_;
		};
	}
}
