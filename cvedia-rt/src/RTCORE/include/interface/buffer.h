/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"
#include "builtin/basetypes.h"

namespace cvedia {
	namespace rt {
		enum BufferDataType : unsigned int {
			GRAY = 0,	// 00
			BGR = 16,	// 16
			BGRA = 24  // 24
		};

		namespace iface {
			class Buffer {
			public:
				virtual ~Buffer() = default;

				virtual cbuffer copy() = 0;
				virtual cbuffer copy(float scaleFactor) = 0;
				virtual cbuffer copy(Rect2f const& rc, bool relative) = 0;
				virtual void copyToRegion(Rect2f const& rc, bool relative, cbuffer destBuffer, Rect2f const& rectDest, bool destRelative) = 0;
				virtual Size getSize() = 0;
				virtual int getChannels() = 0;

				virtual void rotate90deg(int times) = 0;
				virtual void brightness(double delta) = 0;
				virtual void bgr2rgb() = 0;
				virtual void grayscale(bool singleChannel) = 0;
				virtual void blur(float x, float y, float w, float h, float amount) = 0;
				virtual expected<void> resize(Size size) = 0;
				virtual expected<void> resize(Size size, std::string method) = 0;
				virtual expected<cbuffer> resizeAndCopy(Size size) = 0;
				virtual expected<cbuffer> resizeAndCopy(Size size, std::string method) = 0;

				virtual void fishToPano(float xOffset, float yOffset) = 0;

				virtual void* data() = 0;

				virtual expected<void> fill(float color[3], int opacity) = 0;
				virtual expected<void> drawLine(Point2f const& src, Point2f const& dest, float color[3], int thickness) = 0;
				virtual expected<void> drawRect(Rect2f const& pos, float color[3], int thickness) = 0;
				virtual expected<void> drawPoly(std::vector<Point2f> const& pts, float color[3], float alpha) = 0;
				virtual expected<void> drawPolyLine(std::vector<Point2f> const& pts, float color[3], int thickness) = 0;
				virtual expected<void> drawCircle(Point2f const& center, int radius, float color[3], int thickness) = 0;
				virtual expected<void> writeText(Point2f const& pos, std::string const& text, float color[3], int fontface, float fontscale, int thickness) = 0;
				virtual expected<void> writeTextAutoSize(Point2f const& pos, std::string const& text, float color[3], int fontface, int generic_size) = 0;
				virtual std::pair<int, int> getSizeOfTextAutoSize(std::string const& text, int fontface, int generic_size) = 0;
				virtual expected<void> drawBoundingBox(Rect2f const& pos, std::string label, float color[3], int thickness) = 0;

				virtual void createBuffer(int width, int height, BufferDataType type) = 0;
				virtual expected<void> loadImage(std::string const& filename) = 0;
				virtual bool saveImage(std::string const& fileName) = 0;
				virtual std::vector<unsigned char> encodeImage() = 0;
				virtual std::string base64Encode() = 0;
				virtual bool base64Decode(std::string base64String) = 0;
				void timestamp(double timestamp) { timestamp_ = timestamp; }
				double timestamp() const { return timestamp_; }
				void frameid(int frameid) { frameid_ = frameid; }
				int frameid() const { return frameid_; }
			private:
				double timestamp_ = 0;
				int frameid_ = 0;
			};
		}
	}
}
