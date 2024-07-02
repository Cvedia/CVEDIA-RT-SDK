/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#include "securt/securt.h"

#include "common/analytics_manager.h"
#include "interface/instance.h"
#include "api/logging.h"

#include "cvalue.h"
#include "api/thread.h"

#include "builtin/objectregistry.h"
#include "interface/cvdict.h"
#include "interface/instance_controller.h"
#include "interface/motiondetection.h"
#include "interface/input.h"

#include <nlohmann/json.hpp>

#include "api/rt.h"

using namespace cvedia::rt;
using namespace solution;

RemoteDiscovery SecuRT::remoteDiscovery_;

class SecuRTPrivate {
public:
	static SecuRT::Sensitivity _strToSensitivity(std::string const& mode) {
		if (mode == "Low")
			return SecuRT::Low;
		else if (mode == "Medium")
			return SecuRT::Medium;
		else if (mode == "High")
			return SecuRT::High;
		else
			return SecuRT::Medium;
	}

	static std::string _sensitivityToStr(SecuRT::Sensitivity mode) {
		switch (mode)
		{
		case SecuRT::Low:
			return "Low";
		case SecuRT::Medium:
			return "Medium";
		case SecuRT::High:
			return "High";
		default:
			return "Medium";
		}
	}

	static SecuRT::Mode _strToDetectorMode(std::string const& mode) {
		if (mode == "MosaicInference")
		{
			return SecuRT::Mode::SmartDetection;
		}
		else
		{
			return SecuRT::Mode::Detection;
		}
	}

	static SecuRT::Modality _strToModality(std::string const& modality) {
		if (modality == "rgb")
		{
			return SecuRT::Modality::RGB;
		}
		else
		{
			return SecuRT::Modality::Thermal;
		}
	}

	static std::string _modalityToString(SecuRT::Modality modality) {
		if (modality == SecuRT::Modality::RGB)
		{
			return "rgb";
		}
		else
		{
			return "thermal";
		}
	}

	static std::array<double, 4> _getColor(cmap& cm) {
		std::array<double, 4> color = {0, 0, 0, 1};

		if (cm.count("color") == 1)
		{
			auto cv = cm["color"]->getValueOr<cvec>(cvec{});
			if (cv.size() == 4)
			{
				color[0] = cv[0]->getValueOr<double>(0);
				color[1] = cv[1]->getValueOr<double>(0);
				color[2] = cv[2]->getValueOr<double>(0);
				color[3] = cv[3]->getValueOr<double>(1);
			}
		}

		return color;
	}

	static std::unordered_set<std::string> _getClasses(cmap& cm) {
		std::unordered_set<std::string> classes = {};

		if (cm.count("detect_people") == 1 && cm["detect_people"]->getValueOr<bool>(false))
		{
			classes.insert("Person");
		}
		if (cm.count("detect_animals") == 1 && cm["detect_animals"]->getValueOr<bool>(false))
		{
			classes.insert("Animal");
		}
		if (cm.count("detect_vehicles") == 1 && cm["detect_vehicles"]->getValueOr<bool>(false))
		{
			classes.insert("Vehicle");
		}
		if (cm.count("detect_unknowns") == 1 && cm["detect_unknowns"]->getValueOr<bool>(false))
		{
			classes.insert("Unknown");
		}
		if (cm.count("detect_faces") == 1 && cm["detect_faces"]->getValueOr<bool>(false))
		{
			classes.insert("Face");
		}

		return classes;
	}

	static std::vector<std::map<std::string, double>> _getVertices(cmap& cm) {
		std::vector<std::map<std::string, double>> vertices = {};

		if (cm.count("vertices") == 1)
		{
			auto cv = cm["vertices"]->getValueOr<cvec>(cvec{});
			for (auto const& v : cv)
			{
				auto vm = v->getValueOr<cmap>(cmap{});
				if (vm.empty())
				{
					continue;
				}

				if (vm.count("x") == 1 && vm.count("y") == 1)
				{
					auto x = vm["x"]->getValue<double>();
					auto y = vm["y"]->getValue<double>();

					if (x.has_value() && y.has_value())
					{
						std::map<std::string, double> coord;
						coord["x"] = x.value();
						coord["y"] = y.value();
						vertices.push_back(coord);
					}
				}
			}
		}

		return vertices;
	}

	static std::string _getAnalyticsType(cmap& cm) {
		if (cm.count("trigger_on_intrusion") == 1 && cm["trigger_on_intrusion"]->getValueOr<bool>(false))
		{
			return "AreaIntrusion";
		}

		if (cm.count("trigger_loitering") == 1 && cm["trigger_loitering"]->getValueOr<bool>(false))
		{
			return "AreaLoitering";
		}

		if (cm.count("trigger_crowding") == 1 && cm["trigger_crowding"]->getValueOr<bool>(false))
		{
			return "AreaCrowding";
		}

		if (cm.count("trigger_crossing") == 1 && cm["trigger_crossing"]->getValueOr<bool>(false))
		{
			return "LineCrossing";
		}

		return "AreaCrossing";
	}

	static void _setClasses(cmap& cm, std::unordered_set<SecuRT::Classes> const& classes) {
		cm["detect_people"] = VAL(classes.find(SecuRT::Person) != classes.end());
		cm["detect_animals"] = VAL(classes.find(SecuRT::Animal) != classes.end());
		cm["detect_vehicles"] = VAL(classes.find(SecuRT::Vehicle) != classes.end());
		cm["detect_unknowns"] = VAL(classes.find(SecuRT::Unknown) != classes.end());
		cm["detect_faces"] = VAL(classes.find(SecuRT::Face) != classes.end());
	}

	static void _setColor(cmap& cm, std::array<double, 4> const& color) {
		cvec colorVec{VAL(color[0]), VAL(color[1]), VAL(color[2]), VAL(color[3])};
		cm["color"] = VAL(colorVec);
	}

	static cmap _generateBasicAnalyticsSettings(std::string const& name, std::string const& id) {
		return {{"name", VAL(name)}, {"id", VAL(id)}, {"timestamp", VAL(static_cast<double>(TIME_IN_S))}};
	}

	static cvec _generateVertices(std::vector<Point2f> const& area) {
		cvec vertices;
		for (auto const& point : area)
		{
			cmap vertex = {{"x", VAL(point.x)}, {"y", VAL(point.y)}};
			vertices.push_back(VAL(vertex));
		}
		 return vertices;
	}
	
	static cmap _generateZone(std::string const& id, std::string const& name, std::vector<Point2f> const& area,
	                          std::unordered_set<SecuRT::Classes> const& classes, std::array<double, 4> const& color)
	{
		cmap zone = SecuRTPrivate::_generateBasicAnalyticsSettings(name, id);
		SecuRTPrivate::_setColor(zone, color);

		if (!classes.empty())
		{
			SecuRTPrivate::_setClasses(zone, classes);
		}

		zone["check_anchor_point"] = VAL("center");
		zone["vertices"] = VAL(SecuRTPrivate::_generateVertices(area));

		return zone;
	}

	static cmap _generateLine(std::string const& id, std::string const& name, std::vector<Point2f> const& linePoints,
							  std::unordered_set<SecuRT::Classes> const& classes, SecuRT::Direction const dir, std::array<double, 4> const& color)
	{
		cmap line = SecuRTPrivate::_generateBasicAnalyticsSettings(name, id);
		SecuRTPrivate::_setColor(line, color);

		if (!classes.empty())
		{
			SecuRTPrivate::_setClasses(line, classes);
		}

		line["check_anchor_point"] = VAL("bottom_center");
		if (dir == SecuRT::Up)
		{
			line["direction"] = VAL("Up");
		}
		else if (dir == SecuRT::Down)
		{
			line["direction"] = VAL("Down");
		}
		else if (dir == SecuRT::Both)
		{
			line["direction"] = VAL("Both");
		}

		line["vertices"] = VAL(SecuRTPrivate::_generateVertices(linePoints));

		return line;
	}

	static nlohmann::json _extractAnalyticsEntity(cmap cm) {
		nlohmann::json obj;

		if (cm.count("id") == 1)
		{
			obj["id"] = cm["id"]->getValueOr<std::string>("-1");
		}

		if (cm.count("name") == 1)
		{
			obj["name"] = cm["name"]->getValueOr<std::string>("");
		}

		obj["coordinates"] = SecuRTPrivate::_getVertices(cm);
		obj["classes"] = SecuRTPrivate::_getClasses(cm);
		obj["color"] = SecuRTPrivate::_getColor(cm);
		obj["type"] = SecuRTPrivate::_getAnalyticsType(cm);
		
		if (obj["type"] == "AreaCrossing") {
			bool isEnter = cm.count("trigger_on_enter") && cm["trigger_on_enter"]->getValueOr<bool>(false);
			bool isExit = cm.count("trigger_on_exit") && cm["trigger_on_exit"]->getValueOr<bool>(false);

			if (isEnter && isExit) {
				obj["areaEvent"] = "Both";
			}
			else if (isEnter) {
				obj["areaEvent"] = "Enter";
			}
			else if (isExit) {
				obj["areaEvent"] = "Exit";
			}

			if (cm.count("ignore_stationary_objects")) 
			{
				obj["ignoreStationaryObjects"] = cm["ignore_stationary_objects"]->getValueOr<bool>(false);
			}
		}

		if (obj["type"] == "AreaLoitering") {
			if (cm.count("loitering_min_duration")) 
			{
				obj["seconds"] = cm["loitering_min_duration"]->getValueOr<int>(-1);
			}
		}

		if (obj["type"] == "AreaCrowding") {
			if (cm.count("crowding_min_count")) 
			{
				obj["objectCount"] = cm["crowding_min_count"]->getValueOr<int>(-1);
			}
		}

		if (obj["type"] == "LineCrossing") {
			if (cm.count("direction")) 
			{
				obj["direction"] = cm["direction"]->getValueOr<std::string>("");
			}
		}

		return obj;
	}
};


SecuRT::SecuRT() : SolutionManager()
{
	LOGD << "Creating SecuRT instance ";
}

expected<void> SecuRT::initialize()
{
	SolutionManager::initialize();

	setRecommendedFrameRate(10);
	setDetectorMode(SmartDetection);
	setSensorModality(RGB);
	setDetectionSensitivity(Medium);
	setMovementSensitivity(Medium);
	_setThumbnailCreation();
	setupCallbacks();

	return {};
}

void SecuRT::statisticsThread()
{
	long long int lastStats = 0;

	while (isStatisticsRunning_ && api::isRunning())
	{
		if (TIME_IN_MS - lastStats > 5000)
		{
			lastStats = TIME_IN_MS;

			nlohmann::json stats;

			stats["frames_processed"] = std::to_string(getFramesProcessed());
			stats["current_framerate"] = std::to_string(getCurrentFrameRate());
			stats["latency"] = std::to_string(getLatency());
			stats["start_time"] = std::to_string(getStartTime());
			stats["input_queue_size"] = std::to_string(getInputQueueSize());
			stats["dropped_frames_count"] = std::to_string(getDroppedFramesCount());

			auto event = std::make_unique<Event>("statistics", stats.dump());

			std::unique_lock<std::mutex> mdLock(eventsQueueMutex_);
			eventsQueue_.push_back(std::move(event));
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void SecuRT::setupCallbacks()
{
	if (!callbacksInitialized_)
	{
		if (auto const instance = getInstance())
		{
			auto bind = std::bind(&SecuRT::_eventsCallback, this, std::placeholders::_1, std::placeholders::_2,
				std::placeholders::_3);

			instance->bindEvent("events", "event-intrusion:1", bind, 0);
			instance->bindEvent("events", "event-intrusion-end:1", bind, 0);
			instance->bindEvent("events", "event-line-crossing:1", bind, 0);
			instance->bindEvent("events", "event-loitering:1", bind, 0);
			instance->bindEvent("events", "event-loitering-end:1", bind, 0);
			instance->bindEvent("events", "event-crowd-detection:1", bind, 0);
			instance->bindEvent("events", "event-area-enter:1", bind, 0);
			instance->bindEvent("events", "event-area-exit:1", bind, 0);
			instance->bindEvent("events", "event-armed-person-enter:1", bind, 0);
			instance->bindEvent("events", "event-armed-person-exit:1", bind, 0);
			instance->bindEvent("events", "track:1", bind, 0);
			instance->bindEvent("events", "best-crop:1", bind, 0);
			instance->bindEvent("events", "attribute:1", bind, 0);
			instance->bindEvent("events", "diagnostics:1", bind, 0);

			callbacksInitialized_ = true;
		}
	}	
}

void SecuRT::_setThumbnailCreation()
{
	std::vector<std::pair<std::string, CValue>> const thumbnailSettings = {
		{"/PersonTracker/enable_thumbnail_creation", CValue(true)},
		{"/VehicleTracker/enable_thumbnail_creation", CValue(true)},
		{"/AnimalTracker/enable_thumbnail_creation", CValue(true)},
		{"/MotionTracker/enable_thumbnail_creation", CValue(false)}
	};

	if (!setConfigValues(thumbnailSettings))
	{
		LOGE << "Unable to set thumbnail creation for instance " << getInstanceUuid().toString();
	}
}

void SecuRT::_eventsCallback(pCValue data, std::string eventType, float)
{
	std::string const eventData = data->getValueOr<std::string>({});

	// strip the version from the event type
	eventType = eventType.substr(0, eventType.find(':'));

	auto eventMetadataPacket = analytics_->processMetadata(std::make_unique<Event>(eventType, eventData));

	std::unique_lock<std::mutex> mdLock(eventsQueueMutex_);
	std::move(eventMetadataPacket.begin(), eventMetadataPacket.end(), std::back_inserter(eventsQueue_));

	while (eventsQueue_.size() > 500)
	{
		eventsQueue_.pop_front();
	}
}

std::string SecuRT::getAnalyticsEntities(AnalyticsCategory category) const
{
	std::string const areasPath = "/Zone/Zones/";
	std::string const linesPath = "/Tripwire/Tripwires";

	nlohmann::json obj;

	if (category == AnalyticsCategory::All || category == AnalyticsCategory::Area) {
		obj["crossingAreas"] = std::vector<std::string>();
		obj["intrusionAreas"] = std::vector<std::string>();
		obj["loiteringAreas"] = std::vector<std::string>();
		obj["crowdingAreas"] = std::vector<std::string>();

		if (auto const areaConfig = getConfigValue(areasPath))
		{
			for (auto const& area: areaConfig->getValueOr<cmap>(cmap())) {
				auto const areaMap = area.second->getValueOr<cmap>(cmap());
				auto extractedArea = SecuRTPrivate::_extractAnalyticsEntity(areaMap);
				auto const& type = extractedArea["type"].get<std::string>();

				if (type == "AreaCrossing") {
					obj["crossingAreas"].push_back(extractedArea);
				}
				if (type == "AreaIntrusion") {
					obj["intrusionAreas"].push_back(extractedArea);
				}
				if (type == "AreaLoitering") {
					obj["loiteringAreas"].push_back(extractedArea);
				}
				if (type == "AreaCrowding") {
					obj["crowdingAreas"].push_back(extractedArea);
				}
			}
		}
	}

	if (category == AnalyticsCategory::All || category == AnalyticsCategory::Line) {
		obj["crossingLines"] = std::vector<std::string>();

		if (auto const lineConfig = getConfigValue(linesPath))
		{
			for (auto const& line: lineConfig->getValueOr<cmap>(cmap())) {
				auto const lineMap = line.second->getValueOr<cmap>(cmap());
				auto extractedLine = SecuRTPrivate::_extractAnalyticsEntity(lineMap);
				auto const& type = extractedLine["type"].get<std::string>();

				if (type == "LineCrossing") {
					obj["crossingLines"].push_back(extractedLine);
				}
			}
		}
	}

	return obj.dump();
}

std::unordered_set<std::string> SecuRT::getAreaIds() const
{
	if (auto const zonesResp = getConfigValue("/Zone/Zones"))
	{
		auto const zones = zonesResp->getValueOr<cmap>({});

		std::unordered_set<std::string> ids;

		for (auto const& zone : zones)
		{
			ids.insert(zone.first);
		}

		return ids;
	}

	return {};
}

std::unordered_set<std::string> SecuRT::getLineIds() const
{
	if (auto const linesResp = getConfigValue("/Tripwire/Tripwires"))
	{
		auto const lines = linesResp->getValueOr<cmap>({});

		std::unordered_set<std::string> ids;

		for (auto const& line : lines)
		{
			ids.insert(line.first);
		}

		return ids;
	}

	return {};
}

bool SecuRT::_hasZone(std::string const& id) const
{
	return getConfigValue("/Zone/Zones/" + id) != nullptr;
}

bool SecuRT::_hasLine(std::string const& id) const
{
	return getConfigValue("/Tripwire/Tripwires/" + id) != nullptr;
}

bool SecuRT::_createZone(std::string const& id, cmap const& zone) const
{
	if (_hasZone(id))
	{
		return false;
	}

	return setConfigValue("/Zone/Zones/" + id, CValue(zone)) && setStateValue("recalculate_zones", CValue(true));
}

/**
 * @brief Creates an intrusion area with a specific ID, name, area points, classes and color.
 *
 * This function is used to set up an intrusion area in the security system. It takes as input the area ID,
 * name, points that define the area, classes of objects to be detected and color.
 *
 * @param id A unique identifier for the intrusion area.
 * @param name The human-readable name of the intrusion area.
 * @param area A vector of 2D points that define the area. Each point represents a vertex of the area.
 * @param classes An unordered set of Classes which are to be detected in the area.
 * @param color The array of RGBA values representing the color for the area for visualization purposes
 *
 * @return Returns true if the intrusion area is successfully created. It will return false if there are any failures in setting the nodes for the configuration or state.
 */
bool SecuRT::createIntrusionArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area,
                                 std::unordered_set<Classes> const& classes, std::array<double, 4> const& color) const
{
	if (area.empty())
	{
		return false;
	}

	if (classes.empty())
	{
		return false;
	}

	auto zone = SecuRTPrivate::_generateZone(id, name, area, classes, color);
	zone["groupby"] = VAL("intrusion_areas");
	zone["trigger_on_intrusion"] = VAL(true);
	zone["ignore_stationary_objects"] = VAL(true);

	return _createZone(id, zone);
}

/**
 * @brief Creates a loitering area with a specific ID, name, area points, classes, minimum loitering duration and color.
 *
 * This function is used to set up a loitering area in the security system. It takes as input the area ID,
 * name, points that define the area, classes of objects to be detected, the minimum duration of loitering in seconds and the color.
 *
 * @param id A unique identifier for the loitering area.
 * @param name The human-readable name of the loitering area.
 * @param area A vector of 2D points that define the area. Each point represents a vertex of the area.
 * @param classes An unordered set of Classes which are to be detected in the area.
 * @param seconds The minimum duration of loitering for the event to be triggered, measured in seconds.
 * @param color The array of RGBA values representing the color for the area for visualization purposes
 *
 * @return Returns true if the loitering area is successfully created. It will return false if there are any failures in setting the nodes for the configuration or state.
 */
bool SecuRT::createLoiteringArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area,
                                 std::unordered_set<Classes> const& classes, int seconds, std::array<double, 4> const& color) const
{
	if (area.empty())
	{
		return false;
	}

	if (classes.empty())
	{
		return false;
	}

	auto zone = SecuRTPrivate::_generateZone(id, name, area, classes, color);
	zone["groupby"] = VAL("loitering_areas");
	zone["trigger_loitering"] = VAL(true);
	zone["loitering_min_duration"] = VAL(seconds);

	return _createZone(id, zone);
}

/**
 * @brief Creates a crowding area with a specific ID, name, area points, classes, minimum object count and color.
 *
 * This function is used to set up a crowding area in the security system. It takes as input the area ID,
 * name, points that define the area, classes of objects to be detected, the minimum number of objects for crowding and the color.
 *
 * @param id A unique identifier for the crowding area.
 * @param name The human-readable name of the crowding area.
 * @param area A vector of 2D points that define the area. Each point represents a vertex of the area.
 * @param classes An unordered set of Classes which are to be detected in the area.
 * @param objectCnt The minimum number of objects for crowding to be triggered.
 * @param color The array of RGBA values representing the color for the area for visualization purposes
 *
 * @return Returns true if the crowding area is successfully created. It will return false if there are any failures in setting the nodes for the configuration or state.
 */
bool SecuRT::createCrowdingArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area,
                                std::unordered_set<Classes> const& classes, int objectCnt, std::array<double, 4> const& color) const
{
	if (area.empty())
	{
		return false;
	}

	if (classes.empty())
	{
		return false;
	}

	auto zone = SecuRTPrivate::_generateZone(id, name, area, classes, color);
	zone["groupby"] = VAL("loitering_areas");
	zone["trigger_crowding"] = VAL(true);
	zone["crowding_min_count"] = VAL(objectCnt);

	return _createZone(id, zone);
}

/**
 * @brief Creates a crossing area with a specific ID, name, area points, classes, stationary object detection, 
 * type of area event and color for visualization purposes.
 *
 * This function is used to set up a crossing area in the security system. It takes as input the area ID,
 * name, points that define the area, classes of objects to be detected, a flag for stationary object detection, 
 * the type of area event and the color for visualization purposes.
 *
 * @param id A unique identifier for the detection area.
 * @param name The human-readable name of the detection area.
 * @param area A vector of 2D points that define the area. Each point represents a vertex of the area.
 * @param classes An unordered set of Classes which are to be detected in the area.
 * @param ignoreStationaryObjects A boolean flag to ignore stationary objects in the area.
 * @param type The AreaEvent enum which specifies the type of area event to be detected (Enter, Exit, or Both).
 * @param color The array of RGBA values representing the color for the area for visualization purposes
 *
 * @return Returns true if the detection area is successfully created. It will return false if there are any failures in setting the nodes for the configuration or state.
 */
bool SecuRT::createCrossingArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area,
                        		std::unordered_set<Classes> const& classes, bool ignoreStationaryObjects, 
								AreaEvent const type, std::array<double, 4> const& color) const
{
	if (area.empty())
	{
		return false;
	}

	if (classes.empty())
	{
		return false;
	}

	auto zone = SecuRTPrivate::_generateZone(id, name, area, classes, color);
	zone["groupby"] = VAL("generic_area");
	zone["ignore_stationary_objects"] = VAL(ignoreStationaryObjects);

	bool const enter = type == Enter || type == EnterExit;
	bool const exit = type == Exit || type == EnterExit;

	zone["trigger_on_enter"] = VAL(enter);
	zone["trigger_on_exit"] = VAL(exit);

	return _createZone(id, zone);
}

/**
 * @brief Creates an armed person area with a specific ID, name, area points and color.
 *
 * This function is used to set up an armed person area in the security system. It takes as input the area ID,
 * name, points that define the area and the color.
 *
 * @param id A unique identifier for the detection area.
 * @param name The human-readable name of the detection area.
 * @param area A vector of 2D points that define the area. Each point represents a vertex of the area.
 * @param color The array of RGBA values representing the color for the area for visualization purposes
 *
 * @return Returns true if the detection area is successfully created. It will return false if there are any failures in setting the nodes for the configuration or state.
 */
bool SecuRT::createArmedPersonArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area, std::array<double, 4> const& color) const
{
	if (area.empty())
	{
		return false;
	}

	auto zone = SecuRTPrivate::_generateZone(id, name, area, { Classes::Person }, color);
	zone["groupby"] = VAL("generic_area");
	zone["ignore_stationary_objects"] = VAL(true);

	zone["trigger_on_enter"] = VAL(true);
	zone["trigger_on_exit"] = VAL(true);

	zone["restric_person_attribute_CarryingGun"] = VAL(true);
	zone["restrict_person_attributes"] = VAL(true);

	if (_createZone(id, zone) && setConfigValue("/Global/PARClassification/enabled", CValue(true)))
	{
		return true;
	}

	return false;
}

/**
 * @brief Creates a line crossing detection rule with a specific ID, name, line points, classes, direction and color.
 *
 * This function is used to set up a line crossing detection rule in the security system. It takes as input the line ID,
 * name, points that define the line, classes of objects to be detected, the direction of crossing and the color.
 *
 * @param id A unique identifier for the line crossing rule.
 * @param name The human-readable name of the line crossing rule.
 * @param linePoints A vector of 2D points that define the line. Each point represents a vertex of the line.
 * @param classes An unordered set of Classes which are to be detected by the line crossing rule.
 * @param dir The Direction enum which specifies the direction of crossing to be detected.
 * @param color The array of RGBA values representing the color for the area for visualization purposes
 *
 * @return Returns true if the line crossing rule is successfully created. For now, it always returns true.
 */
bool SecuRT::createLineCrossing(std::string const& id, std::string const& name, std::vector<Point2f> const& linePoints,
                                std::unordered_set<Classes> const& classes, Direction const dir, std::array<double, 4> const& color) const
{
	if (linePoints.empty())
	{
		return false;
	}

	if (classes.empty())
	{
		return false;
	}

	if (_hasLine(id))
	{
		return false;
	}

	auto line = SecuRTPrivate::_generateLine(id, name, linePoints, classes, dir, color);
	line["groupby"] = VAL("tripwire_crossing");
	line["trigger_crossing"] = VAL(true);

	return setConfigValue("/Tripwire/Tripwires/" + id, CValue(line)) && setStateValue("recalculate_tripwires", CValue(true));
}

/*
* @brief Deletes all areas in the instance
*/
bool SecuRT::deleteAllAreas() const
{
	auto const& ids = getAreaIds();

	bool success = true;
	for (auto const& id : ids)
	{
		success &= deleteArea(id);
	}

	return success;
}

bool SecuRT::deleteArea(std::string const& id) const
{
	std::string const path = "/Zone/Zones/" + id;

	bool success = deleteConfigValue(path).value_or(false);
	success |= deleteStateValue(path);

	if (success)
	{
		if (setStateValue("recalculate_zones", CValue(true)))
		{
			return true;
		}
	}
	return false;
}

bool SecuRT::deleteAllLines() const
{
	auto const& ids = getLineIds();

	bool success = true;
	for (auto const& id : ids)
	{
		success &= deleteLine(id);
	}

	return success;
}

bool SecuRT::deleteLine(std::string const& id) const
{
	std::string const path = "/Tripwire/Tripwires/" + id;

	bool success = deleteConfigValue(path).value_or(false);
	success |= deleteStateValue(path);

	if (success)
	{
		if (setStateValue("recalculate_tripwires", CValue(true)))
		{
			return true;
		}
	}
	return false;
}

/*
* @brief Sets the sensitivity of the SecuRT AI detector
* 
* The sensitivity of the detector can be set to Low, Medium, or High. This is used to determine the
* confidence threshold for object detection.
* 
* @param mode The sensitivity mode to use, either Low, Medium, or High.
*/
void SecuRT::setDetectionSensitivity(Sensitivity const mode) const
{
	auto const modeStr = SecuRTPrivate::_sensitivityToStr(mode);
	setConfigValue("/Detector/current_sensitivity_preset", CValue(modeStr));
}

/*
* @brief Gets the sensitivity of the SecuRT AI detector
* 
* @return The sensitivity mode of the detector, either Low, Medium, or High.
*/
SecuRT::Sensitivity SecuRT::getDetectionSensitivity() const
{
	if (auto const val = getConfigValue("/Detector/current_sensitivity_preset"))
	{
		if (auto ret = val->getValue<std::string>())
		{
			return SecuRTPrivate::_strToSensitivity(ret.value());
		}
	}

	LOGW << "Unable to get current sensitivity preset, defaulting to Medium";
	return Sensitivity::Medium;
}

/*
* @brief Sets the sensitivity of the SecuRT AI detector
* 
* Movement sensitivity can be set to Low, Medium, or High. This is used to determine the
* amount of motion required to trigger an event.
* 
* @param mode The sensitivity mode to use, either Low, Medium, or High.
*/
void SecuRT::setMovementSensitivity(Sensitivity const mode) const
{
	auto const modeStr = SecuRTPrivate::_sensitivityToStr(mode);
	setConfigValue("/Movement/current_sensitivity_preset", CValue(modeStr));
}

/*
* @brief Gets the sensitivity of the SecuRT AI detector
* 
* @return The sensitivity mode of the detector, either Low, Medium, or High.
*/
SecuRT::Sensitivity SecuRT::getMovementSensitivity() const
{
	if (auto const val = getConfigValue("/Movement/current_sensitivity_preset"))
	{
		if (auto ret = val->getValue<std::string>())
		{
			return SecuRTPrivate::_strToSensitivity(ret.value());
		}
	}

	LOGW << "Unable to get current sensitivity preset, defaulting to Medium";
	return Sensitivity::Medium;
}

/**
 * @brief Change the operating mode of the SecuRT AI detector
 *
 * Smart Detection is a mixture of motion detection and object detection. It provides better
 * overall performance than just object detection, and should be used as the default for most use cases.
 *
 * @param mode The operating mode to use, either SmartDetection or Detection.
 */
void SecuRT::setDetectorMode(Mode const mode) const
{
	std::string strategy;
	std::string preset;

	if (mode == SmartDetection)
	{
		strategy = "motion_guided";
		preset = "MosaicInference";
	}
	else
	{
		strategy = "full_frame";
		preset = "FullRegionInference";
	}

	setConfigValues({ {"/Global/Detection/inference_strategy", CValue(strategy)}, {"/Detector/current_preset", CValue(preset)} });
}

/**
* @brief Get the operating mode of the SecuRT AI detector
*
* @return The operating mode of the detector, either SmartDetection or Detection.
*
*/
SecuRT::Mode SecuRT::getDetectorMode() const
{
	if (auto const val = getConfigValue("/Detector/current_preset"))
	{
		if (auto ret = val->getValue<std::string>())
		{
			return SecuRTPrivate::_strToDetectorMode(ret.value());
		}
	}

	LOGW << "Unable to get current detector mode, defaulting to SmartDetection";
	return SmartDetection;
}

/**
 * @brief Change the modality of the SecuRT AI detector
 *
 * The modality of the detector can be either RGB or Thermal. This is used to determine which
 * AI model to use for object detection.
 *
 * @param modality The modality to use, either RGB or Thermal.
 */
void SecuRT::setSensorModality(Modality const modality) const
{
	setDetectorMode(SmartDetection);
	auto const modalityStr = SecuRTPrivate::_modalityToString(modality);
	setConfigValues({ {"/Global/modality", CValue(modalityStr)}, {"/Global/reload_detector_required", CValue(true)} });
}

SecuRT::Modality SecuRT::getSensorModality() const
{
	if (auto const val = getConfigValue("/Global/modality"))
	{
		if (auto ret = val->getValue<std::string>())
		{
			return SecuRTPrivate::_strToModality(ret.value());
		}
	}

	LOGW << "Unable to get current modality, defaulting to RGB";
	return Modality::RGB;
}

void SecuRT::enableVehicleClassification() const
{
	setConfigValue("/Global/VehicleClassification/enabled", CValue(true));
}

bool SecuRT::setMotionArea(std::vector<Point2f> const& mask)
{
	if (auto const instance = getInstance())
	{
		if (auto motionObj = instance->modules().getObject("Motion"))
		{
			auto const motion = dynamic_cast<iface::MotionDetection*>(motionObj.value().get());

			if (mask.empty())
			{
				motion->setMask({}, false);
				return true;
			}
			else
			{
				motion->setMask({mask}, false);
				return true;
			}
		}
	}
	return false;
}

bool SecuRT::setExclusionAreas(std::vector<std::vector<Point2f>> const& areas)
{
	if (auto const instance = getInstance())
	{
		if (!api::thread::current::isWorkerThread())
		{
			(void)api::thread::registerThread(ThreadType::Worker, -1, instance);
		}

		if (auto inputObj = instance->modules().getObject("Input"))
		{
			auto const input = dynamic_cast<iface::Input*>(inputObj.value().get());

			input->setPrivacyMasks(areas);
			return true;
		}
	}

	return false;
}

void SecuRT::calculateMotionMask()
{
	if (auto const instance = getInstance())
	{
		if (!api::thread::current::isWorkerThread())
		{
			(void)api::thread::registerThread(ThreadType::Worker, -1, instance);
		}
		if (auto motionObj = instance->modules().getObject("Motion"))
		{
			auto const motion = dynamic_cast<iface::MotionDetection*>(motionObj.value().get());
			motion->calculateMask();
		}
	}
}

std::string SecuRT::getArea(std::string const& id) const
{
	if (auto const area = getConfigValue("/Zone/Zones/" + id))
	{
		auto const extractedArea = SecuRTPrivate::_extractAnalyticsEntity(area->getValueOr<cmap>(cmap()));
		return extractedArea.dump();
	}
	return "";
}

std::string SecuRT::getLine(std::string const& id) const
{
	if (auto const line = getConfigValue("/Tripwire/Tripwires/" + id))
	{
		auto const extractedLine = SecuRTPrivate::_extractAnalyticsEntity(line->getValueOr<cmap>(cmap()));
		return extractedLine.dump();
	}
	return "";
}

// This is a callback useful to do operations that require an instance to be loaded
// Upon loading an instance, the callback is called to perform the necessary operations in SecuRT::start()
bool SecuRT::_loadInstanceCallback()
{
	SecuRT::setupCallbacks();
	_setThumbnailCreation();
	calculateMotionMask();
	return true;
}

expected<void> SecuRT::attachToInstanceController(std::weak_ptr<iface::InstanceController> instanceController, bool initialize)
{
	setInstanceController(instanceController);

	auto const ctrl = instanceController.lock();
	if (!ctrl)
	{
		return unexpected(RTErrc::OperationFailed);
	}

	// assign SecuRT to the instance controller
	ctrl->setSolutionManager(shared_from_this());

	// copy settings from the instance and setup SecuRT
	setMetadataMode(getMetadataMode());
	setStatisticsMode(getStatisticsMode());
	setDiagnosticsMode(getDiagnosticsMode());
	setDebugMode(getDebugMode());
	setTentativeTracks(getTentativeTracks());
	setFrameRateLimit(getFrameRateLimit());
	setAutoRestart(getAutoRestart());

	if (initialize)
	{
		SecuRT::initialize();
	}

	return {};
}
