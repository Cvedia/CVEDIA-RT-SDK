/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <lua.h>
#include <vector>
#include <memory>

#include <sol.hpp>

#include "defines.h"

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

				EXPORT pCValue fromSolObject(sol::object const& obj);
				EXPORT sol::object toSolObject(pCValue obj, sol::state* lua);
				EXPORT sol::object toSolObject(pCValue obj, lua_State* lua_);

				EXPORT std::vector<std::vector<float>> getShapeFromSolTable(sol::table table);

				EXPORT sol::table getSolTable(lua_State* lua, std::vector<int> values);
				EXPORT sol::table getSolTable(lua_State* lua, std::vector<float> values);
				EXPORT sol::table getSolTable(lua_State* lua, std::vector<std::string> values);

				EXPORT std::shared_ptr<cvedia::rt::iface::ScriptEngine> getLuaEngine(); // TODO: @AJ try to phase out
				EXPORT void setLuaHelper(std::shared_ptr<cvedia::rt::iface::ScriptEngine> helper); // setLuaEngine
				EXPORT std::shared_ptr<cvedia::rt::iface::ScriptEngine> getLuaUI(); // TODO: @AJ to remove
				EXPORT void setLuaUI(std::shared_ptr<cvedia::rt::iface::ScriptEngine> helper); // TODO: @AJ to remove
				EXPORT bool* enable_lua_profiler_ptr(); //TODO: @AJ to remove
			};
		}
	}
}
