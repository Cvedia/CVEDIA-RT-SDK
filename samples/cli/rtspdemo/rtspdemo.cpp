/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#include <iostream>
#include <thread>

#include "api/logging.h"

#include "interface/securt.h"
#include "interface/instance_controller.h"
#include "api/system.h"
#include "api/rt.h"
#include "api/solutions.h"
#include "api/instances.h"
#include "builtin/functions.h"

#include "argparse/argparse.hpp"

using namespace cvedia::rt;
using namespace iface;

bool startInstance(std::string const& input);

std::map<std::string, std::shared_ptr<iface::InstanceController>> instances;

int main(int argc, const char* argv[]) {

	argparse::ArgumentParser program("RTSP Demo", "v0.1");

	program.add_argument("-i", "--input", "Input URL's", true);
	program.add_argument("-s", "--solutions", "Location of the RT solutions", true);
	program.add_argument("-o", "--output", "Path to output image with drawn inference results", false);
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

	(void)api::initialize();
	(void)api::solutions::discoverSolutions(program.get<std::string>("solutions"));

	auto uriStr = program.get<std::string>("input");
	
	// split uris string by space
	auto uris = split_str(uriStr, ' ');
	for (auto& uri : uris) {
		startInstance(uri);
	}

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}

bool startInstance(std::string const& input) {

	if (auto mgr = api::instances::createInstanceControllerFromMemory(Uuid::randomUuid(), "securt").value_or(nullptr))
	{
		auto srt = std::dynamic_pointer_cast<iface::InstanceController>(mgr);
		instances[input] = srt;

		auto inst = instances[input];

		inst->startInstance();
		if (auto resp = inst->getSolutionManager()) {
			auto sm = std::dynamic_pointer_cast<iface::SecuRT>(resp.value());
			if (sm) {
				sm->setFrameRateLimit(10);
				sm->setInputToRtsp(input);

				// fullscreen area (relative)
				std::vector<Point2f> area = {
					Point2f(0.0f, 0.0f),
					Point2f(1.0f, 0.0f),
					Point2f(1.0f, 1.0f),
					Point2f(0.0f, 1.0f)
				};

				//	inst->enableVehicleClassification();
				return sm->createLoiteringArea("area1", "area1", area, { iface::SecuRT::Classes::Person, iface::SecuRT::Classes::Vehicle }, 1, { 0,0,0,0 });
			}
		}
	}

	return false;
}