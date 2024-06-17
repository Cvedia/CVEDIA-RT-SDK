/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <vector>
#include <memory>

#include "defines.h"
#include "rterror.h"

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
