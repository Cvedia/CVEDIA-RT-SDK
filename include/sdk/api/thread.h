/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <memory>

#include "defines.h"
#include "rterror.h"

namespace cvedia {
	namespace rt {
		namespace internal {
			class ThreadContext;
		}

		enum class ThreadType {
			Global,
			Instance,
			Worker
		};

		namespace iface {
			class Solution;
			class Instance;
		}

		enum class StorageType
		{
			Global,
			Solution,
			Instance,
			None
		};

		enum class Affinity {
			Powersave,
			Performance
		};

		namespace api {
			namespace thread {
				namespace current {
					EXPORT bool isWorkerThread();
					// get/set ActiveInstance
					// get/set Storage
					// get/set Counter
					// get/set Version
				}
				EXPORT void sleep(int ms);
				NODISCARD EXPORT expected<std::shared_ptr<internal::ThreadContext>> getThreadContext();
				NODISCARD EXPORT expected<int> getThreadVersion();
				NODISCARD EXPORT expected<std::shared_ptr<iface::Solution>> getActiveSolution();
				NODISCARD EXPORT expected<void> setActiveSolution(std::weak_ptr<iface::Solution> solution);
				NODISCARD EXPORT expected<std::shared_ptr<iface::Instance>> getActiveInstance();
				NODISCARD EXPORT expected<void> setActiveInstance(std::weak_ptr<iface::Instance> instance);
				NODISCARD EXPORT expected<void> pinOnCpuCore();
				NODISCARD EXPORT expected<void> clearActiveInstance();
				NODISCARD EXPORT expected<void> registerThread(ThreadType t, int version = 1, std::weak_ptr<iface::Instance> instance = {});
				NODISCARD EXPORT expected<void> registerThread(std::thread&& th, ThreadType t, int version = 1);
				NODISCARD EXPORT expected<void> unregisterThread();
				EXPORT void setPerformanceMode(Affinity mode);
				NODISCARD EXPORT expected<void> setThreadStorage(StorageType s); // TODO: @AJ setActiveStorage
				NODISCARD EXPORT expected<int> getVersion();
				NODISCARD EXPORT expected<void> setVersion(int version);
				NODISCARD EXPORT expected<StorageType> getThreadStorage();
				NODISCARD EXPORT expected<void> step(int steps = 1);
				NODISCARD EXPORT int getThreadCount(std::weak_ptr<iface::Instance> instance, ThreadType type);
				NODISCARD EXPORT expected<float> getCounterValue(const std::string& name);
				NODISCARD EXPORT expected<void> setCounterValue(const std::string& name, uint32_t value);

				NODISCARD EXPORT expected<void> waitForThreads(std::weak_ptr<iface::Instance> instance, ThreadType type);
				NODISCARD EXPORT expected<void> waitForThread(std::thread::id id);

				EXPORT expected < std::unique_lock<shared_mutex_class>> getWriteLock(); // TODO: @AJ to remove
				EXPORT expected < std::shared_lock<shared_mutex_class>> getReadLock(); // TODO: @AJ to remove

			};
		}
	}
}
