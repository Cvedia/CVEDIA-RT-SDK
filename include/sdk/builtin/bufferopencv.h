/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#include <shared_mutex>
#include <unordered_map>
#include <opencv2/core.hpp>    // for Mat
#include "builtin/basetypes.h"     // for Rect2f, Size
#include "defines.h"               // for EXPORT, cbuffer, cmap, pCValue
#include "interface/buffer.h"      // for Buffer, BufferDataType
#include "rterror.h"
#include <utility>
#include <set>

namespace cvedia { namespace rt { struct BufferInit; } }


namespace cvedia {
	namespace rt {

		typedef std::tuple<int, int, float, float> BufferCacheMapKey;

		template <class T>
		void hashCombine(std::size_t& seed, const T& v)
		{
			std::hash<T> hasher;
			seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}

		struct BufferCacheMapKeyHash
		{
			std::size_t operator()(const BufferCacheMapKey& k) const
			{
				const auto h1 = std::hash<int>{}(std::get<0>(k));
				const auto h2 = std::hash<int>{}(std::get<1>(k));
				const auto h3 = std::hash<float>{}(std::get<2>(k));
				const auto h4 = std::hash<float>{}(std::get<3>(k));

				std::size_t seed = 0;
				hashCombine(seed, h1);
				hashCombine(seed, h2);
				hashCombine(seed, h3);
				hashCombine(seed, h4);

				return seed;
			}
		};

		struct BufferCacheMapKeyEqual
		{
			bool operator()(const BufferCacheMapKey& v0, const BufferCacheMapKey& v1) const
			{
				return
					std::get<0>(v0) == std::get<0>(v1) &&
					std::get<1>(v0) == std::get<1>(v1) &&
					std::get<2>(v0) == std::get<2>(v1) &&
					std::get<3>(v0) == std::get<3>(v1);
			}
		};

		typedef std::unordered_map<const BufferCacheMapKey, std::pair<cv::Mat, cv::Mat>, BufferCacheMapKeyHash, BufferCacheMapKeyEqual> BufferCacheMap;

		class EXPORT_CLASS BufferOpenCV : public iface::Buffer {
		public:
			EXPORT BufferOpenCV();
			EXPORT ~BufferOpenCV();
			EXPORT BufferOpenCV(cv::Mat const& m);

			EXPORT static void create(BufferInit* init);

			EXPORT cv::Mat getCvMat() const;

			EXPORT cbuffer copy() override;
			EXPORT cbuffer copy(double scaleFactor) override;
			EXPORT cbuffer copy(Rect2f const& rc, bool relative) override;
			EXPORT expected<void> copyBufferToRegion(Rect2f const& rc, bool relative, cbuffer destBuffer, Rect2f const& rectDest, bool destRelative) override;
			EXPORT Size getSize() const override;
			EXPORT int getChannels() const override;

			EXPORT expected<void> fill(cv::Mat mask, float color[3]) override;
			EXPORT expected<void> fill(float color[3], int opacity) override;
			EXPORT expected<void> pixelate(cv::Mat mask, int blockSize) override;
			EXPORT expected<void> pixelate(float x, float y, float w, float h, int blockSize) override;

			EXPORT expected<void> drawLine(Point2f const& src, Point2f const& dest, float color[3], int thickness) override;
			EXPORT expected<void> drawRect(Rect2f const& pos, float color[3], int thickness) override;
			EXPORT expected<void> drawPoly(std::vector<Point2f> const& pts, float color[3], double alpha) override;
			EXPORT expected<void> drawPolyLine(std::vector<Point2f> const& pts, float color[3], int thickness) override;
			EXPORT expected<void> drawCircle(Point2f const& center, int radius, float color[3], int thickness) override;
			EXPORT expected<void> writeText(Point2f const& pos, std::string const& text, float color[3], int fontface, double fontscale, int thickness) override;
			EXPORT expected<void> writeTextAutoSize(Point2f const& pos, std::string const& text, float color[3], int fontface, int genericSize) override;
			EXPORT std::pair<int, int> getTextAutoSize(std::string const& text, int fontface, int genericSize) override;
			EXPORT expected<void> drawBoundingBox(Rect2f const& pos, std::string const& label, float color[3], int thickness) override;

			EXPORT void* data() override;
			EXPORT void brightness(double delta) override;
			EXPORT void grayscale(bool singleChannel) override;

			EXPORT void rotate90deg(int times) override;
			EXPORT void fishToPano(float xOffset, float yOffset) override;
			EXPORT void fishToPanoOld(float xOffset, float yOffset) override;

			EXPORT void clahe(int const& clipLimit, int const& tileGridSizeX, int const& tileGridSizeY) override;
			EXPORT cmap meanStdDev() override;

			EXPORT void bgr2rgb() override;
			EXPORT void blur(cv::Mat mask, double amount) override;
			EXPORT void blur(float x, float y, float w, float h, double amount) override;
			EXPORT expected<void> resize(Size size) override;
			EXPORT expected<void> resize(Size size, std::string const& method) override;
			EXPORT expected<cbuffer> resizeAndCopy(Size size) override;
			EXPORT expected<cbuffer> resizeAndCopy(Size size, std::string const& method) override;

			EXPORT void createBuffer(int width, int height, BufferDataType type) override;
			EXPORT expected<void> createFromBlob(int width, int height, BufferDataType type, void* data, size_t step=0);
			EXPORT expected<void> loadImage(std::string const& filename) override;
			EXPORT expected<void> loadImage(void* data, int data_size) override;
			EXPORT bool saveImage(std::string const& fileName) override;
			EXPORT std::vector<unsigned char> encodeImage() override;
			EXPORT std::string base64Encode() override;
			EXPORT bool base64Decode(std::string const& base64String) override;
		private:
#if TRACY_ENABLE
			void _tracyFree(void* ptr, char const* name);
			void _tracyAlloc(void* ptr, size_t size, char const* name);
#endif

			int _stringToInterp(std::string method);
			void _clampRect(Rect2f& pos);

			cv::Mat mat_;

			static shared_mutex_class tracyMutex_;
			static std::set<void*> tracyCache_;

			static shared_mutex_class fishCacheMutex_;
			static BufferCacheMap fishCacheMap_;

			static float fishOffsetX_;
			static float fishOffsetY_;
			static std::vector<std::vector<cv::Point2f>> fishCache_;

			cv::Mat panoramaMat_;
		};

		EXPORT pCValue composeFrame(cv::Mat const& image, cmap const& meta, double timestamp = 0, int frameid = 0);

	};
}
