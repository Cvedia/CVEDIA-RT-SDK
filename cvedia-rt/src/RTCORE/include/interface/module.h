/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"

namespace cvedia {
	namespace rt {
		class ConfigDescriptor;
		class ConfigCheckResults;

		namespace iface {
			class CVDictRoot;

			class Module
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
				virtual pCValue getConfigSpecs() = 0;

				virtual std::map<std::string, std::shared_ptr<ConfigDescriptor>> getConfigDescriptors() { return {}; };

				virtual std::string getName() = 0;

				virtual void stop() {}

				virtual void setParent(iface::Module*) = 0;
				virtual iface::Module* getParent() = 0;

				virtual expected<std::unique_ptr<iface::CVDictRoot>> getStateDict() const = 0;
				virtual expected<std::unique_ptr<iface::CVDictRoot>> getConfigDict() const = 0;

				virtual expected<void> setConfig(pCValue conf) const = 0;
				virtual std::string getRootNode() const = 0;				
			};
		}
	}
}
