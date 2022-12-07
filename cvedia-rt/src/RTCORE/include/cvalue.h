/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <deque>
#include <map>

#include "builtin/basetypes.h"
#include "defines.h"

namespace cvedia {
	namespace rt {
		class CValue;

		namespace internal {
			class VersionControl;
		}

		
#define VAL std::make_shared<CValue>

		//! CValue object type enum: NONE,NUMBER,BOOL,STRING,VECTOR,MAP,VEC4F
		enum class CValueType
		{
			NONE = -1,
			NUMBER = 0,
			BOOL = 1,
			STRING = 2,
			VECTOR = 3,
			MAP = 4,
			BUFFER = 5,
			MEM = 6
		};

		//! Class for objects used to store values and transfer information inside CVEDIA-RT
		class CValue : public std::enable_shared_from_this<CValue>
		{
		public:
			EXPORT CValue();
			EXPORT CValue(CValueType cvalueType);
			EXPORT explicit CValue(bool x);
			EXPORT CValue(char* x);
			EXPORT CValue(char const x[]);
			EXPORT CValue(int const& x);
			EXPORT CValue(float const& x);
			EXPORT CValue(double const& x);
			EXPORT CValue(std::string const& x);
			EXPORT CValue(std::deque<std::string> const& x);
			EXPORT CValue(std::vector<std::string> const& x);
			EXPORT CValue(std::vector<float> const& x);
			EXPORT CValue(std::vector<int> const& x);
			EXPORT CValue(std::vector<Point2f> const& x);
			EXPORT CValue(std::map<std::string, std::string> const& x);
			EXPORT CValue(cmem const& x);
			EXPORT CValue(Rect2f const& x);
			EXPORT CValue(Point2f const& x);
			EXPORT CValue(Color4f const& x);
			EXPORT CValue(cbuffer const& x);
			//EXPORT CValue(cv::Scalar const& x);
			//EXPORT CValue(cv::Scalar_<int> const& x);
			EXPORT CValue(std::map<std::string, pCValue> const& x);
			EXPORT CValue(std::vector<pCValue> const& x);
			EXPORT CValue(CValue const& x);

			EXPORT pCValue operator[](std::string const& key);
			EXPORT pCValue operator[](std::vector<std::string> const& keys);
			EXPORT pCValue operator[](size_t x);
			EXPORT void operator=(bool x);
			EXPORT void operator=(int x);
			EXPORT void operator=(float x);
			EXPORT void operator=(double x);
			EXPORT void operator=(char* const& x);
			EXPORT void operator=(std::string const& x);
			EXPORT void operator=(std::deque<std::string> const& x);
			EXPORT void operator=(std::vector<std::string> const& x);
			EXPORT void operator=(std::vector<float> const& x);
			EXPORT void operator=(std::vector<int> const& x);
			EXPORT void operator=(std::map<std::string, std::string> const& x);
			EXPORT void operator=(Rect2f const& x);
			EXPORT void operator=(cbuffer const& x);
			EXPORT void operator=(cmem const& x);
			//EXPORT void operator=(cv::Scalar const& x);
			//EXPORT void operator=(cv::Scalar_<int> const& x);

			// Defining the ofstream operator allows sol to use to_string methods on lua for CValue usertypes
			EXPORT std::ostream& operator<<(std::ostream& out) {
				out << this->str();
				return out;
			}

			// Vector functions
			EXPORT bool pushBack(pCValue const& val);
			EXPORT bool pushBackCircular(pCValue const& val, size_t maxBufferSize);
			EXPORT bool removeAt(size_t index);
			EXPORT pCValue at(size_t index);

			EXPORT void clear();

			template <typename T>
			EXPORT expected<T> getValue();

			template <typename T>
			EXPORT expected<T> getValue(std::string const& key);

			template <typename T>
			EXPORT T getValueOr(T const& alt);

			template <typename T>
			EXPORT T getValueOr(std::string const& key, T const& alt);

			template <typename T>
			EXPORT bool getValueIfSet(std::string const& key, T& dest);

			template <typename T>
			EXPORT bool checkValueHasChanged(std::string const& key, T const& value);

			template <typename T>
			EXPORT bool _handleVersioning(T const& x);

			EXPORT bool setValue(std::string const& key, bool x);
			EXPORT bool setValue(std::string const& key, int x);
			EXPORT bool setValue(std::string const& key, float x);
			EXPORT bool setValue(std::string const& key, double x);
			EXPORT bool setValue(std::string const& key, std::string const& x);
			EXPORT bool setValue(std::string const& key, char* const& x);
			EXPORT bool setValue(std::string const& key, Rect2f const& x);
			EXPORT bool setValue(std::string const& key, cbuffer const& x);
			EXPORT bool setValue(std::string const& key, std::vector<float> const& x);
			//EXPORT bool setValue(std::string const& key, cv::Scalar const& x);
			//EXPORT bool setValue(std::string const& key, cv::Scalar_<int> const& x);
			EXPORT bool setValue(std::string const& key, CValue const& x);
			EXPORT bool setValue(std::string const& key, cvec const& x);
			EXPORT bool setValue(std::string const& key, cmap const& x);

			EXPORT bool setValue(bool x);
			EXPORT bool setValue(int x);
			EXPORT bool setValue(float x);
			EXPORT bool setValue(double x);
			EXPORT bool setValue(std::string const& x);
			EXPORT bool setValue(char* x);
			EXPORT bool setValue(char const x[]);
			EXPORT bool setValue(std::deque<std::string> const& x);
			EXPORT bool setValue(std::vector<std::string> const& x);
			EXPORT bool setValue(std::vector<float> const& x);
			EXPORT bool setValue(std::vector<int> const& x);
			EXPORT bool setValue(std::vector<Point2f> const& x);
			EXPORT bool setValue(std::map<std::string, std::string> const& x);
			EXPORT bool setValue(std::map<std::string, pCValue> const& x);
			EXPORT bool setValue(std::shared_ptr<std::vector<unsigned char>> const& x);
			EXPORT bool setValue(Rect2f const& r);
			EXPORT bool setValue(Point2f const& r);
			EXPORT bool setValue(Color4f const& r);
			EXPORT bool setValue(cbuffer const& x);
			EXPORT bool setValue(std::vector<pCValue> const& x);
			EXPORT bool setValue(CValue const& x);
			//EXPORT bool setValue(cv::Scalar const& x);
			//EXPORT bool setValue(cv::Scalar_<int> x);

			EXPORT bool _setValue(bool x);
			EXPORT bool _setValue(int x);
			EXPORT bool _setValue(float x);
			EXPORT bool _setValue(double x);
			EXPORT bool _setValue(std::string const& x);
			EXPORT bool _setValue(char* x);
			EXPORT bool _setValue(char const x[]);
			EXPORT bool _setValue(std::deque<std::string> const& x);
			EXPORT bool _setValue(std::vector<std::string> const& x);
			EXPORT bool _setValue(std::vector<float> const& x);
			EXPORT bool _setValue(std::vector<int> const& x);
			EXPORT bool _setValue(std::vector<Point2f> const& x);
			EXPORT bool _setValue(std::map<std::string, std::string> const& x);
			EXPORT bool _setValue(std::map<std::string, pCValue> const& x);
			EXPORT bool _setValue(std::shared_ptr<std::vector<unsigned char>> const& x);
			EXPORT bool _setValue(Rect2f const& r);
			EXPORT bool _setValue(Point2f const& r);
			EXPORT bool _setValue(Color4f const& r);
			EXPORT bool _setValue(cbuffer const& x);
			EXPORT bool _setValue(std::vector<pCValue> const& x);
			EXPORT bool _setValue(CValue const& x);
			//EXPORT bool _setValue(cv::Scalar const& x);
			//EXPORT bool _setValue(cv::Scalar_<int> x);

			EXPORT void unsetValue(std::string const& key);

			EXPORT std::vector<std::string> getKeyNames() const;
			EXPORT bool Filter(std::string const& key);
			EXPORT int RemoveEmptyEntries();
			EXPORT void RemoveRunTimeVars();
			EXPORT pCValue getChild(std::string const& x);
			EXPORT pCValue getChild(std::vector<std::string> const& x);
			EXPORT pCValue getChildOrCreate(std::string const& x);
			EXPORT pCValue getChild(size_t x);
			EXPORT bool hasChild(std::string const& key) const;
			EXPORT bool hasChild(std::vector<std::string> const& keys);

			EXPORT cmap::iterator begin();
			EXPORT cmap::iterator end();

			EXPORT size_t size() const;
			EXPORT bool isEmpty() const;

			EXPORT std::string str(int numberPrecision = 4);

			EXPORT pCValue diff(pCValue const& base);
			EXPORT pCValue merge(pCValue source);

			EXPORT CValueType getType() const;
			EXPORT void setType(CValueType newType);

			EXPORT int getVersion() const;

			EXPORT pCValue step();

			int version = -1;
			bool runtimeVar = false;
			std::weak_ptr<CValue> prev;
			std::shared_ptr<CValue> head;
		private:
			// Placed here to prevent compiler from erroneously converting
			// CValue * to bool. We don't support pointers as input
			EXPORT CValue(CValue*) {
			}

			EXPORT CValue(CValue const*) {
			}

			EXPORT bool setValue(CValue*) { return false; }
			EXPORT bool setValue(CValue const*) { return false; }
			EXPORT bool setValue(std::string const&, CValue*) { return false; }
			EXPORT bool setValue(std::string const&, CValue const*) { return false; }

			void linkObject(pCValue const& newObj);

			CValueType type = CValueType::NONE;

			void setVersion();

			template <typename T>
			bool _setVecValue(std::vector<T> const& x);

			template <typename T>
			bool _setKeyValue(std::string const& key, T const& x);

			//		template <typename T>
			//		bool _setValue(T const x);

			template <typename T>
			T _getValueOr(std::string const& key, T const& alt);

			template <typename T>
			bool _getValueIfSet(std::string const& key, T& dest);

			template <typename T>
			bool _checkValueHasChanged(std::string const& key, T const& value);

			struct data {
				bool bVal = false;
				double dVal = 0.0;
				std::string sVal;
				std::vector<pCValue> vVal;
				std::map<std::string, pCValue> mVal;
				cbuffer bufferVal;
				std::shared_ptr<std::vector<unsigned char>> memVal;
			};

			data data_;
		};
	}
}
