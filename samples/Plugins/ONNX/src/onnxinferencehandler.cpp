/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#include <plog/Init.h>

#include "api/inference.h"
#include "onnxinferencehandler.h"

using std::shared_ptr;
using std::string;
using std::vector;

using namespace cvedia::rt::module;
using namespace cvedia::rt;

extern "C" EXPORT void registerHandler();
extern "C" EXPORT void logInit(plog::Severity severity, plog::IAppender * appender);

#if LINK_ONNX_STATIC
void cvedia::rt::module::ONNXInferenceHandlerStatic()
{
	api::inference::registerSchemeHandler("onnx", &OnnxInferenceHandler::create);
	api::inference::registerExtHandler(".onnx", &OnnxInferenceHandler::create);
}
#else
extern "C" EXPORT void registerHandler();
extern "C" EXPORT void logInit(plog::Severity severity, plog::IAppender * appender);

void logInit(plog::Severity severity, plog::IAppender* appender)
{
	plog::init(severity, appender); // Initialize the shared library logger.
}

void registerHandler() {
	api::inference::registerSchemeHandler("onnx", &OnnxInferenceHandler::create);
	api::inference::registerExtHandler(".onnx", &OnnxInferenceHandler::create);
}
#endif

std::shared_ptr<iface::InferenceHandler> OnnxInferenceHandler::create(string const& moduleName) {
	return std::make_shared<OnnxInferenceHandler>(moduleName);
}

OnnxInferenceHandler::OnnxInferenceHandler(string const& moduleName) : InferenceHandler(moduleName) {
	core_ = std::make_unique<OnnxCore>();
}

std::vector<std::pair<std::string, std::string>> OnnxInferenceHandler::getDeviceGuids() {
	std::vector<std::pair<std::string, std::string>> out;

	auto const devices = core_->getDeviceGuids();
	for (auto const& device : devices) {
		auto guid = "ONNX." + device.first;

		std::transform(guid.begin(), guid.end(), guid.begin(), [](unsigned char const c) { return static_cast<unsigned char>(std::tolower(c)); });
		out.emplace_back(guid, device.second);
	}

	return out;
}

expected<void> OnnxInferenceHandler::loadBackend() {
	readConfig();

	return core_->loadBackend();
}

expected<void> OnnxInferenceHandler::loadModel(string const& path) {
	readConfig();

	TRY(core_->loadModel(path));

	if (core_->modelNode_) {
		modelConfNode_->setValue(*core_->modelNode_);
	}

	return {};
}

expected<std::vector<xt::xarray<float>>> OnnxInferenceHandler::runInference(vector<Tensor> & inputs) {
	return core_->runInference(inputs);
}

expected<void> OnnxInferenceHandler::readConfig() {
	getConfigWithReadLock(config);

	modelConfNode_ = config->getChildOrCreate("model");

	config->getValueIfSet<string>("backend", core_->pluginConf.backend);
	config->getValueIfSet<string>("execution_mode", core_->pluginConf.execution_mode);
	
	config->getValueIfSet<bool>("profiling", core_->pluginConf.profiling);
	config->getValueIfSet<string>("profiling_prefix", core_->pluginConf.profiling_prefix);
	
	config->getValueIfSet<int>("device_id", core_->pluginConf.device_id);
	
	config->getValueIfSet<string>("trt_cache_path", core_->pluginConf.trt_cache_path);
	config->getValueIfSet<bool>("trt_cache_enabled", core_->pluginConf.trt_cache_enabled);

	return {};
}

expected<void> OnnxInferenceHandler::setDevice(std::string const& guid) {
	return core_->setDevice(guid);
}

pCValue OnnxInferenceHandler::getCapabilities() {
	return core_->getCapabilities();
}

std::vector<int> OnnxInferenceHandler::getInputShape() {
	return core_->getInputShape();
}

std::vector<int> OnnxInferenceHandler::getOutputShape() {
	return core_->getOutputShape();
}
