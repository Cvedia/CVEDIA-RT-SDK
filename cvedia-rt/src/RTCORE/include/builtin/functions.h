/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <string>
#include <vector>
#include <filesystem.hpp>

namespace fs = ghc::filesystem;

inline std::vector<unsigned char> readFile(std::string const& path) {
	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return {};

	// Read contents
	std::size_t const& size = fs::file_size(path);
	std::vector<unsigned char> content(size);
	file.read(reinterpret_cast<char*>(content.data()), static_cast<int>(size));

	return content;
}