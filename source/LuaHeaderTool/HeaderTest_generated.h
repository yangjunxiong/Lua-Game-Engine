#include "HeaderTest.h"
#include "LuaBind.h"

class HeaderTest_generated final
{
private:
	using LuaWrapper = GameEngine::Lua::LuaWrapper<HeaderTest>;
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void RegisterClass(lua_State* L)
	{
		LuaWrapper::Register(L);
	};
	static void RegisterMember(LuaBind& bind)
	{
		bind.SetProperty<HeaderTest>("mInt", &HeaderTest::mInt);
		bind.SetProperty("mFloat", &HeaderTest::mFloat);
		bind.SetProperty("mString", &HeaderTest::mString);
		bind.SetFunction("Do", &HeaderTest::Do);
	};
};
DECLARE_LUA_WRAPPER(HeaderTest, "HeaderTest");

class HeaderTest2_generated final
{
private:
	using LuaWrapper = GameEngine::Lua::LuaWrapper<HeaderTest2>;
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void RegisterClass(lua_State* L)
	{
		LuaWrapper::Register<HeaderTest>(L);
	};
	static void RegisterMember(LuaBind& bind)
	{
		bind.SetProperty("mDouble", &HeaderTest2::mDouble);
	};
};
DECLARE_LUA_WRAPPER(HeaderTest2, "HeaderTest2");

