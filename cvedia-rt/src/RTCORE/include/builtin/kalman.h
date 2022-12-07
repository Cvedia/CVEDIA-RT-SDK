/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/

#include "rtconfig.h"
#if WITH_GENERIC_KALMAN

#include <map>
#include <string>

#include <opencv2/video/tracking.hpp>
#include "defines.h"

namespace cvedia {
	namespace rt {

		class KalmanFilter final {
		public:
			EXPORT void createFilter(std::string const& name, int featureCount);
			EXPORT void updateFilter(std::string const& name, std::vector<float> features);
			EXPORT std::vector<float> queryFilter(std::string const& name);
			EXPORT void deleteFilter(std::string const& name);
		private:
			std::map<std::string, cv::KalmanFilter> kalmanFilters_;
			std::map<std::string, bool> kalmanFiltersInit_;
		};
	}
}

#endif
