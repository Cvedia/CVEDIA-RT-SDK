/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "builtin/lockableobject.h"
#include "defines.h"

namespace cvedia {
	namespace rt {
		template <class T>
		class ObjectRegistry final : private LockableObject
		{
		public:
			EXPORT expected<std::shared_ptr<T>> registerObject(std::string const& objectName, std::shared_ptr<T> object);
			EXPORT expected<void> unregisterObject(std::string const& objectName);
			NODISCARD EXPORT expected<std::shared_ptr<T>> getObject(std::string const& objectName);
			EXPORT bool hasObject(std::string const& objectName);
			EXPORT std::vector<std::shared_ptr<T>> getObjects();
			EXPORT std::vector<std::string> getObjectNames();
			EXPORT int getObjectCount();
			EXPORT void clearObjects();
		private:
			std::map<std::string, std::shared_ptr<T>> objects_;
		};
	}
}