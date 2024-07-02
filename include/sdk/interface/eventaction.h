/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "builtin/moduleimpl.h"

namespace cvedia
{
	namespace rt
	{
		class EventActionImpl;
	}
}

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS EventAction : public ModuleImpl {
			public:
				EventAction(std::string const& moduleName) : ModuleImpl(moduleName) {}

//				virtual void registerEventActionHandler(EventActionTypeEnum actionType, module::createEventActionHandler func);

				virtual expected<bool> loadConfig(pCValue eventActionsConfigObj) = 0;

				virtual void processActions() = 0;
				virtual pCValue processScript(pCValue config, pCValue data) = 0;
				virtual void addEventToQueue(std::string id, pCValue const ev) = 0;

				virtual std::vector<EventActionImpl> getEventActions() = 0;

				virtual expected<bool> process(pCValue eventObj) = 0;
			};
		}
	}
}
