/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <vector>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include "defines.h"
#include "interface/cvdict.h"

namespace cvedia {
	namespace rt {
		namespace api {
			namespace config {
				NODISCARD EXPORT expected<void> loadJsonFromDisk(std::string const& jsonFile);
				NODISCARD EXPORT expected<void> loadFromJsonString(std::string const& data);
				NODISCARD EXPORT expected<void> saveToDiskAsJson(std::string const& path);
				EXPORT expected<void> setNode(std::string const& nodePath, CValue const& value);
				NODISCARD EXPORT bool isNodeSet(std::string const& nodePath);
				NODISCARD EXPORT std::string asJsonString();
				EXPORT pCValue getNodeUnsafe();
				EXPORT pCValue getNodeUnsafe(std::string const& nodePath);
				EXPORT std::unique_lock<__shared_mutex_class> getWriteLock();
				EXPORT std::shared_lock<__shared_mutex_class> getReadLock();
				EXPORT uCVDictRoot getCVDict();
				EXPORT uCVDictRoot getCVDict(std::string const& path);

				EXPORT void addUserParameter(std::string key_name, std::string value);
				EXPORT std::vector<std::pair<std::string, std::string>> getUserParameters();
			};
		}
	}
}
