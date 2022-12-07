/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <shared_mutex>
#include "defines.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class CVDictRoot
			{
			public:
				virtual ~CVDictRoot() = default;
				NODISCARD virtual expected<void> loadJsonFromDisk(std::string const& jsonFile) = 0;
				NODISCARD virtual expected<void> loadFromJsonString(std::string const& jsonString) = 0;
				NODISCARD virtual expected<void> saveToDiskAsJson(std::string const& path) const = 0;
				NODISCARD virtual std::string asJsonString(int indent = 0) const = 0;

				NODISCARD virtual bool isNodeSet(std::string const& nodePath) const = 0;
				NODISCARD virtual pCValue getNodeUnsafe() const = 0;
				NODISCARD virtual pCValue getNodeUnsafe(std::string const& nodePath) const = 0;
				virtual expected<void> setNode(std::string const& nodePath, CValue const& cval) const = 0;

				NODISCARD virtual std::shared_lock<__shared_mutex_class> getReadLock() = 0;
				NODISCARD virtual std::unique_lock<__shared_mutex_class> getWriteLock() = 0;
			};

			class CVDict
			{
			public:
				virtual ~CVDict() = default;
				NODISCARD virtual std::unique_ptr<CVDictRoot> getRoot(std::string const& path) = 0;
				NODISCARD virtual std::unique_ptr<CVDictRoot> getRoot() = 0;
			};
		}
	}
}

using uCVDictRoot = std::unique_ptr<cvedia::rt::iface::CVDictRoot>;
