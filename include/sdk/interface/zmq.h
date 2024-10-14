/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "defines.h"
#include "builtin/moduleimpl.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS ZMQ : public ModuleImpl {
			public:
				ZMQ(std::string const& moduleName) : ModuleImpl(moduleName) {}

				virtual bool connect() = 0;
				virtual bool connect(std::string const& zmqServer) = 0;
				virtual bool disconnect() = 0;

				virtual void eventHandler(pCValue val) = 0;

//				virtual void readEvent() = 0;
				virtual expected<pCValue> readMessage() = 0;
				virtual void writeMessage(std::string topic, pCValue msg) = 0;
			};
		}
	}
}
