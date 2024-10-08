/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <cmath>
#include <array>

#define TO_CVPOINT_FROMRELATIVE(arg, mat) cv::Point2f(arg.x * static_cast<float>(mat.cols), arg.y * static_cast<float>(mat.rows))
#define TO_CVPOINT(arg) cv::Point2f(arg.x, arg.y)
#define TO_CVPOINTD(arg) cv::Point(arg.x, arg.y)
#define TO_CVRECT(arg) cv::Rect2f(arg.x, arg.y, static_cast<float>(arg.width), static_cast<float>(arg.height))
#define TO_CVRECT_FROMRELATIVE(arg, mat) cv::Rect2f(arg.x * static_cast<float>(mat.cols), arg.y * static_cast<float>(mat.rows), static_cast<float>(arg.width) * static_cast<float>(mat.cols), static_cast<float>(arg.height) * static_cast<float>(mat.rows))
#define TO_CVRECTD(arg) cv::Rect(arg.x, arg.y, static_cast<float>(arg.width), static_cast<float>(arg.height))
#define TO_CVRECTI(arg) cv::Rect2i(arg.x, arg.y, static_cast<int>(arg.width), static_cast<int>(arg.height))
#define TO_CVRECTF(arg) cv::Rect2f(arg.x, arg.y, static_cast<float>(arg.width), static_cast<float>(arg.height))
#define TO_CVCOLOR(arg) cv::Scalar(arg[0], arg[1], arg[2], arg[3])
#define TO_CVCOLOR_OPAQUE_8(arg) cv::Scalar(arg[0], arg[1], arg[2], 255)
#define RGBA32_TO_BGRA8(src) cv::Scalar{src[2] * 255.f, src[1] * 255.f, src[0] * 255.f, src[3]}
#define RGBA32_TO_BGRA8_OPAQUE(src) cv::Scalar{src[2] * 255.f, src[1] * 255.f, src[0] * 255.f, 255.f}
#define FROM_CVRECT(arg) Rect2f(arg.x, arg.y, static_cast<float>(arg.width), static_cast<float>(arg.height))
#define FROM_CVPOINT(arg) Point2f(static_cast<float>(arg.x), static_cast<float>(arg.y))
#define BOUNDED_COORD(arg) std::min(std::max(0.f, arg), 1.f)
#define BOUNDED_POINT(arg) Point2f(static_cast<float>(BOUNDED_COORD(arg.x)), static_cast<float>(BOUNDED_COORD(arg.y)))

namespace cvedia {
	namespace rt {

		typedef std::array<float, 4> Color4f;

		template<typename Tp> class Point_
		{
		public:
			typedef Tp value_type;

			//! default constructor
			Point_();
			Point_(Tp _x, Tp _y);
			Point_(Point_ const& pt);

			//! conversion to another data type
			template<typename Tp2> operator Point_<Tp2>() const;

			Tp x; //!< x coordinate of the point
			Tp y; //!< y coordinate of the point
		};

		typedef Point_<int> Point2i;
		typedef Point_<int64_t> Point2l;
		typedef Point_<float> Point2f;
		typedef Point_<double> Point2d;
		typedef Point2i Point;

		template<typename Tp> class Size_
		{
		public:
			typedef Tp value_type;

			//! default constructor
			Size_();
			Size_(Tp _width, Tp _height);
			Size_(Size_ const& sz);
			Size_(Point_<Tp> const& pt);

			Size_& operator = (Size_ const& sz);
			//! the area (width*height)
			Tp area() const;
			//! true if empty
			bool empty() const;

			//! conversion of another data type.
			template<typename Tp2> operator Size_<Tp2>() const;

			Tp width; //!< the width
			Tp height; //!< the height
		};

		typedef Size_<int> Size2i;
		typedef Size_<int64_t> Size2l;
		typedef Size_<float> Size2f;
		typedef Size_<double> Size2d;
		typedef Size2i Size;


		template<typename Tp> class Rect_
		{
		public:
			Rect_() {
				x = 0;
				y = 0;
				width = 0;
				height = 0;
			}
			Rect_(Tp x_, Tp y_, Tp width_, Tp height_) {
				x = x_;
				y = y_;
				width = width_;
				height = height_;
			}

			Point_<Tp> br() const {
				return Point_<Tp>(x + width, y + height);
			}

			Point_<Tp> tl() const
			{
				return Point_<Tp>(x, y);
			}

			Size_<Tp> size() const
			{
				return Size_<Tp>(width, height);
			}

			Tp area() const {
				return width * height;
			}

			bool empty() const;

			Tp x;
			Tp y;
			Tp width;
			Tp height;
		};

		typedef Rect_<int> Rect2i;
		typedef Rect_<float> Rect2f;
		typedef Rect_<double> Rect2d;
		typedef Rect2i Rect;


		template<typename Tp> inline
			Size_<Tp>::Size_()
			: width(0), height(0) {}

		template<typename Tp> inline
			Size_<Tp>::Size_(Tp _width, Tp _height)
			: width(_width), height(_height) {}

		template<typename Tp> inline
			Size_<Tp>& Size_<Tp>::operator = (Size_<Tp> const& sz)
		{
			width = sz.width; height = sz.height;
			return *this;
		}

		template<typename Tp> inline
			Tp Size_<Tp>::area() const
		{
			Tp const result = width * height;
			return result;
		}

		template<typename Tp> inline
			bool Size_<Tp>::empty() const
		{
			return width <= 0 || height <= 0;
		}

		template<typename Tp> inline
			Rect_<Tp>& operator &= (Rect_<Tp>& a, Rect_<Tp> const& b)
		{
			Tp x1 = std::max<Tp>(a.x, b.x);
			Tp y1 = std::max<Tp>(a.y, b.y);
			a.width = std::min<Tp>(a.x + a.width, b.x + b.width) - x1;
			a.height = std::min<Tp>(a.y + a.height, b.y + b.height) - y1;
			a.x = x1;
			a.y = y1;
			if (a.width <= 0 || a.height <= 0)
				a = Rect_<Tp>();
			return a;
		}

		template<typename _Tp> static inline
			Rect_<_Tp>& operator |= (Rect_<_Tp>& a, const Rect_<_Tp>& b)
		{
			if (a.empty()) {
				a = b;
			}
			else if (!b.empty()) {
				_Tp x1 = std::min(a.x, b.x);
				_Tp y1 = std::min(a.y, b.y);
				a.width = std::max(a.x + a.width, b.x + b.width) - x1;
				a.height = std::max(a.y + a.height, b.y + b.height) - y1;
				a.x = x1;
				a.y = y1;
			}
			return a;
		}

		template<typename Tp> inline
			Rect_<Tp> operator & (Rect_<Tp> const& a, Rect_<Tp> const& b)
		{
			Rect_<Tp> c = a;
			return c &= b;
		}

		template<typename _Tp> static inline
			Rect_<_Tp> operator | (const Rect_<_Tp>& a, const Rect_<_Tp>& b)
		{
			Rect_<_Tp> c = a;
			return c |= b;
		}

		template<typename Tp> inline
			Point_<Tp>::Point_()
			: x(0), y(0) {}

		template<typename Tp> inline
			Point_<Tp>::Point_(Tp _x, Tp _y)
			: x(_x), y(_y) {}

		template<typename Tp> inline
			Point_<Tp>::Point_(Point_ const& pt)
			: x(pt.x), y(pt.y) {}

		template<typename Tp> static inline
			Point_<Tp>& operator += (Point_<Tp>& a, Point_<Tp> const& b)
		{
			a.x += b.x;
			a.y += b.y;
			return a;
		}

		template<typename Tp> static inline
			Point_<Tp>& operator -= (Point_<Tp>& a, Point_<Tp> const& b)
		{
			a.x -= b.x;
			a.y -= b.y;
			return a;
		}

		template<typename Tp> static inline
			Point_<Tp>& operator *= (Point_<Tp>& a, int b)
		{
			a.x = static_cast<Tp>(a.x * b);
			a.y = static_cast<Tp>(a.y * b);
			return a;
		}

		template<typename Tp> static inline
			Point_<Tp>& operator *= (Point_<Tp>& a, float b)
		{
			a.x = static_cast<Tp>(a.x * b);
			a.y = static_cast<Tp>(a.y * b);
			return a;
		}

		template<typename Tp> static inline
			Point_<Tp>& operator *= (Point_<Tp>& a, double b)
		{
			a.x = static_cast<Tp>(a.x * b);
			a.y = static_cast<Tp>(a.y * b);
			return a;
		}

		template<typename Tp> static inline
			Point_<Tp>& operator /= (Point_<Tp>& a, int b)
		{
			a.x = static_cast<Tp>(a.x / b);
			a.y = static_cast<Tp>(a.y / b);
			return a;
		}

		template<typename Tp> static inline
			Point_<Tp>& operator /= (Point_<Tp>& a, float b)
		{
			a.x = static_cast<Tp>(a.x / b);
			a.y = static_cast<Tp>(a.y / b);
			return a;
		}

		template<typename Tp> static inline
			Point_<Tp>& operator /= (Point_<Tp>& a, double b)
		{
			a.x = static_cast<Tp>(a.x / b);
			a.y = static_cast<Tp>(a.y / b);
			return a;
		}

		template<typename Tp> static inline
			Point_<Tp> operator + (Point_<Tp> const& a, Point_<Tp> const& b)
		{
			return Point_<Tp>(static_cast<Tp>(a.x + b.x), static_cast<Tp>(a.y + b.y));
		}

		template<typename Tp> static inline
			Point_<Tp> operator - (Point_<Tp> const& a, Point_<Tp> const& b)
		{
			return Point_<Tp>(static_cast<Tp>(a.x - b.x), static_cast<Tp>(a.y - b.y));
		}

		template<typename Tp> static inline
			Point_<Tp> operator - (Point_<Tp> const& a)
		{
			return Point_<Tp>(static_cast<Tp>(-a.x), static_cast<Tp>(-a.y));
		}

		template<typename Tp> static inline
			Point_<Tp> operator * (Point_<Tp> const& a, int b)
		{
			return Point_<Tp>(static_cast<Tp>(a.x * b), static_cast<Tp>(a.y * b));
		}

		template<typename Tp> static inline
			Point_<Tp> operator * (int a, Point_<Tp> const& b)
		{
			return Point_<Tp>(static_cast<Tp>(b.x * a), static_cast<Tp>(b.y * a));
		}

		template<typename Tp> static inline
			Point_<Tp> operator * (Point_<Tp> const& a, float b)
		{
			return Point_<Tp>(static_cast<Tp>(a.x * b), static_cast<Tp>(a.y * b));
		}

		template<typename Tp> static inline
			Point_<Tp> operator * (float a, Point_<Tp> const& b)
		{
			return Point_<Tp>(static_cast<Tp>(b.x * a), static_cast<Tp>(b.y * a));
		}

		template<typename Tp> static inline
			Point_<Tp> operator * (Point_<Tp> const& a, double b)
		{
			return Point_<Tp>(static_cast<Tp>(a.x * b), static_cast<Tp>(a.y * b));
		}

		template<typename Tp> static inline
			Point_<Tp> operator * (double a, Point_<Tp> const& b)
		{
			return Point_<Tp>(static_cast<Tp>(b.x * a), static_cast<Tp>(b.y * a));
		}

		template<typename _Tp> inline
			bool Rect_<_Tp>::empty() const
		{
			return width <= 0 || height <= 0;
		}

		template<typename Tp> static inline
			double norm(Point_<Tp> const& pt)
		{
			return std::sqrt(static_cast<double>(pt.x) * pt.x + static_cast<double>(pt.y) * pt.y);
		}
	}
}
