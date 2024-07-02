/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "builtin/moduleimpl.h"
#include "builtin/basetypes.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			struct PackedJob
			{
				cbuffer buffer;
				std::vector<Rect> rects;
			};

			class EXPORT_CLASS InferenceEngine : public ModuleImpl
			{
			public:
				InferenceEngine(std::string const& moduleName) : ModuleImpl(moduleName) {}
				void* getPtr() override = 0;

				virtual expected<void> runInferenceAsync(cvec const& jobs, int ttl = 60) = 0;
				virtual expected<cvec> getAsyncInferenceResults() = 0;
				virtual expected<void> setAsyncResultsCollectionDuration(int durationMs) = 0;

				virtual expected<cvec> runInference(cvec const& jobs) = 0;
				virtual expected<void> loadModelFromConfig() = 0;
				virtual expected<void> loadModel(std::string const& path) = 0;
				virtual expected<void> loadModel(std::string const& path, CValue& handlerConfig) = 0;

				virtual expected<void> setPoolSize(int poolSize) = 0;

				virtual expected<std::pair<cbuffer, std::vector<Rect2f>>> packJobs(cvec const& jobs) = 0;
				virtual expected<std::pair<cbuffer, std::vector<Rect2f>>> packJobs(cvec const& jobs, int width, int height) = 0;

//				virtual pCValue getBackendConfig() = 0;
				virtual void setBackendConfig(pCValue conf) = 0;

				virtual expected<pCValue> getCapabilities() = 0;
				virtual expected<std::shared_ptr<iface::InferenceHandler>> getActiveHandler() const = 0;
				
				virtual expected<pCValue> getModelConfig() = 0;

				virtual ssize_t inputBatchSize() = 0;
				virtual ssize_t inputWidth() = 0;
				virtual ssize_t inputHeight() = 0;
				virtual ssize_t inputChannels() = 0;

				virtual std::vector<int> inputShape() = 0;
				virtual std::vector<int> outputShape() = 0;
			};
		}
	}
}