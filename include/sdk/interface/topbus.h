/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <shared_mutex>
#include "defines.h"
#include "builtin/moduleimpl.h"
#include "builtin/basetypes.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS TopBus : public ModuleImpl
			{
			public:
				TopBus(std::string const& moduleName) : ModuleImpl(moduleName) {}
				virtual ~TopBus() = default;

				/**
				* Module API
				*/
				virtual void* getPtr() = 0;

				virtual expected<std::string> getTVConfigValue(std::string key) = 0;
				virtual expected<void> setTVConfigValue(std::string key, std::string value) = 0;
				virtual expected<void> sendAnalyticsData(std::string jsonData) = 0;

			};
		}
	}
}
