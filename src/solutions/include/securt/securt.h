/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <unordered_set>
#include <vector>

#include "cvaluefwd.h"
#include "defines.h"
#include "uuid.h"
#include "common/solution_manager.h"
#include "builtin/basetypes.h"
#include "common/discovery.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class Solution;
			class Instance;
		}

		class Json;
	}
}

namespace cvedia {
	namespace rt {
		namespace solution {
			struct SettingsData {
				bool useVmsMotion;
				std::vector<Point2f> detectionArea;
			};

			class EXPORT_CLASS SecuRT : public SolutionManager
			{
			public:
				enum Classes
				{
					Person = 1,
					Animal = 2,
					Vehicle = 3,
					Face = 4,
					Unknown = 5
				};

				enum Mode
				{
					SmartDetection = 1,
					Detection = 2,
				};

				enum Modality {
					RGB = 1,
					Thermal = 2
				};

				enum Sensitivity
				{
					Low = 1,
					Medium = 2,
					High = 3,
				};

				enum AreaEvent
				{
					Enter = 1,
					Exit = 2,
					EnterExit = 3,
				};

				enum Direction
				{
					Up = 1,
					Down = 2,
					Both = 3
				};

				enum AnalyticsCategory
				{
					All = 1,
					Line = 2,
					Area = 3
				};

				EXPORT SecuRT();
				EXPORT ~SecuRT() override = default;

				EXPORT expected<void> initialize();

				EXPORT std::string getAnalyticsEntities(AnalyticsCategory category) const;

				EXPORT std::unordered_set<std::string> getAreaIds() const;
				EXPORT std::unordered_set<std::string> getLineIds() const;

				EXPORT bool createCrossingArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area, std::unordered_set<Classes> const& classes, bool ignoreStationaryObjects, AreaEvent type, std::array<double, 4> const& color) const;
				EXPORT bool createIntrusionArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area, std::unordered_set<Classes> const& classes, std::array<double, 4> const& color) const;
				EXPORT bool createLoiteringArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area, std::unordered_set<Classes> const& classes, int seconds, std::array<double, 4> const& color) const;
				EXPORT bool createCrowdingArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area, std::unordered_set<Classes> const& classes, int objectCnt, std::array<double, 4> const& color) const;
				EXPORT bool createLineCrossing(std::string const& id, std::string const& name, std::vector<Point2f> const& linePoints, std::unordered_set<Classes> const& classes, Direction dir, std::array<double, 4> const& color) const;
				EXPORT bool createArmedPersonArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area, std::array<double, 4> const& color) const;

				EXPORT std::string getArea(std::string const& id) const;
				EXPORT bool deleteArea(std::string const& id) const;
				EXPORT bool deleteAllAreas() const;

				EXPORT std::string getLine(std::string const& id) const;
				EXPORT bool deleteLine(std::string const& id) const;
				EXPORT bool deleteAllLines() const;

				EXPORT Mode getDetectorMode() const;
				EXPORT void setDetectorMode(Mode mode) const;
				EXPORT Modality getSensorModality() const;
				EXPORT void setSensorModality(Modality const modality) const;
				
				EXPORT void setDetectionSensitivity(Sensitivity mode) const;
				EXPORT Sensitivity getDetectionSensitivity() const;

				EXPORT void setMovementSensitivity(Sensitivity mode) const;
				EXPORT Sensitivity getMovementSensitivity() const;

				EXPORT bool setExclusionAreas(std::vector<std::vector<Point2f>> const& areas);

				EXPORT bool setMotionArea(std::vector<Point2f> const& mask);
				EXPORT void calculateMotionMask();

				EXPORT void enableVehicleClassification() const;
				
				EXPORT void setupCallbacks() override;

				EXPORT expected<void> attachToInstanceController(std::weak_ptr<iface::InstanceController> instanceController, bool initialize) override;

				static RemoteDiscovery remoteDiscovery_;

			private:
				void _eventsCallback(pCValue data, std::string eventType, float id);
				void _setThumbnailCreation();
				bool _hasZone(std::string const& id) const;
				bool _hasLine(std::string const& id) const;
				bool _createZone(std::string const& id, cmap const& zone) const;

				bool _loadInstanceCallback() override;

				EXPORT void statisticsThread() override;

				bool callbacksInitialized_ = false;

				//Modality modality_ = RGB;
				//Mode mode_ = SmartDetection;
				//Sensitivity detectionSensitivity_ = Medium;
				//Sensitivity movementSensitivity_ = Medium;
			};
		}
	}
}
