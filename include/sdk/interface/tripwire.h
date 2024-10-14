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
			class EXPORT_CLASS Tripwire : public ModuleImpl
			{
			public:
				Tripwire(std::string const& moduleName) : ModuleImpl(moduleName) {}

				/**
				* Tripwire API
				*/
				NODISCARD virtual expected<std::string> createTripwire(std::string const& name, std::string const& group, std::vector<Point2f> const& shape, std::array<float, 4> const& color, std::string direction = "Both") = 0;
				NODISCARD virtual expected<void> registerKnownTripwires(bool skipRecalculate = false) = 0;
				
				NODISCARD virtual expected<void> processTripwires(std::map<std::string, std::vector<Point2f>> const& points, cmap const& options = {}) = 0;
				NODISCARD virtual expected<void> processTripwires(std::map<std::string, std::vector<Point2f>> const& points, std::vector<std::string> const& tripwiresFilter, cmap const& options = {}) = 0;
				NODISCARD virtual expected<void> processTripwires(std::map<std::string, std::vector<Point2f>> const& points, std::string const& groupFilter, cmap const& options = {}) = 0;
				NODISCARD virtual expected<void> registerCallback(sol::protected_function callback) = 0;
				NODISCARD virtual expected<void> unregisterCallback() = 0;
				virtual bool hasTripwireId(std::string const& tripwireId) = 0;
				NODISCARD virtual expected<std::vector<std::string>> getTripwireIds() = 0;
				NODISCARD virtual expected<pCValue> getTripwireById(std::string tripwireId, int dictType=DICT_STATE) = 0;
				NODISCARD virtual expected<cmap>  getTripwires(int dictType = DICT_STATE) = 0;
				NODISCARD virtual expected<pCValue> getTripwireValue(std::string const& tripwireId, std::string const& key, int dictType = DICT_STATE) = 0;
				NODISCARD virtual expected<cvec> getTripwireValue(std::string const& tripwireId, std::vector<std::string> const& keys, int dictType = DICT_STATE) = 0;
				NODISCARD virtual expected<void> setTripwireValue(std::string const& tripwireId, std::string const& key, pCValue const& value, int dictType = DICT_STATE) = 0;
				NODISCARD virtual expected<void> setTripwireValue(std::string const& tripwireId, cmap const& values, int dictType = DICT_STATE) = 0;
				NODISCARD virtual expected<void> recalculateTripwire(std::string const& tripwireId) = 0;
				NODISCARD virtual expected<void> recalculateTripwires() = 0;
				NODISCARD virtual expected<void> deleteTripwireById(std::string const& tripwireId) = 0;
				NODISCARD virtual expected<void> resetTripwires() = 0;
				NODISCARD virtual expected<void> deleteTripwires() = 0;
				NODISCARD virtual expected<cvec> getTripwireEvents() = 0;
			};
		}
	}
}
