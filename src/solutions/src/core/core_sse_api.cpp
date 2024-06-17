#include "core/core_sse_api.h"
#include <httplib.h>

using namespace cvedia::rt;

CoreSseApi::CoreSseApi(std::string const& server)
{
	server_ = server;
}

CoreSseApi::~CoreSseApi()
{
	for (auto& [instanceId, thread] : eventsThread_)
	{
		thread.join();
	}
}

std::string CoreSseApi::coreConsumeEventsSse(std::string const& instanceId)
{
	if (eventsActive_.find(instanceId) == eventsActive_.end() || eventsActive_[instanceId] == false)
	{
		eventsActive_[instanceId] = true;

		eventsThread_[instanceId] = std::thread([this, instanceId]()
			{
				httplib::Client(server_)
					.Get("/v1/core/instance/" + instanceId + "/consume_events_sse",
						[&](const char* data, size_t data_length)
						{
							std::lock_guard<std::mutex> lock(eventsMutex_[instanceId]);

							if (eventsQueue_[instanceId].size() > 100)
							{
								eventsQueue_[instanceId].pop_front();
							}

							// find newline terminator in data and append it to eventsPartial_
							// emplace_back eventsPartial_ into eventsQueue_
							// and reset eventsPartial_ to empty string
							// save the rest in eventsPartial_ for the next iteration

							char const* newlineTerminator = std::find(data, data + data_length, '\n');
							if (newlineTerminator != data + data_length)
							{
								eventsPartial_[instanceId] += std::string_view(data, newlineTerminator - data);
								eventsQueue_[instanceId].emplace_back(eventsPartial_[instanceId]);
								eventsPartial_[instanceId] = "";
							}
							else
							{
								eventsPartial_[instanceId] += std::string_view(data, data_length);
							}


							//					     eventsQueue_[instanceId].emplace_back(data, data_length);
							return true;
						});
			});
	}

	std::lock_guard<std::mutex> lock(eventsMutex_[instanceId]);

	if (eventsQueue_[instanceId].empty())
	{
		return "";
	}

	auto resp = eventsQueue_[instanceId].front();
	eventsQueue_[instanceId].pop_front();

	return resp;
}
