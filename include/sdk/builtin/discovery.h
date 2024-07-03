/*
    SPDX-FileCopyrightText: 2023 CVEDIA LTD

    SPDX-License-Identifier: Apache-2.0
*/

#pragma once
#include <mutex>
#include <set>
#include <thread>

#include "defines.h"
#include "rterror.h"

namespace cvedia
{
    namespace rt
    {
        class RemoteDiscovery final
        {
        public:
            explicit RemoteDiscovery();
            ~RemoteDiscovery();

            // copy constructor and assignment operators are disabled        
            RemoteDiscovery(const RemoteDiscovery&) = delete;
            RemoteDiscovery& operator=(const RemoteDiscovery&) = delete;
            RemoteDiscovery(RemoteDiscovery&&) = delete;
            RemoteDiscovery& operator=(RemoteDiscovery&&) = delete;

            bool isSearching() const;

            void start();
            void stop();
            std::set<std::string> getFoundServers();
            expected<std::string> getAvailableServer();
            static bool isServerHealthy(const std::string& ip);
            void clear();

            static const bool LOCAL_ONLY;
            static const unsigned short PORT;
            static const int DISCOVERY_INTERVAL;

        private:

            std::thread searchThread_;

            bool isSearching_;
            std::mutex mutex_;
            std::set<std::string> foundServers_;
        };
    }
}
