/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <xtensor/xtensor.hpp>

#include "defines.h"
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
			class EventAction : public ModuleImpl {
			public:
				EventAction(std::string const& moduleName) : ModuleImpl(moduleName) {}

//				virtual void registerEventActionHandler(EventActionTypeEnum actionType, module::createEventActionHandler func);

				virtual expected<bool> loadConfig(pCValue eventActionsConfigObj) = 0;

				virtual void processActions() = 0;
				virtual pCValue processScript(pCValue config, pCValue data) = 0;
				virtual void addEventToQueue(std::string id, const pCValue ev) = 0;

				virtual std::vector<EventActionImpl> getEventActions() = 0;

				virtual expected<bool> process(pCValue eventObj) = 0;
			};
		}
	}
}
