/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
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
				NODISCARD EXPORT expected<module::createInputHandler> getUriHandler(std::string const& protocol);
				EXPORT void registerExtHandler(std::string const& protocol, module::createInputHandler func);
				EXPORT bool hasExtHandler(std::string const& extension);
				EXPORT void removeAllHandlers();

				NODISCARD EXPORT expected<module::createInputHandler> getExtHandler(std::string const& extension);
			}
		}
	}
}