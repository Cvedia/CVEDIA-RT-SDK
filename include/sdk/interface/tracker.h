/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "defines.h"
#include "builtin/moduleimpl.h"
#include <sol/forward.hpp>

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS Tracker : public ModuleImpl
			{
			public:
				Tracker(std::string const& moduleName) : ModuleImpl(moduleName) {}

				virtual expected<cvec> trackObjects(cbuffer const& buffer, cvec const& boxes, sol::protected_function callback) = 0;
				virtual expected<void> updateTracksMovement(cbuffer const& buffer) = 0;
				virtual expected<bool> deleteTrackById(int id) = 0;
				virtual expected<std::vector<int>> getTrackIds(int stateFilter=0) = 0;
				virtual expected<pCValue> getTrackById(int id) = 0;
				virtual expected<pCValue> getTrackValue(int id, std::string const& key) = 0;
				virtual expected<pCValue> getTracks() = 0;
				virtual expected<void> deleteTracks() = 0;
				virtual expected<void> markTrackAsDead(int id) = 0;
				virtual expected<int> clearDeadTracks() = 0;
				virtual expected<bool> hasTrack(int id) = 0;
				virtual expected<bool> saveTrack(int id, CValue const& track) = 0;
				virtual expected<bool> saveTrackValue(int id, std::string const& key, CValue const& track) = 0;
				virtual expected<float> updateTrackAverageEntry(int id, std::string const& entry, float val, int maxBufferSize) = 0;
				virtual expected<bool> updateTracks(float timestep) = 0;

				virtual expected<void> reset() = 0;

				virtual pCValue getNewTrackIds() = 0;
				virtual pCValue getMatchedTracks() = 0;
				virtual pCValue getUnmatchedTracks() = 0;
				virtual expected<int> getTrackCount() = 0;
			};
		}
	}
}
