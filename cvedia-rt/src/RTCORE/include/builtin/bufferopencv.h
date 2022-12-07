/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once

#include <opencv2/core/mat.hpp>    // for Mat
#include <opencv2/core/types.hpp>  // for Point2f
#include "builtin/basetypes.h"     // for Rect2f, Size
#include "defines.h"               // for EXPORT, cbuffer, cmap, pCValue
#include "interface/buffer.h"      // for Buffer, BufferDataType

namespace cvedia { namespace rt { struct BufferInit; } }


namespace cvedia {
	namespace rt {

		class BufferOpenCV : public iface::Buffer {
		public:
			EXPORT BufferOpenCV();
			EXPORT BufferOpenCV(cv::Mat m);

			EXPORT static void create(BufferInit* init);

			EXPORT cv::Mat getCvMat();

			EXPORT cbuffer copy() override;
			EXPORT cbuffer copy(float scaleFactor) override;
			EXPORT cbuffer copy(Rect2f const& rc, bool relative) override;
			EXPORT void copyToRegion(Rect2f const& rc, bool relative, cbuffer destBuffer, Rect2f const& rect_dest, bool dest_relative) override;
			EXPORT Size getSize() override;
			EXPORT int getChannels() override;

			EXPORT expected<void> fill(float color[3], int opacity) override;
			EXPORT expected<void> drawLine(Point2f const& src, Point2f const& dest, float color[3], int thickness) override;
			EXPORT expected<void> drawRect(Rect2f const& pos, float color[3], int thickness) override;
			EXPORT expected<void> drawPoly(std::vector<Point2f> const& pts, float color[3], float alpha) override;
			EXPORT expected<void> drawPolyLine(std::vector<Point2f> const& pts, float color[3], int thickness) override;
			EXPORT expected<void> drawCircle(Point2f const& center, int radius, float color[3], int thickness) override;
			EXPORT expected<void> writeText(Point2f const& pos, std::string const& text, float color[3], int fontface, float fontscale, int thickness) override;
			EXPORT expected<void> writeTextAutoSize(Point2f const& pos, std::string const& text, float color[3], int fontface, int generic_size) override;
			EXPORT std::pair<int, int> getSizeOfTextAutoSize(std::string const& text, int fontface, int generic_size) override;
			EXPORT expected<void> drawBoundingBox(Rect2f const& pos, std::string label, float color[3], int thickness) override;

			EXPORT void* data() override;
			EXPORT void brightness(double delta) override;
			EXPORT void grayscale(bool singleChannel) override;

			EXPORT void rotate90deg(int times) override;
			EXPORT void fishToPano(float xOffset, float yOffset) override;
			
			EXPORT void bgr2rgb() override;
			EXPORT void blur(float x, float y, float w, float h, float amount) override;
			EXPORT expected<void> resize(Size size) override;
			EXPORT expected<void> resize(Size size, std::string method) override;
			EXPORT expected<cbuffer> resizeAndCopy(Size size) override;
			EXPORT expected<cbuffer> resizeAndCopy(Size size, std::string method) override;

			EXPORT void createBuffer(int width, int height, BufferDataType type) override;
			EXPORT expected<void> loadImage(std::string const& filename) override;
			EXPORT bool saveImage(std::string const& fileName) override;
			EXPORT std::vector<unsigned char> encodeImage() override;
			EXPORT std::string base64Encode() override;
			EXPORT bool base64Decode(std::string base64String) override;
		private:
			int stringToInterp(std::string method);
			cv::Point2f fisheyeMap(int x, int y, double R, double cenX, double cenY, double outHeight, double outWidth);
			void clampRect(Rect2f& pos);

			cv::Mat mat_;

			static std::vector<std::vector<cv::Point2f>> fishCache_;
			static float fishXoffset_;
			static float fishYoffset_;
		};

		EXPORT pCValue composeFrame(cv::Mat const& image, cmap const& meta, double timestamp = 0, int frameid = 0);

	};
}
