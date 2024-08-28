/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "defines.h"
#include "rterror.h"
#include "cvaluefwd.h"

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
			static std::shared_ptr<iface::EventDispatcher> eventDispatcher_;
			static std::shared_ptr<iface::EventStorage> eventStorage_;
			static std::shared_ptr<iface::EventAction> eventAction_;
		};
	}
}
