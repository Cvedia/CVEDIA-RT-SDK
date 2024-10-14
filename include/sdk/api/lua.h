/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
//#include <lua.h>
#include <vector>
#include <memory>
#include <sol/forward.hpp>

#include "defines.h"
#include "cvaluefwd.h"

struct lua_State;

namespace sol {
	class state_view;
	class state;
}

namespace cvedia {

	namespace rt {
		namespace iface {
			class ScriptEngine;
		}

		namespace api {
			namespace lua {
				// createSolTable ? getSolTable
				// fromSolObject
				// toSolObject
				// runScript / runCode

				EXPORT CValue fromSolObject(sol::object const& obj);
				CValue _fromSolObject(sol::object const& obj, int &cnt);

				EXPORT sol::object toSolObject(CValue const& obj, sol::state& lua);
				sol::object _toSolObject(CValue const& obj, sol::state& lua, int &cnt);
				sol::object _convertAndMakeObject(CValue const& val, sol::state& lua, int& cnt);
				EXPORT sol::object toSolObject(CValue const& obj, lua_State* lua_);

				EXPORT std::vector<std::vector<float>> getShapeFromSolTable(sol::table const& table);

				EXPORT sol::table getSolTable(lua_State* lua, std::vector<int> const& values);
				EXPORT sol::table getSolTable(lua_State* lua, std::vector<float> const& values);
				EXPORT sol::table getSolTable(lua_State* lua, std::vector<std::string> const& values);

				EXPORT std::shared_ptr<cvedia::rt::iface::ScriptEngine> getLuaEngine(); // TODO: @AJ try to phase out
				EXPORT void setLuaHelper(std::shared_ptr<cvedia::rt::iface::ScriptEngine> helper); // setLuaEngine
				EXPORT std::shared_ptr<cvedia::rt::iface::ScriptEngine> getLuaUI(); // TODO: @AJ to remove
				EXPORT void setLuaUI(std::shared_ptr<cvedia::rt::iface::ScriptEngine> helper); // TODO: @AJ to remove
				EXPORT bool* enable_lua_profiler_ptr(); //TODO: @AJ to remove
			};
		}
	}
}
