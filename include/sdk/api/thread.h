/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <thread>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <memory>

#include "defines.h"
#include "rterror.h"

#include <tracy/Tracy.hpp>

#define REGISTER_THREAD_SCOPED(threadType) \
	auto &&__registrationScopeExp = ::cvedia::rt::api::thread::registerThreadScoped(threadType, 1, __LOCATION__); \
	if (!__registrationScopeExp) { \
		return; \
	} \
	auto __registrationScope = std::move(__registrationScopeExp.value())

#define REGISTER_THREAD_SCOPED_WARN(threadType) \
	auto &&__registrationScopeExp = ::cvedia::rt::api::thread::registerThreadScoped(threadType, 1, __LOCATION__); \
	if (!__registrationScopeExp) { \
		LOGW << "Failed to register thread: " << __registrationScopeExp.error().message(); \
		return; \
	} \
	auto __registrationScope = std::move(__registrationScopeExp.value())

#define TRY_REGISTER_THREAD_SCOPED(threadType) RT_TRY(auto __registrationScope, ::cvedia::rt::api::thread::registerThreadScoped(threadType, 1, __LOCATION__))

#define REGISTER_WORKER_THREAD_SCOPED REGISTER_THREAD_SCOPED(cvedia::rt::ThreadType::Worker)
#define REGISTER_WORKER_THREAD_SCOPED_WARN REGISTER_THREAD_SCOPED_WARN(cvedia::rt::ThreadType::Worker)
#define TRY_REGISTER_WORKER_THREAD_SCOPED TRY_REGISTER_THREAD_SCOPED(cvedia::rt::ThreadType::Worker)

#define REGISTER_INSTANCE_THREAD_SCOPED REGISTER_THREAD_SCOPED(cvedia::rt::ThreadType::Instance)
#define REGISTER_INSTANCE_THREAD_SCOPED_WARN REGISTER_THREAD_SCOPED_WARN(cvedia::rt::ThreadType::Instance)
#define TRY_REGISTER_INSTANCE_THREAD_SCOPED TRY_REGISTER_THREAD_SCOPED(cvedia::rt::ThreadType::Instance)

#define REGISTER_GLOBAL_THREAD_SCOPED REGISTER_THREAD_SCOPED(cvedia::rt::ThreadType::Global)
#define REGISTER_GLOBAL_THREAD_SCOPED_WARN REGISTER_THREAD_SCOPED_WARN(cvedia::rt::ThreadType::Global)
#define TRY_REGISTER_GLOBAL_THREAD_SCOPED TRY_REGISTER_THREAD_SCOPED(cvedia::rt::ThreadType::Global)

#define DETACHED_THREAD_SCOPED ::cvedia::rt::api::thread::DetachedThreadScope __detachedThreadScope

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

				inline std::atomic<int> detachedThreadCount = 0;

				// This class is used to ensure that the detached thread count is handled automatically
				class EXPORT_CLASS DetachedThreadScope {
				public:
					DetachedThreadScope()
					{
						++detachedThreadCount;
					}

					~DetachedThreadScope()
					{
						--detachedThreadCount;
					}
				};

				EXPORT void sleep(int ms);
				EXPORT void dump();
				NODISCARD EXPORT expected<std::shared_ptr<internal::ThreadContext>> getThreadContext();
				NODISCARD EXPORT expected<int> getThreadVersion();
				NODISCARD EXPORT expected<std::shared_ptr<iface::Solution>> getActiveSolution();
				NODISCARD EXPORT expected<void> setActiveSolution(std::weak_ptr<iface::Solution> solution);
				NODISCARD EXPORT expected<std::shared_ptr<iface::Instance>> getActiveInstance();
				NODISCARD EXPORT expected<void> setActiveInstance(std::weak_ptr<iface::Instance> instance);
				NODISCARD EXPORT expected<void> pinOnCpuCore();
				NODISCARD EXPORT expected<void> clearActiveInstance();
				
				NODISCARD EXPORT expected<void> unregisterThread();
				NODISCARD EXPORT expected<void> unregisterThread(std::thread::id id);
				EXPORT void deleteAllThreads();

				// This class is used to ensure that the thread is unregistered when it goes out of scope
				class EXPORT_CLASS ThreadRegistrationScope {
				public:
					~ThreadRegistrationScope()
					{
						(void)::cvedia::rt::api::thread::unregisterThread();
					}
				};

				NODISCARD EXPORT expected<void> registerThread(ThreadType t, int version = 1, std::weak_ptr<iface::Instance> instance = {}, std::string const& location = "");
				NODISCARD EXPORT expected<void> registerThread(std::thread&& th, ThreadType t, int version = 1, std::string const& location = "");
				NODISCARD EXPORT expected<std::unique_ptr<ThreadRegistrationScope>> registerThreadScoped(ThreadType t, int version = 1, std::string const& location = "");
				NODISCARD EXPORT expected<std::unique_ptr<ThreadRegistrationScope>> registerThreadScoped(ThreadType t, int version, std::weak_ptr<iface::Instance> instance, std::string const& location);

				NODISCARD EXPORT bool isRegistered();
				NODISCARD EXPORT bool isRegistered(std::thread::id id);

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
