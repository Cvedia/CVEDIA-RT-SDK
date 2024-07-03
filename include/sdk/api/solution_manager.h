/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <string>

#include "defines.h"
#include "builtin/moduleimpl.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class SolutionManager;
		}

		namespace api {
			namespace solution_manager {
				EXPORT void registerSolutionManagerHandler(std::string const& solutionId, module::createSolutionManagerHandler func);
				EXPORT bool hasSolutionManagerHandler(std::string const& solutionId);
				NODISCARD EXPORT expected<module::createSolutionManagerHandler> getSolutionManagerHandler(std::string const& solutionId);
				EXPORT void removeAllHandlers();
				EXPORT std::shared_ptr<iface::SolutionManager> createSolutionManager(std::string const& solutionId);
			}
		}
	}
}