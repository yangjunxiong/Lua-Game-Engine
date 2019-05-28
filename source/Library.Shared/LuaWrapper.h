#pragma once
namespace GameEngine::Lua
{
	template <typename T>
	class LuaWrapper final
	{
	public:
		LuaWrapper(const LuaWrapper& other) = delete;
		LuaWrapper(LuaWrapper&& other) = delete;
		~LuaWrapper();

		static void Register(lua_State* L);
		static void Unregister(lua_State* L);
		static LuaWrapper* Create(lua_State* L, T* ptr, const std::string& name = std::string(), int index = 0);

		T* const mObject = nullptr;
		const bool mLuaObject = false;

	protected:
		const static std::string sName;

		LuaWrapper(bool luaObject, T* ptr);
		static int __new(lua_State* L);
		static int __gc(lua_State* L);
		static int __name(lua_State* L);
		static int __set(lua_State* L);
		static int __get(lua_State* L);
		static int __tostring(lua_State* L);
	};
}

#include "LuaWrapper.inl"