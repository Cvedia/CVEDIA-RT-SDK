/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <string>

#include "defines.h"
#include "interface/inputhandler.h"
#include "builtin/moduleimpl.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class Input : public ModuleImpl
			{
			public:
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

				virtual expected<std::shared_ptr<iface::InputHandler>> getInputHandler() = 0;
				virtual bool canRead() = 0;
				virtual void forceRead() = 0;
				virtual bool isPaused() = 0;
				virtual void pause(bool state) = 0;
				virtual std::vector<iface::InputFormat> getSupportedFormats() = 0;
				virtual void setSourceFormat(iface::InputFormat fmt) = 0;
				virtual iface::InputFormat getSourceFormat() = 0;

				virtual void clearReadAheadQueue() = 0;
				virtual int getReadAheadQueueSize() = 0;
				virtual bool isReadAheadOn() = 0;
								
			};
		}
	}
}
