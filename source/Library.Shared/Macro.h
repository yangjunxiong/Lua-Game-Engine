#pragma once

// Allow lua wrapper to use type registry to check for runtime pointer/reference as member function caller or function arguments in Lua
// With type check, if lua script uses incorrect type, will trigger lua error and print error log.
// Without type check, pointer is not guranteed to be right type in C++, thus will cause many undefined behaviors, and it's super hard to debug
// The cost of doing type check is very high since wrapper needs to traverse the entire inheritance tree
// By default it's on in debug mode, in release mode it's auto-off
#define RUN_TIME_TYPE_CHECK 1
#define TYPE_CHECK(...) assert(!RUN_TIME_TYPE_CHECK || __VA_ARGS__)

#define LUA_DEFINE_POINTER_TYPE(_type)																						 \
template<> static inline _type* GameEngine::Lua::LuaBind::_FromLuaStack(lua_State* L, int index)                             \
{																															 \
	TYPE_CHECK(CheckArgType<_type>(L, index));  \
	return static_cast<LuaWrapper<_type>*>(lua_touserdata(L, index))->mObject;			 \
}																															 \
template<> static inline const _type* GameEngine::Lua::LuaBind::_FromLuaStack(lua_State* L, int index)						 \
{																															 \
	TYPE_CHECK(CheckArgType<_type>(L, index));  \
	return static_cast<const LuaWrapper<_type>*>(lua_touserdata(L, index))->mObject;		 \
}																															 \
template<> static inline _type& GameEngine::Lua::LuaBind::_FromLuaStack(lua_State* L, int index)							 \
{																															 \
	TYPE_CHECK(CheckArgType<_type>(L, index));  \
	return *static_cast<LuaWrapper<_type>*>(lua_touserdata(L, index))->mObject;			 \
}																															 \
template<> static inline const _type& GameEngine::Lua::LuaBind::_FromLuaStack(lua_State* L, int index)						 \
{																															 \
	TYPE_CHECK(CheckArgType<_type>(L, index));  \
	return *static_cast<const LuaWrapper<_type>*>(lua_touserdata(L, index))->mObject;	 \
}																															 \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStackTerminal(lua_State* L, _type* value)						 \
{																															 \
	_SetProperty(L, value);																									 \
}																															 \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStackTerminal(lua_State* L, const _type* value)				 \
{																															 \
	_SetProperty(L, const_cast<_type*>(value));																				 \
}																															 \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStackTerminal(lua_State* L, _type& value)						 \
{																															 \
	_SetProperty(L, &value);																								 \
}																															 \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStackTerminal(lua_State* L, const _type& value)				 \
{																															 \
	_SetProperty(L, &const_cast<_type&>(value));																			 \
}

#define DECLARE_LUA_WRAPPER(_type, _name)                            \
const std::string GameEngine::Lua::LuaWrapper<_type>::sName = _name; \
const uint64_t GameEngine::Lua::LuaWrapper<_type>::sTypeId = reinterpret_cast<uint64_t>(&GameEngine::Lua::LuaWrapper<_type>::sTypeId);  \
LUA_DEFINE_POINTER_TYPE(_type)

#define CLASS(...) __VA_ARGS__
#define PROPERTY(...) __VA_ARGS__
#define FUNCTION(...) __VA_ARGS__