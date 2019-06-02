#pragma once

namespace GameEngine::Lua
{
	template <typename T>
	LuaWrapper<T>::LuaWrapper(bool luaObject, T* ptr) :
		mLuaObject(luaObject),
		mObject(ptr)
	{};

	template <typename T>
	LuaWrapper<T>::~LuaWrapper()
	{
		assert(mObject != nullptr);
		if (mLuaObject)
		{
			delete mObject;
		}
	}

	template <typename T>
	void LuaWrapper<T>::Register(lua_State* L)
	{
		assert(!sName.empty());
		int newTable = luaL_newmetatable(L, sName.c_str());
		assert(("Each class mapped to Lua must have unique name", newTable));

		// Set __index metafunction
		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");

		// Set constructor as global function
		lua_register(L, sName.c_str(), &LuaWrapper<T>::__new);

		// Set __gc metafunction
		lua_pushstring(L, "__gc");
		lua_pushcfunction(L, &LuaWrapper<T>::__gc);
		lua_rawset(L, -3);

		// Set __tostring metafunction
		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, &LuaWrapper<T>::__tostring);
		lua_rawset(L, -3);

		// Set name function
		lua_pushstring(L, "Name");
		lua_pushcfunction(L, &LuaWrapper<T>::__name);
		lua_rawset(L, -3);

		// Set set value function
		lua_pushstring(L, "Set");
		lua_pushcfunction(L, &LuaWrapper<T>::__set);
		lua_rawset(L, -3);

		// Set get value function
		lua_pushstring(L, "Get");
		lua_pushcfunction(L, &LuaWrapper<T>::__get);
		lua_rawset(L, -3);

		// Pop metatable
		lua_pop(L, 1);
	}

	template <typename T>
	void LuaWrapper<T>::Unregister(lua_State* L)
	{
		assert(!sName.empty());
		if (L != nullptr)
		{
			lua_pushnil(L);
			lua_setfield(L, LUA_REGISTRYINDEX, sName.c_str());
		}
	}

	template <typename T>
	int LuaWrapper<T>::__new(lua_State* L)
	{
		LuaWrapper<T>* pointer = static_cast<LuaWrapper<T>*>(lua_newuserdata(L, sizeof(LuaWrapper<T>)));
		new(pointer) LuaWrapper<T>(true, new T);

		int newTable = luaL_newmetatable(L, sName.c_str());
		assert(!newTable);
		lua_setmetatable(L, -2);

		return 1;
	}

	template <typename T>
	int LuaWrapper<T>::__gc(lua_State* L)
	{
		void* pointer = luaL_checkudata(L, 1, sName.c_str());
		assert(pointer != nullptr);
		LuaWrapper<T>* wrap = static_cast<LuaWrapper<T>*>(pointer);
		wrap->~LuaWrapper();
		return 0;
	}

	template <typename T>
	int LuaWrapper<T>::__name(lua_State* L)
	{
		lua_pushstring(L, sName.c_str());
		return 1;
	}

	template <typename T>
	int LuaWrapper<T>::__set(lua_State* L)
	{
		lua_error(L);
		return 0;
	}

	template <typename T>
	int LuaWrapper<T>::__get(lua_State* L)
	{
		lua_error(L);
		return 0;
	}

	template <typename T>
	int LuaWrapper<T>::__tostring(lua_State* L)
	{
		lua_error(L);
		return 0;
	}

#pragma region int
	template<> const std::string LuaWrapper<int>::sName = "int32";

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
	template<> const std::string LuaWrapper<long long>::sName = "int64";

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
	template<> const std::string LuaWrapper<float>::sName = "float";

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
	template<> const std::string LuaWrapper<double>::sName = "double";

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
	template<> const std::string LuaWrapper<bool>::sName = "bool";

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
	template<> const std::string LuaWrapper<char>::sName = "char";

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
	template<> const std::string LuaWrapper<std::string>::sName = "String";

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