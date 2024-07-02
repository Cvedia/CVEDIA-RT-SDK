/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <set>
#include <vector>

#include "defines.h"
#include "rterror.h"

namespace cvedia {

	/**
		* \ingroup Managed
		* @{
		*/
		//! API for controlling the server and lifecycle management of runtime instances
	namespace rt {
		namespace api {
			EXPORT void reloadConfig();
			EXPORT void setPluginPath(std::string const& path);
			EXPORT std::vector<std::string> getLoadedPlugins();
			NODISCARD EXPORT expected<void> initialize();

			EXPORT expected<void> shutdown();
			EXPORT expected<void> stopAllInstances();
			EXPORT expected<void> unloadAllInstances();
			EXPORT NODISCARD std::string getVersion();

			EXPORT void stop();
			EXPORT bool isDevMode();
			EXPORT void setDevMode(bool devMode);
			EXPORT bool isRunning();
			EXPORT std::string getRtVersion();
			EXPORT void startWebserver();
			EXPORT void stopWebserver();

			EXPORT void startAutoDiscovery();
			EXPORT void stopAutoDiscovery();
			EXPORT expected<std::set<std::string>> findRemoteRTs(bool localOnly, unsigned short port);
		};
	}
}
