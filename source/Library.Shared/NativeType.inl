#pragma once
#include "glm/glm.hpp"

namespace GameEngine::Lua
{
#pragma region int
	DECLARE_LUA_WRAPPER(int, "int32");
	LUA_DEFINE_NATIVE_OBJECT_TYPE(int);

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
		(newTable);
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

	DECLARE_LUA_WRAPPER(unsigned int, "uint32");
	LUA_DEFINE_NATIVE_OBJECT_TYPE(unsigned int);

	template<> static inline int LuaWrapper<unsigned int>::__new(lua_State* L)
	{
		int value = 0;
		if (lua_gettop(L) > 0)
		{
			value = static_cast<unsigned int>(luaL_checknumber(L, 1));
		}

		LuaWrapper* pointer = static_cast<LuaWrapper*>(lua_newuserdata(L, sizeof(LuaWrapper)));
		new(pointer) LuaWrapper(true, new unsigned int(value));

		int newTable = luaL_newmetatable(L, sName.c_str());
		(newTable);
		assert(!newTable);
		lua_setmetatable(L, -2);

		return 1;
	}

	template<> static inline int LuaWrapper<unsigned int>::__set(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_Number number = luaL_checknumber(L, 2);
		*pointer->mObject = static_cast<unsigned int>(number);
		return 0;
	}

	template<> static inline int LuaWrapper<unsigned int>::__get(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushinteger(L, static_cast<lua_Integer>(*pointer->mObject));
		return 1;
	}

	template<> static inline int LuaWrapper<unsigned int>::__tostring(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushstring(L, to_string(*pointer->mObject).c_str());
		return 1;
	}
#pragma endregion

#pragma region int64
	DECLARE_LUA_WRAPPER(long long, "int64");
	LUA_DEFINE_NATIVE_OBJECT_TYPE(long long);

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
		(newTable);
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

	DECLARE_LUA_WRAPPER(unsigned long long, "uint64");
	LUA_DEFINE_NATIVE_OBJECT_TYPE(unsigned long long);

	template<> static inline int LuaWrapper<unsigned long long>::__new(lua_State* L)
	{
		long long value = 0;
		if (lua_gettop(L) > 0)
		{
			value = static_cast<unsigned long long>(luaL_checknumber(L, 1));
		}

		LuaWrapper* pointer = static_cast<LuaWrapper*>(lua_newuserdata(L, sizeof(LuaWrapper)));
		new(pointer) LuaWrapper(true, new unsigned long long(value));

		int newTable = luaL_newmetatable(L, sName.c_str());
		(newTable);
		assert(!newTable);
		lua_setmetatable(L, -2);

		return 1;
	}

	template<> static inline int LuaWrapper<unsigned long long>::__set(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_Number number = luaL_checknumber(L, 2);
		*pointer->mObject = static_cast<unsigned long long>(number);
		return 0;
	}

	template<> static inline int LuaWrapper<unsigned long long>::__get(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushinteger(L, *pointer->mObject);
		return 1;
	}

	template<> static inline int LuaWrapper<unsigned long long>::__tostring(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushstring(L, to_string(*pointer->mObject).c_str());
		return 1;
	}
#pragma endregion

#pragma region float
	DECLARE_LUA_WRAPPER(float, "float");
	LUA_DEFINE_NATIVE_OBJECT_TYPE(float);

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
		(newTable);
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
	LUA_DEFINE_NATIVE_OBJECT_TYPE(double);

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
		(newTable);
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
	LUA_DEFINE_NATIVE_OBJECT_TYPE(bool);

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
		(newTable);
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
	LUA_DEFINE_NATIVE_OBJECT_TYPE(char);

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
		(newTable);
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

	const std::string LuaWrapper<unsigned char>::sName = "uchar";
	const uint64_t LuaWrapper<unsigned char>::sTypeId = reinterpret_cast<uint64_t>(&LuaWrapper<unsigned char>::sTypeId);

	template<> static inline int LuaWrapper<unsigned char>::__new(lua_State* L)
	{
		char value = 0;
		if (lua_gettop(L) > 0)
		{
			value = static_cast<unsigned char>(luaL_checknumber(L, 1));
		}

		LuaWrapper* pointer = static_cast<LuaWrapper*>(lua_newuserdata(L, sizeof(LuaWrapper)));
		new(pointer) LuaWrapper(true, new unsigned char(value));

		int newTable = luaL_newmetatable(L, sName.c_str());
		(newTable);
		assert(!newTable);
		lua_setmetatable(L, -2);

		return 1;
	}

	template<> static inline int LuaWrapper<unsigned char>::__set(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_Number number = luaL_checknumber(L, 2);
		*pointer->mObject = static_cast<unsigned char>(number);
		return 0;
	}

	template<> static inline int LuaWrapper<unsigned char>::__get(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushinteger(L, static_cast<lua_Integer>(*pointer->mObject));
		return 1;
	}

	template<> static inline int LuaWrapper<unsigned char>::__tostring(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		lua_pushstring(L, to_string(*pointer->mObject).c_str());
		return 1;
	}
#pragma endregion

#pragma region std::string
	DECLARE_LUA_WRAPPER(std::string, "String");
	LUA_DEFINE_NATIVE_OBJECT_TYPE(std::string);

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
		(newTable);
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

#pragma region glm::vec4
	DECLARE_LUA_WRAPPER(glm::vec4, "vec4");
	LUA_DEFINE_CUSTOM_OBJECT_TYPE(glm::vec4);
	LUA_DEFINE_CUSTOM_COPY_TYPE(glm::vec4);

	template<> static inline void LuaBind::_AdditionalRegister<glm::vec4>(LuaBind& bind)
	{
		bind.SetProperty("x", &glm::vec4::x);
		bind.SetProperty("y", &glm::vec4::y);
		bind.SetProperty("z", &glm::vec4::z);
		bind.SetProperty("w", &glm::vec4::w);
	}

	template<> static inline int LuaWrapper<glm::vec4>::__new(lua_State* L)
	{
		glm::vec4 value(0);
		if (lua_gettop(L) > 0)
		{
			if (lua_gettop(L) == 4)
			{
				value = glm::vec4(static_cast<float>(luaL_checknumber(L, 1)), static_cast<float>(luaL_checknumber(L, 2)), static_cast<float>(luaL_checknumber(L, 3)), static_cast<float>(luaL_checknumber(L, 4)));
			}
			else
			{
				value = glm::vec4(static_cast<float>(luaL_checknumber(L, 1)));
			}
		}

		LuaWrapper* pointer = static_cast<LuaWrapper*>(lua_newuserdata(L, sizeof(LuaWrapper)));
		new(pointer) LuaWrapper(true, new glm::vec4(value));

		int newTable = luaL_newmetatable(L, sName.c_str());
		(newTable);
		assert(!newTable);
		lua_setmetatable(L, -2);

		return 1;
	}

	template<> static inline int LuaWrapper<glm::vec4>::__set(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		LuaWrapper* other = static_cast<LuaWrapper*>(luaL_checkudata(L, 2, sName.c_str()));
		*pointer->mObject = static_cast<glm::vec4>(*other->mObject);
		return 0;
	}

	template<> static inline int LuaWrapper<glm::vec4>::__tostring(lua_State* L)
	{
		LuaWrapper* pointer = static_cast<LuaWrapper*>(luaL_checkudata(L, 1, sName.c_str()));
		glm::vec4& vec = *pointer->mObject;
		char str[256];
		sprintf_s(str, "(%.2f, %.2f, %.2f, %.2f)", vec.x, vec.y, vec.z, vec.w);
		lua_pushstring(L, str);
		return 1;
	}
#pragma endregion

}
