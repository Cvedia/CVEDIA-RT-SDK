/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "cvaluefwd.h"

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

			class EXPORT_CLASS ClassifierEval
			{
			public:
				virtual ~ClassifierEval() = default;

                virtual iface::ClassifierSummary evaluate(cvec detections, cvec groundtruth, std::vector<std::string> categories) = 0;
				virtual iface::ClassifierSummary diff(cvec detections1, cvec detections2, cvec groundtruth, std::vector<std::string> categories) = 0;
			};
		}
	}
}
