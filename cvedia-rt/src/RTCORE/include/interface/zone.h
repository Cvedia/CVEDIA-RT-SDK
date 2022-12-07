/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"
#include "builtin/moduleimpl.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class Zone : public ModuleImpl
			{
			public:
				Zone(std::string const& moduleName) : ModuleImpl(moduleName) {}

				/**
				* Zone API
				*/
				virtual bool setConfigContainer(std::string node) = 0;
				virtual std::string createZone(std::string const& name, std::string const& type, std::vector<Point2f> const& shape, float color[4], std::string const& id = "") = 0;
				virtual expected<void> deleteZoneById(std::string zoneid) = 0;
				virtual pCValue checkZones(pCValue const& args) = 0;
				virtual pCValue checkZones(pCValue const& args, pCValue const& argzones) = 0;
				virtual pCValue getZoneIds() = 0;
				virtual expected<pCValue> getZoneById(std::string zoneid) = 0;
				virtual cvec getZoneList() = 0;
				virtual std::vector<std::string> isPointInZone(Point2f pt) = 0;
				virtual expected<void> updateZoneShape(std::string zoneid, std::vector<Point2f> const& shape) = 0;
				virtual expected<void> updateZoneColor(std::string zoneid, float color[4]) = 0;
				virtual expected<void> updateZoneHits(std::string zoneid, int const& hits) = 0;
				virtual expected<void> updateZoneType(std::string zoneid, std::string const& type) = 0;
				virtual expected<void> updateZoneName(std::string zoneid, std::string const& name) = 0;
			
				virtual bool saveZone(std::string id, pCValue const& zone) = 0;
				virtual void clearZones() = 0;
			};
		}
	}
}
