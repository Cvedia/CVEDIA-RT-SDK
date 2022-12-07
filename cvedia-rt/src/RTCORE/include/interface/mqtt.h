/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"
#include "builtin/moduleimpl.h"
#include <nlohmann/json.hpp>

namespace cvedia {
	namespace rt {
		namespace iface {
			class MQTT : public ModuleImpl {
			public:
				MQTT(std::string const& moduleName) : ModuleImpl(moduleName) {}

				virtual bool connect() = 0;
				virtual bool connect(std::string const& mqttServer, std::string const& clientId) = 0;
				//virtual bool disconnect() = 0;

				//virtual void eventHandler(pCValue val) = 0;

//				virtual void readEvent() = 0;
				virtual std::string readMessage() = 0;
				virtual pCValue readJsonMessage(bool blockingCall) = 0;
				virtual void writeMessage(std::string topic, pCValue msg) = 0;
			};
		}
	}
}
