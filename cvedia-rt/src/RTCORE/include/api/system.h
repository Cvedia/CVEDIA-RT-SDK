/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <vector>
#include <memory>
#include <filesystem.hpp>

#include "system/info.h"
#include "defines.h"

namespace fs = ghc::filesystem;

namespace cvedia {
	namespace rt {
		namespace api {
			namespace system {
				EXPORT std::vector<std::string> getRegisteredPlugins();
				EXPORT fs::path getApplicationPath();
				EXPORT std::shared_ptr<rt::system::Info> getSystemInfo();
				EXPORT std::string getModelsPath(); // TODO: @AJ to move to inferencemanager
				EXPORT std::string generateGuid();
			}
		}
	}
}