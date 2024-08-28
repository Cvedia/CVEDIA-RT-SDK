/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <string>

#include "cvaluefwd.h"
#include "defines.h"

namespace cvedia {
	namespace rt {

		class InputFormat
		{
		public:
			std::string Name;
			bool UseDefaultFormat;
			int Height{};
			int ColorFormat{};
			bool IsSoftware{};

			EXPORT InputFormat() = default;

			EXPORT InputFormat(std::string name_, bool use_default_, int height_, int color_format_, bool is_software_);

			EXPORT InputFormat(CValue const& cval);

			EXPORT pCValue toCValue() const;
		};

		static InputFormat SourceFormat("Same as Source", true, 0, 0, false);
		static InputFormat SoftwareFormat480P("480p (Software)", false, 480, 0, true);
		static InputFormat SoftwareFormat720P("720p (Software)", false, 720, 0, true);
		static InputFormat SoftwareFormat1080P("1080p (Software)", false, 1080, 0, true);
	}
}
