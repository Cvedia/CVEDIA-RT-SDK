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
			class MotionDetection : public ModuleImpl
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

				virtual expected<pCValue> detectMotion(cbuffer buffer) = 0;
			};
		}
	}
}
