/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <string>

#include "defines.h"

namespace cv {
	class Mat;
}

struct ImGuiContext;

typedef void* (*ImGuiMemAllocFunc)(size_t sz, void* user_data);             // Function signature for ImGui::SetAllocatorFunctions()
typedef void (*ImGuiMemFreeFunc)(void* ptr, void* user_data);               // Function signature for ImGui::SetAllocatorFunctions()

namespace cvedia {
	namespace rt {
		namespace iface {
			class Renderer
			{
			public:
				Renderer() = default;
				virtual ~Renderer() = default;

				virtual void setCurrentContext(ImGuiContext* ctx) = 0;
				virtual ImGuiContext* getCurrentContext() = 0;
				virtual void setAllocatorFunctions(ImGuiMemAllocFunc allocFunc, ImGuiMemFreeFunc freeFunc, void* userData = NULL) = 0;

				virtual void* matToTexture(cv::Mat img) = 0;
				virtual void* matToTexture(std::string name, cv::Mat img, bool update) = 0;

				virtual void createWindow(std::string windowTitle, int windowWidth, int windowHeight) = 0;
				virtual bool messageLoop() = 0;
				virtual bool startFrame() = 0;
				virtual void endFrame() = 0;
				virtual void cleanup() = 0;
			};
		}
	}
}
