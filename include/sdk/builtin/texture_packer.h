/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#include "defines.h"
#include "rterror.h"
#include "builtin/basetypes.h"

namespace cvedia {
	namespace rt {

		class TexturePacker
		{
		public:
			EXPORT expected<std::pair<cv::Mat, std::vector<Rect2f>>> packTextures(std::vector<std::pair<cv::Mat, cv::Rect2f>> const& textures, cv::Size atlasSize, int const packingInterpolationMethod) const;
		};
	}
}
