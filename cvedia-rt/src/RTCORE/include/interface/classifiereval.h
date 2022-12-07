/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <shared_mutex>
#include "defines.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			struct ClassifierSummary
            {
				std::vector<std::vector<int>> confusionMatrix;
				std::vector<std::vector<double>> confusionMatrixPerc;

                float avgSystemAccuracy;
				float systemErr;
				float precisionMicro;
				float recallMicro;
				float f1scoreMicro;
				float precisionMacro;
				float recallMacro;
				float f1scoreMacro;
            };

			class ClassifierEval
			{
			public:
				virtual ~ClassifierEval() = default;

                virtual iface::ClassifierSummary evaluate(cvec detections, cvec groundtruth, std::vector<std::string> categories) = 0;
			};
		}
	}
}
