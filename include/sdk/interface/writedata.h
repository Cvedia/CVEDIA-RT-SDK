/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "builtin/moduleimpl.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS WriteData : public ModuleImpl {
			public:
				WriteData(std::string const& moduleName) : ModuleImpl(moduleName) {}
#if WITH_WRITEDATA_MODULE_VIDEO
				virtual void createVideo(std::string key, std::string filename, int width, int height, int fps) = 0;
				virtual void endVideo(std::string const& key) = 0;
				virtual void writeFrame(std::string key, int x, int y, int width, int height, std::vector<cbuffer> sources, bool resizeOutput = false) = 0;
#endif
				virtual void writeImage(int x, int y, int width, int height, std::string filename, cbuffer buffer) = 0;
				virtual void writeStructuredData(std::string filename, std::string sdata, bool append = true) = 0;
			};
		}
	}
}
