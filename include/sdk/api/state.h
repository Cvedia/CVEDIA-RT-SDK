/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <memory>
#include <mutex>
#include <shared_mutex>
#include "defines.h"
#include "interface/cvdict.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class CVDict;
		}
	}
}

namespace cvedia {
	namespace rt {
		namespace api {
			namespace state {
				NODISCARD EXPORT expected<void> loadJsonFromDisk(std::string const& jsonFile);
				NODISCARD EXPORT expected<void> loadFromJsonString(std::string const& data);
				NODISCARD EXPORT expected<void> saveToDiskAsJson(std::string const& path);
				NODISCARD EXPORT expected<void> setNode(std::string const& nodePath, CValue const& value);
				NODISCARD EXPORT bool isNodeSet(std::string const& nodePath);
				NODISCARD EXPORT std::string asJsonString();
				EXPORT pCValue getNodeUnsafe();
				EXPORT pCValue getNodeUnsafe(std::string const& nodePath);
				EXPORT std::unique_lock<shared_mutex_class> getWriteLock();
				EXPORT std::shared_lock<shared_mutex_class> getReadLock();
				EXPORT uCVDictRoot getCVDict();
				EXPORT uCVDictRoot getCVDict(std::string const& path);
				EXPORT void clear();
			};
		}
	}
}
