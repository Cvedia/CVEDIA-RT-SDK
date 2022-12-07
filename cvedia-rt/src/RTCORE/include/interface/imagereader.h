/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "cvalue.h"
#include "defines.h"
#include "inputhandler.h"

namespace cvedia {
	namespace rt {

		namespace iface {

			class ImageReader : public InputHandler {
			public:
				ImageReader(std::string const& moduleName) : InputHandler(moduleName) {}

				virtual int getFrameCount() = 0;
				virtual void setRepeat(bool repeat) = 0;
				virtual bool willRepeat() = 0;
			};
		}
	}
}
