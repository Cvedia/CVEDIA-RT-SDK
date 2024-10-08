/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "cvaluefwd.h"
#include "builtin/moduleimpl.h"
#include "builtin/inputformat.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			enum InputRepeatMode
			{
				RepeatMode_ALL = 0,
				RepeatMode_VIDEO = 1,
				RepeatMode_NONE = 2,
			};

			enum FPSType
			{
				FPSType_INPUT = 0,
				FPSType_OUTPUT = 1,
				FPSType_TARGET = 2,
				FPSType_REAL = 3,
			};

			class InputHandler : public ModuleImpl {
			public:
				InputHandler(std::string const& moduleName) : ModuleImpl(moduleName) {}

				virtual expected<void> openUri(std::string const& uri) = 0;
				virtual void close() = 0;

				virtual expected<cvec> readFrame(bool ignoreSkipFrame = false, cmap frameSettings = {}) = 0;
				virtual int getCurrentFrameIndex() = 0;
				virtual float getCurrentFps(FPSType fpsType = FPSType::FPSType_TARGET) = 0;
				virtual double getCurrentTimestamp() = 0;

				virtual bool canRead() = 0;

				virtual std::vector<InputFormat> getSupportedFormats() = 0;
				virtual void setSourceFormat(InputFormat fmt) = 0;
				virtual InputFormat getSourceFormat() = 0;
			};
		}
	}
}
