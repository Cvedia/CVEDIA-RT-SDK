/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <vector>
#include <memory>

#include "defines.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class Solution;
		}

		namespace api {
			namespace solutions
			{
				EXPORT expected<std::shared_ptr<iface::Solution>> addExistingSolution(std::string const& basePath);
				EXPORT expected<void> removeSolution(std::string const& name);
				EXPORT expected<std::shared_ptr<iface::Solution>> getSolutionByName(std::string const& name);

				EXPORT expected<int> discoverSolutions(std::string const& path, bool discoverInstanceTree = false);

				EXPORT void removeAllSolutions();

				EXPORT std::vector<std::shared_ptr<iface::Solution>> getSolutions();
			};
		}
	}
}
