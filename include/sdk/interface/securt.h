/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <unordered_set>
#include <vector>

#include "cvaluefwd.h"
#include "defines.h"
#include "interface/solution_manager.h"
#include "builtin/basetypes.h"

namespace cvedia {
	namespace rt {
		namespace iface
		{
			class EXPORT_CLASS SecuRT : virtual public cvedia::rt::iface::SolutionManager
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

				enum class AppearanceMode
				{
					Off = 0,
					Person = 1,
					Vehicle = 2,
					Both = 3
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

				virtual ~SecuRT() = default;

				virtual std::string getAnalyticsEntities(AnalyticsCategory category) const = 0;

				virtual std::unordered_set<std::string> getAreaIds() const = 0;
				virtual std::unordered_set<std::string> getLineIds() const = 0;

				NODISCARD virtual bool createCrossingArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area, std::unordered_set<Classes> const& classes, bool ignoreStationaryObjects, AreaEvent type, std::array<double, 4> const& color) const = 0;
				NODISCARD virtual bool createIntrusionArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area, std::unordered_set<Classes> const& classes, std::array<double, 4> const& color) const = 0;
				NODISCARD virtual bool createLoiteringArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area, std::unordered_set<Classes> const& classes, int seconds, std::array<double, 4> const& color) const = 0;
				NODISCARD virtual bool createCrowdingArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area, std::unordered_set<Classes> const& classes, int objectCnt, std::array<double, 4> const& color) const = 0;
				NODISCARD virtual bool createLineCrossing(std::string const& id, std::string const& name, std::vector<Point2f> const& linePoints, std::unordered_set<Classes> const& classes, Direction dir, std::array<double, 4> const& color) const = 0;
				NODISCARD virtual bool createArmedPersonArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area, std::array<double, 4> const& color) const = 0;
				NODISCARD virtual bool createObjectLeftArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area, int seconds, std::array<double, 4> const& color) const = 0;
				NODISCARD virtual bool createObjectRemovedArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area, int seconds, std::array<double, 4> const& color) const = 0;
				NODISCARD virtual bool createTailgatingLine(std::string const& id, std::string const& name, std::vector<Point2f> const& linePoints, std::unordered_set<Classes> const& classes, int seconds, Direction dir, std::array<double, 4> const& color) const = 0;
				NODISCARD virtual bool createFallenPersonArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area, std::array<double, 4> const& color) const = 0;

				NODISCARD virtual bool createLicensePlateAccessControlArea(std::string const& id, std::string const& name, std::vector<Point2f> const& area, std::array<double, 4> const& color) const = 0;

				virtual std::string getArea(std::string const& id) const = 0;
				NODISCARD virtual bool deleteArea(std::string const& id) const = 0;
				NODISCARD virtual bool deleteAllAreas() const = 0;

				NODISCARD virtual bool setAppearanceSearchMode(AppearanceMode mode) const = 0;

				virtual std::string getLine(std::string const& id) const = 0;
				NODISCARD virtual bool deleteLine(std::string const& id) const = 0;
				NODISCARD virtual bool deleteAllLines() const = 0;
				virtual Mode getDetectorMode() const = 0;
				virtual void setDetectorMode(Mode mode) const = 0;
				virtual Modality getSensorModality() const = 0;
				virtual void setSensorModality(Modality const modality) const = 0;
				virtual void setDetectionSensitivity(Sensitivity mode) const = 0;
				virtual Sensitivity getDetectionSensitivity() const = 0;
				virtual void setMovementSensitivity(Sensitivity mode) const = 0;
				virtual Sensitivity getMovementSensitivity() const = 0;
				NODISCARD virtual bool setExclusionAreas(std::vector<std::vector<Point2f>> const& areas) = 0;
				NODISCARD virtual bool setMotionArea(std::vector<Point2f> const& mask) = 0;
				virtual void calculateMotionMask() = 0;
				virtual void enableVehicleClassification() const = 0;
			};
		}
	}
}
