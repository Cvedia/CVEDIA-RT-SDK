#pragma once
#if defined(_WIN32)
//  Microsoft
#define EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
//  GCC
#define EXPORT __attribute__((visibility("default")))
#else
//  do nothing and hope for the best?
#define EXPORT
#pragma warning Unknown dynamic link import/export semantics.
#endif

#define RT_C_TRY(var, expr) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		return -1; \
	} \
	var = std::move(UNIQUE_NAME(tmp).value())

#define RT_C_TRY_STR(var, expr) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		return nullptr; \
	} \
	var = std::move(UNIQUE_NAME(tmp).value())

#define RT_C_CATCH(expr) do { auto &&UNIQUE_NAME(tmp) = expr; if (!UNIQUE_NAME(tmp)) return -1; } while (0)
