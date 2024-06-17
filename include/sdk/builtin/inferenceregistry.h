/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once 

#include <memory>
#include <expected.hpp>          // for expected
#include <map>                   // for map
#include <string>                // for string
#include <utility>               // for pair
#include <vector>                // for vector

#include "builtin/moduleimpl.h"  // for createInferenceHandler
#include "defines.h"             // for NODISCARD, pCValue
#include "rtconfig.h"            // for shared_mutex_class
#include <xtensor/xarray.hpp>    // for xarray_container
#include "rterror.h"
#include "api/factory.h"

namespace cvedia { namespace rt { class CValue; } }
namespace cvedia { namespace rt { class Tensor; } }  // lines 19-19
namespace cvedia { namespace rt { namespace iface { class InferenceEngine; } } }  // lines 26-26
namespace cvedia { namespace rt { namespace internal { struct InferenceContext; } } }  // lines 22-22


namespace cvedia {
	namespace rt {

		namespace module {
			struct ModelPaths {
				module::createInferenceHandler handler;
				std::string architecture;
				std::string device;
				std::string capability;
				std::string domain;
				std::string variant;
				std::string version;
				std::string filepath;
				std::string uri;
				std::string query;
			};

#ifdef _WIN32
			typedef expected<std::vector<std::vector<Tensor>>>(__stdcall* preprocessHandler)(internal::InferenceContext& ctx, CValue* inferenceConf);
			typedef expected<cvec>(__stdcall* postprocessHandler)(internal::InferenceContext& ctx, std::vector<xt::xarray<float>> & output, CValue* inferenceConf);
#else
			typedef expected<std::vector<std::vector<Tensor>>>(*preprocessHandler)(internal::InferenceContext& ctx, CValue* inferenceConf);
			typedef expected<cvec>(*postprocessHandler)(internal::InferenceContext& ctx, std::vector<xt::xarray<float>>& output, CValue* inferenceConf);
#endif
		}

		namespace internal {
			class InferenceRegistry final {
			public:
				static expected<module::ModelPaths> resolveUri(std::string const& uri);
				static void initialize();
				static void reset();

				//! \cond
				static void registerSchemeHandler(std::string const& scheme, module::createInferenceHandler func);
				static void registerExtHandler(std::string const& extension, module::createInferenceHandler func);
				//! \endcond

				NODISCARD static expected<module::createInferenceHandler> getSchemeHandler(std::string const& backend);
				NODISCARD static expected<module::createInferenceHandler> getExtHandler(std::string const& backend);
				NODISCARD static expected<module::preprocessHandler> getPreProcessHandler(std::string const& handler);
				NODISCARD static expected<module::postprocessHandler> getPostProcessHandler(std::string const& handler);

				NODISCARD static expected<std::shared_ptr<iface::InferenceEngine>> createBackend(std::string const& name, std::string const& implementation = api::factory::DEFAULT);
//				NODISCARD static expected<std::shared_ptr<iface::InferenceEngine>> getBackendByName(std::string const& name);

				NODISCARD static std::vector<std::string> getAllSchemeHandlers();
				NODISCARD static std::vector<std::pair<std::string, std::string>> getAllDevices();
				NODISCARD static std::map<std::string, pCValue> getAllCapabilities();

				//! \cond
				static void registerPreProcessHandler(std::string name, module::preprocessHandler func);
				static void registerPostProcessHandler(std::string name, module::postprocessHandler func);
				//! \endcond

			private:
				//! \cond
				static std::map<std::string, module::createInferenceHandler> schemeHandlers_;
				static std::map<std::string, module::createInferenceHandler> extHandlers_;
				//! \endcond

				//! \cond
				static std::map<std::string, module::preprocessHandler> preHandlers_;
				static std::map<std::string, module::postprocessHandler> postHandlers_;
				//! \endcond

				static std::map<std::string, std::shared_ptr<iface::InferenceEngine>> loadedModels_;

				//static std::vector<std::pair<std::string, std::string>> foundDevices_;

				static std::shared_mutex muxBackends_;
			};
		}
	}
}

