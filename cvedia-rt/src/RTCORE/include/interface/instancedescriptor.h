/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class Solution;
			class Instance;

			class InstanceDescriptor {
			public:
				virtual ~InstanceDescriptor() = default;

				NODISCARD virtual expected<pCValue> getConfigFromDisk(bool mergeSolutionConfig = true) const = 0;
				virtual expected<void> saveConfigToDisk(CValue& config) const = 0;
				virtual expected<void> deleteConfigFromDisk() const = 0;
				virtual expected<void> saveInstanceConfigToDisk() const = 0;
				virtual expected<void> loadInstanceConfigFromDisk() const = 0;
				virtual expected<void> configureInstance(std::weak_ptr<iface::Instance> const& instance, std::weak_ptr<iface::Solution> const& solution) = 0;
				virtual expected<void> renameTo(std::string const& newName) = 0;
				
				virtual std::string const& getName() = 0;
				virtual std::string const& getConfigFile() = 0;
			};
		}
	}
}
