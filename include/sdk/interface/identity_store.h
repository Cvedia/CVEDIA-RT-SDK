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
			class EXPORT_CLASS IdentityStore
			{
			public:
				IdentityStore() = default;
				virtual ~IdentityStore() = default;

				IdentityStore(IdentityStore const&) = delete;
				IdentityStore& operator =(IdentityStore const&) = delete;

				virtual expected<void> setField(Uuid const& id, std::string const& key, std::string const& data) = 0;
				virtual expected<std::string> getField(Uuid const& id, std::string const& key) = 0;

				virtual expected<void> deleteField(Uuid const& id, std::string const& key) = 0;
				virtual expected<void> deleteFields(Uuid const& id) = 0;

				virtual expected<void> deleteEvents(Uuid const& id) = 0;

				virtual expected<void> addEvent(Uuid const& id, std::chrono::milliseconds eventTime, std::string const& eventData) = 0;
				virtual expected<std::vector<std::pair<std::chrono::milliseconds, std::string>>> getEvents(Uuid const& id, std::chrono::milliseconds startTime, std::chrono::milliseconds endTime) = 0;
				virtual expected<std::vector<std::pair<std::chrono::milliseconds, std::string>>> getEvents(Uuid const& id) = 0;
			};
		}
	}
}
