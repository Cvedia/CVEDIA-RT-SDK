/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "cvaluefwd.h"
#include "rterror.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class CVDict;
		}

		class RtConfig {
		public:
			static expected<void> initialize();
			static uCVDictRoot config();
			static uCVDictRoot config(std::string const& path);
			static void reset();
		private:
			static std::shared_ptr<iface::CVDict> config_;
		};
	}
}
