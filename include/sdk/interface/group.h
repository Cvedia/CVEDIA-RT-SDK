/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/

#pragma once
#include <list>
#include <unordered_set>

#include "defines.h"
#include "instance_controller.h"
#include <list>

namespace cvedia {
	namespace rt {
		namespace iface {
			class Solution;
			class Instance;
			class InstanceDescriptor;

			class EXPORT_CLASS Group : public std::enable_shared_from_this<Group>
			{
			public:
				Group() = default;
				virtual ~Group() = default;
				Group(std::string const& path, bool userInstances);

				Group(Group const&) = delete;
				Group& operator =(Group const&) = delete;

				NODISCARD virtual expected<void> initialize() = 0;
				virtual std::string const& getName() = 0;
				virtual std::string const& getPath() = 0;
				virtual void setPath(std::string const& path) = 0;
				virtual void setName(std::string const& name) = 0;

				NODISCARD virtual expected<void> startAllInstances() = 0;
				NODISCARD virtual expected<void> stopAllInstances() = 0;

				virtual expected<void> rename(std::string const& newName) = 0;
				NODISCARD virtual std::list<std::weak_ptr<iface::InstanceController>> getInstanceControllers() = 0;
			};
		}
	}
}
