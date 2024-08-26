/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#include "onnxcore.h"
#include <xtensor/xadapt.hpp>
#include <xtensor/xsort.hpp>
#include <xtensor/xview.hpp>
#include "api/logging.h"

#include <cstring>

#include "builtin/tensor.h"
#include "builtin/functions.h"
#include "xtensor/xpad.hpp"

using std::make_shared;
using std::vector;
using std::string;
using std::unique_lock;
using std::mutex;

using namespace cvedia::rt;
using namespace cvedia::rt::module;

OnnxCore::OnnxCore(): modelLoaded_(false), numInputNodes_(0) {
}

void OnnxCore::initialize() {
//	Inference::initialize();

}

expected<void> OnnxCore::loadBackend() {

	unique_lock<mutex> m(sessMux_);

	//if (onnxConf->model_file == "") {
	//	LOGE << "No model_file specified";
	//	return;
	//}

	if (pluginConf.trt_cache_enabled) {
		#ifdef _WIN32
		SetEnvironmentVariableA("ORT_TENSORRT_ENGINE_CACHE_ENABLE", "1");
		SetEnvironmentVariableA("ORT_TENSORRT_CACHE_PATH", pluginConf.trt_cache_path.c_str());
		LOGD << "Setting TensorRT cache patch to " << pluginConf.trt_cache_path;
		#endif
	}

	auto envPtr = std::make_unique<Ort::Env>(ORT_LOGGING_LEVEL_ERROR, "ONNXPlugin");
	mEnv_ = std::move(envPtr);

	// initialize session options if needed
	sessionOptions_.SetInterOpNumThreads(pluginConf.ort_interop_num_threads);
	sessionOptions_.SetIntraOpNumThreads(pluginConf.ort_intraop_num_threads);

	if (pluginConf.execution_mode == "PARALLEL") {
		sessionOptions_.SetExecutionMode(ExecutionMode::ORT_PARALLEL);
	}
	else if (pluginConf.execution_mode == "SEQUENTIAL") {
		sessionOptions_.SetExecutionMode(ExecutionMode::ORT_SEQUENTIAL);
	}

	if (pluginConf.profiling) {
		LOGD << "Enabling ONNX profiling";
#ifdef _WIN32
		std::wstring const prof(pluginConf.profiling_prefix.begin(), pluginConf.profiling_prefix.end());
		sessionOptions_.EnableProfiling(prof.c_str());
#else
		sessionOptions_.EnableProfiling(pluginConf.profiling_prefix.c_str());
#endif
	}
	else {
		sessionOptions_.DisableProfiling();
	}

	bool found = false;

	if (iequals(pluginConf.backend, "CPU")) {
		found = true;
		LOGD << "Using CPU only";
	}
#ifdef ONNX_HAS_CUDA
	if (iequals(pluginConf.backend,"CUDA")) {
		found = true;
		LOGD << "Creating CUDA backend";

		sessionOptions_.EnableMemPattern();
		sessionOptions_.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_DISABLE_ALL);

		LOGD << "Using DeviceID " << pluginConf.device_id;
		OrtCUDAProviderOptions cudaOptions{};
		cudaOptions.device_id = pluginConf.device_id;
		sessionOptions_.AppendExecutionProvider_CUDA(cudaOptions);
		LOGD << "CUDA backend added";
	}
#endif
#ifdef ONNX_HAS_DML
	if (iequals(pluginConf.backend, "DIRECTML")) {
		found = true;
		LOGD << "Creating DirectML backend";
		sessionOptions_.DisableMemPattern();
		sessionOptions_.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_DISABLE_ALL);

		LOGD << "Using DeviceID " << pluginConf.device_id;
		Ort::ThrowOnError(OrtSessionOptionsAppendExecutionProvider_DML(sessionOptions_, pluginConf.device_id));
	}
#endif
#ifdef ONNX_HAS_TENSORRT
	if (iequals(pluginConf.backend, "TENSORRT")) {
		found = true;
		LOGD << "Creating TensorRT backend";

		sessionOptions_.EnableMemPattern();
		sessionOptions_.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

		LOGD << "Using DeviceID " << pluginConf.device_id;
		Ort::ThrowOnError(OrtSessionOptionsAppendExecutionProvider_Tensorrt(sessionOptions_, pluginConf.device_id));
	}
#endif
	if (!found) {
		LOGF << "Unsupported backend selected: " << pluginConf.backend;
		return unexpected(RTErrc::FailedInitBackend);
	}

	// create input tensor object from data values
	memoryInfo_ = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

	backendLoaded_ = true;
	return {};
}

pCValue OnnxCore::getCapabilities() const {
	return VAL();
}

std::vector<std::pair<std::string, std::string>> OnnxCore::getDeviceGuids() const {
	std::vector<std::pair<std::string, std::string>> out;

	out.emplace_back(string("cpu"), "CPU");
	out.emplace_back(string("tensorrt"), "TensorRT");
	out.emplace_back(string("directml"), "DirectML");
	out.emplace_back(string("cuda"), "CUDA");

	return out;
}

expected<void> OnnxCore::setDevice(std::string const& device) {
	//if (backendLoaded_) {
	//	unloadBackend();
	//}

	pluginConf.backend = device;

	//if (!loadBackend()) {
	//	return unexpected(RTErrc::NoSuchDevice);
	//}

	return {};
}

expected<void> OnnxCore::loadModel(string const& path) {

	if (!backendLoaded_) {
		LOGE << "Load a valid backend before loading a model!";
		return unexpected(RTErrc::NotInitialized);
	}

	unique_lock<mutex> m(sessMux_);

	try {
		string s = path;

		std::wstring const modelFile(s.begin(), s.end());

		LOGD << "Loading " << path << " in Ort::Session";
#ifdef _WIN32
		mSession_ = std::make_unique<Ort::Session>(*mEnv_, modelFile.c_str(), sessionOptions_);
#else
		mSession_ = std::make_unique<Ort::Session>(*mEnv_, path.c_str(), sessionOptions_);
#endif
		LOGD << "Loading successful";
	}
	catch (Ort::Exception const& e) {
		LOGF << e.what();
		return unexpected(RTErrc::FailedInitBackend);
	}
	catch (...) {
		LOGE << "Exception occured trying to initialize ONNX";
		return unexpected(RTErrc::FailedInitBackend);
	}

	// print model input layer (node names, types, shape etc.)
	Ort::AllocatorWithDefaultOptions allocator;

	// print number of model input nodes
	numInputNodes_ = mSession_->GetInputCount();
	inputNodeNames_ = std::vector<char const*>(numInputNodes_);

	// iterate over all input nodes
	for (std::size_t i = 0; i < numInputNodes_; i++) {
		char const* input_name = mSession_->GetInputName(i, allocator);
		inputNodeNames_[i] = input_name;

		// print input node types
		Ort::TypeInfo type_info = mSession_->GetInputTypeInfo(i);
		auto tensor_info = type_info.GetTensorTypeAndShapeInfo();

		// print input shapes/dims
		inputShape_.clear();
		for (auto const shape : tensor_info.GetShape()) {
			inputShape_.push_back(static_cast<int>(shape));
		}

		type_info.release();
		tensor_info.release();
	}

	//if (inputWidth_ == -1 || inputBatchSize_== -1) {
	//	inputBatchSize_ = 1;
	//	inputWidth_ = 320;
	//	inputHeight_ = 320;

	//	input_node_dims[2] = inputHeight_;
	//	input_node_dims[3] = inputWidth_;
	//}

	std::size_t const numOutputNodes = mSession_->GetOutputCount();
	outputNodeNames_.clear();

	for (unsigned int i = 0; i < numOutputNodes; i++) {
		outputNodeNames_.push_back(mSession_->GetOutputName(i, allocator));
	}

	//	assert(input_tensor.IsTensor());

	modelLoaded_ = true;
	return {};
}

expected<vector<xt::xarray<float>>> OnnxCore::runInference(vector<Tensor>& inputs) {
	if (!backendLoaded_) {
		LOGE << "Load a valid backend before loading a model!";
		return unexpected(RTErrc::NotInitialized);
	}

	// Remove the unique_lock
	// unique_lock<mutex> m(sessMux_);

	vector<xt::xarray<float>> output;

	if (inputs.empty())
		return output;

	auto mt = inputs[0].move<float>();
	auto inputTensorSize = static_cast<unsigned int>(std::accumulate(std::begin(inputShape_), std::end(inputShape_), static_cast<int64_t>(1), std::multiplies<>()));
	vector<int64_t> localShape(inputShape_.begin(), inputShape_.end());

	bool dynamic_size = false;
	for (size_t i = 0; i < localShape.size(); i++) {
		if (localShape[i] == -1) {
			localShape[i] = static_cast<int64_t>(mt.shape()[i]);
			dynamic_size = true;
		}
	}

	if (mt.shape()[0] < static_cast<unsigned int>(localShape[0]))
	{
		// pad the tensor to match the network input size.... pretty ugly
		mt = xt::pad(mt, { {0,localShape[0] - mt.shape()[0]}, {0,0}, {0,0}, {0,0} });
	}

	if (dynamic_size) {
		auto const data_tensor_size = static_cast<unsigned int>(std::accumulate(std::begin(mt.shape()), std::end(mt.shape()), static_cast<int64_t>(1), std::multiplies<>()));
		inputTensorSize = data_tensor_size;
	}

	// Resize inputBatchTensor_ only when necessary
	if (inputBatchTensor_.size() != inputTensorSize) {
		inputBatchTensor_.resize(inputTensorSize);
	}

	// Use xt::adapt directly with the input tensor data
	input_tensor = Ort::Value::CreateTensor<float>(memoryInfo_, mt.data(), inputTensorSize, localShape.data(), 4);

	auto outputTensors = mSession_->Run(Ort::RunOptions{ nullptr }, inputNodeNames_.data(), &input_tensor, inputNodeNames_.size(), outputNodeNames_.data(), outputNodeNames_.size());

	for (auto& outputTensor : outputTensors) {
		auto* data = outputTensor.GetTensorMutableData<float>();
		auto shape = outputTensor.GetTensorTypeAndShapeInfo().GetShape();

		auto const outputSize = static_cast<size_t>(std::accumulate(std::begin(shape), std::end(shape), static_cast<int64_t>(1), std::multiplies<>()));

		std::vector<size_t> sizetShape(shape.begin(), shape.end());
		auto xarr = xt::adapt(data, outputSize, xt::no_ownership(), sizetShape);

		output.emplace_back(xarr);
	}

	input_tensor.release();

	return output;
}

void OnnxCore::unloadBackend() {

	unique_lock<mutex> m(sessMux_);

	if (input_tensor != nullptr)
		input_tensor.release();
	if (sessionOptions_ != nullptr)
		sessionOptions_.release();
	//if (mSession != nullptr)
	//	mSession->release();
	if (memoryInfo_ != nullptr)
		memoryInfo_.release();
	//if (mEnv != nullptr)
	//	mEnv->release();

	if (!inputBatchTensor_.empty()) {
		inputBatchTensor_.clear();
		inputBatchTensor_.resize(0);
	}

	mSession_ = nullptr;
	mEnv_ = nullptr;

	backendLoaded_ = false;
}

OnnxCore::~OnnxCore()
{
	unloadBackend();
}
