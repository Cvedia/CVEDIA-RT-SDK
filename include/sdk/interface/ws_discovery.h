/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <memory>
#include <nlohmann/json_fwd.hpp>

#include "defines.h"
#include "rterror.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class OnvifDevice;

			class EXPORT_CLASS WsDiscovery {
			public:
				virtual ~WsDiscovery() = default;

				virtual expected<std::vector<std::shared_ptr<OnvifDevice>>> discoverOnvifDevices() = 0;
				virtual expected<std::shared_ptr<iface::OnvifDevice>> deserializeDevice(nlohmann::json const& json) const = 0;
			};
		}
	}
}
