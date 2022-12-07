/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <mutex>
#include <shared_mutex>

#include "defines.h"

namespace cvedia {
	namespace rt {
		class LockableObject
		{
		public:
			LockableObject() = default;
			LockableObject(LockableObject const& other) = delete;
			LockableObject& operator=(LockableObject const& other) = delete;
			LockableObject(LockableObject&& other) = delete;
			LockableObject& operator=(LockableObject&& other) = delete;

			EXPORT std::unique_lock<__shared_mutex_class> getWriteLock() const;
			EXPORT std::shared_lock<__shared_mutex_class> getReadLock() const;
		private:
			mutable __shared_mutex_class mutex_;
		};
	}
}