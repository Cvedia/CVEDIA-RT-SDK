/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <memory>
#include <string>

#include "interface/inferencehandler.h"
#include "onnxcore.h"

#include "defines.h"

namespace cvedia {
	namespace rt {
		namespace module {
			EXPORT void ONNXInferenceHandlerStatic();

			class OnnxInferenceHandler final : public iface::InferenceHandler {
			public:
				EXPORT OnnxInferenceHandler(std::string const& moduleName);

				EXPORT static std::shared_ptr<iface::InferenceHandler> create(std::string const& moduleName);

				EXPORT expected<void> loadBackend() override;
				EXPORT expected<void> loadModel(std::string const& path) override;

				EXPORT std::vector<std::pair<std::string, std::string>> getDeviceGuids() override;

				EXPORT expected<std::vector<xt::xarray<float>>> runInference(std::vector<Tensor> & inputs) override;
				EXPORT internal::ResourceUsage getResourceUsage() override { return { 0, 0 }; }

				EXPORT expected<void> setDevice(std::string const& guid) override;

				EXPORT std::string getActiveDevice() override { return "ONNX"; }

				EXPORT std::string getDefaultScheme() const override { return "onnx"; }
				EXPORT std::string getDefaultExtension() const override { return ".onnx"; }

				EXPORT pCValue getCapabilities() override;
				EXPORT std::vector<int> getInputShape() override;
				EXPORT std::vector<int> getOutputShape() override;
			private:
				expected<void> readConfig();

				std::unique_ptr<OnnxCore> core_;
				pCValue modelConfNode_;
			};
		}
	}
}
