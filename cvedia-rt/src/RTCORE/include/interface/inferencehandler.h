/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <xtensor/xtensor.hpp>

#include "defines.h"
#include "builtin/basetypes.h"
#include "builtin/moduleimpl.h"

namespace cvedia {
	namespace rt {
		class Tensor;

		enum MemoryLayout
		{
			NCHW,
			NHWC
		};

		namespace iface {
			class Instance;
		}

		namespace internal {
			struct DetectorResults {
				std::vector<std::vector<cvedia::rt::Rect2f>> bbox;
				std::vector<std::vector<float>> confidence;
				std::vector<std::vector<int>> classid;
			};

			struct InferenceContext {
				std::vector<cbuffer> frameBuffer;
				int inputWidth{};
				int inputHeight{};
				int inputChannels{};
				int inputBatchsize{};
				MemoryLayout inputLayout{};

				pCValue modelConfig;

				double* inferenceCount{};
				
				std::shared_ptr<iface::Instance> rtInstance;
				std::vector<cbuffer> inputMat;
				//			std::vector<xt::xarray<float>> inputTensor;
				std::vector<Rect2f> rects;
				std::vector<pCValue> cust;
			};
		}

		namespace iface {
			class InferenceHandler : public ModuleImpl {
			public:
				InferenceHandler(std::string const& moduleName) : ModuleImpl(moduleName) {}
				virtual pCValue getCapabilities() = 0;
				virtual expected<void> loadBackend() = 0;
				NODISCARD virtual expected<void> loadModel(std::string const& path) = 0;

				virtual expected<std::vector<xt::xarray<float>>> runInference(std::vector<Tensor> & input) = 0;

				virtual std::vector<int> getInputShape() = 0;
				virtual std::vector<int> getOutputShape() = 0;

				virtual std::string getDefaultScheme() const = 0;
				virtual std::string getDefaultExtension() const = 0;

				NODISCARD virtual expected<void> setDevice(std::string const& guid) = 0;
				virtual std::vector<std::pair<std::string, std::string>> getDeviceGuids() = 0;
			};
		}
	}
}
