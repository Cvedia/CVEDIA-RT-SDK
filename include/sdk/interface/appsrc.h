/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "defines.h"
#include "interface/videoreader.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class AppSrc : public iface::VideoReader
			{
			public:
				AppSrc(std::string const& moduleName) : VideoReader(moduleName) {}

				/**
				* Module API
				*/
				virtual expected<void> pushRawFrame(cbuffer image, cmap const& metadata) = 0;
				virtual expected<void> pushRawFrame(void const* buffer, int width, int height, std::chrono::milliseconds timestamp, cmap const& metadata) = 0;
				virtual expected<void> pushCompressedFrame(unsigned char const* data, size_t dataSize, std::chrono::milliseconds timestamp) = 0;
				virtual expected<void> pushH264Frame(unsigned char const* data, size_t dataSize, std::chrono::milliseconds timestamp) = 0;
				virtual expected<void> pushH265Frame(unsigned char const* data, size_t dataSize, std::chrono::milliseconds timestamp) = 0;

				virtual void setTargetFps(int fps) = 0;
				virtual void setMaxPixels(int pixels) = 0;

				virtual int getQueueSize() = 0;
			};
		}
	}
}
