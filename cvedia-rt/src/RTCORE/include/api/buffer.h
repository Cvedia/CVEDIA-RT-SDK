/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <string>
#include "defines.h"

namespace cvedia {
	namespace rt {
		enum BufferDataType : unsigned;

		namespace api {
			namespace buffer {
				EXPORT cbuffer createBuffer(int width, int height, BufferDataType type);
				EXPORT cbuffer createBuffer(int width, int height, BufferDataType type, std::string const& name);
				EXPORT expected<cbuffer> createBufferFromImage(std::string const& imagePath);
			};
		}
	}
}
