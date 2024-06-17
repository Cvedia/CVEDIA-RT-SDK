/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <nlohmann/json_fwd.hpp>

#include "defines.h"
#include "rterror.h"
#include "uuid.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			struct OnvifStream {
				std::string name;
				std::string codec;
				std::string uri;
				int width;
				int height;
				float fps;
			};

			enum OnvifDeviceState {
				Ok,
				Unknown,
				Unauthorized
			};

			class EXPORT_CLASS OnvifDevice {
			public:
				virtual ~OnvifDevice() = default;

				virtual std::string const& getName() const = 0;
				virtual std::string const& getXaddr() const = 0;
				virtual std::string const& getScopes() const = 0;
				virtual std::string const& getTypes() const = 0;
				virtual std::string const& getMetadataVersion() const = 0;
				virtual std::string const& getManufacturer() const = 0;
				virtual std::string const& getModel() const = 0;
				virtual std::string const& getFirmwareVersion() const = 0;
				virtual std::string const& getSerialNumber() const = 0;
				virtual std::string const& getHardwareId() const = 0;
				virtual std::string const& getUsername() const = 0;
				virtual std::string const& getPassword() const = 0;
				virtual void setUsername(std::string const& username) = 0;
				virtual void setPassword(std::string const& password) = 0;

				virtual OnvifDeviceState getState() const = 0;

				virtual std::map<std::string, OnvifStream> const& getStreams() const = 0;

				virtual expected<void> populate() = 0;
				virtual nlohmann::json serialize() = 0;
				virtual Uuid getUuid() const = 0;
			};
		}
	}
}
