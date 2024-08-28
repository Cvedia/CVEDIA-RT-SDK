#pragma once

#include "rtconfig.h"
#include "defines.h"
#include "uuid.h"

#include <map>
#include <mutex>
#include <shared_mutex>

#if WITH_REST_CLIENT
#include <rt_rest_client/ApiConfiguration.h>
#include <rt_rest_client/ApiClient.h>
#endif

#if WITH_CORE_REST_CLIENT
#include <rt_rest_client/api/RTCoreApi.h>
#include <core/core_sse_api_client.h>
#endif

#if WITH_SECURT_REST_CLIENT
#include <rt_rest_client/api/SecuRTApi.h>
#include <rt_rest_client/api/SecuRTAreasApi.h>
#include <rt_rest_client/api/SecuRTLinesApi.h>
#endif

#define GET_CONTEXT(var, handle) \
	auto lock = cvedia::rt::ContextStore::getReadLock(); \
	auto const& var = cvedia::rt::ContextStore::getContextUnsafe(handle); \
	if (var == nullptr) { \
		return -1; \
	}

#define GET_CONTEXT_STR(var, handle) \
    auto lock = cvedia::rt::ContextStore::getReadLock(); \
    auto const& var = cvedia::rt::ContextStore::getContextUnsafe(handle); \
    if (var == nullptr) { \
        return nullptr; \
    }

namespace cvedia::rt
{
	struct ConnectionData
	{
		std::string ip;

#if WITH_REST_CLIENT
		std::shared_ptr<cvedia::rt::rest::client::ApiConfiguration> apiConfig;
		std::shared_ptr<cvedia::rt::rest::client::ApiClient> apiClient;
#endif

#if WITH_CORE_REST_CLIENT
		std::shared_ptr<cvedia::rt::rest::client::RTCoreApi> coreApi;
		std::unique_ptr<cvedia::rt::rest::client::CoreSseApiClient> coreSseApi;
#endif

#if WITH_SECURT_REST_CLIENT
		std::shared_ptr<cvedia::rt::rest::client::SecuRTApi> secuRTApi;
		std::shared_ptr<cvedia::rt::rest::client::SecuRTAreasApi> secuRTAreasApi;
		std::shared_ptr<cvedia::rt::rest::client::SecuRTLinesApi> secuRTLinesApi;
#endif
	};

	struct Context
	{
		cvedia::rt::Uuid instanceUuid;
		std::string remoteIp;

		ConnectionData connections;
	};

	using ContextId = int;

	class ContextStore final
	{
	public:
		static std::unique_lock<shared_mutex_class> getWriteLock();
		static std::shared_lock<shared_mutex_class> getReadLock();
		
		static ContextId createContext(char const* instanceUuid, char const* remoteIp);
		static ContextId createContextUnsafe(char const* instanceUuid, char const* remoteIp);
		
		static int deleteContext(ContextId const contextId);
		static int deleteContextUnsafe(ContextId const contextId);
		
		static std::unique_ptr<Context> const& getContextUnsafe(ContextId const contextId);

		static void reset();

	private:
		static std::map<int, std::unique_ptr<Context>> contextData_;
		static shared_mutex_class contextMutex_;
		static ContextId incContextId_;
	};
}