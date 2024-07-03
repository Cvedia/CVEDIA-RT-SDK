/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "defines.h"

#include <shared_mutex>
#include <mutex>

#include "cvaluefwd.h"
#include "rterror.h"
#include "builtin/basetypes.h"

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

				NODISCARD virtual pCValue getCopyOfNode() const = 0;
				NODISCARD virtual pCValue getCopyOfNode(std::string const& nodePath) const = 0;
				NODISCARD virtual expected<pCValue> getCopyOfNodeIfSet(std::string const& nodePath) const = 0;
				NODISCARD virtual pCValue getNodeUnsafe() const = 0;
				NODISCARD virtual pCValue getNodeUnsafe(std::string const& nodePath) const = 0;
				NODISCARD virtual expected<pCValue> getNodeIfSetUnsafe(std::string const& nodePath) const = 0;
				NODISCARD virtual bool isNodeSet(std::string const& nodePath) const = 0;
				virtual expected<void> setNode(std::string const& nodePath, CValue const& cval) = 0;
				virtual bool deleteNode(std::string const& nodePath) = 0;
				virtual void merge(pCValue const& other, bool refSourceIsSelf) = 0;
				NODISCARD virtual expected<void> mergeAt(std::string const& path, pCValue const& other, bool refSourceIsSelf) = 0;
				virtual void override(pCValue const& other) = 0;
				NODISCARD virtual expected<void> overrideAt(std::string const& path, pCValue const& other) = 0;

				NODISCARD virtual std::vector<std::string> getKeys() const = 0;

				virtual bool setValue(std::string const& path, bool x) = 0;
				virtual bool setValue(std::string const& path, int x) = 0;
				virtual bool setValue(std::string const& path, float x) = 0;
				virtual bool setValue(std::string const& path, double x) = 0;
				virtual bool setValue(std::string const& path, std::string const& x) = 0;
				virtual bool setValue(std::string const& path, char* const& x) = 0;
				virtual bool setValue(std::string const& path, Rect2f const& x) = 0;
				virtual bool setValue(std::string const& path, cbuffer const& x) = 0;
				virtual bool setValue(std::string const& path, std::vector<float> const& x) = 0;
				virtual bool setValue(std::string const& path, CValue const& x) = 0;
				virtual bool setValue(std::string const& path, cvec const& x) = 0;
				virtual bool setValue(std::string const& path, cmap const& x) = 0;

				NODISCARD virtual std::shared_lock<shared_mutex_class> getReadLock() const = 0;
				NODISCARD virtual std::unique_lock<shared_mutex_class> getWriteLock() const = 0;
				NODISCARD virtual std::shared_lock<shared_mutex_class> getReadLockAdopt() const = 0;
				NODISCARD virtual std::unique_lock<shared_mutex_class> getWriteLockAdopt() const = 0;
				NODISCARD virtual std::shared_lock<shared_mutex_class> getReadLockDefer() const = 0;
				NODISCARD virtual std::unique_lock<shared_mutex_class> getWriteLockDefer() const = 0;
				NODISCARD virtual shared_mutex_class& getMutex() const = 0;
			};

			class EXPORT_CLASS CVDict
			{
			public:
				virtual ~CVDict() = default;
				NODISCARD virtual std::unique_ptr<CVDictRoot> getRoot(std::string const& path) = 0;
				NODISCARD virtual std::unique_ptr<CVDictRoot> getRoot() = 0;
				virtual void setData(pCValue&& cv) = 0;
			};
		}
	}
}

