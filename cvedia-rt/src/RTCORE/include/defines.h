/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "rtconfig.h"
#include "rterror.h"

#if defined(_MSC_VER)
#ifdef _WIN64
using ssize_t = __int64;
#else
using ssize_t = int;
#endif
#endif

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

#ifndef UNIQUE_NAME
#define UNIQUE_NAME(base) CONCAT(base, __LINE__)
#endif

#define TIME_IN_MS std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
#define TIME_IN_US std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
#define TIME_IN_S std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()

template<typename T> struct argument_type;
template<typename T, typename U> struct argument_type<T(U)> { typedef U type; };
#define DEFINE_STATIC_LOCAL(t, name, arguments) static argument_type<void(t)>::type& name = *new argument_type<void(t)>::type arguments

	//! \cond
#if defined(WIN32)
//  Microsoft
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
//  GCC
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
//  do nothing and hope for the best?
#define EXPORT
#define IMPORT
#pragma warning Unknown dynamic link import/export semantics.
#endif

#ifndef WIN32 // or something like that...
#define __stdcall
#define strcpy_s strcpy
#endif
//! \endcond

typedef std::vector<std::string> STRVECT;

EXPORT STRVECT split_str(std::string const& s, char delimiter);
EXPORT bool is_number(const std::string& s);
EXPORT bool in_vector(const std::vector<std::string>& vec, const std::string& s);
EXPORT bool in_string(const std::string& full, const std::string& subString);
EXPORT bool in_vector(const std::vector<int>& vec, const int& i);
EXPORT bool file_exists(const std::string& file_path);

EXPORT std::string generate_uuid_v4();
EXPORT std::string to_string_with_precision(const float value, const int n = 6);
EXPORT std::string time_in_HH_MM_SS_MMM();

inline bool iequals(const std::string& a, const std::string& b)
{
	return std::equal(a.begin(), a.end(),
		b.begin(), b.end(),
		[](char a, char b) {
			return tolower(a) == tolower(b);
	});
}

inline std::string get_time_string(const std::string& time_format)
{
	const auto t = std::time(nullptr);
#if defined(WIN32)
	struct tm timeinfo;
	localtime_s(&timeinfo, &t);
#else
	const auto timeinfo = *std::localtime(&t);
#endif
	
	char buf[80];
	strftime(buf, sizeof(buf), time_format.c_str(), &timeinfo);
	return std::string(buf);
}

inline bool str_replace(std::string& str, const std::string& from, const std::string& to) {
	do {
		const size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
	} while (true);

	return true;
}


#define FN_CATCH_ALL_ERRORS 1
#ifdef FN_CATCH_ALL_ERRORS
#define STD_FUNCTION_ERROR(X) throw X;
#else
#define STD_FUNCTION_ERROR(X) return false;
#endif

// VS throws an error if an attribute newer than the requested standard level
// is used; everyone else just ignores it (or warns) like they are supposed to
//#if __cplusplus >= 201703L
//	#define NODISCARD    [[nodiscard]]

#if defined(_MSC_VER)
	#define NODISCARD [[nodiscard]]
#elif defined (__has_cpp_attribute)
	#if __has_cpp_attribute(nodiscard)
		#define NODISCARD    [[nodiscard]]
	#else
		#define NODISCARD
	#endif
#else
	#define NODISCARD
#endif

namespace cvedia {
	namespace rt {
		class CValue;
		namespace iface {
			class Buffer;
		}

		typedef std::unique_ptr<CValue> uCValue;
		typedef std::shared_ptr<CValue> pCValue;
		typedef std::map<std::string, pCValue> cmap;
		typedef std::vector<pCValue> cvec;
		typedef std::shared_ptr<iface::Buffer> cbuffer;
		typedef std::shared_ptr<std::vector<unsigned char>> cmem;
	}
}
