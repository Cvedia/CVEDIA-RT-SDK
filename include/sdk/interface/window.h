/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "cvaluefwd.h"

namespace cvedia {
	namespace rt {
		namespace iface {

			class Window
			{
			public:
				Window(std::string name) : name_(name) {}
				virtual ~Window() = default;

				virtual bool draw() = 0;
				virtual bool initialize() = 0;

				virtual pCValue serialize() = 0;
				virtual void deserialize(pCValue value) = 0;
			protected:
				std::string name_;
			};
		}
	}
}
