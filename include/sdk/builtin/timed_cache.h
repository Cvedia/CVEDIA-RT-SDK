/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once 

#include "defines.h"
#include "rterror.h"
#include "builtin/basetypes.h"

#include <unordered_map>
#include <shared_mutex>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>

namespace cvedia {
	namespace rt {

        template<typename Key_t, typename Value_t>
        class TimedCache {
        public:
            TimedCache(bool autocleanUp = false, 
					   std::chrono::seconds cleanUpPeriod = std::chrono::seconds(60), 
					   std::chrono::seconds defaultTimeout = std::chrono::seconds(60))
                : stopcleanUpThread_(false), cleanUpPeriod_(cleanUpPeriod), defaultTimeout_(defaultTimeout)
        	{
                if (autocleanUp) 
                {
                    cleanUpThread_ = std::thread(&TimedCache::_cleanUpThreadFunc, this);
                }
            }

            ~TimedCache()
            {
                if (cleanUpThread_.joinable())
                {
                    stopcleanUpThread_ = true;
                    cleanUpThread_.join();
                }
            }

        	void insert(const Key_t& key, const Value_t& value)
            {
	            insert(key, value, defaultTimeout_);
            }

        	void insert(const Key_t& key, const Value_t& value, std::chrono::seconds timeout)
        	{
                std::unique_lock<std::shared_mutex> lock(mutex_);

                data_[key] = value;
                timeouts_[key] = std::chrono::system_clock::now() + timeout;
            }

            expected<Value_t> get(const Key_t& key) const
        	{
                std::shared_lock<std::shared_mutex> lock(mutex_);

                auto it = data_.find(key);
                if (it == data_.end()) 
                {
                    return unexpected(RTErrc::KeyNotFound);
                }
                return it->second;
            }

            Value_t get(const Key_t& key, const Value_t& valueIfNotExists) const
        	{
                std::shared_lock<std::shared_mutex> lock(mutex_);

                auto it = data_.find(key);
                if (it == data_.end()) 
                {
                    return valueIfNotExists;
                }
                return it->second;
            }

            expected<Value_t> getWithKeepAlive(const Key_t& key, std::chrono::seconds keepAliveFor)
            {
                std::unique_lock<std::shared_mutex> lock(mutex_);

                auto itData = data_.find(key);
                if (itData == data_.end())
                {
                    return unexpected(RTErrc::KeyNotFound);
                }

            	auto itTimeout = timeouts_.find(key);
                if (itTimeout == timeouts_.end())
                {
                    return unexpected(RTErrc::KeyNotFound);
                }

                itTimeout->second = std::chrono::system_clock::now() + keepAliveFor;
                return itData->second;
            }

            Value_t getWithKeepAlive(const Key_t& key, std::chrono::seconds keepAliveFor, const Value_t& valueIfNotExists)
            {
                std::unique_lock<std::shared_mutex> lock(mutex_);

                auto itData = data_.find(key);
                if (itData == data_.end())
                {
                    return valueIfNotExists;
                }

                auto itTimeout = timeouts_.find(key);
                if (itTimeout == timeouts_.end())
                {
                    return valueIfNotExists;
                }

                itTimeout->second = std::chrono::system_clock::now() + keepAliveFor;
                return itData->second;
            }

            void cleanUp()
        	{
                std::unique_lock<std::shared_mutex> lock(mutex_);

                auto now = std::chrono::system_clock::now();
                for (auto it = timeouts_.begin(); it != timeouts_.end();) 
                {
                    if (it->second <= now)
                    {
                        data_.erase(it->first);
                        it = timeouts_.erase(it);
                    }
                    else 
                    {
                        ++it;
                    }
                }
            }

        private:
            std::unordered_map<Key_t, Value_t> data_;
            std::unordered_map<Key_t, std::chrono::time_point<std::chrono::system_clock>> timeouts_;

            mutable std::shared_mutex mutex_;
            std::atomic<bool> stopcleanUpThread_;
            std::thread cleanUpThread_;
            std::chrono::seconds cleanUpPeriod_;
            std::chrono::seconds defaultTimeout_;

            void _cleanUpThreadFunc()
            {
                while (!stopcleanUpThread_)
                {
                    for (int i = 0; i < cleanUpPeriod_.count() && !stopcleanUpThread_; ++i)
                    {
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                    }
                    if (!stopcleanUpThread_)
                    {
                        cleanUp();
                    }
                }
            }
        };
	}
}
