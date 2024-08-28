/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "defines.h"
#include "rterror.h"
#include <vector>

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS Encryption {
			public:
				virtual ~Encryption() = default;

				virtual void resetIv() = 0;

				virtual expected<std::string> getIvString() = 0;
				virtual std::vector<unsigned char> getIvBytes() = 0;
				virtual size_t getIvSize() = 0;

				virtual expected<void> setIvString(std::string const& iv) = 0;
				virtual expected<void> setIvBytes(std::vector<unsigned char> iv) = 0;

				NODISCARD virtual expected<std::string> decryptString(std::string const& message) = 0;
				NODISCARD virtual expected<std::string> encryptString(std::string const& message) = 0;

				NODISCARD virtual expected<std::vector<unsigned char>> decryptBytes(std::vector<unsigned char> const& data) = 0;
				NODISCARD virtual expected<std::vector<unsigned char>> encryptBytes(std::vector<unsigned char> const& data) = 0;
			};
		}
	}
}
