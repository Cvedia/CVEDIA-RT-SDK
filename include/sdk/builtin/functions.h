/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#include <string>
#include <vector>
#include <ctime>
#include <chrono>

#include "defines.h"
#include "cvaluefwd.h"
#include "rterror.h"

using namespace cvedia::rt;

EXPORT void setEnvStr(std::string key, std::string value);
EXPORT std::string getEnvStr(std::string key);

EXPORT std::string toString(float const& val, int const precision);
EXPORT std::string toString(double const& val, int const precision);
EXPORT std::string toString(int const& val, int const precision);

EXPORT std::string toLowerCase(std::string const& val);

EXPORT std::vector<unsigned char> readFile(std::string const& path);
EXPORT expected<std::string> readFileAsString(std::string const& path);
EXPORT bool saveToFile(std::string const& path, std::string const& data);
EXPORT std::string get_time_string(std::string const& time_format);
EXPORT bool iequals(std::string const& a, std::string const& b);
EXPORT bool str_replace(std::string& str, std::string const& from, std::string const& to);
EXPORT std::vector<std::string> split_str(std::string const& s, char delimiter);
EXPORT bool is_number(std::string const& s);
EXPORT bool in_vector(std::vector<std::string> const& vec, std::string const& s);
EXPORT bool in_string(std::string const& full, std::string const& subString);
EXPORT bool in_vector(std::vector<int> const& vec, int const& i);
EXPORT bool file_exists(std::string const& file_path);

EXPORT std::string generate_uuid_v4();
EXPORT std::string to_string_with_precision(float const value, int const n = 6);

EXPORT std::string time_in_HH_MM_SS_MMM();
EXPORT NODISCARD expected<std::string> timePointToISOString(std::time_t const& timePoint);
EXPORT NODISCARD expected<std::time_t> isoStringToTime(std::string const& timeInIsoFormat);
EXPORT NODISCARD expected<std::string> getCurrentTimeInISOFormat(int daysToAdd = 0);
EXPORT NODISCARD expected<std::time_t> getCurrentTimeInUtc();
EXPORT NODISCARD std::chrono::milliseconds getCurrentTimeInUtcMs();

EXPORT std::string getEnvVar(std::string const& varName);
EXPORT bool setEnvVar(std::string const& name, std::string const& value);

EXPORT void safeStrcpy(char* dest, char const* src, size_t const destSize);
EXPORT bool isValidFilename(std::string const& filename);

EXPORT std::string toUtf8(std::wstring const& wstr);
EXPORT std::wstring fromUtf8(std::string const& str);

EXPORT std::string urlEncode(std::string const& url);
EXPORT std::string urlDecode(std::string const& url);
EXPORT bool isValidIPAddress(const std::string& ipAddress);
EXPORT bool isValidPort(const std::string& port);
EXPORT bool isValidNonNegativeInteger(const std::string& fileSize);

EXPORT std::string getCurrentDirectory();