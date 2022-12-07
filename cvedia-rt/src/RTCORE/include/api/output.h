/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <memory>
#include <string>

#include "defines.h"
#include "interface/output.h"

namespace cvedia {
	namespace rt {
		namespace api {
			namespace output {
				EXPORT void registerUriHandler(std::string const& protocol, module::createOutputHandler func);
				EXPORT bool hasUriHandler(std::string const& protocol);
				EXPORT expected<module::createOutputHandler> getUriHandler(std::string const& protocol);
			}
		}
	}
}