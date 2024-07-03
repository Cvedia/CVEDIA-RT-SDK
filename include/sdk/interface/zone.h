/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "builtin/moduleimpl.h"
#include <sol/forward.hpp>

#include "builtin/basetypes.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS Zone : public ModuleImpl
			{
			public:
				Zone(std::string const& moduleName) : ModuleImpl(moduleName) {}

				/**
				* Zone API
				*/
				NODISCARD virtual expected<std::string> createZone(std::string const& name, std::string const& group, std::vector<Point2f> const& shape, std::array<float, 4> const& color) = 0;
				NODISCARD virtual expected<void> registerKnownZones(bool skipRecalculate = false) = 0;
				NODISCARD virtual expected<void> deleteZoneById(std::string const& zoneid) = 0;

				NODISCARD virtual expected<void> registerCallback(sol::protected_function callback) = 0;
				NODISCARD virtual expected<void> unregisterCallback() = 0;

				NODISCARD virtual expected<void> processZones(std::map<std::string, std::vector<Point2f>> const& points, cmap const& options = {}) = 0;
				NODISCARD virtual expected<void> processZones(std::map<std::string, std::vector<Point2f>> const& points, std::vector<std::string> const& zonesFilter, cmap const& options = {}) = 0;
				NODISCARD virtual expected<void> processZones(std::map<std::string, std::vector<Point2f>> const& points, std::string const& groupFilter, cmap const& options = {}) = 0;

				virtual bool hasZoneId(std::string const& zoneid) = 0;
				NODISCARD virtual expected<std::vector<std::string>> getZoneIds() = 0;
				NODISCARD virtual expected<pCValue> getZoneById(std::string const& zoneid, int dictType = DICT_STATE) = 0;
				NODISCARD virtual expected<cmap> getZones(int dictType = DICT_STATE) = 0;

				NODISCARD virtual expected<pCValue> getZoneValue(std::string const& zoneid, std::string const& key, int dictType = DICT_STATE) = 0;
				NODISCARD virtual expected<cvec> getZoneValue(std::string const& zoneid, std::vector<std::string> const& keys, int dictType = DICT_STATE) = 0;
				NODISCARD virtual expected<void> setZoneValue(std::string const& zoneid, std::string const& key, pCValue const& value, int dictType = DICT_STATE) = 0;
				NODISCARD virtual expected<void> setZoneValue(std::string const& zoneid, cmap const& values, int dictType = DICT_STATE) = 0;

				NODISCARD virtual expected<void> recalculateZone(std::string const& zoneid) = 0;
				NODISCARD virtual expected<void> recalculateZones() = 0;
			
				NODISCARD virtual expected<void> resetZones() = 0;
				NODISCARD virtual expected<void> deleteZones() = 0;

				NODISCARD virtual expected<std::vector<std::string>> isPointInZone(Point2f const& pt) = 0;

				NODISCARD virtual expected<cvec> getZoneEvents() const = 0;

				// targets / filtered / types / enter / exit
				// targets / filtered / groups / enter / exit
				// checkZones
				//
				// processZones(points)
				// processZones(points, guidsFilter)
				// processZones(points, groupFilter)
				// getZoneEnters()
				// getZoneExits()
				// getZoneOccupants()
				//
			};
		}
	}
}
 