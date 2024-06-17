/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once 
#include "builtin/moduleimpl.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class UiCallback
			{
			public:
				UiCallback() = default;
				virtual ~UiCallback() = default;
				UiCallback(UiCallback const& other) = default;
				UiCallback& operator=(UiCallback const& other) = default;

				UiCallback(UiCallback&& other) = default;
				UiCallback& operator=(UiCallback&& other) = default;

				virtual void registerHandler(std::string const& eventName, std::string const& handlerName, module::uiHandler func) = 0;
				virtual void unregisterHandler(std::string const& eventName, std::string const& handlerName) = 0;
				virtual bool hasEventHandlers(std::string const& eventName) = 0;
				virtual std::map<std::string, module::uiHandler> getEventHandlers(std::string const& eventName) = 0;
				NODISCARD virtual expected<module::uiHandler> getEventHandlerByName(std::string const& eventName, std::string const& handlerName) = 0;

				virtual void reset() = 0;
			};
		}
	}
}
