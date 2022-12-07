/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <xtensor/xtensor.hpp>

#include "defines.h"
#include "module.h"
#include "builtin/basetypes.h"
#include "interface/cvdict.h"
#include "builtin/moduleimpl.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class EventActionHandler : public ModuleImpl {
			public:
				EventActionHandler(std::string const& moduleName) : ModuleImpl(moduleName) {}

				virtual expected<bool> process(pCValue event) = 0;

				iface::EventActionTypeEnum actionType_;
				pCValue config_;
			};
		}
	}
}
