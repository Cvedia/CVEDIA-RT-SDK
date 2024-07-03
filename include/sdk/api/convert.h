/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#include <unordered_set>

#include "nlohmann/json_fwd.hpp"
#include "defines.h"
#include "cvaluefwd.h"
#include "rterror.h"

namespace cvedia {
	namespace rt {
		namespace api {
			namespace convert {
				NODISCARD EXPORT nlohmann::json cvalueToJson(CValue const& cv, bool hideRuntimeVars = false, bool hideEmptyEntries = false, bool encodeBuffers = true);
				NODISCARD EXPORT std::string cvalueToJsonString(CValue const& cv, bool hideRuntimeVars, bool hideEmptyEntries, bool encodeBuffers, int indent);
				NODISCARD EXPORT pCValue cvalueFromJson(nlohmann::json const& obj);
				NODISCARD EXPORT expected<pCValue> cvalueFromJsonString(std::string const& data);
				EXPORT bool checkForCyclicDependencies(pCValue const& node, std::unordered_set<pCValue>& visited);
			}
		}
	}
}
