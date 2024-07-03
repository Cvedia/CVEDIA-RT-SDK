/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once 
#include <system_error>
#include <expected.hpp>

// Checks if the LUA object has the plugin property set (if error throws it)
#define CHECK_LUA(obj) do {if ((obj) == nullptr) {LOGE << "Invalid Lua object passed (nullptr)"; luaL_error(s, "%s", "Invalid Lua object passed");}} while(0)
// Checks if given pointer is not null (if error throws it)
#if WITH_UI
#define CHECK_NULLPTR(ptr) do {if ((ptr) == nullptr){std::string msg = std::string("Object is not initialized: ") + std::string(#ptr); api::ui::showNotification("Error", "", msg.c_str(), "", 4000); luaL_error(s, "%s", msg.c_str()); }} while(0)
#else
#define CHECK_NULLPTR(ptr) do {if ((ptr) == nullptr){std::string msg = std::string("Object is not initialized: ") + std::string(#ptr); luaL_error(s, "%s", msg.c_str()); }} while(0)
#endif

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

#if WITH_UI
// Check if value is expected, if not throws error
#define TRY_LUA(var) do {auto &&tmp = var; if (!tmp) { std::string msg = "Function call returned: " + tmp.error().message(); api::ui::showNotification("Error", "", msg.c_str(), "", 4000); luaL_error(s, "%s", msg.c_str());}} while(0)
// Check if value is expected, if not throws error. If value is expected it sets it to val variable.
#define TRY_VAL_LUA(var, val) do {auto &&tmp = var; if (!tmp) { std::string msg = "Function call returned: " + tmp.error().message(); api::ui::showNotification("Error", "", msg.c_str(), "", 4000); luaL_error(s, "%s", msg.c_str());} (val) = std::move(tmp.value());} while(0)
// Check if value is expected, it not print warning and continue
#define TRY_LUA_CONT(var) do {auto &&tmp = var; if (!tmp) { std::string msg = "Function returned: " + tmp.error().message(); api::ui::showNotification("Warning", "", msg.c_str(), "", 4000);}} while(0)
#else
// Check if value is expected, if not throws error
#define TRY_LUA(var) do {auto &&tmp = var; if (!tmp) { std::string msg = "Function call returned: " + tmp.error().message(); luaL_error(s, "%s", msg.c_str());}} while(0)
// Check if value is expected, if not throws error. If value is expected it sets it to val variable.
#define TRY_VAL_LUA(var, val) do {auto &&tmp = var; if (!tmp) { std::string msg = "Function call returned: " + tmp.error().message(); luaL_error(s, "%s", msg.c_str());} (val) = std::move(tmp.value());} while(0)
// Check if value is expected, it not print warning and continue
#define TRY_LUA_CONT(var) do {auto &&tmp = var; if (!tmp) { std::string msg = "Function returned: " + tmp.error().message();}} while(0)
#endif

#define RT_LUA_TRY(var, expr) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		std::string msg = "Function call returned: " + UNIQUE_NAME(tmp).error().message(); \
		luaL_error(s, "%s", msg.c_str()); \
	} \
	var = std::move(UNIQUE_NAME(tmp).value())

#define RT_ASSERT(expr, message) \
	if (!(expr)) { \
		LOGE << message; \
		return ::cvedia::rt::unexpected(RTErrc::InvalidArgument); \
	}

#define RT_TRY(var, expr) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		return ::cvedia::rt::unexpected(UNIQUE_NAME(tmp).error()); \
	} \
	var = std::move(UNIQUE_NAME(tmp).value())

#define RT_TRY_REASON(var, expr, err) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		return ::cvedia::rt::unexpected(err); \
	} \
	var = std::move(UNIQUE_NAME(tmp).value())

#define RT_TRY_WARNING(var, expr, message) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		LOGW << message; \
		return ::cvedia::rt::unexpected(UNIQUE_NAME(tmp).error()); \
	} \
	var = std::move(UNIQUE_NAME(tmp).value())

#define RT_TRY_ERROR(var, expr, message) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		LOGE << message; \
		return ::cvedia::rt::unexpected(UNIQUE_NAME(tmp).error()); \
	} \
	var = std::move(UNIQUE_NAME(tmp).value())

#define RT_TRY_DEBUG(var, expr, message) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		LOGD << message; \
		return ::cvedia::rt::unexpected(UNIQUE_NAME(tmp).error()); \
	} \
	var = std::move(UNIQUE_NAME(tmp).value())

// Hacky workaround in case you have a macros that requires 2 RT_TRY's
// the UNIQUE_NAME would conflict
#define RT_TRY2(var, expr) \
	auto &&UNIQUE_NAME(tmp2) = expr; \
	if (!UNIQUE_NAME(tmp2)) { \
		return ::cvedia::rt::unexpected(UNIQUE_NAME(tmp2).error()); \
	} \
	var = std::move(UNIQUE_NAME(tmp2).value())

// Executes expression and returns unexpected object to caller on failure
#define RT_CATCH(expr) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) \
		return ::cvedia::rt::unexpected(UNIQUE_NAME(tmp).error())

#define RT_CATCH_WARNING(expr, message) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		LOGW << message; \
		return ::cvedia::rt::unexpected(UNIQUE_NAME(tmp).error()); \
	}

#define RT_CATCH_ERROR(expr, message) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		LOGE << message; \
		return ::cvedia::rt::unexpected(UNIQUE_NAME(tmp).error()); \
	}

#define RT_CATCH_DEBUG(expr, message) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		LOGD << message; \
		return ::cvedia::rt::unexpected(UNIQUE_NAME(tmp).error()); \
	}

#define RT_IF(var, expr) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
	} else { var = std::move(UNIQUE_NAME(tmp).value());

#define RT_TRY_REST(var, expr, errc) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		response.status = static_cast<int>(errc); \
		return; \
	} \
	var = std::move(UNIQUE_NAME(tmp).value())

#define RT_TRY_REST_MAP(var, expr, errcMap) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		if (errcMap.find(UNIQUE_NAME(tmp).error()) == errcMap.end()) { \
			LOGW << "Cannot map error " << UNIQUE_NAME(tmp).error().message() << " to an HTTP status code"; \
			response.status = 500; \
			return; \
		} \
		response.status = static_cast<int>(errcMap[UNIQUE_NAME(tmp).error()]); \
		return; \
	} \
	var = std::move(UNIQUE_NAME(tmp).value())

#define RT_CATCH_REST(expr, errc) \
	auto &&UNIQUE_NAME(tmp) = expr; \
	if (!UNIQUE_NAME(tmp)) { \
		response.status = static_cast<int>(errc); \
		return; \
	}

namespace cvedia { namespace rt {
	template<class T> using expected = tl::expected<T, std::error_code>;
	using unexpected = tl::unexpected<std::error_code>;
}}

enum class RTErrc
{
	// no 0
	Success = 0,
	ParseError = 1,              // unable to parse
	NoSuchBackend,               // specified backend does not exist
	NoSuchDevice,                // specified device does not exist
	FailedInitBackend,           // Failed to initialize backend
	UnknownExtension,            // Extension has not been registered
	OperationFailed,             // Method did not execute correctly
	ConnectionRefused,           // Could not connect to resource
	ConnectionError,             // Error occured on connection
	FileNotFound,                // File does not exist
	LayerNotFound,               // Could not find specific layer
	LoadModelFailed,             // Failed to load model
	DeviceInitFailed,            // Device initialize failed
	UnsupportedOperation,        // Unsupported operation
	UnsupportedCodec,            // Unsupported codec
	InvalidArgument,             // Invalid argument specified
	InstanceNotLoaded,			 // Instance not loaded	
	InstanceNotRunning,			 // Instance not running
	FailedToDownload,			 // Failed to download
	AlreadyExists,               // Already exists
	BufferTooSmall,				 // Buffer is too small	
	FileAlreadyExists,		     // File already exists
	NotEnoughData,               // Not enough data
	ConfigIsExternal,			 // Config is external
	AlreadyInitialized,			 // Already initialized
	IsRunning,                   // Is running
	MissingData,                 // Data is missing
	PluginFailed,                // Plugin failed
	PluginMissing,               // Plugin is missing
	SolutionNotFound,			 // Solution not found
	InstanceAlreadyLoaded,		 // Instance already loaded
	DoesNotExist,                // Does not exist
	ReadOnly,                    // Read only
	NotInitialized,              // Not initialized
	NotImplemented,              // Not implemented
	InvalidConfig,               // Invalid config
	InvalidPath,                 // Invalid path
	NoFrameToRead,               // No frame available to read
	UnsupportedModel,            // Unsupported model
	ModelNotLoaded,              // Model not loaded
	SolutionNotSet,				 // Solution not set
	IsDisabled,                  // Is disabled
	KeyNotFound,                 // No key found in config
	UnknownError,                // Unknown error or case
	FailedToCreate,              // Failed to create
	EndOfFile,                   // End of file
	ReadFailed,                  // Read failed
	OutOfRange,                  // Number Out Of Range
	Empty,                       // Queue, List, Vector is empty
	QueueFull,                   // Queue is full
	InvalidThread,               // Invalid thread
	AuthorizationRequired,       // Authorization required
	UnsupportedEvent,            // Unsupported event
	LicenseIsInvalid,            // License is not valid
	LicenseParseError,           // License data cannot be parsed
	LicenseSchemaError,          // License data does not have the expected schema
	LicenseDecodeError,          // License data cannot be decoded
	LicenseAlreadyExists,        // License requested to add is already in the system
	LicenseDoesNotExists,        // License to be operated is not found in the system
	LicenseEncryptionError,      // License data cannot be encrypted
	LicenseDecryptionError,      // License data cannot be decrypted
	LicenseActivationError,      // Unable to activate the current machine for the license
	LicenseDeactivationError,    // Unable to deactivate the current machine for the license
	LicenseFileCannotBeSaved,    // License file(s) cannot be saved
	LicenseFileCannotBeLoaded,   // License file(s) cannot be loaded
	LicenseDataRetrievalError,   // Error occurred while retrieving license data from the server
	LicenseFingerprintMismatch,  // The fingerprint of the current machine is not activated for the license
	LicenseMetaFileDoesNotExist, // The meta file for the license is not found, please remove and add the license again
	NoLicensesLoaded,		     // There are no licenses in the system, please try adding valid license keys
	MaxNumberOfStreamsReached,   // The system reached its maximum number of streams allowed by the license(s)
	SchemaError,				 // Schema error
	FileCannotBeOpened,			 // File cannot be opened
	NetworkError,				 // Network error (socket, connection, etc.)
	NoServerAvailable,			 // No server available
	InvalidChecksum,			 // Invalid checksum
	InferenceTimedOut,			 // Inference timed out
	ShuttingDown,				 // Shutting down
	UnknownThread,				 // Unknown thread
};

#ifdef _WIN32
__declspec(dllexport) std::error_code make_error_code(RTErrc);
#else
__attribute__((visibility("default"))) std::error_code make_error_code(RTErrc);
#endif

namespace std
{
	template <>
	struct is_error_code_enum<RTErrc> : true_type {};
}
