/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#include "common/analytics_manager.h"

#include <algorithm>
#include <future>
#include "nlohmann/json.hpp"

#include <interface/buffer.h>
#include "api/logging.h"

#include "cvalue.h"

using namespace cvedia::rt;
using namespace solution;

AnalyticsManager::AnalyticsManager() {
	sendDiagnostics_ = false;
	enableDebug_ = false;
	enableTentativeTracks_ = true;
}

void AnalyticsManager::setDebugMode(bool enableDebug) {
	enableDebug_ = enableDebug;
}

void AnalyticsManager::setDiagnostics(bool enable) {
	sendDiagnostics_ = enable;
}

void AnalyticsManager::setTentativeTracks(bool enable) {
	enableTentativeTracks_ = enable;
}

std::vector<std::unique_ptr<Event>> AnalyticsManager::processMetadata(std::unique_ptr<Event> event) {

	_cleanStaleTracks();

	std::vector<std::unique_ptr<Event>> metadataVec;

	if (event->dataType == "track")
	{
		return _maybeDelayTrack(std::move(event));
	}
	else if (event->dataType == "best-crop")
	{
		return _maybeDelayBestCrop(std::move(event));
	}
	else if (event->dataType == "attribute")
	{
		return _pruneAttribute(std::move(event));
	}

	metadataVec.push_back(std::move(event));
	return metadataVec;
}

std::vector<std::unique_ptr<Event>> AnalyticsManager::_pruneAttribute(std::unique_ptr<Event> attributeEvent)
{
	auto attribute = nlohmann::json::parse(attributeEvent->jsonObject, nullptr, false, false);
	auto const trackId = attribute["ref_tracking_id"].get<std::string>();
	Uuid const uuid(trackId);

	// Only send the attribute if its track is known and caused any events
	if (trackHistory_.find(uuid) != trackHistory_.end() && trackHistory_[uuid].causedEvent)
	{
		std::vector<std::unique_ptr<Event>> metadata;
		metadata.emplace_back(std::move(attributeEvent));
		return metadata;
	}

	auto& histEntry = trackHistory_[uuid];
	histEntry.lastSeen = TIME_IN_S;

	return {};
}

std::vector<std::unique_ptr<Event>> AnalyticsManager::_maybeDelayBestCrop(std::unique_ptr<Event> bestCropEvent) {
	auto bestCrop = nlohmann::json::parse(bestCropEvent->jsonObject, nullptr, false, false);
	auto const trackId = bestCrop["ref_tracking_id"].get<std::string>();
	Uuid const uuid(trackId);

	// if track is already known, just send out the crop
	if (trackHistory_.find(uuid) != trackHistory_.end())
	{
		std::vector<std::unique_ptr<Event>> metadata;
		metadata.emplace_back(std::move(bestCropEvent));

		return metadata;
	}

	auto& histEntry = trackHistory_[uuid];
	histEntry.lastSeen = TIME_IN_S;
	histEntry.bestCrop = std::move(bestCropEvent);

	return {};
}

std::vector<std::unique_ptr<Event>> AnalyticsManager::_maybeDelayTrack(std::unique_ptr<Event> trackEvent) {
	std::vector<std::unique_ptr<Event>> metadata;

	auto track = nlohmann::json::parse(trackEvent->jsonObject, nullptr, false, false);

	auto const trackId = track["tracking_id"].get<std::string>();
	Uuid const uuid(trackId);

	auto& histEntry = trackHistory_[uuid];

	if (track["events"].size() > 0)
	{
		histEntry.causedEvent = true;
	}

	if (!enableTentativeTracks_) {
		histEntry.tracks.clear();
	}

	histEntry.tracks.push_back(std::move(trackEvent));

	if (histEntry.causedEvent || enableDebug_)
	{
		std::move(histEntry.tracks.begin(), histEntry.tracks.end(), std::back_inserter(metadata));
		histEntry.tracks.clear();

		// in case we have a best crop event waiting, send it out
		if (histEntry.bestCrop != nullptr)
		{
			metadata.push_back(std::move(histEntry.bestCrop));
			histEntry.bestCrop = nullptr;
		}
	}

	histEntry.lastSeen = TIME_IN_S;

	return metadata;
}

void AnalyticsManager::_cleanStaleTracks() {
	auto const now = TIME_IN_S;
	for (auto it = trackHistory_.begin(); it != trackHistory_.end();)
	{
		if (now - it->second.lastSeen > 15)
		{
			it = trackHistory_.erase(it);
		}
		else
		{
			++it;
		}
	}
}

//Uuid AnalyticsManager::_processSingleTrack(std::chrono::milliseconds const frameTimestamp, CValue const& track, cbuffer const image, std::vector<std::unique_ptr<Event>>& metadata) {
//	auto trackData = _extractTrackData(track);
//
//	trackData->timestamp = frameTimestamp;
//
//	auto const& uuid = trackData->uuid;
//	auto &histEntry = trackHistory_[uuid];
//	histEntry.uuid = uuid;
//
//	if (auto const t = track.getChildIfSet("events").value_or(nullptr))
//	{
//		auto const cv = t->getValueOr<cvec>({});
//		histEntry.eventCount += static_cast<int>(cv.size());
//	}
//
//	trackData->hasCausedEvent = histEntry.causedEvent;
//
//	histEntry.isMoving = trackData->isMoving;
//
//	if (trackData->label != "Unknown")
//	{
//		if (sendDiagnostics_)
//		{
//			if (auto const mosaic = track.getValue<cbuffer>("atlas_crop").value_or(nullptr))
//			{
//				_updateDiagnosticCrop(uuid, mosaic, trackData->confidence, trackData->label);
//			}
//		}
//	}
//
//	histEntry.tracks.push_back(std::move(trackData));
//
//	if (histEntry.causedEvent || enableDebug_)
//	{
//		std::move(histEntry.tracks.begin(), histEntry.tracks.end(), std::back_inserter(metadata));
//		histEntry.tracks.clear();
//	}
//
//	return uuid;
//}

void AnalyticsManager::_updateDiagnosticCrop(Uuid const& /*uuid*/, cbuffer const /*image*/, float const /*confidence*/, std::string const& /*label*/) {
	//	auto& track = trackHistory_[uuid];
		//auto& cropObj = track.crops;

		//cropObj.label = label;

		//if (confidence < 0.01f)
		//	return;

		//if (cropObj.diagnosticConfidence - BEST_CROP_UPDATE_DELTA > confidence || cropObj.diagnosticCrop == nullptr)
		//{
		//	cropObj.diagnosticConfidence = confidence;
		//	cropObj.diagnosticCrop = image;
		//	cropObj.hasRecovered = false;
		//}

		//if (cropObj.diagnosticConfidence < 0.01f && confidence > 0.0f)
		//{
		//	cropObj.hasRecovered = true;
		//}
}

/**
 * @brief Cleans up track metadata for lost (no longer seen) tracks.
 *
 * This function iterates over all currently tracked objects and removes
 * those that are not present in the given set of seen UUIDs. This indicates
 * the objects are no longer present in the scene or the tracking was lost.
 *
 * For each removed object, if it was moving but had no associated events,
 * a diagnostic message is printed with the number of frames and events associated
 * with the track.
 *
 * @param seenUuids A set of UUIDs for objects currently seen in the scene.
 */
 //std::vector<std::unique_ptr<BestCrop>> AnalyticsManager::_closeLostTracks(std::unordered_set<Uuid> const& seenUuids) {
 //	std::vector<std::unique_ptr<BestCrop>> metadataVec;
 //
 //	// check if there's any tracks that were not seen and delete their data
 //	for (auto it = trackHistory_.begin(); it != trackHistory_.end();)
 //	{
 //		auto const& uuid = it->first;
 //		if (seenUuids.find(uuid) == seenUuids.end())
 //		{
 //			auto & track = it->second;
 //
 //			if (track.isMoving && !track.causedEvent)
 //			{
 ////				NX_PRINT << "Object " << uuid << " was moving but had no events (" << track.tracks.size() << ") and " << track.eventCount << " events on track";
 //			}
 //
 //			auto cropsMeta = _processCrops(track);
 //			std::move(cropsMeta.begin(), cropsMeta.end(), std::back_inserter(metadataVec));
 //
 //			it = trackHistory_.erase(it);
 //		}
 //		else
 //		{
 //			++it;
 //		}
 //	}
 //
 //	return metadataVec;
 //}
 //
 //std::vector<std::unique_ptr<BestCrop>> AnalyticsManager::_processCrops(TrackHistory & track) {
 //	std::vector<std::unique_ptr<BestCrop>> bestCrops;
 //
 //	auto & crops = track.crops;
 //
 //	if (crops.diagnosticConfidence < 0.4 && sendDiagnostics_ && TIME_IN_S - lastDiagnostics_ > 15 && crops.diagnosticCrop != nullptr)
 //	{
 //		// Only allow images without detections if the track has recovered.
 //		// This prevents shots of objects that left the scene.
 //		if (!(crops.diagnosticConfidence < 0.01f && !crops.hasRecovered))
 //		{
 //			// lowercase the label
 //			std::string label = crops.label;
 //			std::transform(label.begin(), label.end(), label.begin(), ::tolower);
 //
 //			cbuffer worstCrop = crops.diagnosticCrop;
 //			float worstConfidence = crops.diagnosticConfidence * 100;
 //
 //			// asynchronously call _uploadImage
 //			std::thread([this, worstCrop, label, worstConfidence]()
 //				{
 //					Cloudinary::uploadImage(worstCrop, "label=" + label + "|confidence=" + std::to_string(static_cast<int>(worstConfidence)) + "|instance=" + instanceName_, instanceName_);
 //				}).detach();
 //
 //				lastDiagnostics_ = TIME_IN_S;
 //		}
 //	}
 //
 //	// and it caused an event
 //	if (track.causedEvent && crops.bestCrop != nullptr)
 //	{
 //		crops.uuid = track.uuid;
 //		bestCrops.emplace_back(std::make_unique<BestCrop>(crops));
 //	}
 //
 //	return bestCrops;
 //}
