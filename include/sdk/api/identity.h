/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#include <chrono>

#include "defines.h"
#include "rterror.h"
#include "uuid.h"

namespace cvedia {
	namespace rt {
		namespace api {
			namespace identity {
				namespace provider {
					// Identity system
					EXPORT expected<Uuid> lookup(std::string const& name, std::vector<float> const& feature);
					EXPORT expected<Uuid> lookup(std::string const& name, std::string const& feature);

					EXPORT expected<void> deleteIdentity(std::string const& name, Uuid const& id);
					EXPORT expected<void> link(std::string const& name, Uuid const& destId, Uuid const& srcId);

					EXPORT expected<void> setConfigValue(std::string const& name, std::string const& key, std::string const& value);
					EXPORT expected<void> setConfigValue(std::string const& name, std::string const& key, float const& value);
					EXPORT expected<void> setConfigValue(std::string const& name, std::string const& key, int const& value);
					EXPORT expected<std::string> getConfigValue(std::string const& name, std::string const& key);
				}

				namespace store
				{
					EXPORT expected<void> addEvent(Uuid const& id, std::chrono::milliseconds eventTime, std::string const& eventData);
					EXPORT expected<void> deleteEvents(Uuid const& id);

					EXPORT expected<std::vector<std::pair<std::chrono::milliseconds, std::string>>> getEvents(Uuid const& id, std::chrono::milliseconds startTime, std::chrono::milliseconds endTime);
					EXPORT expected<std::vector<std::pair<std::chrono::milliseconds, std::string>>> getEvents(Uuid const& id);

					EXPORT expected<void> setField(Uuid const& id, std::string const& key, std::string const& data);
					EXPORT expected<std::string> getField(Uuid const& id, std::string const& key);
					EXPORT expected<void> deleteField(Uuid const& id, std::string const& key);
					EXPORT expected<void> deleteFields(Uuid const& id);
				}

				EXPORT expected<void> loadVectorsFromDisk(std::string const& name, std::string const& path, std::string const& model);
				EXPORT expected<void> loadStringsFromDisk(std::string const& name, std::string const& path);
			};
		}
	}
}
