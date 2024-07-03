/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "builtin/moduleimpl.h"
#include "builtin/basetypes.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS NBus : public ModuleImpl
			{
			public:
				NBus(std::string const& moduleName) : ModuleImpl(moduleName) {}
				virtual ~NBus() = default;

				/**
				* Module API
				*/
				virtual void* getPtr() = 0;

				NODISCARD virtual expected<std::string> getNBConfigValue(std::string key) = 0;
				NODISCARD virtual expected<void> setNBConfigValue(std::string key, std::string value) = 0;
				NODISCARD virtual expected<void> sendAnalyticsData(std::string jsonData) = 0;

			};
		}
	}
}
