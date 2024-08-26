/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "inputhandler.h"

namespace cvedia {
	namespace rt {

		namespace iface {

			class EXPORT_CLASS ImageReader : public InputHandler {
			public:
				ImageReader(std::string const& moduleName) : InputHandler(moduleName) {}

				virtual int getFrameCount() = 0;
				virtual void setRepeat(bool repeat) = 0;
				virtual bool willRepeat() = 0;
			};
		}
	}
}
