/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "defines.h"
#include "interface/cvdict.h"

namespace cvedia {
	namespace rt {
		class LockableObject;
		class CVDictImpl;

		class CVDictRootImpl final : public iface::CVDictRoot
		{
		public:
			CVDictRootImpl(CVDictImpl&, pCValue cvDict);

			NODISCARD EXPORT expected<void> loadJsonFromDisk(std::string const& jsonFile) override;
			NODISCARD EXPORT expected<void> loadFromJsonString(std::string const& jsonString) override;
			NODISCARD EXPORT expected<void> saveToDiskAsJson(std::string const& path) const override;
			NODISCARD EXPORT std::string asJsonString(int indent = 0) const override;

			NODISCARD EXPORT pCValue getCopyOfNode() const override;
			NODISCARD EXPORT pCValue getCopyOfNode(std::string const& nodePath) const override;
			NODISCARD EXPORT expected<pCValue> getCopyOfNodeIfSet(std::string const& nodePath) const override;
			NODISCARD EXPORT pCValue getNodeUnsafe() const override;
			NODISCARD EXPORT pCValue getNodeUnsafe(std::string const& nodePath) const override;
			NODISCARD EXPORT expected<pCValue> getNodeIfSetUnsafe(std::string const& nodePath) const override;
			NODISCARD EXPORT bool isNodeSet(std::string const& nodePath) const override;
			EXPORT expected<void> setNode(std::string const& nodePath, CValue const& cval) override;
			EXPORT bool deleteNode(std::string const& nodePath) override;
			EXPORT void merge(pCValue const& other, bool refSourceIsSelf) override;
			NODISCARD EXPORT expected<void> mergeAt(std::string const& path, pCValue const& other, bool refSourceIsSelf) override;
			EXPORT void override(pCValue const& other) override;
			NODISCARD EXPORT expected<void> overrideAt(std::string const& path, pCValue const& other) override;

			EXPORT std::vector<std::string> getKeys() const override;

			EXPORT bool setValue(std::string const& path, bool x) override;
			EXPORT bool setValue(std::string const& path, int x) override;
			EXPORT bool setValue(std::string const& path, float x) override;
			EXPORT bool setValue(std::string const& path, double x) override;
			EXPORT bool setValue(std::string const& path, std::string const& x) override;
			EXPORT bool setValue(std::string const& path, char* const& x) override;
			EXPORT bool setValue(std::string const& path, Rect2f const& x) override;
			EXPORT bool setValue(std::string const& path, cbuffer const& x) override;
			EXPORT bool setValue(std::string const& path, std::vector<float> const& x) override;
			EXPORT bool setValue(std::string const& path, CValue const& x) override;
			EXPORT bool setValue(std::string const& path, cvec const& x) override;
			EXPORT bool setValue(std::string const& path, cmap const& x) override;

			NODISCARD EXPORT std::shared_lock<shared_mutex_class> getReadLock() const override;
			NODISCARD EXPORT std::unique_lock<shared_mutex_class> getWriteLock() const override;
			NODISCARD EXPORT std::shared_lock<shared_mutex_class> getReadLockAdopt() const override;
			NODISCARD EXPORT std::unique_lock<shared_mutex_class> getWriteLockAdopt() const override;
			NODISCARD EXPORT std::shared_lock<shared_mutex_class> getReadLockDefer() const override;
			NODISCARD EXPORT std::unique_lock<shared_mutex_class> getWriteLockDefer() const override;
			NODISCARD EXPORT shared_mutex_class& getMutex() const override;
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
			EXPORT void setData(pCValue&& cv) override;
		private:

			std::shared_ptr<LockableObject> mux_;
			pCValue cvDict_;

			friend class CVDictRootImpl;
		};
	}
}
