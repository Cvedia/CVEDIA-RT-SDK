/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#include <shared_mutex>
#include <vector>
#include <string>
#include <map>
#include <deque>

#include "defines.h"

#define MAX_PERFORMANCE_BUFFER 200

namespace cvedia {
	namespace rt {
		namespace internal {
			class PerformanceBase {

			public:
				EXPORT PerformanceBase();
				EXPORT ~PerformanceBase();

				EXPORT void setPerformanceCounterEnabled(bool const enable) { isEnabled_ = enable; }
				EXPORT bool isPerformanceCounterEnabled() const { return isEnabled_; }
				EXPORT void writePerformanceCounter(std::string const& name, float value, int maxHist = MAX_PERFORMANCE_BUFFER);
				EXPORT std::vector<std::pair<std::string, float>> getAllPerformanceCounters(std::vector<std::string> const& filter = std::vector<std::string>());
				EXPORT std::vector<std::pair<std::string, std::vector<float>>> getPerformanceCounterStats(std::vector<std::string> const& filter = std::vector<std::string>());
				EXPORT float getLastPerformanceCounter(std::string const& name);
				EXPORT void updatePerformance();
				EXPORT void resetPerformance();
				EXPORT PerformanceBase* getPerformanceCounterPtr() { return this; }

				EXPORT void loadProfilerPreset(std::string fileName);
				EXPORT bool saveProfilerPreset(std::string fileName);
				EXPORT std::map<std::string, float> getProfilerPresets() { return presetCounters_; }
				EXPORT void setProfilerSimulationEnabled(bool const enabled) { isSimulationEnabled_ = enabled; }

			private:
				bool isEnabled_;
				bool isSimulationEnabled_ = false;
				std::map<std::string, std::deque<float>> updatedPerformanceStats_;
				std::map<std::string, std::deque<float>> updatedPerformanceCounters_;
				std::map<std::string, std::deque<float>> runtimePerformanceCounters_;
				std::map<std::string, float> presetCounters_;
				mutable shared_mutex_class performanceMux_;
			};
		}
	}
}
