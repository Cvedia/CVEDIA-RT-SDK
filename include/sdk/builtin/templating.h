/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once 
#include "defines.h"
#include "rterror.h"

#include <nlohmann/json_fwd.hpp>

namespace inja {
	class Environment;
}

namespace cvedia {
	namespace rt {

		class Templating
		{
		public:
			EXPORT static std::string parse(std::string const& txt);
			EXPORT static std::string parse(std::string const& txt, nlohmann::json const& js);
		private:
			static void _initialize();

			static bool isInit_;
			static std::unique_ptr<inja::Environment> env_;
		};
	}
}
