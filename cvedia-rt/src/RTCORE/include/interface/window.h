/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"

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
