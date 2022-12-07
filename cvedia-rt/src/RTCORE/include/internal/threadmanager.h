/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once 

#include <shared_mutex>
#include <memory>
#include <thread>
#include <utility>

#include "api/thread.h"

#include "defines.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class Instance;
			class Solution;
		}

		namespace internal {
			class VersionControl;
		}

		class PluginState {
		public:
			PluginState(pCValue configVal, pCValue stateVal, int versionVal) : version(versionVal), config(std::move(configVal)), state(std::move(stateVal)) {
				
			}

			int version;
			pCValue const config;
			pCValue const state;
		};

		namespace internal {

#define PLUGIN_STATE std::unique_ptr<PluginState>

			class ThreadContext {
			public:
				std::thread th;
				StorageType storage = StorageType::Global;
				ThreadType type = ThreadType::Global;
				int curVersion = 1;
				std::weak_ptr<iface::Instance> instance;
				std::weak_ptr<iface::Solution> solution;
				std::map<std::string, uint32_t> counters;
			};

			class ThreadManager {
			public:
				static int getThreadId();
				NODISCARD static expected<void> registerThread(ThreadType t, int version = 1);
				NODISCARD static expected<void> registerThread(std::thread th, ThreadType t, int version = 1);
				NODISCARD static expected< std::shared_ptr<iface::Instance>> getActiveInstance();
				NODISCARD static expected< std::shared_ptr<iface::Solution>> getActiveSolution();
				NODISCARD static expected<void> setActiveInstance(std::weak_ptr<iface::Instance> instance);
				NODISCARD static expected<void> clearActiveInstance();
				NODISCARD static expected<void> setActiveSolution(std::weak_ptr<iface::Solution> solution);
				static bool isWorkerThread();
				NODISCARD static expected<void> unregisterThread();
				NODISCARD static expected<void> unregisterThread(std::shared_ptr<std::thread> const& th);
				NODISCARD static expected<std::shared_ptr<ThreadContext>> getThreadContext();
				NODISCARD static expected<std::shared_ptr<ThreadContext>> getThreadContextById(std::thread::id id);
				NODISCARD static expected<void> setThreadVersion(int version);
				NODISCARD static expected<void> stepThread(int steps = 1);
//				EXPORT NODISCARD static expected<std::shared_ptr<LuaEngine>> getLuaEngine();
#if WITH_VERSION_CONTROL
				static std::vector<int> getThreadVersionsByVC(std::shared_ptr<VersionControl> const& versioning);
#endif
				NODISCARD static expected<int> getThreadVersion();
				static int getThreadCount(std::weak_ptr<iface::Instance> instance, ThreadType type);
				static expected<void> waitForThreads(std::weak_ptr<iface::Instance> instance, ThreadType type);
				static expected<void> waitForThread(std::thread::id id);
				static std::vector<std::thread::id> getJoinableThreadIds(std::weak_ptr<iface::Instance> instance, ThreadType type);
				//NODISCARD static expected<std::unique_lock<__shared_mutex_class>> getWriteLock();
				//NODISCARD static expected<std::shared_lock<__shared_mutex_class>> getReadLock();
				static void unregisterAllThreads();
				NODISCARD static expected<float> getCounterValue(std::string const& name);
				NODISCARD static expected<void> setCounterValue(std::string const& name, uint32_t value);
				NODISCARD static expected<void> setThreadStorage(StorageType s);
				NODISCARD static expected<StorageType> getThreadStorage();
				NODISCARD static int getThreadCount();
			private:
				static std::map<std::thread::id, std::shared_ptr<ThreadContext>> ctxThreadMap_;
			};
		}
	}
}
