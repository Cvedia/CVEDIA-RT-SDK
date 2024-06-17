/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "defines.h"
#include "cvaluefwd.h"

namespace cvedia {
	namespace rt {
		namespace iface {
            struct APAR
            {
                bool ap = true;
                float iouThr = 0.0;
                std::string areaRng;
                int maxDets = 0;
                float mean;
            };

			struct CocoSummary
            {
                std::vector<APAR> apar;
                float f1Score;
                float ap;
                float ar;
            };

            // Annotation data for a single object instance in an image
            struct InstanceAnnotation {
                InstanceAnnotation(
                    uint64_t id,
                    double score,
                    double area,
                    bool is_crowd,
                    bool ignore)
                    : id{ id }, score{ score }, area{ area }, is_crowd{ is_crowd }, ignore{ ignore } {}
                uint64_t id;
                double score = 0.;
                double area = 0.;
                bool is_crowd = false;
                bool ignore = false;
            };

            // Stores intermediate results for evaluating detection results for a single
            // image that has D detected instances and G ground truth instances. This stores
            // matches between detected and ground truth instances
            struct ImageEvaluation {
                // For each of the D detected instances, the id of the matched ground truth
                // instance, or 0 if unmatched
                std::vector<uint64_t> detection_matches;

                // The detection score of each of the D detected instances
                std::vector<double> detection_scores;

                // Marks whether or not each of G instances was ignored from evaluation (e.g.,
                // because it's outside area_range)
                std::vector<bool> ground_truth_ignores;

                // Marks whether or not each of D instances was ignored from evaluation (e.g.,
                // because it's outside aRng)
                std::vector<bool> detection_ignores;
            };

            template <class T>
            using ImageCategoryInstances = std::vector<std::vector<std::vector<T>>>;

			class EXPORT_CLASS CocoEval
			{
			public:
				virtual ~CocoEval() = default;

				//virtual std::vector<ImageEvaluation> evaluateImages(
				//	int max_detections,
				//	const ImageCategoryInstances<std::vector<double>>& image_category_ious,
				//	const ImageCategoryInstances<InstanceAnnotation>&
				//	image_category_ground_truth_instances,
				//	const ImageCategoryInstances<InstanceAnnotation>&
				//	image_category_detection_instances) = 0;

    //            virtual void accumulate(const std::vector<iface::ImageEvaluation>& evalutations) = 0;
    //            virtual pCValue summarize() = 0;
                virtual iface::CocoSummary evaluate(cvec detections, cvec groundtruth, std::vector<std::string> categories, int refWidth, int refHeight) = 0;
                virtual std::string summaryToString(iface::CocoSummary) const = 0;
			};
		}
	}
}
