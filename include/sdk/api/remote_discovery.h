/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <string>
#include <set>
#include "defines.h"
#include "rterror.h"

namespace cvedia {
	namespace rt {
		namespace api {
			namespace remote_discovery {
				EXPORT bool isSearching();
				EXPORT void start();
				EXPORT void stop();
				EXPORT std::set<std::string> getFoundServers();
				EXPORT NODISCARD expected<std::string> getAvailableServer();
				EXPORT bool isServerHealthy(const std::string& ip);
				EXPORT void clear();
			}
		}
	}
}