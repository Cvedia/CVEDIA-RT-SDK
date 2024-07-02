/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <xtensor/xarray.hpp>

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

			struct BatchDetectionResults {
				std::vector<cvedia::rt::Rect2f> bboxes;
				std::vector<float> confidences;
				std::vector<int> classIds;
			};

			struct ResourceUsage
			{
				unsigned int memoryUsage{};
				unsigned int computeUsage{};
			};

			struct AssociatedBatchDetectionResults : public BatchDetectionResults {
				std::vector<std::size_t> originalIndices;
			};

			struct InferenceContext {
				std::vector<cbuffer> sourceBuffer;
				int inputWidth{};
				int inputHeight{};
				int inputChannels{};
				int inputBatchsize{};
				MemoryLayout inputLayout{};

				pCValue modelConfig;

				double* inferenceCount{};
				
				std::shared_ptr<iface::Instance> rtInstance;
				std::vector<cbuffer> inferenceBuffer;
				//			std::vector<xt::xarray<float>> inputTensor;
				std::vector<Rect2f> cropLocations;
				std::vector<pCValue> customMetadata;
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

				virtual internal::ResourceUsage getResourceUsage() = 0;

				virtual std::string getActiveDevice() = 0;

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
