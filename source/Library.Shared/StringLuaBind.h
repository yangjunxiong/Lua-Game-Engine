#pragma once
#include "LuaObjectWrapper.h"

namespace GameEngine::Lua
{
	class StringLuaWrapper : LuaWrapper<std::string>
	{
	public:
		static void Register(lua_State* L)
		{
			LuaWrapper<std::string>::Register(L, sName);

			int newTable = luaL_newmetatable(L, sName.c_str());
			assert(!newTable);

			// Set constructor
			lua_register(L, sName.c_str(), &StringLuaWrapper::__new);

			// Set __tostring metafunction
			lua_pushstring(L, "__tostring");
			lua_pushcfunction(L, &StringLuaWrapper::__tostring);
			lua_rawset(L, -3);

			// Pop metatable
			lua_pop(L, 1);
		}

	protected:
		static inline const std::string sName = "String";

		StringLuaWrapper(bool luaObject, std::string* ptr) : LuaWrapper<std::string>(luaObject, ptr)
		{};

		static int __new(lua_State* L)
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

			StringLuaWrapper* pointer = static_cast<StringLuaWrapper*>(lua_newuserdata(L, sizeof(StringLuaWrapper)));
			new(pointer) StringLuaWrapper(true, new std::string(temp));

			int newTable = luaL_newmetatable(L, sName.c_str());
			assert(!newTable);
			lua_setmetatable(L, -2);

			return 1;
		}

		static int __tostring(lua_State* L)
		{
			LuaWrapper* p = static_cast<LuaWrapper*>(luaL_checkudata(L, -1, sName.c_str()));
			lua_pushstring(L, p->mObject->c_str());
			return 1;
		}
	};
}