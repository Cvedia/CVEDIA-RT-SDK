/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#include <memory>

#include "defines.h"
#include "rterror.h"

namespace cvedia
{
namespace rt
{

    namespace iface
    {
        class SolutionUpdater;
    }

    namespace api
    {
        namespace update
        {

            /*
             * @brief Retrieves the currently assigned solution updater.
             * @returns Pointer to the solution updater or error if unsuccessful.
             */
            NODISCARD EXPORT expected<std::shared_ptr<iface::SolutionUpdater>> getSolutionUpdater();

        }
    }
}
}