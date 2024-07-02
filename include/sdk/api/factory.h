/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <map>
#include <memory>

#include "defines.h"
#include "rterror.h"
#include "cvalue.h"
#include "uuid.h"

namespace cvedia {
	namespace rt {
		enum Codec : size_t;
		namespace iface {
			class WsDiscovery;
			class VideoDecoder;
			class Exporter;
			class Visualizer;
			class DatasetHandler;
			class VideoReader;
			class ImageReader;
			class Tripwire;
			class LuaWorkers;
			class InstanceController;
			class WriteData;
			class MQTT;
			class ZMQ;
			class CocoEval;
			class ClassifierEval;
			class EventDispatcher;
			class EventStorage;
			class EventAction;
			class Input;
			class Output;
			class REST;
			class Zone;
			class Tracker;
			class MotionDetection;
			class InferenceEngine;
			class ScriptEngine;
			class Instance;
			class Solution;
			class CVDict;
			class AmbaOut;
			class TopBus;
			class NBus;
			class Renderer;
			class Encryption;
		}
	}
}

namespace cvedia {
	namespace rt {
		namespace api {
			namespace factory {
				template <class T, typename ...Args>
				class Factory
				{
					typedef std::unique_ptr<T> (*createFunc)(Args && ...args);

				public:
					NODISCARD EXPORT static cvedia::rt::expected<std::shared_ptr<T>> create(std::string const& implementation, Args && ...args);
					EXPORT static void registerImplementation(std::string const& implementation, createFunc);
					EXPORT static void provideExistingInstance(std::shared_ptr<T>);
				private:
					static std::shared_ptr<T> instance_;
					static std::map<std::string, createFunc> func_;
				};

				void initialize();

				std::string const DEFAULT = "DEFAULT";

				// Define convenience alias
				using CVDict = Factory<cvedia::rt::iface::CVDict>;
				using Solution = Factory<cvedia::rt::iface::Solution, std::string const&>;
				using Instance = Factory<cvedia::rt::iface::Instance, Uuid const&, std::string const&, pCVDict const&>;
				using DatasetHandler = Factory<cvedia::rt::iface::DatasetHandler, std::string const&>;
				using LuaWorkers = Factory<cvedia::rt::iface::LuaWorkers, std::weak_ptr<iface::Instance> const&>;
				using CocoEval = Factory<cvedia::rt::iface::CocoEval>;
				using ClassifierEval = Factory<cvedia::rt::iface::ClassifierEval>;
				using InstanceController = Factory<cvedia::rt::iface::InstanceController>;
				using Input = Factory<cvedia::rt::iface::Input, std::string const&>;
				using Output = Factory<cvedia::rt::iface::Output, std::string const&, std::weak_ptr<cvedia::rt::iface::Instance> const&>;
				using ImageReader = Factory<cvedia::rt::iface::ImageReader, std::string const&>;
				using VideoReader = Factory<cvedia::rt::iface::VideoReader, std::string const&>;
				using EventAction = Factory<cvedia::rt::iface::EventAction, std::string const&>;
				using EventStorage = Factory<cvedia::rt::iface::EventStorage, std::shared_ptr<CValue>>;
				using EventDispatcher = Factory<cvedia::rt::iface::EventDispatcher>;
				using Exporter = Factory<cvedia::rt::iface::Exporter>;
				using InferenceEngine = Factory<cvedia::rt::iface::InferenceEngine, std::string const&>;
				using MotionDetection = Factory<cvedia::rt::iface::MotionDetection, std::string const&>;
				using Zone = Factory<cvedia::rt::iface::Zone, std::string const&>;
				using REST = Factory<cvedia::rt::iface::REST, std::string const&>;
				using Tripwire = Factory<cvedia::rt::iface::Tripwire, std::string const&>;
				using Tracker = Factory<cvedia::rt::iface::Tracker, std::string const&>;
				using WriteData = Factory<cvedia::rt::iface::WriteData, std::string const&>;
				using MQTT = Factory<cvedia::rt::iface::MQTT, std::string const&>;
				using ZMQ = Factory<cvedia::rt::iface::ZMQ, std::string const&>;
				using ScriptEngine = Factory<cvedia::rt::iface::ScriptEngine, std::string const&, bool>;
				using AmbaOut = Factory<cvedia::rt::iface::AmbaOut, std::string const&>;
				using TopBus = Factory<cvedia::rt::iface::TopBus, std::string const&>;
				using NBus = Factory<cvedia::rt::iface::NBus, std::string const&>;
				using Renderer = Factory<cvedia::rt::iface::Renderer>;
				using Visualizer = Factory<cvedia::rt::iface::Visualizer>;
				using WsDiscovery = Factory<cvedia::rt::iface::WsDiscovery>;
				using VideoDecoder = Factory<cvedia::rt::iface::VideoDecoder, std::string const&, Codec const&>;
				using Encryption = Factory<cvedia::rt::iface::Encryption, std::string const&>;
			}
		}
	}
}
