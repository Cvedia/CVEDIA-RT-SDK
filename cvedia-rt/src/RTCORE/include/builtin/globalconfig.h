/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <memory>

#include "defines.h"
#include "interface/cvdict.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class CVDict;
		}

		class GlobalConfig {
		public:
			static expected<void> initialize();
			static uCVDictRoot config();
			static uCVDictRoot config(std::string const& path);
		private:
			static std::unique_ptr<iface::CVDict> config_;
		};
	}
}
