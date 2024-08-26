/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <opencv2/core.hpp>

#include "cvalue.h"

using namespace cvedia::rt;

using std::vector;

inline cv::Scalar_<float> cvalueToColor(pCValue cv, cv::Scalar_<float> alt) {

	if (cv != nullptr && cv->getType() == CValueType::VECTOR)
	{
		auto vec = cv->getValue<cvec>();

		if (auto& v = vec) {
			if (v->size() == 4 || v->size() == 3)
			{
				float vals[4] = { 0,0,0,1 };
				for (size_t i = 0; i < v->size(); i++)
				{
					vals[i] = v->at(i)->getValue<float>().value();

					//Convert from 0-255 range to 0-1 range if needed 
					if (vals[i] > 1 && vals[i] <= 255) {
						vals[i] = vals[i] / 255.0f;
					}
				}

				return cv::Scalar(vals[0], vals[1], vals[2], vals[3]);
			}
		}
	}
	return alt;
}

inline bool uiCheckbox(int id, std::string label, bool value) {
	ImGui::PushID(id + ImGui::GetID(label.c_str()));
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("%s", label.c_str());
	ImGui::TableNextColumn();
	ImGui::SetWindowFontScale(0.8f);
	ImGui::Checkbox("", &value);
	ImGui::SetWindowFontScale(1.0f);
	ImGui::PopID();

	return value;
}

inline float uiSliderFloat(int id, std::string label, float value, float min, float max, std::string format = "%.2f") {
	ImGui::PushID(id + ImGui::GetID(label.c_str()));
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("%s", label.c_str());
	ImGui::TableNextColumn();
	const float elemWidth = ImGui::GetContentRegionAvail().x * 0.90f;
	ImGui::SetNextItemWidth(elemWidth);
	ImGui::SetWindowFontScale(0.8f);
	ImGui::SliderFloat("", &value, min, max, format.c_str());
	ImGui::SetWindowFontScale(1.0f);
	ImGui::PopID();

	return value;
}

inline std::string uiTextEdit(int id, std::string label, std::string value) {
	ImGui::PushID(id + ImGui::GetID(label.c_str()));
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("%s", label.c_str());
	ImGui::TableNextColumn();
	const float elemWidth = ImGui::GetContentRegionAvail().x * 0.90f;
	ImGui::SetNextItemWidth(elemWidth);
	char buf[512];
	strcpy_s(buf, value.c_str());
	ImGui::SetWindowFontScale(0.8f);
	ImGui::InputText("", buf, 512);
	ImGui::SetWindowFontScale(1.0f);
	ImGui::PopID();

	return std::string(buf);
}

inline std::string uiTextMultiline(int id, std::string label, std::string value) {
	ImGui::PushID(id + ImGui::GetID(label.c_str()));
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("%s", label.c_str());
	ImGui::TableNextColumn();
	const float elemWidth = ImGui::GetContentRegionAvail().x * 0.90f;
	ImGui::SetNextItemWidth(elemWidth);
	char buf[512];
	strcpy_s(buf, value.c_str());
	ImGui::SetWindowFontScale(0.8f);
	ImGui::InputTextMultiline("", buf, 512);
	ImGui::SetWindowFontScale(1.0f);
	ImGui::PopID();

	return std::string(buf);
}

inline int uiSliderInt(int id, std::string label, int value, int min, int max) {
	ImGui::PushID(id + ImGui::GetID(label.c_str()));
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("%s", label.c_str());
	ImGui::TableNextColumn();
	const float elemWidth = ImGui::GetContentRegionAvail().x * 0.90f;
	ImGui::SetNextItemWidth(elemWidth);
	ImGui::SetWindowFontScale(0.8f);
	ImGui::SliderInt("", &value, min, max);
	ImGui::SetWindowFontScale(1.0f);
	ImGui::PopID();

	return value;
}

inline vector<float> uiColorPickerOverride(int id, std::string label, vector<float> value) {

	if (value.size() < 4)
	{
		value.resize(4);
	}
	int curIdx = 0;

	ImGui::PushID(id + ImGui::GetID(label.c_str()));
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("%s", label.c_str());
	ImGui::TableNextColumn();
	std::string nullTerminatedOptions = "Test\0Test 2\0\0";
	ImGui::Combo("", &curIdx, nullTerminatedOptions.c_str());
	ImGui::SetWindowFontScale(0.8f);
	ImGui::ColorEdit4("", value.data(), ImGuiColorEditFlags_NoInputs);
	ImGui::SetWindowFontScale(1.0f);
	ImGui::PopID();

	return value;
}

inline vector<float> uiColorPicker(int id, std::string label, vector<float> value) {

	if (value.size() < 4)
	{
		value.resize(4);
	}

	ImGui::PushID(id + ImGui::GetID(label.c_str()));
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("%s", label.c_str());
	ImGui::TableNextColumn();
	ImGui::SetWindowFontScale(0.8f);
	ImGui::ColorEdit4("", value.data(), ImGuiColorEditFlags_NoInputs);
	ImGui::SetWindowFontScale(1.0f);
	ImGui::PopID();

	return value;
}

inline std::string uiComboBox(int id, std::string label, std::string curValue, std::vector<std::pair<std::string, std::string>> options) {
	ImGui::PushID(id + ImGui::GetID(label.c_str()));
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("%s", label.c_str());
	ImGui::TableNextColumn();

	// Find curValue in first column of options
	int curIdx = 0;
	for (size_t i = 0; i < options.size(); i++)
	{
		if (options[i].first == curValue)
		{
			curIdx = static_cast<int>(i);
			break;
		}
	}

	// Converted second column of options into null terminated string
	std::string nullTerminatedOptions;
	for (size_t i = 0; i < options.size(); i++)
	{
		nullTerminatedOptions += options[i].second;
		if (!options.empty() && i < options.size() - 1) {
			nullTerminatedOptions += '\0';
		}
	}

	nullTerminatedOptions += '\0';

//	const float elemWidth = ImGui::GetContentRegionAvail().x * 0.90f;
//	ImGui::SetNextItemWidth(elemWidth);

	ImGui::SetWindowFontScale(0.8f);
	ImGui::Combo("", &curIdx, nullTerminatedOptions.c_str());
	ImGui::SetWindowFontScale(1.0f);
	ImGui::PopID();

	if (options.size() > 0)
	{
		return options[curIdx].first;
	}
	else
	{
		return "";
	}
}
