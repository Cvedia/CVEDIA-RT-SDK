/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <map>

#include "defines.h"
#include "interface/restapibase.h"

namespace cvedia {
	namespace rt {
		namespace api {
			namespace web {
				EXPORT void listen(std::string const& ip, int port);
				EXPORT void stopListen();
				EXPORT std::string webclient_post(std::string url, std::string path, std::string payload);
				EXPORT std::string webclient_get(std::string url, std::string path);
				EXPORT void registerRestHandler(std::unique_ptr<RestApiBase> api);
			};
		}
	}
}