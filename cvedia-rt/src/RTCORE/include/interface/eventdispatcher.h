/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class EventDispatcher {
			public:
				virtual ~EventDispatcher() = default;
				virtual void bind(const std::string descriptor, std::function< void(pCValue, std::string, float)>&& slot, float callbackId = 0) = 0;

				virtual long long post(const pCValue eventValues, const std::string& type, const std::string& sourceDesc, cbuffer img, const std::string& timestamp, bool processActions) = 0;

				virtual void process() = 0;
				virtual void reset() = 0;
			};
		}
	}
}
