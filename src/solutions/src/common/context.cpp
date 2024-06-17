#include "common/context.h"
#include "api/logging.h"
#include "builtin/functions.h"

namespace cvedia::rt {

	std::map<int, std::unique_ptr<Context>> ContextStore::contextData_;
	shared_mutex_class ContextStore::contextMutex_;
	ContextId ContextStore::incContextId_ = 1;

	void ContextStore::reset()
	{
		auto lock = getWriteLock();

		contextData_.clear();
		incContextId_ = 1;
	}

	std::unique_lock<shared_mutex_class> ContextStore::getWriteLock()
	{
		return std::unique_lock(contextMutex_);
	}
	
	std::shared_lock<shared_mutex_class> ContextStore::getReadLock()
	{
		return std::shared_lock(contextMutex_);
	}

	ContextId ContextStore::createContext(char const* instanceUuid, char const* remoteIp)
	{
		auto lock = getWriteLock();

		return createContextUnsafe(instanceUuid, remoteIp);
	}

	ContextId ContextStore::createContextUnsafe(char const* instanceUuid, char const* remoteIp)
	{
		auto context = std::make_unique<Context>();

		if (instanceUuid == nullptr || instanceUuid[0] == '\0')
		{
			context->instanceUuid = cvedia::rt::Uuid::randomUuid();
		}
		else
		{
			context->instanceUuid = cvedia::rt::Uuid(instanceUuid);
			if (!context->instanceUuid.isValid())
			{
				LOGE << "Invalid instance UUID";
				return -1;
			}
		}

#if WITH_REST_CLIENT
		if (remoteIp && remoteIp[0] != '\0')
		{
			std::string remoteIpStr(remoteIp);
			int port = 3546;
			if (strchr(remoteIp, ':'))
			{
				auto const spl = split_str(remoteIp, ':');

				port = atoi(spl[1].c_str());
				remoteIpStr = spl[0];
			}

			context->remoteIp = remoteIpStr;
			context->connections.ip = std::string(remoteIpStr);

			context->connections.apiConfig = std::make_shared<cvedia::rt::rest::client::ApiConfiguration>();
			context->connections.apiConfig->setBaseUrl(utility::conversions::to_string_t("http://" + context->connections.ip + ":" + std::to_string(port)));
			context->connections.apiClient = std::make_shared<cvedia::rt::rest::client::ApiClient>(context->connections.apiConfig);
			context->connections.secuRTApi = std::make_shared<cvedia::rt::rest::client::SecuRTApi>(context->connections.apiClient);

#if WITH_CORE_REST_CLIENT
			context->connections.coreApi = std::make_shared<cvedia::rt::rest::client::RTCoreApi>(context->connections.apiClient);
			context->connections.coreSseApi = std::make_unique<cvedia::rt::CoreSseApi>("http://" + context->connections.ip + ":" + std::to_string(port));
#endif

#if WITH_SECURT_REST_CLIENT
			context->connections.secuRTAreasApi = std::make_shared<cvedia::rt::rest::client::SecuRTAreasApi>(context->connections.apiClient);
			context->connections.secuRTLinesApi = std::make_shared<cvedia::rt::rest::client::SecuRTLinesApi>(context->connections.apiClient);
#endif	
		}
#else
		if (!std::string(remoteIp).empty())
		{
			LOGE << "REST client not enabled, cannot set remote IP";
			return -1;
		}
#endif

		contextData_.insert({ incContextId_, std::move(context) });

		return incContextId_++;
	}

	int ContextStore::deleteContext(ContextId const contextId)
	{
		auto lock = getWriteLock();

		return deleteContextUnsafe(contextId);
	}

	int ContextStore::deleteContextUnsafe(ContextId const contextId)
	{
		if (contextData_.find(contextId) == contextData_.end())
		{
			return 0;
		}

		contextData_.erase(contextId);
		return 1;
	}

	std::unique_ptr<Context> const& ContextStore::getContextUnsafe(ContextId const contextId)
	{
		// Return this in case of an invalid contextId, needed as we are returning a reference to a unique_ptr
		static std::unique_ptr<Context> nullContext = nullptr;

		auto it = contextData_.find(contextId);
		if (it == contextData_.end()) {
			return nullContext;
		}
		return it->second;
	}

} // namespace cvedia::rt