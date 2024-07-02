/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <memory>

#include "defines.h"
#include "interface/eventaction.h"
#include "interface/eventstorage.h"

namespace cvedia {
	namespace rt {
		namespace api {
			namespace events {
				// Event system
				EXPORT void addEventToQueue(std::string id, const pCValue ev);
				EXPORT void registerEventActionHandler(iface::EventActionTypeEnum actionType, module::createEventActionHandler func);
				EXPORT bool hasEventActionHandler(iface::EventActionTypeEnum actionType);
				EXPORT expected<module::createEventActionHandler> getEventActionHandler(iface::EventActionTypeEnum actionType);
				EXPORT void bindEvent(const std::string& descriptor, std::function<void(pCValue, std::string, float)> handler, float callback_id = 0);
				EXPORT void postEvent(pCValue data, const std::string& type = "", bool persist = true, const std::string& source_desc = "", cbuffer img = nullptr, const std::string& timestamp = "");
				EXPORT pCValue queryEvents(const std::string& query, int list_size = -1);
				EXPORT void processActions();
				EXPORT void loadConfig(pCValue eventActions);
				EXPORT pCValue processScript(pCValue config, pCValue data);
			};
		}
	}
}
