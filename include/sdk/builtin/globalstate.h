/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "cvaluefwd.h"
#include "defines.h"
#include "rterror.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class CVDict;
		}

		class GlobalState {
		public:
			static expected<void> initialize();
			EXPORT static uCVDictRoot state();
			EXPORT static uCVDictRoot state(std::string const& path);
		private:
			static std::shared_ptr<iface::CVDict> state_;
		};
	}
}
