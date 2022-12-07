/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <shared_mutex>
#include <sol.hpp>

#include "defines.h"

namespace cvedia {
	namespace rt {

		namespace module
		{
			using createLuaInitHandler = std::function<bool(sol::state_view engine)>;
			using createLuaCreateHandler = std::function<void(sol::table&)>;
		}

		namespace iface {
			class LuaManager
			{
			public:
				EXPORT static void registerInitHandler(std::string name, module::createLuaInitHandler func);
				EXPORT static void registerCreateHandler(std::string name, module::createLuaCreateHandler func);
				EXPORT static std::map<std::string, module::createLuaInitHandler> getInitHandlers();
				EXPORT static std::map<std::string, module::createLuaCreateHandler> getCreateHandlers();
				EXPORT static void initHandlers(sol::state_view engine);
			private:
				static std::map<std::string, module::createLuaInitHandler> initHandlers_;
				static __shared_mutex_class muxInitHandlers_;
				static std::map<std::string, module::createLuaCreateHandler> createHandlers_;
				static __shared_mutex_class muxCreateHandlers_;
			};
		}
	}
}