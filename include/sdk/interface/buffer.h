/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "cvaluefwd.h"
#include "builtin/basetypes.h"
#include "rterror.h"

namespace cv {
	class Mat;
}

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

				/* 2. Copying and resizing */
				virtual cbuffer copy() = 0;
				virtual cbuffer copy(double scaleFactor) = 0;
				virtual cbuffer copy(Rect2f const& rc, bool relative) = 0;
				virtual expected<void> copyBufferToRegion(Rect2f const& rc, bool relative, cbuffer destBuffer, Rect2f const& rectDest, bool destRelative) = 0;
				virtual expected<void> resize(Size size) = 0;
				virtual expected<void> resize(Size size, std::string const& method) = 0;
				virtual expected<cbuffer> resizeAndCopy(Size size) = 0;
				virtual expected<cbuffer> resizeAndCopy(Size size, std::string const& method) = 0;

				virtual expected<void> pixelate(cv::Mat mask, int blockSize) = 0;
				virtual expected<void> pixelate(float x, float y, float w, float h, int blockSize) = 0;

				/* 3. Image manipulation */
				virtual void rotate90deg(int times) = 0;
				virtual void brightness(double delta) = 0;
				virtual void bgr2rgb() = 0;
				virtual void grayscale(bool singleChannel) = 0;
				virtual void blur(cv::Mat mask, double amount) = 0;
				virtual void blur(float x, float y, float w, float h, double amount) = 0;
				virtual void fishToPano(float xOffset, float yOffset) = 0;
				virtual void fishToPanoOld(float xOffset, float yOffset) = 0;
				virtual void clahe(int const& clipLimit, int const& tileGridSizeX, int const& tileGridSizeY)  = 0;
				virtual cmap meanStdDev()  = 0;
				

				/* 4. Drawing and text */
				virtual expected<void> fill(cv::Mat mask, float color[3]) = 0;
				virtual expected<void> fill(float color[3], int opacity) = 0;
				virtual expected<void> drawLine(Point2f const& src, Point2f const& dest, float color[3], int thickness) = 0;
				virtual expected<void> drawRect(Rect2f const& pos, float color[3], int thickness) = 0;
				virtual expected<void> drawPoly(std::vector<Point2f> const& pts, float color[3], double alpha) = 0;
				virtual expected<void> drawPolyLine(std::vector<Point2f> const& pts, float color[3], int thickness) = 0;
				virtual expected<void> drawCircle(Point2f const& center, int radius, float color[3], int thickness) = 0;
				virtual expected<void> writeText(Point2f const& pos, std::string const& text, float color[3], int fontface, double fontscale, int thickness) = 0;
				virtual expected<void> writeTextAutoSize(Point2f const& pos, std::string const& text, float color[3], int fontface, int genericSize) = 0;
				virtual std::pair<int, int> getTextAutoSize(std::string const& text, int fontface, int genericSize) = 0;
				virtual expected<void> drawBoundingBox(Rect2f const& pos, std::string const& label, float color[3], int thickness) = 0;

				/* 5. Loading, saving and encoding images */
				virtual void createBuffer(int width, int height, BufferDataType type) = 0;
				virtual expected<void> createFromBlob(int width, int height, BufferDataType type, void* data, size_t step=0) = 0;
				virtual expected<void> loadImage(std::string const& filename) = 0;
				virtual expected<void> loadImage(void* data, int data_size) = 0;
				virtual bool saveImage(std::string const& fileName) = 0;
				virtual std::vector<unsigned char> encodeImage() = 0;
				virtual std::string base64Encode() = 0;
				virtual bool base64Decode(std::string const& base64String) = 0;

				/* 6. Miscellaneous */
				virtual Size getSize() const = 0;
				virtual int getChannels() const = 0;
				virtual void* data() = 0;
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
