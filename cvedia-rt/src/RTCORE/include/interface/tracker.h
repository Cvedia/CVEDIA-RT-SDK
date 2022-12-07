/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"
#include "builtin/moduleimpl.h"
#include <sol.hpp>

namespace cvedia {
	namespace rt {
		namespace iface {
			class Tracker : public ModuleImpl
			{
			public:
				Tracker(std::string const& moduleName) : ModuleImpl(moduleName) {}

				virtual expected<pCValue> trackObjects(pCValue boxes, sol::protected_function callback) = 0;
				virtual expected<bool> deleteTrackById(int id) = 0;
				virtual expected<std::vector<int>> getTrackIds() = 0;
				virtual expected<pCValue> getTrackById(int id) = 0;
				virtual expected<pCValue> getTrackValue(int id, std::string key) = 0;
				virtual expected<pCValue> getTracks() = 0;
				virtual expected<void> deleteTracks() = 0;
				virtual expected<bool> hasTrack(int id) = 0;
				virtual expected<bool> saveTrack(int id, pCValue track) = 0;
				virtual expected<bool> saveTrackValue(int id, std::string key, pCValue track) = 0;
				virtual expected<float> updateTrackAverageEntry(int id, std::string entry, float val, int maxBufferSize) = 0;
				virtual expected<bool> updateTracks(float timestep) = 0;

				virtual pCValue getNewTrackIds() = 0;
				virtual pCValue getMatchedTracks() = 0;
				virtual pCValue getUnmatchedTracks() = 0;
				virtual expected<int> getTrackCount() = 0;
			};
		}
	}
}
