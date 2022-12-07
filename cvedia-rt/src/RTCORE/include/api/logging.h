/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <string>
#include <deque>

#include <plog/Log.h>
#include <plog/Severity.h>

#include "defines.h"

namespace cvedia {
	namespace rt {
		namespace api {
			namespace logging {
				EXPORT std::deque<std::string> getLog();
				EXPORT void log(std::string msg, plog::Severity level);
				EXPORT void enableLogging(bool writeConsole, std::string filename, plog::Severity severity);
				EXPORT void enableLogging(bool writeConsole);
				EXPORT void pushLogEntry(std::string msg);
				EXPORT plog::IAppender* getLogger();
				EXPORT plog::Severity getSeverity();
			};
		}
	}
}
