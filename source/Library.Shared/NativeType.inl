#pragma once

namespace GameEngine::Lua
{
#pragma region int
	DECLARE_LUA_WRAPPER(int, "int32");

	template<> static inline int LuaWrapper<int>::__new(lua_State* L)
	{
		int value = 0;
		if (lua_gettop(L) > 0)
		{
			value = static_cast<int>(luaL_checknumber(L, 1));
		}

		LuaWrapper* pointer = static_cast<LuaWrapper*>(lua_newuserdata(L, sizeof(LuaWrapper)));
		new(pointer) LuaWrapper(true, new int(value));

		int newTable = luaL_newmetatable(L, sName.c_str());
		assert(!newTable);
		lua_setmetatable(L, -2);

		return 1;
	}

	template<> static inline int LuaWrapper<int>::__set(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_Number number = luaL_checknumber(L, 2);
		*pointer->mObject = static_cast<int>(number);
		return 0;
	}

	template<> static inline int LuaWrapper<int>::__get(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushinteger(L, static_cast<lua_Integer>(*pointer->mObject));
		return 1;
	}

	template<> static inline int LuaWrapper<int>::__tostring(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushstring(L, to_string(*pointer->mObject).c_str());
		return 1;
	}
#pragma endregion

#pragma region int64
	DECLARE_LUA_WRAPPER(long long, "int64");

	template<> static inline int LuaWrapper<long long>::__new(lua_State* L)
	{
		long long value = 0;
		if (lua_gettop(L) > 0)
		{
			value = static_cast<long long>(luaL_checknumber(L, 1));
		}

		LuaWrapper* pointer = static_cast<LuaWrapper*>(lua_newuserdata(L, sizeof(LuaWrapper)));
		new(pointer) LuaWrapper(true, new long long(value));

		int newTable = luaL_newmetatable(L, sName.c_str());
		assert(!newTable);
		lua_setmetatable(L, -2);

		return 1;
	}

	template<> static inline int LuaWrapper<long long>::__set(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_Number number = luaL_checknumber(L, 2);
		*pointer->mObject = static_cast<long long>(number);
		return 0;
	}

	template<> static inline int LuaWrapper<long long>::__get(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushinteger(L, *pointer->mObject);
		return 1;
	}

	template<> static inline int LuaWrapper<long long>::__tostring(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushstring(L, to_string(*pointer->mObject).c_str());
		return 1;
	}
#pragma endregion

#pragma region float
	DECLARE_LUA_WRAPPER(float, "float");

	template<> static inline int LuaWrapper<float>::__new(lua_State* L)
	{
		float value = 0.f;
		if (lua_gettop(L) > 0)
		{
			value = static_cast<float>(luaL_checknumber(L, 1));
		}

		LuaWrapper* pointer = static_cast<LuaWrapper*>(lua_newuserdata(L, sizeof(LuaWrapper)));
		new(pointer) LuaWrapper(true, new float(value));

		int newTable = luaL_newmetatable(L, sName.c_str());
		assert(!newTable);
		lua_setmetatable(L, -2);

		return 1;
	}

	template<> static inline int LuaWrapper<float>::__set(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_Number number = luaL_checknumber(L, 2);
		*pointer->mObject = static_cast<float>(number);
		return 0;
	}

	template<> static inline int LuaWrapper<float>::__get(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushnumber(L, *pointer->mObject);
		return 1;
	}

	template<> static inline int LuaWrapper<float>::__tostring(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushstring(L, to_string(*pointer->mObject).c_str());
		return 1;
	}
#pragma endregion

#pragma region double
	DECLARE_LUA_WRAPPER(double, "double");

	template<> static inline int LuaWrapper<double>::__new(lua_State* L)
	{
		double value = 0.0;
		if (lua_gettop(L) > 0)
		{
			value = static_cast<double>(luaL_checknumber(L, 1));
		}

		LuaWrapper* pointer = static_cast<LuaWrapper*>(lua_newuserdata(L, sizeof(LuaWrapper)));
		new(pointer) LuaWrapper(true, new double(value));

		int newTable = luaL_newmetatable(L, sName.c_str());
		assert(!newTable);
		lua_setmetatable(L, -2);

		return 1;
	}

	template<> static inline int LuaWrapper<double>::__set(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_Number number = luaL_checknumber(L, 2);
		*pointer->mObject = static_cast<double>(number);
		return 0;
	}

	template<> static inline int LuaWrapper<double>::__get(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushnumber(L, *pointer->mObject);
		return 1;
	}

	template<> static inline int LuaWrapper<double>::__tostring(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushstring(L, to_string(*pointer->mObject).c_str());
		return 1;
	}
#pragma endregion

#pragma region bool
	DECLARE_LUA_WRAPPER(bool, "bool");

	template<> static inline int LuaWrapper<bool>::__new(lua_State* L)
	{
		bool value = true;
		if (lua_gettop(L) > 0)
		{
			value = lua_toboolean(L, 1);
		}

		LuaWrapper* pointer = static_cast<LuaWrapper*>(lua_newuserdata(L, sizeof(LuaWrapper)));
		new(pointer) LuaWrapper(true, new bool(value));

		int newTable = luaL_newmetatable(L, sName.c_str());
		assert(!newTable);
		lua_setmetatable(L, -2);

		return 1;
	}

	template<> static inline int LuaWrapper<bool>::__set(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		if (lua_isboolean(L, 2))
		{
			*pointer->mObject = lua_toboolean(L, 2);
		}
		else
		{
			lua_error(L);
		}
		return 0;
	}

	template<> static inline int LuaWrapper<bool>::__get(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushboolean(L, *pointer->mObject);
		return 1;
	}

	template<> static inline int LuaWrapper<bool>::__tostring(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushstring(L, *pointer->mObject ? "true" : "false");
		return 1;
	}
#pragma endregion

#pragma region char
	const std::string LuaWrapper<char>::sName = "char";
	const uint64_t LuaWrapper<char>::sTypeId = reinterpret_cast<uint64_t>(&LuaWrapper<char>::sTypeId);

	template<> static inline int LuaWrapper<char>::__new(lua_State* L)
	{
		char value = 0;
		if (lua_gettop(L) > 0)
		{
			value = static_cast<char>(luaL_checknumber(L, 1));
		}

		LuaWrapper* pointer = static_cast<LuaWrapper*>(lua_newuserdata(L, sizeof(LuaWrapper)));
		new(pointer) LuaWrapper(true, new char(value));

		int newTable = luaL_newmetatable(L, sName.c_str());
		assert(!newTable);
		lua_setmetatable(L, -2);

		return 1;
	}

	template<> static inline int LuaWrapper<char>::__set(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_Number number = luaL_checknumber(L, 2);
		*pointer->mObject = static_cast<char>(number);
		return 0;
	}

	template<> static inline int LuaWrapper<char>::__get(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushinteger(L, static_cast<lua_Integer>(*pointer->mObject));
		return 1;
	}

	template<> static inline int LuaWrapper<char>::__tostring(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushstring(L, to_string(*pointer->mObject).c_str());
		return 1;
	}
#pragma endregion

#pragma region std::string
	DECLARE_LUA_WRAPPER(std::string, "String");

	template<> static inline int LuaWrapper<std::string>::__new(lua_State* L)
	{
		std::string temp;
		if (lua_gettop(L) >= 1)
		{
			const char* str = luaL_checkstring(L, 1);
			if (str != nullptr)
			{
				temp.assign(str);
			}
		}

		LuaWrapper* pointer = static_cast<LuaWrapper*>(lua_newuserdata(L, sizeof(LuaWrapper)));
		new(pointer) LuaWrapper(true, new std::string(temp));

		int newTable = luaL_newmetatable(L, sName.c_str());
		assert(!newTable);
		lua_setmetatable(L, -2);

		return 1;
	}

	template<> static inline int LuaWrapper<std::string>::__set(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		const char* str = luaL_checkstring(L, 2);
		pointer->mObject->assign(str);
		return 0;
	}

	template<> static inline int LuaWrapper<std::string>::__get(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushstring(L, pointer->mObject->c_str());
		return 1;
	}

	template<> static inline int LuaWrapper<std::string>::__tostring(lua_State* L)
	{
		LuaWrapper* p = static_cast<LuaWrapper*>(luaL_checkudata(L, -1, sName.c_str()));
		lua_pushstring(L, p->mObject->c_str());
		return 1;
	}
#pragma endregion
}
