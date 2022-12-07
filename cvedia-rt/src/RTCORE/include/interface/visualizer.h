/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"
#include <imgui_internal.h>

namespace cvedia {
	namespace rt {
		namespace iface {

			class Visualizer {
			public:
				Visualizer() = default;
				virtual ~Visualizer() = default;
				Visualizer(Visualizer const& other) = default;
				Visualizer& operator=(Visualizer const& other) = default;

				Visualizer(Visualizer&& other) = default;
				Visualizer& operator=(Visualizer&& other) = default;

				virtual void setCurrentContext(ImGuiContext* ctx) = 0;
				virtual ImGuiContext* getCurrentContext() = 0;
				virtual void setAllocatorFunctions(ImGuiMemAllocFunc allocFunc, ImGuiMemFreeFunc freeFunc, void* userData = nullptr) = 0;

				virtual pCValue draw(pCValue input, pCValue config) = 0;
				virtual pCValue visualize(pCValue input) = 0;
				virtual pCValue configData(pCValue input) = 0;
				virtual pCValue config(pCValue data) = 0;
				virtual pCValue onLeftMouseClick(pCValue input, pCValue config) = 0;
				virtual pCValue onLeftMouseDrag(pCValue input, pCValue config) = 0;
				virtual pCValue inspector(pCValue input, pCValue config) = 0;
				virtual pCValue onRightMouseClick(pCValue input) = 0;
				virtual pCValue getEntityById(pCValue input) = 0;
			};
		}
	}
}
