/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "rtconfig.h"
#if BUILD_LUA_LIB

#include "defines.h"
#include <sol.hpp>

namespace cvedia {
	namespace rt {
		class BufferMgr;

		class BufferMgrLua {
		public:
			EXPORT BufferMgrLua(std::shared_ptr<BufferMgr> plugin);
			EXPORT ~BufferMgrLua();

#if BUILD_LUA_LIB == 1
			EXPORT static bool luaInit(sol::state_view lua);
#endif

			std::shared_ptr<BufferMgr> plugin_;
		};
	}
}
#endif
