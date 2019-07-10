#pragma once
namespace GameEngine::Lua
{
	class LuaBind;

	class LuaRegister
	{
	public:
		static void RegisterLua(LuaBind& bind);
		static void UnregisterLua(LuaBind& bind, bool all);
	};
}