/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <map>
#include <vector>
#include <memory>
#include <unordered_set>

#include "builtin/lockableobject.h"
#include "defines.h"
#include "rterror.h"
#include "uuid.h"

namespace cvedia {
	namespace rt {
		template <class T, class KeyType = std::string>
		class EXPORT_CLASS ObjectRegistry : private LockableObject
		{
		public:
			EXPORT expected<std::shared_ptr<T>> registerObject(KeyType const& objectName, std::shared_ptr<T> object);
			EXPORT expected<void> unregisterObject(KeyType const& objectName);
			EXPORT expected<void> renameObject(KeyType const& oldId, KeyType const& newId);
			NODISCARD EXPORT expected<std::shared_ptr<T>> getObject(KeyType const& objectName) const;
			EXPORT bool hasObject(KeyType const& objectName) const;
			EXPORT std::unordered_set<std::shared_ptr<T>> getObjects() const;
			EXPORT std::unordered_set<KeyType> getObjectKeys() const;
			EXPORT int getObjectCount() const;
			NODISCARD EXPORT expected<void> clearObjects();
		private:
			EXPORT expected<void> _unregisterObject(KeyType const& objectName);

			std::map<KeyType, std::shared_ptr<T>> objects_;
		};
	}
}