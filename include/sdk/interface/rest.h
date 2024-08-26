/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "defines.h"
#include "builtin/moduleimpl.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS REST : public ModuleImpl {
			public:
				REST(std::string const& moduleName) : ModuleImpl(moduleName) {}

				std::string post(std::string ip, std::string path, std::string payload, std::string content_type);
				std::string get(std::string ip, std::string path);
			};
		}
	}
}
