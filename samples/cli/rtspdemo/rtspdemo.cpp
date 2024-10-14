/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#include <iostream>
#include <thread>

#include "api/logging.h"

#include "api/config.h"
#include "interface/securt.h"
#include "interface/instance_controller.h"
#include "api/system.h"
#include "api/rt.h"
#include "api/solutions.h"
#include "api/instances.h"
#include "builtin/functions.h"
#include "api/solution_manager.h"

#include "cvalue.h"

#include "argparse/argparse.hpp"

using namespace cvedia::rt;
using namespace iface;

int startInstance(std::string const& input);

std::map<std::string, std::shared_ptr<iface::InstanceController>> instances;

int main(int argc, const char* argv[]) {

	argparse::ArgumentParser program("RTSP Demo", "v0.1");

	program.add_argument("-i", "--input", "Input URL's", true);
	program.add_argument("-s", "--solutions", "Location of the RT solutions", true);
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

	cvedia::rt::api::config::setNode(cvedia::rt::api::config::LOGGING_LOG_TO_CONSOLE, CValue(true));
	if (program.exists("verbose")) {
		cvedia::rt::api::config::setNode(cvedia::rt::api::config::LOGGING_LOG_LEVEL, CValue("verbose"));
	}
	else {
		cvedia::rt::api::config::setNode(cvedia::rt::api::config::LOGGING_LOG_LEVEL, CValue("info"));
	}

	auto initRet = api::initialize();
	if (!initRet.has_value()) {
		LOGE << "Failed to initialize RT API";
		return -1;
	}
	auto discoverRet = api::solutions::discoverSolutions(program.get<std::string>("solutions"));
	if (!discoverRet.has_value()) {
		LOGE << "Failed to discover solutions";
		api::shutdown();
		return -1;
	}

	if (!api::solution_manager::hasSolutionManagerHandler("securt")) {
		LOGE << "SecuRT solution not found";
		api::shutdown();
		return -1;
	}

	auto uriStr = program.get<std::string>("input");
	
	// split uris string by space
	auto uris = split_str(uriStr, ' ');
	for (auto& uri : uris) {
		startInstance(uri);
	}

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	api::shutdown();

	return 0;
}

int startInstance(std::string const& input) {

	auto instanceGUID = Uuid::randomUuid();
	auto solutionID = "securt";

	// Create instance controller - This allows controlling the instance lifecycle
	auto instanceCtrl = api::instances::createInstanceControllerFromMemory(instanceGUID, solutionID).value_or(nullptr);
	if (!instanceCtrl) {
		LOGE << "Failed to create instance controller";
		return -1;
	}

	// Load the solution manager - This is the main entry point for setting the solution
	auto eSolMgr = instanceCtrl->getSolutionManagerOrCreate();
	if (!eSolMgr.has_value()) {
		LOGE << "Failed to get solution manager";
		return -1;
	}

	// Cast the solution manager to SecuRT - Enables adding areas etc.
	auto securt = std::dynamic_pointer_cast<iface::SecuRT>(eSolMgr.value());
	if (!securt) {
		LOGE << "Failed to cast to SecuRT";
		return -1;
	}

	// Load the instance to memory - This must be called after the creation of the solution manager
	if (!instanceCtrl->loadInstance().has_value()) {
		LOGE << "Failed to load instance";
		return -1;
	}

	if (!securt->setInputToRtsp(input)) {
		LOGE << "Failed to set input to RTSP stream " << input;
		return -1;
	}

	// Create an intrusion area / zone
	std::string areaId = "area1";
	std::string areaName = "area1";
	std::vector<Point2f> areaPolygon = {
		Point2f(0.3f, 0.3f),
		Point2f(0.7f, 0.3f),
		Point2f(0.7f, 0.7f),
		Point2f(0.3f, 0.7f)
	};
	std::unordered_set<SecuRT::Classes> triggeringClasses = {
		SecuRT::Classes::Person,
		SecuRT::Classes::Vehicle
	};
	std::array<double, 4> areaColor = { 0.0, 0.0, 0.0, 0.0 }; // BGRA
	if (!securt->createIntrusionArea(areaId, areaName, areaPolygon, triggeringClasses, areaColor)) {
		LOGE << "Failed to create area";
		return -1;
	}

	// Start the instance
	auto startStatus = instanceCtrl->startInstance();
	if (!startStatus.has_value()) {
		LOGE << "Failed to start instance";
		return -1;
	}

	return 0;
}