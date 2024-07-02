/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <functional>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <sol/forward.hpp>

#include "defines.h"

namespace sol {
	class state_view;
	class state;
}

namespace cvedia {
	namespace rt {

		namespace module
		{
			using createLuaInitHandler = std::function<bool(sol::state_view &engine)>;
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
				static shared_mutex_class muxInitHandlers_;
				static std::map<std::string, module::createLuaCreateHandler> createHandlers_;
				static shared_mutex_class muxCreateHandlers_;
			};
		}
	}
}