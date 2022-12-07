/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <string>

#include "defines.h"
#include "interface/inferencehandler.h"
#include "interface/cvdict.h"
#include "builtin/moduleimpl.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class InferenceEngine : public ModuleImpl
			{
			public:
				InferenceEngine(std::string const& moduleName) : ModuleImpl(moduleName) {}
				void* getPtr() override = 0;

				virtual expected<pCValue> runInference(cvec const& jobs) = 0;
				virtual expected<void> loadModelFromConfig() = 0;
				virtual expected<void> loadModel(std::string const& path) = 0;
				virtual expected<void> loadModel(std::string const& path, CValue& handlerConfig) = 0;

				virtual expected<void> setPoolSize(int poolSize) = 0;

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