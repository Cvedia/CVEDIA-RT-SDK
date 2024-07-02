/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <string>
#include "rtconfig.h"

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

#define M_PIF 3.141592653589793238462643383279502884e+00F

#define TIME_IN_MS std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
#define TIME_IN_US std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
#define TIME_IN_S std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()

template<typename T> struct argument_type;
template<typename T, typename U> struct argument_type<T(U)> { typedef U type; };
#define DEFINE_STATIC_LOCAL(t, name, arguments) static argument_type<void(t)>::type& name = *new argument_type<void(t)>::type arguments

#define POST_DIAGNOSTICS_EVENT(instance, severity, title, body) \
{ \
	cmap msg; \
	msg["instance_id"] = VAL(instance->getInstanceId().toString()); \
	msg["severity"] = VAL(severity); \
	msg["title"] = VAL(title); \
	msg["body"] = VAL(body); \
	\
	cmap container; \
	container["diagnostics"] = VAL(msg); \
	\
	instance->postEvent("global", "output", VAL(container)); \
}


	//! \cond
#if defined(_WIN32)
//  Microsoft
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#define EXPORT_CLASS
#elif defined(__GNUC__)
//  GCC
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#define EXPORT_CLASS EXPORT
#else
//  do nothing and hope for the best?
#define EXPORT
#define IMPORT
#define EXPORT_CLASS
#pragma warning Unknown dynamic link import/export semantics.
#endif

#ifndef _WIN32 // or something like that...
#define __stdcall
#define strcpy_s strcpy
#endif
//! \endcond

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
	#if __clang__
		#define NODISCARD __attribute__((warn_unused_result))
	#else
		#define NODISCARD [[nodiscard]]
	#endif
#elif defined (__has_cpp_attribute)
	#if __has_cpp_attribute(nodiscard)
		#define NODISCARD    [[nodiscard]]
	#else
		#define NODISCARD
	#endif
#else
	#define NODISCARD
#endif
