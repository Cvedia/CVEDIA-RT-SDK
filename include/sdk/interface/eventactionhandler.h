/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "eventstorage.h"
#include "builtin/moduleimpl.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class EventActionHandler : public ModuleImpl {
			public:
				EventActionHandler(std::string const& moduleName) : ModuleImpl(moduleName), actionType_() {
				}

				virtual expected<bool> process(pCValue event) = 0;

				iface::EventActionTypeEnum actionType_;
				pCValue config_;
			};
		}
	}
}
