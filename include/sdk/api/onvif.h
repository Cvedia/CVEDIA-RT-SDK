/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"
#include "rterror.h"
#include "uuid.h"

#include "interface/onvif_device.h"

namespace cvedia {
	namespace rt {
		namespace api {
			namespace onvif {
				EXPORT expected<void> discoverDevices();

				EXPORT expected<std::shared_ptr<iface::OnvifDevice>> getDevice(Uuid const& deviceUuid);
				EXPORT std::map<Uuid, std::shared_ptr<iface::OnvifDevice>> const& getDevices();
				EXPORT std::map<Uuid, std::shared_ptr<iface::OnvifDevice>> getDevicesSeen();

				EXPORT expected<std::vector<iface::OnvifStream>> getStreams(Uuid const& deviceUuid);
				EXPORT expected<bool> setCredentials(Uuid const& deviceUuid, std::string const& username, std::string const& password);
			}
		};
	}
}
