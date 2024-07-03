/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <vector>
#include <memory>

#include "defines.h"
#include "rterror.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class Solution;
		}

		namespace api {
			namespace solutions
			{
			    NODISCARD EXPORT expected<std::shared_ptr<iface::Solution>> addExistingSolution(std::string const& basePath);
				NODISCARD EXPORT expected<void> removeSolutionById(std::string const& id);
				NODISCARD EXPORT expected<std::shared_ptr<iface::Solution>> getSolutionById(std::string const& id);
				EXPORT bool solutionExists(std::string const& id);

				NODISCARD EXPORT expected<int> discoverSolutions(std::string const& root, bool scanInstances = true);
				NODISCARD EXPORT expected<void> removeAllSolutions();

				NODISCARD EXPORT std::string getRootPath();

				NODISCARD EXPORT std::vector<std::shared_ptr<iface::Solution>> getSolutions();
			};
		}
	}
}
