/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once 
#include <system_error>
#include <expected.hpp>

// Checks if the LUA object has the plugin property set (if error throws it)
#define CHECK_LUA(obj) do {if ((obj) == nullptr) luaL_error(s, "%s", "Invalid Lua object passed");} while(0)
// Checks if given pointer is not null (if error throws it)
#define CHECK_NULLPTR(ptr) do {if ((ptr) == nullptr){std::string msg = std::string("Object is not initialized: ") + std::string(#ptr); api::ui::showNotification("Error", "", msg.c_str(), "", 4000); luaL_error(s, "%s", msg.c_str()); }} while(0)

// Check if returned value is expected (othervise prints error message and retruns -1)
#define CHECK_RET(var) do {auto &&tmp = var; if (!tmp) {std::cout << "Function call returned: " + tmp.error().message(); return -1;}} while(0)
// Check if returned value is expected (othervise prints error message)
#define CHECK_IGN(var) do {auto &&tmp = var; if (!tmp) {std::cout << "Function call returned: " + tmp.error().message();}} while(0)
// Check if returned value is expected and sets it to the val variable (othervise prints error message and retruns -1)
#define CHECK_VAL(var, val) do {auto &&tmp = var; if (!tmp) {std::cout << "Function call returned: " + tmp.error().message(); return -1;} else { (val) = std::move(tmp.value());}} while(0)

// Checks if value is expected (othervise return unexpected with error)
#define TRY(var) do {auto &&tmp = var; if (!tmp) return ::cvedia::rt::unexpected(tmp.error());} while(0)

#define TRY_ERRC(var) do {auto &&errc = var; if (!errc) return errc;} while(0)
// Try setting value from expected type, if fails, returns unexpected with message.
#define TRY_VAL(var, val) do {auto &&tmp = var; if (!tmp) return cvedia::rt::unexpected(tmp.error()); (val) = std::move(tmp.value());} while(0)

// Copy the value if set
#define COPY_VAL_IF_SET(var, val) do {auto &&tmp = var; if (tmp) {(val) = std::move(tmp.value());}} while(0)

// Check if value is expected, if not throws error
#define TRY_LUA(var) do {auto &&tmp = var; if (!tmp) { std::string msg = "Function call returned: " + tmp.error().message(); api::ui::showNotification("Error", "", msg.c_str(), "", 4000); luaL_error(s, "%s", msg.c_str());}} while(0)
// Check if value is expected, if not throws error. If value is expected it sets it to val variable.
#define TRY_VAL_LUA(var, val) do {auto &&tmp = var; if (!tmp) { std::string msg = "Function call returned: " + tmp.error().message(); api::ui::showNotification("Error", "", msg.c_str(), "", 4000); luaL_error(s, "%s", msg.c_str());} (val) = std::move(tmp.value());} while(0)
// Check if value is expected, it not print warning and continue
#define TRY_LUA_CONT(var) do {auto &&tmp = var; if (!tmp) { std::string msg = "Function returned: " + tmp.error().message(); api::ui::showNotification("Warning", "", msg.c_str(), "", 4000);}} while(0)

#define RT_TRY(var, expr) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		return ::cvedia::rt::unexpected(UNIQUE_NAME(tmp).error()); \
	} \
	var = std::move(UNIQUE_NAME(tmp).value())

// Executes expression and returns unexpected object to caller on failure
#define RT_CATCH(expr) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) \
		return ::cvedia::rt::unexpected(UNIQUE_NAME(tmp).error())

#define RT_IF(var, expr) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
	} else { var = std::move(UNIQUE_NAME(tmp).value());


namespace cvedia { namespace rt {
	template<class T> using expected = tl::expected<T, std::error_code>;
	using unexpected = tl::unexpected<std::error_code>;
}}

enum class RTErrc
{
	// no 0
	Success = 0,
	ParseError = 1,       // unable to parse
	NoSuchBackend,      // specified backend does not exist
	NoSuchDevice,       // specified device does not exist
	FailedInitBackend,	// Failed to initialize backend
	UnknownExtension,	// Extension has not been registered
	OperationFailed,	// Method did not execute correctly
	ConnectionRefused,	// Could not connect to resource
	ConnectionError,	// Error occured on connection
	FileNotFound,	// File does not exist
	LayerNotFound,		// Could not find specific layer
	LoadModelFailed,	// Failed to load model
	DeviceInitFailed,	// Device initialize failed
	UnsupportedOperation, // Unsupported operation
	InvalidArgument,	// Invalid argument specified
	AlreadyExists,		// Already exists
	IsRunning,			// Is running
	MissingData,		// Data is missing
	PluginFailed,		// Plugin failed
	DoesNotExist,		// Does not exist
	NotInitialized,		// Not initialized
	NotImplemented,		// Not implemented
	InvalidConfig,		// Invalid config
	NoFrameToRead,		// No frame available to read
	UnsupportedModel,	// Unsupported model
	ModelNotLoaded,		// Model not loaded
	IsDisabled,			// Is disabled
	KeyNotFound,		// No key found in config
	UnknownError,		// Unknown error or case
	FailedToCreate,		// Failed to create
	EndOfFile,			// End of file
	ReadFailed,			// Read failed
	OutOfRange,			// Number Out Of Range
	Empty,				// Queue, List, Vector is empty
};

#ifdef WIN32
__declspec(dllexport) std::error_code make_error_code(RTErrc);
#else
std::error_code make_error_code(RTErrc);
#endif

namespace std
{
	template <>
	struct is_error_code_enum<RTErrc> : true_type {};
}
