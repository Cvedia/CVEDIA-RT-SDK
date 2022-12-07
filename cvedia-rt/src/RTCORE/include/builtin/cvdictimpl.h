/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <shared_mutex>

#include "defines.h"
#include "interface/cvdict.h"
#include "builtin/lockableobject.h"

namespace cvedia {
	namespace rt {
		class CVDictImpl;

		class CVDictRootImpl final : public iface::CVDictRoot
		{
		public:
			CVDictRootImpl(CVDictImpl&, pCValue cvDict);
			NODISCARD EXPORT expected<void> loadJsonFromDisk(std::string const& jsonFile) override;
			NODISCARD EXPORT expected<void> loadFromJsonString(std::string const& jsonString) override;
			NODISCARD EXPORT expected<void> saveToDiskAsJson(std::string const& path) const override;
			NODISCARD EXPORT std::string asJsonString(int indent = 0) const override;
			NODISCARD EXPORT bool isNodeSet(std::string const& nodePath) const override;
			NODISCARD EXPORT pCValue getNodeUnsafe() const override;
			NODISCARD EXPORT pCValue getNodeUnsafe(std::string const& nodePath) const override;
			EXPORT expected<void> setNode(std::string const& nodePath, CValue const& cval) const override;
			NODISCARD EXPORT std::shared_lock<__shared_mutex_class> getReadLock() override;
			NODISCARD EXPORT std::unique_lock<__shared_mutex_class> getWriteLock() override;
		private:

			CVDictImpl& parent_;
			pCValue cvDict_;

			friend class CVDictImpl;
		};

		class CVDictImpl final : public iface::CVDict
		{
		public:
			CVDictImpl();

			EXPORT ~CVDictImpl() override = default;
			EXPORT static std::unique_ptr<CVDict> create();

			NODISCARD EXPORT std::unique_ptr<iface::CVDictRoot> getRoot(std::string const& path) override;
			NODISCARD EXPORT std::unique_ptr<iface::CVDictRoot> getRoot() override;
		private:

			std::shared_ptr<LockableObject> mux_;
			pCValue cvDict_;

			friend class CVDictRootImpl;
		};
	}
}
