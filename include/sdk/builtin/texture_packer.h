/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once 
#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>

#include "defines.h"
#include "rterror.h"
#include "builtin/basetypes.h"

namespace cvedia {
	namespace rt {

		class TexturePacker
		{
		public:
			EXPORT expected<std::pair<cv::Mat, std::vector<Rect2f>>> packTextures(std::vector<std::pair<cv::Mat, cv::Rect2f>> const& textures, cv::Size atlasSize, int const packingInterpolationMethod) const;
		private:
			static std::vector<cv::Rect2f> _getCropRects(std::vector<stbrp_rect> const& rects, std::vector<std::pair<cv::Mat, cv::Rect2f>> const& textures, cv::Size atlasSize);
			cv::Mat _createAtlas(std::vector<stbrp_rect> const& rects, std::vector<std::pair<cv::Mat, cv::Rect2f>> const& textures, cv::Size atlasSize, int const packingInterpolationMethod, std::array<int, 3> bgColor) const;
			static std::vector<stbrp_rect> _getRects(std::vector<std::pair<cv::Mat, cv::Rect2f>> const& textures, float scaleFactor, cv::Size atlasSize);
		};
	}
}
