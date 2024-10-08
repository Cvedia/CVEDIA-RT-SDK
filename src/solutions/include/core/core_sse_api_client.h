#pragma once
#include <deque>
#include <map>
#include <mutex>
#include <string_view>
#include <thread>

namespace cvedia::rt::rest::client
{
	class CoreSseApiClient {
	public:
		CoreSseApiClient(std::string const& server);
		~CoreSseApiClient();

		std::string coreConsumeEventsSse(std::string const& instanceId);
	private:
		std::map<std::string, std::thread> eventsThread_;
		std::map<std::string, std::mutex> eventsMutex_;
		std::map<std::string, std::string> eventsPartial_;
		std::map<std::string, std::deque<std::string>> eventsQueue_;
		std::map<std::string, bool> eventsActive_;

		std::string server_;
	};
}
