/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <string>
#include <vector>
#include <mutex>
#include <memory>

#include <locale>

#include "defines.h"

#ifdef _WIN32
#include "dml_provider_factory.h"
#endif

#ifdef ONNX_HAS_TENSORRT
#include "tensorrt_provider_factory.h"
#endif

#include <onnxruntime_cxx_api.h>
#include <xtensor/xarray.hpp>

#include "cvalue.h"

namespace cvedia {
	namespace rt {
		class Tensor;
	}
}

namespace cvedia {
	namespace rt {
		namespace module {
			/**
			 * \ingroup Core
			 * @{
			 */
			class OnnxCore
			{
			public:
				struct Config {
					std::string backend = "CPU";
					std::string execution_mode = "PARALLEL";
					bool profiling = false;
					std::string profiling_prefix = "";
					int device_id = 0;
					std::string trt_cache_path = "";
					bool trt_cache_enabled = true;
					int ort_intraop_num_threads = 8;
					int ort_interop_num_threads = 8;
				};

				struct ModelConfig {
					std::string channel_layout = "";
					float mean = 0;
					float stddev = 0;
					bool normalize_input = false;
					bool transpose_input = false;
				};

				struct Stats {
				};

				OnnxCore();
				~OnnxCore();

				bool backendLoaded_ = false;

				void initialize();
				expected<void> loadModel(std::string const& model);
				expected<void> loadBackend();
				void unloadBackend();

				pCValue getCapabilities() const;

				expected<void> setDevice(std::string const& device);
				std::vector<std::pair<std::string, std::string>> getDeviceGuids() const;

				std::vector<int> getInputShape() {
					return inputShape_;
				}

				std::vector<int> getOutputShape() {
					return outputShape_;
				}

				expected<std::vector<xt::xarray<float>>> runInference(std::vector<Tensor>& inputs);

				Config pluginConf;
				Stats pluginStats;
				ModelConfig modelConf;

				pCValue modelNode_;
			private:

				std::vector<int> inputShape_;
				std::vector<int> outputShape_;

				bool modelLoaded_;

				std::unique_ptr<Ort::Session> mSession_;
				std::unique_ptr<Ort::Env> mEnv_;

				std::size_t numInputNodes_;
				std::vector<const char*> inputNodeNames_;
				std::vector<const char*> outputNodeNames_;

				Ort::Value input_tensor{ nullptr };
				Ort::MemoryInfo memoryInfo_{ nullptr };

				std::mutex sessMux_;

				std::vector<float> inputBatchTensor_;

				Ort::SessionOptions sessionOptions_;
				std::string curChannelLayout_;
			};
			/** @} */
		}
	}
}
