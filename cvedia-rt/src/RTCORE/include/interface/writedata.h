/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"
#include "builtin/moduleimpl.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class WriteData : public ModuleImpl {
			public:
				WriteData(std::string const& moduleName) : ModuleImpl(moduleName) {}

				virtual void createVideo(std::string key, std::string filename, int width, int height, int fps) = 0;
				virtual void writeImage(int x, int y, int width, int height, std::string filename, cbuffer buffer) = 0;
				virtual void writeStructuredData(std::string filename, std::string sdata, bool append = true) = 0;
				virtual void writeFrame(std::string key, int x, int y, int width, int height, std::vector<cbuffer> sources, bool resizeOutput = false) = 0;
			};
		}
	}
}
