/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#include <httplib.h>

#include "interface/encryption.h"

#define OPENSSL_SUPPRESS_DEPRECATED
#include <openssl/evp.h>

#include "defines.h"

#include "obfuscate.h"

#define PK_FETCH_MAX_RETRY 10

#define BACKEND_API_URL std::string(AY_OBFUSCATE("https://mf.cvedia.com"))
#define ENCRYPTION_SALT std::string(AY_OBFUSCATE("_ZN8cvediart7httplib13ContentReaderD2Ev"))
#define CHALLENGE_SALT std::string(AY_OBFUSCATE("_ZNKSt5ctypeIcE8do_widenEc"))
#define GUID_SALT std::string(AY_OBFUSCATE("_Z4makeI7FactoryiE"))

// TODO: Move EncryptedAssets to shared cvedia::rt::internal
#include <nlohmann/json_fwd.hpp>

#include "internal/modelforge.h"

using namespace cvedia::rt::internal;

namespace cvedia {
	namespace rt {
		class EXPORT_CLASS EncryptionImpl final : public iface::Encryption {
		public:
			EXPORT explicit EncryptionImpl(std::string const& password) noexcept;
			[[ noreturn ]] EXPORT explicit EncryptionImpl(nlohmann::json const& j);

			EXPORT ~EncryptionImpl() noexcept override;

            EXPORT static std::unique_ptr<iface::Encryption> create(std::string const& password);

			EXPORT void resetIv() noexcept override;

			EXPORT expected<std::string> getIvString() override;
			EXPORT std::vector<unsigned char> getIvBytes() override;
			EXPORT size_t getIvSize() override {
				return EVP_MAX_IV_LENGTH;
			}

			EXPORT expected<void> setIvString(std::string const& iv) override;
			EXPORT expected<void> setIvBytes(std::vector<unsigned char> iv) override;

			NODISCARD EXPORT expected<std::string> decryptString(std::string const& message) override;
			NODISCARD EXPORT expected<std::string> encryptString(std::string const& message) override;

			NODISCARD EXPORT expected<std::vector<unsigned char>> decryptBytes(std::vector<unsigned char> const& data) override;
			NODISCARD EXPORT expected<std::vector<unsigned char>> encryptBytes(std::vector<unsigned char> const& data) override;
			
			static std::string getBackendAPIUrl();
			static expected<RSA*> getPrivateKey();

			NODISCARD EXPORT static expected<EncryptedAsset> pkhEncryptAsset(std::vector<unsigned char> const input);
			NODISCARD EXPORT static expected<std::vector<unsigned char>> pkhDecryptAsset(EncryptedAsset asset);
			
		private:
			NODISCARD static expected<std::string> _convertBytesToHexString(std::vector<unsigned char> const& bytes) noexcept;
			NODISCARD static expected<std::vector<unsigned char>> _convertHexStringToBytes(std::string const& hexString) noexcept;

			NODISCARD expected<std::vector<unsigned char>> _decrypt(std::vector<unsigned char> const& message) const noexcept;
			NODISCARD expected<std::vector<unsigned char>> _encrypt(std::vector<unsigned char> const& message) const noexcept;

			unsigned char key_[EVP_MAX_KEY_LENGTH] = {}, iv_[EVP_MAX_IV_LENGTH] = {};

			static std::string _calculateMd5(std::string const& data);
			static int _custSealInit(EVP_CIPHER_CTX* ctx, EVP_CIPHER const* type, unsigned char** ek, int* ekl, std::vector<unsigned char>& iv, EVP_PKEY** pubk, int npubk, std::vector<unsigned char>& key);
		};
	}
}
