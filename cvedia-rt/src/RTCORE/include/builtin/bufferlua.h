/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once

#include "defines.h"

namespace cvedia {
	namespace rt {
		class BufferLua {
		public:
			EXPORT BufferLua();
			EXPORT BufferLua(cbuffer buffer);
			EXPORT ~BufferLua();

			cbuffer buffer_;
			bool valid;
		};
	}
}
