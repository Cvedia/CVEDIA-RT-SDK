/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#include <deque>
#include <future>
#include <memory>
#include <string>
#include <map>
#include <unordered_set>

#include "cvaluefwd.h"
#include "rterror.h"
#include "interface/inferenceengine.h"

namespace cvedia {
	namespace rt {
		struct RouteConfig
		{
			std::string deviceId;
			int poolSize;
			std::string variant;
		};

		struct Job
		{
			std::string modelUri;
			long long timestamp;
			pCValue data;
			std::promise<pCValue> promise;
		};

		class Worker
		{
		public:
			EXPORT Worker(std::string const& modelUri, int poolSize);
			EXPORT ~Worker();

			EXPORT size_t getQueueSize() const;
			EXPORT expected<void> enqueueJob(Job job);
		private:
			void _queueProcessingThread();

			std::condition_variable cv_;
			mutable std::mutex queueMutex_;
			// Pair of routing key and job
			std::deque<Job> queue_;

			std::vector<std::thread> threads_;
			std::string deviceId_;
			std::string modelUri_;
			std::shared_ptr<iface::InferenceEngine> inferenceManaged_;
			long long lastRun_;
			int poolSize_;
			bool isRunning_ = true;

			static std::shared_mutex totalJobsQueueMutex_;
			static int totalJobsQueue_;
		};

		// Every device has its own specific job queue. Many routings can share this queue.
		class DeviceGroup
		{
		public:
			EXPORT ~DeviceGroup();

			EXPORT expected<void> enqueueJob(std::string const& modelUri, pCValue const& job, std::promise<pCValue> promise);
			EXPORT size_t getQueueSize(std::string const& modelUri) const;
			EXPORT bool hasModel(std::string const& modelUri) const;
			EXPORT expected<void> loadModel(std::string const& modelUri, RouteConfig const& config);
			EXPORT expected<void> unloadModel(std::string const& modelUri);
		private:
			mutable std::mutex workersMutex_;

			// Pair of model uri and worker
			std::map<std::string, std::shared_ptr<Worker>> workers_;
		};

		class InferenceDispatcher {
		public:
			EXPORT static expected<void> initialize();
			EXPORT static void shutdown();

			EXPORT static expected<cvec> runInference(std::string const& modelUri, cvec const& jobs);

			EXPORT static expected<void> addRoute(std::string const& routingPath, std::string const& deviceId, int poolSize, std::string const& variant);
			EXPORT static expected<void> removeRoute(std::string const& routingPath, std::string const& deviceId);
			EXPORT static std::vector<std::pair<std::string, RouteConfig>> getRoutes();
			EXPORT static std::vector<RouteConfig> getRoutes(std::string const& routingPath);
		protected:
			EXPORT static expected<std::vector<RouteConfig>> _pathToRouteConfig(std::string const& modelPath);

		private:
			static expected<void> _parseRouteFromConfig(std::string const& path, pCValue const& routeConfig);

			static expected<void> enqueueJob(std::string const& modelUri, pCValue const& job, std::promise<pCValue>);

			// Map of routing path and its device ID's
			static std::map<std::string, std::vector<RouteConfig>> routingConfigs_;
			static std::shared_mutex routingConfigsMutex_;

			// Map of device id and its queue
			static std::map<std::string, std::shared_ptr<DeviceGroup>> deviceQueues_;
			static std::shared_mutex deviceQueuesMutex_;
		};
	}
}

