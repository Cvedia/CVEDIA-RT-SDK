/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <string>
#include "defines.h"
#include "rterror.h"
#include "cvaluefwd.h"

namespace cvedia {
	namespace rt {
		enum BufferDataType : unsigned;

		namespace api {
			namespace buffer {
				EXPORT cbuffer createBuffer(int width, int height, BufferDataType type);
				EXPORT cbuffer createBuffer(int width, int height, BufferDataType type, std::string const& name);
				EXPORT expected<cbuffer> createFromBlob(int width, int height, BufferDataType type, void* data, size_t step=0);
				EXPORT expected<cbuffer> loadImage(void* data, int data_size);
				NODISCARD EXPORT expected<cbuffer> createBufferFromImage(std::string const& imagePath);
			};
		}
	}
}
