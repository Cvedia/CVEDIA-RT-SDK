/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <memory>
#include <string>

#include "defines.h"
#include "interface/input.h"

namespace cvedia {
	namespace rt {
		namespace api {
			namespace input {
				EXPORT void registerUriHandler(std::string const& protocol, module::createInputHandler func);
				EXPORT bool hasUriHandler(std::string const& protocol);
				EXPORT expected<module::createInputHandler> getUriHandler(std::string const& protocol);
				EXPORT void registerExtHandler(std::string const& protocol, module::createInputHandler func);
				EXPORT bool hasExtHandler(std::string const& extension);
				EXPORT void removeAllHandlers();

				EXPORT expected<module::createInputHandler> getExtHandler(std::string const& extension);
			}
		}
	}
}