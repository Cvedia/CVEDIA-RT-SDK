/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "defines.h"
#include "interface/inputhandler.h"
#include "builtin/moduleimpl.h"
#include "builtin/basetypes.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS Input : public ModuleImpl
			{
			public:
				enum PrivacyMode
				{
					NONE = 0,
					BLUR,
					PIXELATE,
					FILL
				};

				Input(std::string const& moduleName) : ModuleImpl(moduleName) {}

				//virtual NODISCARD expected<bool> setSourceList(std::vector<std::string> const& source) = 0;
				//virtual std::vector<std::string> getSourceList() = 0;
				//virtual bool setSourceByIndex(int index) = 0;
				NODISCARD virtual expected<bool> setSource(std::string const& source) = 0;
				NODISCARD virtual expected<bool> setSourceFromConfig() = 0;

				NODISCARD virtual expected<cvec> readFrame(bool ignore_skip_frame = false, cmap frameSettings = {}) = 0;
				virtual int getCurrentFrameIndex() = 0;
				virtual bool setFps(float fps) = 0;
				virtual float getCurrentFps(iface::FPSType fpsType = iface::FPSType::FPSType_TARGET) = 0;
				virtual double getCurrentTimestamp() = 0;
				virtual expected<bool> setBufferSize(size_t size) = 0;

				virtual expected<void> setPrivacyMasks(std::vector<std::vector<Point2f>> const& masks) = 0;
				virtual expected<void> setPrivacyMode(std::string mode, CValue const& options) = 0;

				virtual expected<iface::InputHandler*> getInputHandler() = 0;
				virtual bool canRead() = 0;
				virtual void forceRead() = 0;
				virtual bool isPaused() = 0;
				virtual void pause(bool state) = 0;
				virtual std::vector<InputFormat> getSupportedFormats() = 0;
				virtual void setSourceFormat(InputFormat fmt) = 0;
				virtual InputFormat getSourceFormat() = 0;

				virtual bool setOrientation(int orientation) = 0;

				virtual void clearReadAheadQueue() = 0;
				virtual int getReadAheadQueueSize() = 0;
				virtual int getDroppedFramesCount() const = 0;

				virtual bool isReadAheadOn() = 0;

				virtual double getReadFrameLatency() = 0;
			};
		}
	}
}
