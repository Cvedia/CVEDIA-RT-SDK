/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#include <string>
#include <httplib.h>

#include "defines.h"

namespace cvedia {
	namespace rt {
		class WebClient {
		public:
			EXPORT static std::string post(std::string const& ip, std::string const& path, std::string const& payload, std::string const& contentType = "text/plain");
			EXPORT static std::string get(std::string const& ip, std::string const& path);
			EXPORT static void setupProxy(httplib::Client& cli);
		private:
		};
	}
}
