/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#include <iostream>

#include "api/logging.h"
#include "cvalue.h"
#include "api/convert.h"
#include "api/factory.h"
#include "api/rt.h"
#include "argparse/argparse.hpp"
#include "interface/inferenceengine.h"
#include "interface/input.h"
#include "interface/module.h"

using namespace cvedia::rt;
using namespace iface;

int main(int argc, const char* argv[]) {

	std::shared_ptr<Instance> inst;
	std::shared_ptr<Module> plugin;

	argparse::ArgumentParser program("CVEDIA-RT Run Inference", "v0.2");

	program.add_argument("-i", "--image", "Path to input image", true);
	program.add_argument("-m", "--model", "Path to CNN Model", true);
	program.add_argument("-v", "--verbose", "", false);

	auto err = program.parse(argc, argv);
	if (err) {
		std::cout << err << std::endl;
		program.print_help();
		return -1;
	}

	if (program.exists("help")) {
		program.print_help();
		return 0;
	}

	// Discover and register all plugins
	CHECK_RET(api::initialize(true, program.exists("verbose")));

	// Cast to managed inference
	auto inference = api::factory::InferenceEngine::create(api::factory::DEFAULT, "-").value();

	auto const uri = program.get<std::string>("m");

	// Load an AI model using the ONNX backend
	if (!inference->loadModel(uri)) {
		std::cout << "Failed to load model" << std::endl;
		return -1;
	}

	// Cast to managed input
	auto input = api::factory::Input::create(api::factory::DEFAULT, "Input").value();

	auto src = program.get<std::string>("i");

	if (input->setSource(src)) {
		while (auto ret = input->readFrame())
		{
			auto cm = cmap();
			cm["source"] = VAL(ret.value()[0]->getValue<cbuffer>("image").value());
			cm["width"] = VAL(1.0f);
			cm["height"] = VAL(1.0f);

			// Run actual inference (forward pass)
			auto res = inference->runInference(cvec{VAL(cm)});
			if (!res.has_value()) {
				std::cout << "Failed to run Inference: " << res.error().message() << std::endl;
				return -1;
			}
			
			std::cout << api::convert::cvalueToJson(*res.value(), false, false, false);
		}
	}

	/*cvec cv = *res->getValue<cvec>();

	size_t cnt = cv.size();
	if (cnt == 0) {
		std::cout << "No detections" << std::endl;
		return -1;
	}

	std::cout << "Detected " << cnt << " objects" << std::endl;

	int idx = 0;

	for (auto &cventry : cv) {
		cmap entry = *cventry->getValue<cmap>();

		int x = *entry["x"]->getValue<int>();
		int y = *entry["y"]->getValue<int>();
		int width = *entry["width"]->getValue<int>();
		int height = *entry["height"]->getValue<int>();
		int classid = *entry["label"]->getValue<int>();
		float confidence = *entry["confidence"]->getValue<float>();

		std::cout << (idx + 1) << ": x=" << x << " y=" << y <<
			" w=" << width << " h=" << height <<
			" classid=" << classid << " confidence=" << confidence << std::endl;

		idx++;
	}*/

	api::shutdown();

	return 0;
}
