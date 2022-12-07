/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <vector>
#include <memory>
#include <string>

#include "defines.h"
#include "builtin/moduleimpl.h"
#include "internal/inferenceregistry.h"

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

				EXPORT expected<module::preprocessHandler> getPreProcessHandler(std::string const& handler);
				EXPORT expected<module::postprocessHandler> getPostProcessHandler(std::string const& handler);
				EXPORT expected<module::createInferenceHandler> getSchemeHandler(std::string const& backend);
				EXPORT expected<module::createInferenceHandler> getExtHandler(std::string const& backend);

				NODISCARD EXPORT expected<std::shared_ptr<iface::InferenceEngine>> createInferenceEngine(std::string const& name);
				NODISCARD EXPORT expected<std::shared_ptr<iface::InferenceEngine>> loadModel(
					std::string const& uri, std::string const& name);
				NODISCARD EXPORT expected<std::shared_ptr<iface::InferenceEngine>> loadModel(
					std::string const& uri, std::string const& name, int poolSize, pCValue const& handlerConfig);
				EXPORT expected<std::shared_ptr<iface::InferenceEngine>> getBackendByName(std::string const& name);
				EXPORT std::vector<std::pair<std::string, std::string>> getAllDevices();
				EXPORT void removeAllHandlers();

				EXPORT expected<module::ModelPaths> resolveUri(std::string const& uri);
			};
		}
	}
}
