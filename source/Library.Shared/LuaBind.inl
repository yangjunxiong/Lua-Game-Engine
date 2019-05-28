#pragma once

using namespace GameEngine;
using namespace GameEngine::Lua;
using namespace std;

LUA_DEFINE_POINTER_TYPE(std::string);
LUA_DEFINE_POINTER_TYPE(int);
LUA_DEFINE_POINTER_TYPE(long long);
LUA_DEFINE_POINTER_TYPE(float);
LUA_DEFINE_POINTER_TYPE(double);
LUA_DEFINE_POINTER_TYPE(bool);

#pragma region Function
#define EXP(...) __VA_ARGS__
#define SET_FUNCTION_BODY(_params, ...)                                                                                \
_SetLuaValue([this, &value]()																						   \
{																													   \
	auto wrapper = [](lua_State* L) -> int																			   \
	{																												   \
		auto* func = static_cast<std::function<Ret(__VA_ARGS__)>*>(lua_touserdata(L, lua_upvalueindex(1)));			   \
		return _CallCFunction<Ret>(L, [func](lua_State* L) -> Ret													   \
		{																											   \
			L;																										   \
			return (*func)(_params);																				   \
		});																											   \
	};																												   \
	new (lua_newuserdata(mLuaState, sizeof(value))) std::function<Ret(__VA_ARGS__)>(value);							   \
	lua_pushcclosure(mLuaState, wrapper, 1);																		   \
}, key);

template <typename Ret>
void LuaBind::SetFunction(const std::string& key, const std::function<Ret()>& value)
{
	SET_FUNCTION_BODY(EXP());
}

template <typename Ret, typename Param1>
void LuaBind::SetFunction(const std::string& key, const std::function<Ret(Param1)>& value)
{
	SET_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, -1)), Param1);
}

template <typename Ret, typename Param1, typename Param2>
void LuaBind::SetFunction(const std::string& key, const std::function<Ret(Param1, Param2)>& value)
{
	SET_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, -2), _FromLuaStack<Param2>(L, -1)), Param1, Param2);
}

template <typename Ret, typename Param1, typename Param2, typename Param3>
void LuaBind::SetFunction(const std::string& key, const std::function<Ret(Param1, Param2, Param3)>& value)
{
	SET_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, -3), _FromLuaStack<Param2>(L, -2), _FromLuaStack<Param3>(L, -1)), Param1, Param2, Param3);
}

template <typename Ret, typename Param1, typename Param2, typename Param3, typename Param4>
void LuaBind::SetFunction(const std::string& key, const std::function<Ret(Param1, Param2, Param3, Param4)>& value)
{
	SET_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, -4), _FromLuaStack<Param2>(L, -3), _FromLuaStack<Param3>(L, -2), _FromLuaStack<Param4>(L, -1)), Param1, Param2, Param3, Param4);
}

template <typename Ret, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
void LuaBind::SetFunction(const std::string& key, const std::function<Ret(Param1, Param2, Param3, Param4, Param5)>& value)
{
	SET_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, -5), _FromLuaStack<Param2>(L, -4), _FromLuaStack<Param3>(L, -3), _FromLuaStack<Param4>(L, -2), _FromLuaStack<Param5>(L, -1)), Param1, Param2, Param3, Param4, Param5);
}

template <typename Ret, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6>
void LuaBind::SetFunction(const std::string& key, const std::function<Ret(Param1, Param2, Param3, Param4, Param5, Param6)>& value)
{
	SET_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, -6), _FromLuaStack<Param2>(L, -5), _FromLuaStack<Param3>(L, -4), _FromLuaStack<Param4>(L, -3), _FromLuaStack<Param5>(L, -2), _FromLuaStack<Param6>(L, -1)), Param1, Param2, Param3, Param4, Param5, Param6);
}

template <typename Ret>
static inline int LuaBind::_CallCFunction(lua_State* L, const std::function<Ret(lua_State*)>& wrap)
{
	_ToLuaStack(L, wrap(L));
	return 1;
}

template<> static inline int LuaBind::_CallCFunction<void>(lua_State* L, const std::function<void(lua_State*)>& wrap)
{
	wrap(L);
	return 0;
}
#pragma endregion

#pragma region SetLuaValue
template<> inline void LuaBind::SetLuaValue(const std::string& key, const int& value)
{
	_SetLuaValue([this, &value]()
	{
		lua_pushnumber(mLuaState, static_cast<LUA_NUMBER>(value));
	}, key);
}

template<> inline void LuaBind::SetLuaValue(const std::string& key, const long long& value)
{
	_SetLuaValue([this, &value]()
	{
		lua_pushnumber(mLuaState, static_cast<LUA_NUMBER>(value));
	}, key);
}

template<> inline void LuaBind::SetLuaValue(const std::string& key, const float& value)
{
	_SetLuaValue([this, &value]()
	{
		lua_pushnumber(mLuaState, static_cast<LUA_NUMBER>(value));
	}, key);
}

template<> inline void LuaBind::SetLuaValue(const std::string& key, const double& value)
{
	_SetLuaValue([this, &value]()
	{
		lua_pushnumber(mLuaState, static_cast<LUA_NUMBER>(value));
	}, key);
}

template<> inline void LuaBind::SetLuaValue(const std::string& key, const char& value)
{
	_SetLuaValue([this, &value]()
	{
		lua_pushnumber(mLuaState, static_cast<LUA_NUMBER>(value));
	}, key);
}

template<> inline void LuaBind::SetLuaValue(const std::string& key, const bool& value)
{
	_SetLuaValue([this, &value]()
	{
		lua_pushboolean(mLuaState, value);
	}, key);
}

template<> inline void LuaBind::SetLuaValue(const std::string& key, char* const& value)
{
	_SetLuaValue([this, &value]()
	{
		lua_pushstring(mLuaState, value);
	}, key);
}

template<> inline void LuaBind::SetLuaValue(const std::string& key, const std::string& value)
{
	_SetLuaValue([this, &value]()
	{
		lua_pushstring(mLuaState, value.c_str());
	}, key);
}
#pragma endregion

#pragma region FromLuaStack
template<> static inline int LuaBind::_FromLuaStack(lua_State* L, int index)
{
	return static_cast<int>(luaL_checknumber(L, index));
}

template<> static inline long long LuaBind::_FromLuaStack(lua_State* L, int index)
{
	return static_cast<long long>(luaL_checknumber(L, index));
}

template<> static inline float LuaBind::_FromLuaStack(lua_State* L, int index)
{
	return static_cast<float>(luaL_checknumber(L, index));
}

template<> static inline double LuaBind::_FromLuaStack(lua_State* L, int index)
{
	return luaL_checknumber(L, index);
}

template<> static inline bool LuaBind::_FromLuaStack(lua_State* L, int index)
{
	if (lua_isboolean(L, index))
	{
		return lua_toboolean(L, index) == 0 ? false : true;
	}
	lua_error(L);
	return false;
}

template<> static inline char LuaBind::_FromLuaStack(lua_State* L, int index)
{
	return static_cast<char>(luaL_checknumber(L, index));
}

template<> static inline char* LuaBind::_FromLuaStack(lua_State* L, int index)
{
	return const_cast<char*>(luaL_checkstring(L, index));
}

template<> static inline const char* LuaBind::_FromLuaStack(lua_State* L, int index)
{
	return luaL_checkstring(L, index);
}

template<> static inline std::string LuaBind::_FromLuaStack(lua_State* L, int index)
{
	return string(luaL_checkstring(L, index));
}
#pragma endregion

#pragma region ToLuaStack
template <typename... Args>
static void LuaBind::_ToLuaStack(lua_State* L, Args&&... args)
{
	size_t count = sizeof...(Args);
	if (count > 0)
	{
		_ToLuaStackStep(L, args...);
	}
}

template <typename T, typename... Args>
static inline void LuaBind::_ToLuaStackStep(lua_State* L, T value, Args&&... args)
{
	_ToLuaStackTerminal(L, value);
	_ToLuaStackStep(L, args...);
}

template<> static inline void LuaBind::_ToLuaStackTerminal(lua_State* L, int value)
{
	lua_pushinteger(L, static_cast<lua_Integer>(value));
}

template<> static inline void LuaBind::_ToLuaStackTerminal(lua_State* L, long long value)
{
	lua_pushinteger(L, value);
}

template<> static inline void LuaBind::_ToLuaStackTerminal(lua_State* L, float value)
{
	lua_pushnumber(L, static_cast<lua_Number>(value));
}

template<> static inline void LuaBind::_ToLuaStackTerminal(lua_State* L, double value)
{
	lua_pushnumber(L, value);
}

template<> static inline void LuaBind::_ToLuaStackTerminal(lua_State* L, bool value)
{
	lua_pushboolean(L, value);
}

template<> static inline void LuaBind::_ToLuaStackTerminal(lua_State* L, char value)
{
	lua_pushlstring(L, &value, 1);
}

template<> static inline void LuaBind::_ToLuaStackTerminal(lua_State* L, char* value)
{
	lua_pushstring(L, value);
}

template<> static inline void LuaBind::_ToLuaStackTerminal(lua_State* L, const char* value)
{
	lua_pushstring(L, value);
}

template<> static inline void LuaBind::_ToLuaStackTerminal(lua_State* L, std::string value)
{
	lua_pushstring(L, value.c_str());
}
#pragma endregion
