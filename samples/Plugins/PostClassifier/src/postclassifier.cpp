/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#include "defines.h"

#include "cvalue.h"
#include "api/inference.h"
#include "interface/inferencehandler.h"
#include "api/logging.h"

#include <plog/Initializers/RollingFileInitializer.h>

#include <xtensor/xtensor.hpp>
#include <xtensor/xsort.hpp>

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::map;
using std::vector;

using namespace cvedia::rt;
using namespace cvedia::rt::internal;

extern "C" EXPORT void registerHandler();
extern "C" EXPORT void logInit(plog::Severity severity, plog::IAppender * appender);

void logInit(plog::Severity severity, plog::IAppender * appender)
{
	plog::init(severity, appender); // Initialize the shared library logger.
}

static int labelToInt(cvec& labels, string label) {

	for (size_t idx = 0; idx < labels.size(); idx++)
	{
		auto key = *labels[idx]->getValue<string>();
		// convert key to lowercase
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);

		if (key == label)
			return static_cast<int>(idx);
	}

	return -1;
}

static void softmax(float* input, size_t size) {
	//	assert(0 <= size <= sizeof(input) / sizeof(double));

	size_t i;
	float m, sum, constant;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnan-infinity-disabled"
#endif
	m = -INFINITY;
#ifdef __clang__
#pragma clang diagnostic pop
#endif

	for (i = 0; i < size; ++i) {
		if (m < input[i]) {
			m = input[i];
		}
	}

	sum = 0.0;
	for (i = 0; i < size; ++i) {
		sum += static_cast<float>(exp(input[i] - m));
	}

	constant = m + static_cast<float>(log(sum));
	for (i = 0; i < size; ++i) {
		input[i] = static_cast<float>(exp(input[i] - constant));
	}
}

static void sigmoid(float* input, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		input[i] = 1.0f / (1.0f + exp(-input[i]));
	}
}

static expected<cvec> postClassifier(InferenceContext& ctx, std::vector<xt::xarray<float>>& output, CValue* inferenceConf) {
	cvec allResults;

	if (output.size() != 1) {
		LOGE << "Classifier only supports 1 tensor, " << output.size() << " received";
		return cvedia::rt::unexpected(RTErrc::UnsupportedOperation);
	}

	auto data = output[0];
	auto shape = data.shape();

	if (shape.size() > 2) {
		LOGE << "Classifier only supports 1D tensors for now";
		return cvedia::rt::unexpected(RTErrc::UnsupportedOperation);
	}

	cvec results;
	cvec labels;

	bool useSoftmax = false;
	bool useSigmoid = false;

	auto modelConf = ctx.modelConfig;
	modelConf->getValueIfSet<bool>("softmax", useSoftmax);
	modelConf->getValueIfSet<bool>("sigmoid", useSigmoid);

	auto remap = inferenceConf->getValueOr<cmap>("remap", {});

	if (modelConf->hasChild("labels")) {
		labels = modelConf->getChild("labels")->getValue<cvec>().value();

		if (labels.size() != shape[1]) {
			LOGE << "Labels (" << labels.size() << ") does not match output shape (" << shape[1] << ")";
			return cvedia::rt::unexpected(RTErrc::UnsupportedOperation);
		}
	}

	std::map<int, int> remapids;

	if (remap.size() > 0)
	{
		remapids.clear();

		for (auto const& kv : remap)
		{
			string key = kv.first;
			string label = kv.second->getValueOr<string>("");

			int keyid = labelToInt(labels, key);
			int replid = labelToInt(labels, label);
			if (keyid == -1)
			{
				LOGE << "Failed to lookup label " << key << " for remapping. Did the model change?";
				break;
			}
			if (replid == -1)
			{
				LOGE << "Failed to lookup label " << label << " for remapping. Did the model change?";
				break; 
			}
			if (keyid != replid)
			{
				remapids[keyid] = replid;
			}
		}
	}

	// Calculate winning classes
	auto maxret = xt::argmax(data, 1);
	auto csz = ctx.customMetadata.size();
	
	//LOGD << "inputBatchsize: " << ctx.inputBatchsize << " customMetadata.size(): " << csz;
	//LOGD << "Context cust. size (" << ctx.customMetadata.size() << ") does not match output batch size (" << shape[0] << ")";

	// if (ctx.customMetadata.size() < shape[0]) {
	// 	LOGE << "Context cust. size (" << ctx.customMetadata.size() << ") does not match output batch size (" << shape[0] << ")";
	// 	return cvedia::rt::unexpected(RTErrc::UnsupportedOperation);
	// }

	//for (size_t batchIdx = 0; batchIdx < shape[0]; batchIdx++) {
	for (size_t batchIdx = 0; batchIdx < csz; batchIdx++) {
		cmap result;
		cvec feat;

		if (useSoftmax) {
			softmax(&data(batchIdx, 0), shape[1]);
		}
		else if (useSigmoid) {
			sigmoid(&data(batchIdx, 0), shape[1]);
		}

		//if (!remapids.empty())
		//{
		//	for (unsigned int featIdx = 0; featIdx < shape[1]; featIdx++) {
		//		// Move feat to remapped entry
		//		if (remapids.count(featIdx) == 1) {
		//			data(batchIdx, remapids[featIdx]) = data(batchIdx, featIdx);
		//			// Clear old entry
		//			data(batchIdx, featIdx) = 0.0f;
		//		}
		//	}
		//}

		for (unsigned int featIdx = 0; featIdx < shape[1]; featIdx++) {
			float t = data(batchIdx, featIdx);
			feat.push_back(make_shared<CValue>(t));
		}

		if (ctx.customMetadata.size() > batchIdx) {
			result["custom"] = ctx.customMetadata[batchIdx];
		}

		if (ctx.inferenceCount) {
			result["id"] = VAL(std::to_string(static_cast<int>((*ctx.inferenceCount)++)));
		}

		result["feat"] = make_shared<CValue>(feat);
		result["labels"] = make_shared<CValue>(labels);
		result["confidence"] = make_shared<CValue>(data(batchIdx, maxret[batchIdx]));
		if (labels.size() > maxret[batchIdx])
			result["label"] = labels[maxret[batchIdx]];
		result["job"] = ctx.customMetadata[batchIdx];

		//		cv::Mat newBuffer;
				// FIXME : We need to make sure that saveImage() inside the buffer converts
				// this back to int format. Might also break the UI if we don't
		//		ctx.inputMat[batchIdx].convertTo(newBuffer, CV_8UC3);

		result["image"] = VAL(ctx.inferenceBuffer[batchIdx]); // VAL(make_shared<BufferBase>(newBuffer));

		results.push_back(make_shared<CValue>(result));
	}

	//	allResults.push_back(make_shared<CValue>(results));

	return results;
}

void registerHandler() {
	api::inference::registerPostprocessHandler("classifier", postClassifier);
}
