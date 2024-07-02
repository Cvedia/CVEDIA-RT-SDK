/*
    SPDX-FileCopyrightText: 2023 CVEDIA LTD

    SPDX-License-Identifier: Apache-2.0
*/

#include <httplib.h>

#include "common/discovery.h"

#include <securt/securt_capi.h>

#include "api/rt.h"
#include "api/logging.h"

using namespace cvedia::rt;

const bool RemoteDiscovery::LOCAL_ONLY = false;
const unsigned short RemoteDiscovery::PORT = 50000;
const int RemoteDiscovery::DISCOVERY_INTERVAL = 10;

RemoteDiscovery::RemoteDiscovery()
{
	isSearching_ = false;
}

RemoteDiscovery::~RemoteDiscovery()
{
	stop();
}

bool RemoteDiscovery::isSearching() const
{
	return isSearching_;
}

void RemoteDiscovery::start()
{
	if (isSearching_)
	{
		return;
	}

	LOGD << "Remote Discovery: starting...";
	isSearching_ = true;
	foundServers_.clear();
	searchThread_ = std::thread([this]()
	{
		while (isSearching_)
		{
			auto result = rt::api::findRemoteRTs(LOCAL_ONLY, PORT);
			if (result.has_value())
			{
				auto rts = result.value();
				for (auto& rt : rts)
				{
					std::lock_guard<std::mutex> lock(mutex_);
					if (foundServers_.find(rt) == foundServers_.end())
					{
						foundServers_.insert(rt);
						//						engine_->pushPluginDiagnosticEvent(::nx::sdk::IPluginDiagnosticEvent::Level::info, "Remote Discovery", "Found remote CVEDIA-RT instance: " + rt);
						LOGD << "Remote Discovery: found remote CVEDIA-RT instance: " << rt;
					}
				}
			}
			else
			{
				// TODO: this can be incredibly spammy, introduce a cooldown?
				//				engine_->pushPluginDiagnosticEvent(::nx::sdk::IPluginDiagnosticEvent::Level::warning, "Remote Discovery", "An error occurred when attempting to discover remote CVEDIA-RT instances: " + result.error().message());
				LOGD << "Remote Discovery: an error occurred: " << result.error().message();
			}

			std::this_thread::sleep_for(std::chrono::seconds(DISCOVERY_INTERVAL));
		}
	});
}

void RemoteDiscovery::stop()
{
	if (!isSearching_)
	{
		return;
	}

	LOGD << "Remote Discovery: stopping...";
	isSearching_ = false;
	searchThread_.join();

	foundServers_.clear();
}

std::set<std::string> RemoteDiscovery::getFoundServers()
{
	std::lock_guard<std::mutex> lock(mutex_);
	auto result = std::set<std::string>(foundServers_.begin(), foundServers_.end());
	return result;
}

bool RemoteDiscovery::isServerHealthy(std::string const& ip)
{
	auto const ipStr = std::string(ip);
	const int port = 8080;

	// Create a client instance
	httplib::Client cli(ip, port);

	auto const res = cli.Get("/openapi.yaml");
	if (res && res->status == 200)
	{
		return true;
	}
	else
	{
		return false;
	}
}

expected<std::string> RemoteDiscovery::getAvailableServer()
{
	std::lock_guard<std::mutex> lock(mutex_);
	for (auto& server : foundServers_)
	{
		if (isServerHealthy(server))
		{
			return server;
		}
	}

	return unexpected(RTErrc::NoServerAvailable);
}

void RemoteDiscovery::clear()
{
	std::lock_guard<std::mutex> lock(mutex_);
	foundServers_.clear();
}
