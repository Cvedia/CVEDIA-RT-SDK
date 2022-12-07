/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"
#include "builtin/basetypes.h"
#include "interface/inferencehandler.h"

namespace cvedia {
	namespace rt {
		namespace api {
			namespace util {
				NODISCARD EXPORT expected<pCValue> loadJsonFromDisk(std::string const& filename);
				NODISCARD EXPORT expected<void> saveJsonToDisk(std::string const& filename, CValue& dict, bool append = false);
				NODISCARD EXPORT expected<bool> checkValidJson(std::string const& jsonData, std::vector<std::string> const& checkNodesExist = {});

				NODISCARD EXPORT internal::DetectorResults NMS(internal::DetectorResults& results, bool nmsMergeBatches, float nmsScoreThreshold, float nmsIouThreshold);
				NODISCARD EXPORT expected<void> detsToAbsCoords(internal::DetectorResults& results, std::vector<Rect2f> const& areas, bool relToAbs, bool absToWorld, Size const& worldSize);
				EXPORT void filterEdgeDetections(internal::DetectorResults& results, std::vector<Rect2f> const& areas, std::vector<cbuffer> const& source);

				EXPORT std::string getDynamicString(std::string inputString, pCValue obj);
			}
		}
	}
}
