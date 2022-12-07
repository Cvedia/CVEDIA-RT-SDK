/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <memory>

#include "defines.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class EventAction;
			class EventDispatcher;
			class EventStorage;
		}

		class GlobalEvents {
		public:
			EXPORT static expected<void> initialize(pCValue eventActionsConfigObj);

			EXPORT static iface::EventDispatcher& dispatcher();
			EXPORT static iface::EventStorage& storage();
			EXPORT static iface::EventAction& action();

		private:
			static std::unique_ptr<iface::EventDispatcher> eventDispatcher_;
			static std::unique_ptr<iface::EventStorage> eventStorage_;
			static std::unique_ptr<iface::EventAction> eventAction_;
		};
	}
}
