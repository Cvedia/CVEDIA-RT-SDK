/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <map>
#include <shared_mutex>

#include "cvaluefwd.h"
#include "defines.h"
#include "lockableobject.h"
#include "rterror.h"
#include "nlohmann/json_fwd.hpp"

namespace cvedia {
	namespace rt {
		namespace iface {
			class CVDict;
		}

		class SystemRegistry final : public LockableObject {
		public:
			EXPORT static void initialize();

			EXPORT static expected<void> setKeyValue(std::string const& key, CValue const& value);
			EXPORT static expected<void> incrementRef(std::string const& key);
			EXPORT static expected<void> decrementRef(std::string const& key);
			EXPORT static expected<pCValue> getKeyValue(std::string const& key);
			EXPORT static bool deleteKey(std::string const& key);
			EXPORT static std::vector<std::string> getKeys();
			EXPORT static nlohmann::json getJsonRegistryUnsafe();
			EXPORT static std::unique_lock<shared_mutex_class> getWriteLock();
			EXPORT static std::shared_lock<shared_mutex_class> getReadLock();
		private:
			static expected<void> _setKeyValueCV(std::string const& key, CValue const& value);
			static expected<void> _setKeyValueJson(std::string const& key, CValue const& value);

			static shared_mutex_class mutex_;

			static nlohmann::json jsonSystemRegistry_;
			static std::shared_ptr<iface::CVDict> cvalueSystemRegistry_;
		};
	}
}
