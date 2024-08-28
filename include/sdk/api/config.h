/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <vector>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include "defines.h"
#include "interface/cvdict.h"

namespace cvedia {
	namespace rt {
		namespace api {
			namespace config {
				std::string const WEBSERVER_ENABLED = "/web_server/enabled";
				std::string const WEBSERVER_HOST = "/web_server/ip_address";
				std::string const WEBSERVER_PORT = "/web_server/port";

				std::string const SYSTEM_PIN_INSTANCE_ON_CORE = "/system/pin_instance_on_core";
				std::string const SYSTEM_OPENCV_NUM_THREADS = "/system/opencv_num_threads";
				std::string const SYSTEM_LICENSE_MANAGER = "/system/license_manager";

				std::string const LOGGING_LOG_FILE = "/logging/log_file";
				std::string const LOGGING_LOG_LEVEL = "/logging/log_level";
				std::string const LOGGING_LOG_TO_CONSOLE = "/logging/log_to_console";

				std::string const LOGGING_MAX_LOG_FILE_SIZE = "/logging/max_log_file_size";
				std::string const LOGGING_MAX_LOG_FILES = "/logging/max_log_files";

				std::string const ONVIF_DISCOVERY_ENABLED = "/onvif/discovery_enabled";
				std::string const SOLUTION_UPDATER_ENABLED = "/solution_updater/enabled";

				std::string const LICENSE_HOME = "/licensing/home_dir";
				std::string const LICENSE_URL = "/licensing/url";

				std::string const PREFERENCES = "/preferences";

				EXPORT void reset();

				NODISCARD EXPORT expected<void> loadJsonFromDisk(std::string const& jsonFile);
				NODISCARD EXPORT expected<void> loadFromJsonString(std::string const& data);
				NODISCARD EXPORT expected<void> saveToDiskAsJson(std::string const& path);
				EXPORT expected<void> setNode(std::string const& nodePath, CValue const& value);
				NODISCARD EXPORT bool isNodeSet(std::string const& nodePath);
				NODISCARD EXPORT std::string asJsonString();
				EXPORT pCValue getNodeUnsafe();
				EXPORT pCValue getNodeUnsafe(std::string const& nodePath);
				EXPORT std::unique_lock<shared_mutex_class> getWriteLock();
				EXPORT std::shared_lock<shared_mutex_class> getReadLock();
				EXPORT uCVDictRoot getCVDict();
				EXPORT uCVDictRoot getCVDict(std::string const& path);
				
				template <typename T>
				EXPORT expected<T> getValue(std::string const& nodePath);
				template <>
				EXPORT expected<std::string> getValue<std::string>(std::string const& nodePath);
				template <>
				EXPORT expected<cmap> getValue<cmap>(std::string const& nodePath);
				template <>
				EXPORT expected<int> getValue<int>(std::string const& nodePath);
				template <>
				EXPORT expected<float> getValue<float>(std::string const& nodePath);
				template <>
				EXPORT expected<bool> getValue<bool>(std::string const& nodePath);

				EXPORT void addUserParameter(std::string key_name, std::string value);
				EXPORT std::vector<std::pair<std::string, std::string>> getUserParameters();
			};
		}
	}
}
