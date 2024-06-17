/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/

#pragma once
#include "defines.h"
#include "rterror.h"
#include "uuid.h"
#include "cvaluefwd.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS IdentityProvider
			{
			public:
				IdentityProvider() = default;
				virtual ~IdentityProvider() = default;

				IdentityProvider(IdentityProvider const&) = delete;
				IdentityProvider& operator =(IdentityProvider const&) = delete;

				virtual expected<void> deleteIdentity(Uuid const& id) = 0;

				virtual expected<Uuid> insertVector(std::vector<float> const& feature) = 0;
				virtual expected<Uuid> vectorLookup(std::vector<float> const& feature) = 0;
				virtual expected<Uuid> stringLookup(std::string const& feature) = 0;
				virtual expected<void> link(Uuid const& destId, Uuid const& srcId) = 0;

				virtual expected<void> setConfigValue(std::string const& key, CValue const& val) = 0;
				virtual expected<CValue> getConfigValue(std::string const& key) = 0;
			};
		}
	}
}
