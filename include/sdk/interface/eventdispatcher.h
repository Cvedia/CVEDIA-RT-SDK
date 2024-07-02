/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <functional>
#include "cvaluefwd.h"
#include "defines.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS EventDispatcher
			{
			public:
				virtual ~EventDispatcher() = default;
				virtual std::vector<std::string> getBoundEventTypes() = 0;
				virtual void bind(std::string descriptor, std::function<void(pCValue, std::string, float)>&& slot, float callbackId = 0) = 0;

				virtual long long post(pCValue eventValues, std::string const& type, std::string const& sourceDesc, cbuffer img, std::string const& timestamp, bool processActions) = 0;

				virtual void reset() = 0;
			};
		}
	}
}
