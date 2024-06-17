/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "defines.h"
#include "builtin/basetypes.h"
#include "interface/inferencehandler.h"
#include <nlohmann/json_fwd.hpp>

namespace cvedia {
	namespace rt {
		namespace api {
			namespace util {
				NODISCARD EXPORT expected<pCValue> loadJsonFromDisk(std::string const& filename);
				NODISCARD EXPORT expected<void> saveJsonToDisk(std::string const& filename, CValue const& dict, bool append = false);
				NODISCARD EXPORT expected<bool> checkValidJson(std::string const& jsonData, std::vector<std::string> const& checkNodesExist = {});
				EXPORT std::string getDynamicString(std::string inputString, pCValue obj);
				EXPORT std::string parseTemplate(std::string inputString);
				EXPORT std::string parseTemplate(std::string inputString, nlohmann::json const& js);
				EXPORT pCValue NMS(pCValue detectionsObj, float const nmsScoreThreshold, float const nmsIouThreshold);
			}
		}
	}
}
