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
			NODISCARD EXPORT expected<std::shared_ptr<T>> getObject(KeyType const& objectName);
			EXPORT bool hasObject(KeyType const& objectName);
			EXPORT std::unordered_set<std::shared_ptr<T>> getObjects();
			EXPORT std::unordered_set<KeyType> getObjectKeys();
			EXPORT int getObjectCount();
			EXPORT void clearObjects();
		private:
			std::map<KeyType, std::shared_ptr<T>> objects_;
		};
	}
}