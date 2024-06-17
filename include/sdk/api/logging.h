/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#define PLOG_CAPTURE_FILE

#include <string>
#include <deque>
#include <set>
#include <mutex>

#include <plog/Log.h>
#include <plog/Severity.h>

#include "defines.h"
#include "uuid.h"

static std::set<std::string> logOnceKeys;
static std::mutex logOnceMutex;

#define LOG_ONCE(message, level) \
    do { \
        std::lock_guard<std::mutex> lock(logOnceMutex); \
        auto key = __FILE__ + std::to_string(__LINE__); \
        if (logOnceKeys.find(key) == logOnceKeys.end()) \
        { \
            logOnceKeys.insert(key); \
            cvedia::rt::api::logging::log(message, level); \
        } \
    } while (0)

namespace plog {
	inline Record& operator<<(Record& record, const cvedia::rt::Uuid& pt) // Implement a stream operator for our type.
	{
		return record << pt.toString();
	}

	class IAppender;
}

namespace cvedia {
	namespace rt {
		namespace api {
			namespace logging {
				EXPORT std::deque<std::wstring> getLog();
				EXPORT void log(std::wstring msg, plog::Severity level);
				EXPORT void log(std::string msg, plog::Severity level);
				EXPORT void enableLogging(bool writeConsole, std::string filename, plog::Severity severity);
				EXPORT void enableLogging();
				EXPORT void pushLogEntry(std::wstring msg);
				EXPORT plog::IAppender* getLogger();
				EXPORT plog::Severity getSeverity();
			};
		}
	}
}
