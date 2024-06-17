/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <vector>
#include <memory>
#include <string>

#include "defines.h"
#include "builtin/moduleimpl.h"
#include "builtin/inferenceregistry.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class InferenceEngine;
		}
		namespace api {
			namespace inference {
				EXPORT void registerPreprocessHandler(std::string const& name, module::preprocessHandler func);
				EXPORT void registerPostprocessHandler(std::string const& name, module::postprocessHandler func);
				EXPORT void registerSchemeHandler(std::string const& protocol, module::createInferenceHandler func);
				EXPORT void registerExtHandler(std::string const& extension, module::createInferenceHandler func);

				NODISCARD EXPORT expected<module::preprocessHandler> getPreProcessHandler(std::string const& handler);
				NODISCARD EXPORT expected<module::postprocessHandler> getPostProcessHandler(std::string const& handler);
				NODISCARD EXPORT expected<module::createInferenceHandler> getSchemeHandler(std::string const& backend);
				NODISCARD EXPORT expected<module::createInferenceHandler> getExtHandler(std::string const& backend);

				NODISCARD EXPORT expected<cvec> runInference(std::string const& modelUri, std::string const& solutionId, cvec const& jobs);
				NODISCARD EXPORT expected<std::pair<size_t, size_t>> queryModel(std::string const& modelUri);
				NODISCARD EXPORT expected<std::vector<unsigned long long>> runInferenceAsync(std::string const& modelUri, std::string const& solutionId, cvec const& jobs);
				NODISCARD EXPORT expected<cvec> getInferenceResults(std::string const& modelUri, std::vector<unsigned long long> const& jobIds);

				NODISCARD EXPORT expected<std::shared_ptr<iface::InferenceEngine>> loadModel(
					std::string const& uri, std::string const& name);
				NODISCARD EXPORT expected<std::shared_ptr<iface::InferenceEngine>> loadModel(
					std::string const& uri, std::string const& name, int poolSize, pCValue const& handlerConfig);
//				NODISCARD EXPORT expected<std::unique_ptr<iface::InferenceEngine>> getBackendByName(std::string const& name);
				NODISCARD EXPORT std::vector<std::pair<std::string, std::string>> getAllDevices();
				NODISCARD EXPORT std::vector<std::string> getAllSchemeHandlers();
				NODISCARD EXPORT std::map<std::string, pCValue> getAllCapabilities();

				NODISCARD EXPORT expected<std::pair<cbuffer, std::vector<Rect2f>>> packJobs(cvec const& jobs, int width, int height, int packingInterpolationMethod = 0);

				EXPORT void removeAllHandlers();

				NODISCARD EXPORT std::vector<std::string> findModels(std::string const& uri);
				NODISCARD EXPORT expected<module::ModelPaths> resolveUri(std::string const& uri);
			};
		}
	}
}
