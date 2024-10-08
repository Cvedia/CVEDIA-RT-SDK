/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "cvaluefwd.h"
#include "rterror.h"
#include "defines.h"

#define DICT_STATE 0
#define DICT_CONFIG 1
#define DICT_STATE_CONFIG_MERGED 2

namespace cvedia {
	namespace rt {
		class ConfigDescriptor;
		class ConfigCheckResults;

		namespace iface {
			class CVDictRoot;

			class EXPORT_CLASS Module
			{
			public:
				Module() = default;
				virtual ~Module() = default;
				Module(Module const& other) = default;
				Module& operator=(Module const& other) = default;

				Module(Module&& other) = default;
				Module& operator=(Module&& other) = default;

				virtual void* getPtr() = 0;

				virtual void verifyConfig(pCValue instanceConfig, ConfigCheckResults& configCheckResults) = 0;
				virtual pCValue getConfigSpecs() const = 0;

				virtual std::map<std::string, std::shared_ptr<ConfigDescriptor>> getConfigDescriptors() const { return {}; };

				virtual std::string const& getName() const = 0;
				virtual void setName(std::string const& name) = 0;

				virtual void stop() {}

				virtual void setParent(iface::Module*) = 0;
				virtual iface::Module* getParent() const = 0;

				virtual expected<pCValue> getValue(std::string const& key, int dictType) const = 0;
				virtual expected<cvec> getValue(std::vector<std::string> const& keys, int dictType) const = 0;
				virtual expected<void> setValue(std::string const& key, pCValue const& value, int dictType) = 0;
				virtual expected<void> setValue(cmap const& values, int dictType) = 0;

				virtual expected<std::unique_ptr<iface::CVDictRoot>> getStateDict() const = 0;
				virtual expected<std::unique_ptr<iface::CVDictRoot>> getConfigDict() const = 0;

				virtual expected<void> setConfig(pCValue const& conf) const = 0;
				virtual std::string getRootNode() const = 0;				
			};
		}
	}
}
