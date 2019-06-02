#pragma once
namespace GameEngine::Lua
{
	template <typename T>
	class LuaWrapper final
	{
	public:
		LuaWrapper(bool luaObject, T* ptr);
		LuaWrapper(const LuaWrapper& other) = delete;
		LuaWrapper(LuaWrapper&& other) = delete;
		LuaWrapper& operator=(const LuaWrapper& other) = delete;
		LuaWrapper& operator=(LuaWrapper&& other) = delete;
		~LuaWrapper();

		static void Register(lua_State* L);
		static void Unregister(lua_State* L);

		T* const mObject = nullptr;
		const bool mLuaObject = false;
		const static std::string sName;

	protected:
		static int __new(lua_State* L);
		static int __gc(lua_State* L);
		static int __name(lua_State* L);
		static int __set(lua_State* L);
		static int __get(lua_State* L);
		static int __tostring(lua_State* L);
	};
}

#include "LuaWrapper.inl"