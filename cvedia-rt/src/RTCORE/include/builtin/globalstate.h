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

		class GlobalState {
		public:
			static expected<void> initialize();
			EXPORT static uCVDictRoot state();
			EXPORT static uCVDictRoot state(std::string const& path);
		private:
			static std::unique_ptr<iface::CVDict> state_;
		};
	}
}
