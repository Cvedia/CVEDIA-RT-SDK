/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <vector>
#include <memory>

#include "defines.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class Instance;
		}
	}
}

namespace cvedia {
	namespace rt {
		namespace api {
			namespace registry
			{
				EXPORT std::vector<std::shared_ptr<iface::Instance>> getLoadedInstances();

				namespace internal {
					EXPORT expected<void> registerInstance(const std::shared_ptr<iface::Instance>&);
					EXPORT expected<void> unregisterInstance(const std::shared_ptr<iface::Instance>&);
				}
			};
		}
	}
}
