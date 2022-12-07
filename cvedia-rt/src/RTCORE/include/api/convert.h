/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once

#include "nlohmann/json.hpp"
#include "defines.h"

namespace cvedia {
	namespace rt {
		namespace api {
			namespace convert {
				EXPORT nlohmann::json cvalueToJson(CValue& cv, bool hideRuntimeVars = false, bool hideEmptyEntries = false, bool encodeBuffers = true);
				EXPORT pCValue cvalueFromJson(nlohmann::json const& obj);
			}
		}
	}
}