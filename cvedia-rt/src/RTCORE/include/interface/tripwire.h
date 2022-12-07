/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <shared_mutex>
#include "defines.h"
#include "builtin/moduleimpl.h"
#include "builtin/basetypes.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class Tripwire : public ModuleImpl
			{
			public:
				Tripwire(std::string const& moduleName) : ModuleImpl(moduleName) {}
				virtual ~Tripwire() = default;

				virtual bool setConfigContainer(std::string node) = 0;

				/**
				* Tripwire API
				*/
				virtual std::string createTripwire(std::string const& name, std::string const& type, std::vector<Point2f> const& shape, float color[4], std::string direction = "Both", std::string id = "") = 0;
				virtual expected<void> deleteTripwireById(std::string wireid) = 0;
				virtual pCValue checkTripwires(pCValue const& args) = 0;
				virtual pCValue checkTripwires(pCValue const& args, pCValue const& wires) = 0;
				virtual cvec getTripwireIds() = 0;
				virtual cvec getTripwires() = 0;
				virtual pCValue getTripwireById(std::string wireid) = 0;
				virtual std::vector<std::string> isLineTripping(Point2f const& pt1, Point2f const& pt2) = 0;
				virtual void updateTripwireShape(std::string wireid, std::vector<Point2f> const& shape) = 0;
				virtual void updateTripwireColor(std::string wireid, float color[4]) = 0;
				virtual expected<void> updateTripwireType(std::string wireid, std::string const& type) = 0;
				virtual void updateTripwireDirection(std::string wireid, std::string const& direction) = 0;
				virtual expected<void> updateTripwireName(std::string wireid, std::string const& name) = 0;
				virtual void updateTripwireHits(std::string wireid, int const& hits) = 0;
				virtual bool saveTripwire(std::string wireid, pCValue const& tripwire) = 0;
				virtual void clearTripwires() = 0;
			};
		}
	}
}
