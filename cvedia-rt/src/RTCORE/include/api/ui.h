/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"

#include <opencv2/imgproc.hpp>

namespace cvedia {
	namespace rt {
		namespace iface {
			class Instance;
		}
	}
}

namespace cvedia {
	namespace rt {
		namespace api {
			namespace ui {
				EXPORT bool startConfigTable(std::string label, bool showHeader, int headerFlags);
				EXPORT bool configSliderInt(std::string const& label, iface::Instance* inst, std::string const& keyPath, int minValue, int maxValue, int defaultValue, int* retValue = nullptr);
				EXPORT bool configSliderFloat(std::string const& label, iface::Instance* inst, std::string const& keyPath, float minValue, float maxValue, float defaultValue, float* retValue = nullptr);
				EXPORT bool configCheckbox(std::string const& label, iface::Instance* inst, std::string const& keyPath, bool defaultValue, bool* retValue = nullptr);
				EXPORT bool configComboBox(std::string const& label, iface::Instance* inst, std::string const& keyPath, std::vector<std::pair<std::string, std::string>> const& items, std::string defaultValue, std::string* retValue = nullptr);
				EXPORT bool configText(std::string const& label, iface::Instance* inst, std::string const& keyPath, std::string defaultValue, std::string* retValue = nullptr);
				EXPORT bool configColor(std::string const& label, iface::Instance* inst, std::string const& keyPath, std::vector<float> const& defaultValue, std::vector<float>* retValue = nullptr);
				EXPORT void configEmptyLine(std::string const& text);
				EXPORT void endConfigTable();

				EXPORT bool setLoadingBar(std::string title, int value);
				EXPORT std::pair<std::string, int> getLoadingBar();
				EXPORT void setupNotificationSystem(std::function<bool(char const*, char const*, char const*, int)>&& notifier);
				EXPORT bool showNotification(char const* type, char const* title, char const* message, char const* messageArg, int durationMs);

				EXPORT int drawConvexPolyFilled(std::vector<cv::Point2f> const& pointVec, cv::Scalar cvColor);
				EXPORT cv::Point2f getPointInDir(cv::Point2f const pt, float const rad, float const dist);
				EXPORT void drawArrowOnLine(std::vector<cv::Point2f> const& line, bool const up);
			};
		}
	}
}
