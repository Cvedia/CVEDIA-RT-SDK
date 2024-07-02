/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <vector>
#include <string>
#include <ctime>
#include <optional>

#include "defines.h"
#include "rterror.h"

namespace cvedia {
	namespace rt {
		namespace api {
			namespace license {
				struct LicenseInfo {
					std::string key;
					int maxNumberOfStreams;
					int maxNumberOfMachines;
					std::time_t licenseExpiresAt;
					bool isExpired;
				};

				EXPORT expected<std::string> pathToLicensesDirectory();
				EXPORT expected<std::vector<std::string>> getCurrentLicenseKeys();
				EXPORT expected<std::vector<LicenseInfo>> getCurrentLicenseInfo();
				EXPORT expected<void> updateLicenseKey(std::string const& licenseKey);
				EXPORT expected<void> addLicenseKey(std::string const& licenseKey);
				EXPORT expected<void> removeLicenseKey(std::string const& licenseKey, bool force = false);
				EXPORT expected<void> loadLicenses();
				EXPORT expected<void> unloadLicenses();
				EXPORT expected<bool> activateTrialLicense();
			}
		}
	}
}