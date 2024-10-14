/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "builtin/moduleimpl.h"

#include <cvalue.h>

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS MQTT : public ModuleImpl {
			public:
				MQTT(std::string const& moduleName) : ModuleImpl(moduleName) {}

				virtual bool connect() = 0;
				virtual bool connect(std::string const& mqttServer, std::string const& clientId, pCValue const& credentials=VAL(cmap{})) = 0;
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
