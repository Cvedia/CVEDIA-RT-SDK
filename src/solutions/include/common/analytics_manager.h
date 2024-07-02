/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once

#include <chrono>
#include <deque>
#include <unordered_set>
#include <builtin/basetypes.h>
#include <nlohmann/json_fwd.hpp>
#include <cvaluefwd.h>
#include "uuid.h"

namespace cvedia {
	namespace rt {
		namespace solution {
			class Event
			{
			public:
				Event(std::string const& dataType, std::string const& jsonObject) {
					this->dataType = dataType;
					this->jsonObject = jsonObject;
				}
				std::string dataType;
				std::string jsonObject;
			};

			class AnalyticsManager
			{
			public:
				AnalyticsManager();

				std::vector<std::unique_ptr<Event>> processMetadata(std::unique_ptr<Event> event);
//				std::vector<std::unique_ptr<Event>> processTracks(std::chrono::milliseconds frameTimestamp, rt::cvec const& tracks, rt::cbuffer image);

				void setDebugMode(bool enableDebug);
				void setDiagnostics(bool enable);
				void setTentativeTracks(bool enable);
			private:
				struct TrackHistory
				{
					// List of all historic track entries that have not yet been sent out to NX
					std::deque<std::unique_ptr<Event>> tracks;
					std::unique_ptr<Event> bestCrop = nullptr;
					bool isMoving;
					bool causedEvent;
					int eventCount;
					long long lastSeen;
				};

				//void _updateBestCrop(Track const& trackData, rt::cbuffer image, std::chrono::milliseconds frameTimestamp);
				void _updateDiagnosticCrop(rt::Uuid const& uuid, rt::cbuffer image, float confidence, std::string const& label);
				//std::vector<std::unique_ptr<BestCrop>> _processCrops(TrackHistory & track);
				std::vector<std::unique_ptr<Event>> _maybeDelayTrack(std::unique_ptr<Event> trackEvent);
				std::vector<std::unique_ptr<Event>> _maybeDelayBestCrop(std::unique_ptr<Event> bestCropEvent);
				std::vector<std::unique_ptr<Event>> _pruneAttribute(std::unique_ptr<Event> attributeEvent);
				void _cleanStaleTracks();

				//static std::unique_ptr<Track> _extractTrackData(rt::CValue const& track);
				//rt::Uuid _processSingleTrack(std::chrono::milliseconds frameTimestamp, rt::CValue const& track, rt::cbuffer image, std::vector<std::unique_ptr<Event>>& metadata);

//				std::vector<std::unique_ptr<BestCrop>> _closeLostTracks(std::unordered_set<rt::Uuid> const& seenUuids);

				bool enableDebug_;
				bool sendDiagnostics_;
				bool enableTentativeTracks_;

				std::map<rt::Uuid, TrackHistory> trackHistory_;
			};
		}
	}
}