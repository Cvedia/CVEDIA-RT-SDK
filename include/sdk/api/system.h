/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <vector>
#include <memory>
#include <shared_mutex>

#include "cvaluefwd.h"
#include "defines.h"
#include "rterror.h"

namespace cvedia {
	namespace rt {
		namespace system {
			class Info;
		}
		namespace api {
			namespace system {
				namespace registry {
					EXPORT expected<void> setKeyValue(std::string const& key, CValue const& value);
					EXPORT expected<pCValue> getKeyValue(std::string const& key);
					EXPORT std::vector<std::string> getKeys();
					EXPORT bool deleteKey(std::string const& key);
					EXPORT expected<void> incrementRef(std::string const& key);
					EXPORT expected<void> decrementRef(std::string const& key);
				}

				EXPORT std::shared_mutex& getCudaMutex();

				EXPORT std::vector<std::string> getRegisteredPlugins();
				EXPORT std::string getPreferences();
				EXPORT std::string getApplicationPath();
				EXPORT std::string getRtHomeDir();
				EXPORT void setRtHomeDir(std::string const& path);
				EXPORT void setRtInstanceDir(std::string const& path);
				EXPORT std::string getRtInstanceDir();
				EXPORT std::shared_ptr<rt::system::Info> getSystemInfo();
				EXPORT std::string getModelsPath(); // TODO: @AJ to move to inferencemanager
				EXPORT std::string generateGuid();
			}
		}
	}
}
