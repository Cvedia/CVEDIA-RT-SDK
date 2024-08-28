/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "rterror.h"
#include "cvaluefwd.h"

#include "builtin/basetypes.h"
#include "defines.h"

namespace cvedia {
	namespace rt {
		class CValue;

		namespace internal {
			class VersionControl;
		}

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

#define VAL std::make_shared<cvedia::rt::CValue>

		//! Class for objects used to store values and transfer information inside CVEDIA-RT
		class EXPORT_CLASS CValue final : public std::enable_shared_from_this<CValue>
		{
		public:
			EXPORT CValue();
			EXPORT ~CValue();

			explicit CValue(int* x) = delete;
			explicit CValue(float* x) = delete;
			explicit CValue(double* x) = delete;
			EXPORT explicit CValue(CValueType cvalueType);
			EXPORT explicit CValue(bool x);
			EXPORT explicit CValue(char* x);
			EXPORT explicit CValue(char const x[]);
			EXPORT explicit CValue(int const& x);
			EXPORT explicit CValue(float const& x);
			EXPORT explicit CValue(double const& x);
			EXPORT explicit CValue(std::string const& x);
			EXPORT explicit CValue(std::string&& x);
			EXPORT explicit CValue(std::vector<std::string> const& x);
			EXPORT explicit CValue(std::vector<float> const& x);
			EXPORT explicit CValue(std::vector<int> const& x);
			EXPORT explicit CValue(std::vector<Point2f> const& x);
			EXPORT explicit CValue(std::map<std::string, std::string> const& x);
			EXPORT explicit CValue(cmem const& x);
			EXPORT explicit CValue(Rect2f const& x);
			EXPORT explicit CValue(Point2f const& x);
			EXPORT explicit CValue(Color4f const& x);
			EXPORT explicit CValue(cbuffer const& x);
			EXPORT explicit CValue(std::map<std::string, pCValue> const& x);
			EXPORT explicit CValue(std::map<std::string, pCValue>&& x);
			EXPORT explicit CValue(std::vector<pCValue> const& x);
			EXPORT explicit CValue(std::vector<pCValue>&& x);
			EXPORT explicit CValue(CValue const& x);

			EXPORT CValue deepCopy() const;

			EXPORT pCValue operator[](std::string const& key);
			EXPORT pCValue operator[](std::vector<std::string> const& keys);
			EXPORT pCValue operator[](size_t x);
			EXPORT void operator=(bool x);
			EXPORT void operator=(int x);
			EXPORT void operator=(float x);
			EXPORT void operator=(double x);
			EXPORT void operator=(char* const& x);
			EXPORT void operator=(std::string const& x);
			EXPORT void operator=(std::vector<std::string> const& x);
			EXPORT void operator=(std::vector<float> const& x);
			EXPORT void operator=(std::vector<int> const& x);
			EXPORT void operator=(std::map<std::string, std::string> const& x);
			EXPORT void operator=(Rect2f const& x);
			EXPORT void operator=(cbuffer const& x);
			EXPORT void operator=(cmem const& x);

			// Defining the ofstream operator allows sol to use to_string methods on lua for CValue usertypes
			EXPORT std::ostream& operator<<(std::ostream& out) const;

			// Vector functions
			EXPORT bool pushBack(pCValue const& val);
			EXPORT bool pushBackCircular(pCValue const& val, size_t maxBufferSize);
			EXPORT bool removeAt(size_t index);
			EXPORT pCValue at(size_t index);

			EXPORT void clear();

			EXPORT expected<void> addValue(double val);
			EXPORT expected<void> subValue(double val);
			EXPORT expected<void> addValue(std::string const& key, double val);
			EXPORT expected<void> subValue(std::string const& key, double val);

			template <typename T>
			EXPORT expected<T> getValue() const;

			template <typename T>
			EXPORT T const& getValueRef() const;

			template <typename T>
			expected<T> _getValue(std::string const& key) const;

			template <typename T>
			EXPORT expected<T> getValue(std::string const& key) const;

			template <typename T>
			EXPORT T getValueOr(T const& alt) const;

			template <typename T>
			EXPORT T getValueOr(std::string const& key, T const& alt) const;

			template <typename T>
			EXPORT bool getValueIfSet(std::string const& key, T& dest) const;

			template <typename T>
			EXPORT bool checkValueHasChanged(std::string const& key, T const& value) const;

			EXPORT bool setValue(std::string const& key, bool x);
			EXPORT bool setValue(std::string const& key, int x);
			EXPORT bool setValue(std::string const& key, float x);
			EXPORT bool setValue(std::string const& key, double x);
			EXPORT bool setValue(std::string const& key, std::string const& x);
			EXPORT bool setValue(std::string const& key, char* const& x);
			EXPORT bool setValue(std::string const& key, Rect2f const& x);
			EXPORT bool setValue(std::string const& key, cbuffer const& x);
			EXPORT bool setValue(std::string const& key, std::vector<float> const& x);
			EXPORT bool setValue(std::string const& key, CValue const& x);
			EXPORT bool setValue(std::string const& key, cvec const& x);
			EXPORT bool setValue(std::string const& key, cmap const& x);

			EXPORT bool setValue(bool x);
			EXPORT bool setValue(int x);
			EXPORT bool setValue(float x);
			EXPORT bool setValue(double x);
			EXPORT bool setValue(std::string const& x);
			EXPORT bool setValue(std::string&& x);
			EXPORT bool setValue(char* x);
			EXPORT bool setValue(char const x[]);
			EXPORT bool setValue(std::vector<std::string> const& x);
			EXPORT bool setValue(std::vector<float> const& x);
			EXPORT bool setValue(std::vector<int> const& x);
			EXPORT bool setValue(std::vector<Point2f> const& x);
			EXPORT bool setValue(std::map<std::string, std::string> const& x);
			EXPORT bool setValue(std::map<std::string, pCValue> const& x);
			EXPORT bool setValue(std::map<std::string, pCValue>&& x);
			EXPORT bool setValue(std::shared_ptr<std::vector<unsigned char>> const& x);
			EXPORT bool setValue(Rect2f const& r);
			EXPORT bool setValue(Point2f const& r);
			EXPORT bool setValue(Color4f const& r);
			EXPORT bool setValue(cbuffer const& x);
			EXPORT bool setValue(std::vector<pCValue> const& x);
			EXPORT bool setValue(std::vector<pCValue>&& x);
			EXPORT bool setValue(CValue const& x);

			EXPORT bool setValue(CValue*) { return false; }
			EXPORT bool setValue(CValue const*) { return false; }
			EXPORT bool setValue(std::string const&, CValue*) { return false; }
			EXPORT bool setValue(std::string const&, CValue const*) { return false; }

			EXPORT void unsetValue(std::string const& key);

			EXPORT std::vector<std::string> getKeyNames() const;
			EXPORT bool filter(std::string const& key);
			EXPORT int removeEmptyEntries();
			EXPORT void removeRunTimeVars();
			EXPORT pCValue getChild(std::string const& x) const;
			EXPORT pCValue getChild(std::vector<std::string> const& x) const;
			EXPORT pCValue getChildOrCreate(std::string const& x);
			EXPORT std::vector<std::string> getKeys();
			EXPORT expected<pCValue> getChildIfSet(std::string const& x) const;
			EXPORT expected<pCValue> findChild(std::string const& path) const;
			EXPORT expected<pCValue> findChild(std::vector<std::string> const& path) const;
			EXPORT expected<void> deleteChild(std::string const& path);
			EXPORT pCValue findChildOrCreate(std::string const& path);
			EXPORT pCValue getChild(size_t x) const;
			EXPORT bool hasChild(std::string const& key) const;
			EXPORT bool hasChild(std::vector<std::string> const& keys);

			EXPORT cmap::iterator begin();
			EXPORT cmap::iterator end();

			EXPORT size_t size() const;
			EXPORT bool isEmpty() const;

			EXPORT std::string str(int numberPrecision = 4) const;

			EXPORT pCValue diff(pCValue const& base);
			EXPORT pCValue merge(pCValue source);
			EXPORT pCValue override(pCValue source);

			EXPORT CValueType const& getType() const;

			EXPORT int getVersion() const;

#if WITH_VERSION_CONTROL
			EXPORT pCValue step();
#endif

			EXPORT CValue(CValue&& src);

			EXPORT CValue& operator=(CValue const& val);

			bool runtimeVar_ = false;
		private:
			void _destroy();
			void _copy(CValue const& val);

			// Placed here to prevent compiler from erroneously converting
			// CValue * to bool. We don't support pointers as input
			CValue(CValue*) {
			}

			CValue(CValue const*) {
			}

			template <typename T>
			bool _handleVersioning(T const& x);

			bool _setValue(bool x);
			bool _setValue(int x);
			bool _setValue(float x);
			bool _setValue(double x);
			bool _setValue(std::string const& x);
			bool _setValue(std::string&& x);
			bool _setValue(char* x);
			bool _setValue(char const x[]);
			bool _setValue(std::vector<std::string> const& x);
			bool _setValue(std::vector<float> const& x);
			bool _setValue(std::vector<int> const& x);
			bool _setValue(std::vector<Point2f> const& x);
			bool _setValue(std::map<std::string, std::string> const& x);
			bool _setValue(std::map<std::string, pCValue> const& x);
			bool _setValue(std::map<std::string, pCValue>&& x);
			bool _setValue(std::shared_ptr<std::vector<unsigned char>> const& x);
			bool _setValue(Rect2f const& r);
			bool _setValue(Point2f const& r);
			bool _setValue(Color4f const& r);
			bool _setValue(cbuffer const& x);
			bool _setValue(std::vector<pCValue> const& x);
			bool _setValue(std::vector<pCValue>&& x);
			bool _setValue(CValue const& x);

			expected<std::pair<pCValue, std::string>> _traversePath(std::vector<std::string> const& path, bool returnParent) const;
			expected<std::pair<pCValue, std::string>> _traversePath(std::string const& path, bool returnParent) const;

			std::vector<std::string> _getKeys(CValue& obj, std::string const& curPath) const;

#if WITH_VERSION_CONTROL
			void _linkObject(pCValue const& newObj);
#endif

			void _setVersion();

			template <typename T>
			bool _setVecValue(std::vector<T> const& x);

			template <typename T>
			bool _setKeyValue(std::string const& key, T const& x);

			//		template <typename T>
			//		bool _setValue(T const x);

			template <typename T>
			T _getValueOr(std::string const& key, T const& alt) const;

			template <typename T>
			bool _getValueIfSet(std::string const& key, T& dest) const;

			template <typename T>
			bool _checkValueHasChanged(std::string const& key, T const& value) const;

			CValueType type_ = CValueType::NONE;

#if WITH_VERSION_CONTROL
			int version_ = -1;
			std::weak_ptr<CValue> prev_;
			std::shared_ptr<CValue> head_;
#endif

			union
			{
				bool bVal;
				double dVal;
				std::string sVal;
				std::vector<pCValue> vVal;
				std::map<std::string, pCValue> mVal;
				cbuffer bufferVal;
				std::shared_ptr<std::vector<unsigned char>> memVal;
			};
		};
	}
}
