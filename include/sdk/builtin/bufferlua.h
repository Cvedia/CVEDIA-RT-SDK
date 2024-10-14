/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#include "defines.h"
#include "cvaluefwd.h"

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
