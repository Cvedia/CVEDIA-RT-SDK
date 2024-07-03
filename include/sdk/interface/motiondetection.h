/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "builtin/moduleimpl.h"
#include <builtin/basetypes.h>

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS MotionDetection : public ModuleImpl
			{
			public:
				MotionDetection(std::string const& moduleName) : ModuleImpl(moduleName) {}
				~MotionDetection() override = default;

				virtual void initialize() = 0;
				virtual expected<pCValue> getDetections() = 0;

				virtual cbuffer getPreprocessFrame() = 0;
				virtual cbuffer getPostprocessFrame() = 0;

				virtual expected<pCValue> getDetection(int id) = 0;
				virtual expected<int> deleteDetection(int id) = 0;
				virtual expected<int> deleteDetections() = 0;

				virtual void setMask(std::vector<std::vector<Point2f>> const& masks, bool exclusion) = 0;
				virtual void calculateMask() = 0;
				virtual expected<cvec> detectMotion(iface::Buffer const& buffer) = 0;
			};
		}
	}
}
