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
				const std::string WEBSERVER_ENABLED = "/web_server/enabled";
				const std::string WEBSERVER_HOST = "/web_server/ip_address";
				const std::string WEBSERVER_PORT = "/web_server/port";
				
				const std::string SYSTEM_PIN_INSTANCE_ON_CORE = "/system/pin_instance_on_core";
				const std::string SYSTEM_OPENCV_NUM_THREADS = "/system/opencv_num_threads";
				const std::string SYSTEM_LICENSE_MANAGER = "/system/license_manager";

				const std::string LOGGING_LOG_FILE = "/logging/log_file";
				const std::string LOGGING_LOG_LEVEL = "/logging/log_level";
				const std::string LOGGING_LOG_TO_CONSOLE = "/logging/log_to_console";

				const std::string LOGGING_MAX_LOG_FILE_SIZE = "/logging/max_log_file_size";
				const std::string LOGGING_MAX_LOG_FILES = "/logging/max_log_files";

				const std::string ONVIF_DISCOVERY_ENABLED = "/onvif/discovery_enabled";
				const std::string SOLUTION_UPDATER_ENABLED = "/solution_updater/enabled";

				const std::string LICENSE_HOME = "/licensing/home_dir";
				const std::string LICENSE_URL = "/licensing/url";

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
