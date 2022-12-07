/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "cvalue.h"
#include "defines.h"
#include "builtin/moduleimpl.h"

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

			class InputFormat
			{
			public:
				std::string Name;
				bool UseDefaultFormat;
				int Height{};
				int ColorFormat{};
				bool IsSoftware{};

				InputFormat() = default;

				InputFormat(std::string name_, bool use_default_, int height_, int color_format_, bool is_software_)
				{
					Name = name_;
					UseDefaultFormat = use_default_;
					Height = height_;
					ColorFormat = color_format_;
					IsSoftware = is_software_;
				}

				InputFormat(CValue cval)
				{
					Name = cval.getValueOr<std::string>("name", "");
					UseDefaultFormat = cval.getValueOr<bool>("default_format", false);
					Height = cval.getValueOr<int>("height", 0);
					ColorFormat = cval.getValueOr<int>("color_format", 0);
					IsSoftware = cval.getValueOr<bool>("is_software", false);
				}

				pCValue toCValue() const {
					CValue cval(CValueType::MAP);
					cval.getChildOrCreate("name")->setValue(Name);
					cval.getChildOrCreate("default_format")->setValue(UseDefaultFormat);
					cval.getChildOrCreate("height")->setValue(Height);
					cval.getChildOrCreate("color_format")->setValue(ColorFormat);
					cval.getChildOrCreate("is_software")->setValue(IsSoftware);
					return std::make_shared<CValue>(cval);
				}
			};

			static InputFormat SourceFormat("Same as Source", true, 0, 0, false);
			static InputFormat SoftwareFormat480P("480p (Software)", false, 480, 0, true);
			static InputFormat SoftwareFormat720P("720p (Software)", false, 720, 0, true);
			static InputFormat SoftwareFormat1080P("1080p (Software)", false, 1080, 0, true);

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

				virtual std::vector<iface::InputFormat> getSupportedFormats() = 0;
				virtual void setSourceFormat(iface::InputFormat fmt) = 0;
				virtual iface::InputFormat getSourceFormat() = 0;
			};
		}
	}
}
