/*
	SPDX-FileCopyrightText: 2024 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#include <iostream>
#include <thread>

#include "api/logging.h"

#include "api/buffer.h"
#include "api/config.h"
#include "api/instances.h"
#include "api/rt.h"
#include "api/solution_manager.h"
#include "api/solutions.h"
#include "api/system.h"
#include "builtin/functions.h"
#include "interface/buffer.h"
#include "interface/instance_controller.h"
#include "interface/securt.h"

#include "cvalue.h"

#include "argparse/argparse.hpp"
#include <nlohmann/json.hpp>

#include <filesystem.hpp>
#include <set>

using namespace cvedia::rt;
using namespace iface;

namespace fs = ghc::filesystem;

int startInstance(std::string const& input, size_t const& fps);

double getCurrentTimestamp() {
	return static_cast<double>(TIME_IN_MS) / 1000.0;
}

int main(int argc, const char* argv[]) {

	argparse::ArgumentParser program("Push Frame C++ Demo", "v1.0");
	program.add_argument("-i", "--input", "Input directory", true);
	program.add_argument("-s", "--solutions", "Location of the RT solutions", true);
	program.add_argument("-f", "--fps", "FPS rate of the images in the input directory (default 30)", false);
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

	size_t fps = 30;

	if (program.exists("fps")) {
		fps = program.get<size_t>("fps");
	}

	cvedia::rt::api::config::setNode(cvedia::rt::api::config::LOGGING_LOG_TO_CONSOLE, CValue(true));
	if (program.exists("verbose")) {
		cvedia::rt::api::config::setNode(cvedia::rt::api::config::LOGGING_LOG_LEVEL, CValue("verbose"));
	} else {
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

	auto retVal = startInstance(uriStr, fps);

	api::shutdown();

	return retVal;
}

int startInstance(std::string const& input, size_t const& fps) {
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

	// Set the input to manual - This is required for pushing frames - Other option is using RTSP (see other demo)
	if (!securt->setInputToManual()) {
		LOGE << "Failed to set input to manual";
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
	if(!securt->createIntrusionArea(areaId, areaName, areaPolygon, triggeringClasses, areaColor)) {
		LOGE << "Failed to create area";
		return -1;
	}

	// Start the instance
	auto startStatus = instanceCtrl->startInstance();
	if (!startStatus.has_value()) {
		LOGE << "Failed to start instance";
		return -1;
	}

	LOGI << "Instance started - warming up on fake frames";

	for (int i = 0; i < 20; i++) {

		// Create a fake buffer
		auto const buffer = api::buffer::createBuffer(800, 600, BGR);

		// IMPORTANT!!! Set the timestamp - it is used internally for tracking
		buffer->timestamp(getCurrentTimestamp());
		
		// Push the frame to the RT processing algorithm
		if (!securt->pushRawFrame(buffer) ) {
			LOGE << "Failed to push frame";
		}
		// Other frames options:
		// securt->pushCompressedFrame(DATA_PTR, DATA_SIZE, TIMESTAMP_MS);
		// securt->pushH264Frame("H264_DATA", "H264_DATA_SIZE", getCurrentTimestamp());
		// securt->pushH265Frame("H265_DATA", "H265_DATA_SIZE", TIMESTAMP_MS);

		// Simulate a frame rate
		std::this_thread::sleep_for(std::chrono::milliseconds(1000/fps));
	}

	LOGI << "TEST FRAMERATE = " << securt->getCurrentFrameRate() << " FPS - Expected around " << fps << " FPS";

	LOGI << "Instance started - processing frames from directory (" << input << ")";

	fs::path path = input;
	if (!fs::is_directory(path)) {
		LOGE << "Input is not a directory";
		return -1;
	}

	std::set<fs::path> sorted_by_name;

	for (auto& p : fs::directory_iterator(path)) {
		sorted_by_name.insert(p);
	}

	for (auto& p : sorted_by_name) {
		if (!fs::is_regular_file(p)) {
			continue;
		}
		LOGD << "Processing file: " << p.string();

		// Try to import the image
		auto const& bfr = api::buffer::createBufferFromImage(p.string());
		if (!bfr.has_value()) {
			LOGE << "Failed to create buffer from " << p.string();
			continue;
		}

		// IMPORTANT!!! Set the timestamp - it is used internally for tracking
		bfr.value()->timestamp(getCurrentTimestamp());

		// Push the frame to the RT processing algorithm
		if (!securt->pushRawFrame(bfr.value())) {
			LOGE << "Failed to push frame";
		}
		// Other frames options:
		// securt->pushCompressedFrame(DATA_PTR, DATA_SIZE, TIMESTAMP_MS);
		// securt->pushH264Frame("H264_DATA", "H264_DATA_SIZE", getCurrentTimestamp());
		// securt->pushH265Frame("H265_DATA", "H265_DATA_SIZE", TIMESTAMP_MS);

		// Simulate a frame rate
		std::this_thread::sleep_for(std::chrono::milliseconds(1000/fps));

		// Process potential events
		auto events = securt->consumeAllEvents();
		for (auto& event : events) {
			nlohmann::json j = nlohmann::json::parse(event->jsonObject, nullptr, false);
			if (!j.contains("$id") || !j["$id"].is_string()){
				continue;
			}

			auto id = j["$id"].get<std::string>();
			if (id == "event-intrusion") {
				LOGI << "===== INTRUSION - DETECTED =====";
			}
			else if (id == "event-intrusion-end") {
				LOGI << "===== INTRUSION - STOPPED =====";
			} 
			// See other events at: https://bin.cvedia.com/schema/
			LOGV << j.dump(4);
		}
	}

	instanceCtrl->stopInstance(false);

	return 0;
}