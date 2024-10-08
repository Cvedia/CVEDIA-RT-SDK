/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "defines.h"

#include <shared_mutex>
#include <mutex>

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

			EXPORT std::unique_lock<shared_mutex_class> getWriteLock() const;
			EXPORT std::shared_lock<shared_mutex_class> getReadLock() const;
			EXPORT std::unique_lock<shared_mutex_class> getWriteLockAdopt() const;
			EXPORT std::shared_lock<shared_mutex_class> getReadLockAdopt() const;
			EXPORT std::unique_lock<shared_mutex_class> getWriteLockDefer() const;
			EXPORT std::shared_lock<shared_mutex_class> getReadLockDefer() const;
			EXPORT shared_mutex_class& getMutex() const;
		private:
			mutable shared_mutex_inst(mutex_);
		};
	}
}