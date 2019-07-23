#pragma once

// Allow lua wrapper to use type registry to check for runtime pointer/reference as member function caller or function arguments in Lua
// With type check, if lua script uses incorrect type, will trigger lua error and print error log.
// Without type check, pointer is not guranteed to be right type in C++, thus will cause many undefined behaviors, and it's super hard to debug
// The cost of doing type check is very high since wrapper needs to traverse the entire inheritance tree
// By default it's on in debug mode, in release mode it's auto-off
#define RUN_TIME_TYPE_CHECK 1
#define TYPE_CHECK(...) assert(!RUN_TIME_TYPE_CHECK || __VA_ARGS__)

// A dummy placeholder to fill in other macro expressions
#define EXP(...) __VA_ARGS__

// Similar to assert(), but will trigger a Lua error instead of C++ error
#ifdef _DEBUG
	#define LUA_ASSERT(L, ...)  \
	if (!(__VA_ARGS__))			\
	{							\
		lua_error(L);			\
	}
#else
	#define LUA_ASSERT(L, ...)
#endif

/*
Allow C++ to bind pointer and reference type to Lua.
From Lua's perspective, it treats the object, pointer, and reference with no difference
Custom object is always bound to Lua as reference and therefore must use this macro
*/
#define LUA_DEFINE_POINTER_TYPE(_type)																						 \
template<> static inline _type* GameEngine::Lua::LuaBind::_FromLuaStack(lua_State* L, int index)                             \
{																															 \
	if (lua_isnil(L, index))  \
	{						  \
		return nullptr;		  \
	}						  \
	TYPE_CHECK(CheckArgType<_type>(L, index));  \
	return static_cast<LuaWrapper<_type>*>(lua_touserdata(L, index))->mObject;			 \
}																															 \
template<> static inline const _type* GameEngine::Lua::LuaBind::_FromLuaStack(lua_State* L, int index)						 \
{																															 \
	if (lua_isnil(L, index))  \
	{						  \
		return nullptr;		  \
	}						  \
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
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStack(lua_State* L, _type* value)						 \
{																															 \
	if (value == nullptr)  \
	{					   \
		lua_pushnil(L);	   \
	}					   \
	else				\
	{							 \
		_SetProperty(L, value);  \
	}							  \
}																															 \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStack(lua_State* L, const _type* value)				 \
{																															 \
	if (value == nullptr)  \
	{					   \
		lua_pushnil(L);	   \
	}					   \
	else                                                     \
	{														 \
		_SetProperty(L, const_cast<_type*>(value));			 \
	}														 \
}																															 \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStack(lua_State* L, _type& value)						 \
{																															 \
	_SetProperty(L, &value);																								 \
}																															 \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStack(lua_State* L, const _type& value)				 \
{																															 \
	_SetProperty(L, &const_cast<_type&>(value));																			 \
}																															  \

/*
For native data type (e.g. int, float, std::string, glm::vec4) only.
This is used for member property that is of native type.
If this is a value stored in class, when Lua tries to access native member property, it will get the value instead of the reference.
If this is a pointer or reference to native value stored somewhere else, Lua will get reference to that value.
*/
#define LUA_DEFINE_NATIVE_OBJECT_TYPE(_type)																				  \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStack(lua_State* L, _type* address, int /*flag*/)              \
{																															  \
	_ToLuaStack(L, *address);																								  \
}

/*
For member properties with custom data type.
No matter it's a value stored in class or a pointer to the custom data type in somewhere, Lua always gets the reference to the object.

Note: Retrieval behavior is the same for inclass object and pointed object
If this is the object in class, Lua gets reference to the object.
If this is the pointer to the object in somewhere else, Lua gets reference to that object.

Note: Writing behavior is different
If this is the object in class, assigning an Lua ref of same type to it will overwrite it's data with the data in that ref
If this is the pointer to the object in somewhere else, assigning an Lua ref of same type to it will overrite the pointer value, making it point to the object in that Lua ref
Use assignment operator with caution.
*/
#define LUA_DEFINE_CUSTOM_OBJECT_TYPE(_type)																				  \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStack(lua_State* L, _type* address, int /*flag*/)              \
{																															  \
	_SetProperty(L, address);																								  \
}																															   \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStack(lua_State* L, _type** address, int /*flag*/)              \
{																															  \
	if (address == nullptr)			 \
	{								 \
		lua_pushnil(L);				 \
	}								 \
	else							 \
	{								 \
		_SetProperty(L, *address);  \
	}								\
}																															   \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStack(lua_State* L, const _type* address, int /*flag*/)              \
{																															  \
	_SetProperty(L, const_cast<_type*>(address));																								  \
}																															   \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStack(lua_State* L, const _type** address, int /*flag*/)              \
{																															  \
	if (address == nullptr)                                  \
	{														 \
		lua_pushnil(L);										 \
	}														 \
	else													 \
	{														 \
		_SetProperty(L, *const_cast<_type**>(address));		 \
	}														 \
}																															   \

/*
This macro is for passing the custom object by value.
Lua doesn't pass user object by value in all cases, this is only used if C++ function accepts pass-by-value object or return object by value.
If C++ function accepts pass-by-value object, one temporary copy object will be created with Lua ref.
If C++ function returns an object by value, a Lua ref is created with that return value and that object's life cycle is then controlled by Lua.
*/
#define LUA_DEFINE_CUSTOM_COPY_TYPE(_type)																						 \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStack(lua_State* L, _type value)									\
{																																\
LuaWrapper<_type>* pointer = static_cast<LuaWrapper<_type>*>(lua_newuserdata(L, sizeof(LuaWrapper<_type>)));					\
new(pointer) LuaWrapper<_type>(true, new _type(value));																			\
int newTable = luaL_newmetatable(L, LuaWrapper<_type>::sName.c_str());														 \
		(newTable);\
assert(!newTable);																											 \
lua_setmetatable(L, -2);																									 \
}																															  \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStack(lua_State* L, _type&& value)									\
{																																\
LuaWrapper<_type>* pointer = static_cast<LuaWrapper<_type>*>(lua_newuserdata(L, sizeof(LuaWrapper<_type>)));					\
new(pointer) LuaWrapper<_type>(true, new _type(std::move(value)));																			\
int newTable = luaL_newmetatable(L, LuaWrapper<_type>::sName.c_str());														 \
		(newTable);\
assert(!newTable);																											 \
lua_setmetatable(L, -2);																									 \
}																															  \
template<> static inline _type GameEngine::Lua::LuaBind::_FromLuaStack(lua_State* L, int index)								   \
{																															   \
TYPE_CHECK(CheckArgType<_type>(L, index));																				   \
return *static_cast<LuaWrapper<_type>*>(lua_touserdata(L, index))->mObject;												   \
}

#define LUA_DEFINE_ENUM(_type)                                                                                                                \
template<> static inline _type GameEngine::Lua::LuaBind::_FromLuaStack(lua_State* L, int index)												  \
{																																			  \
	return static_cast<_type>(luaL_checkinteger(L, index));																					  \
}																																			  \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStack(lua_State* L, _type value)												  \
{																																			  \
	lua_pushinteger(L, static_cast<int>(value));																								  \
}																																			  \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStack(lua_State* L, _type* address, int /*flag*/)								  \
{																																			  \
	lua_pushinteger(L, static_cast<int>(*address));																							  \
}																																			  \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStack(lua_State* L, const _type* address, int /*flag*/)						  \
{																																			  \
	lua_pushinteger(L, static_cast<int>(*address));																							  \
}

/*
Define the wrapper type for custom data type with custom name in Lua
*/
#define DECLARE_LUA_WRAPPER(_type, _name, _authority)                            \
const std::string GameEngine::Lua::LuaWrapper<_type>::sName = _name; \
const uint64_t GameEngine::Lua::LuaWrapper<_type>::sTypeId = reinterpret_cast<uint64_t>(&GameEngine::Lua::LuaWrapper<_type>::sTypeId);  \
const bool GameEngine::Lua::LuaWrapper<_type>::sLuaAuthority = _authority;  \
LUA_DEFINE_POINTER_TYPE(_type)

// Vector functions for primitive types
// Will pass by value instead of reference
#define VECTOR_PASS_BY_VALUE(_type)																															  \
template<> static inline int VectorWrapper<_type>::Get(lua_State* L)																											  \
{																																								  \
	LuaWrapper<std::vector<_type>>* obj = static_cast<LuaWrapper<std::vector<_type>>*>(luaL_checkudata(L, 1, LuaWrapper<std::vector<_type>>::sName.c_str()));\
	std::vector<_type>& vec = *obj->mObject;																													   \
	size_t index = luaL_checkinteger(L, 2);																														   \
	LUA_ASSERT(L, index < vec.size());																															   \
																																								   \
	LuaBind* bind = static_cast<LuaBind*>(lua_touserdata(L, lua_upvalueindex(1)));																				   \
	bind->PushToLua<_type>(vec[index]);																															   \
	return 1;																																					   \
}																																									\
template<> static inline int VectorWrapper<_type>::Set(lua_State* L)																												\
{																																									\
	LuaWrapper<std::vector<_type>>* obj = static_cast<LuaWrapper<std::vector<_type>>*>(luaL_checkudata(L, 1, LuaWrapper<std::vector<_type>>::sName.c_str()));	\
	std::vector<_type>& vec = *obj->mObject;																														\
	size_t index = luaL_checkinteger(L, 2);																															\
	LUA_ASSERT(L, index < vec.size());																																\
																																									\
	LuaBind* bind = static_cast<LuaBind*>(lua_touserdata(L, lua_upvalueindex(1)));																					\
	vec[index] = bind->GetFromLua<_type>(3);																														\
	return 0;																																						\
}																																									 \
template<> static inline int VectorWrapper<_type>::PushBack(lua_State* L)																											 \
{																																									 \
	LuaWrapper<std::vector<_type>>* obj = static_cast<LuaWrapper<std::vector<_type>>*>(luaL_checkudata(L, 1, LuaWrapper<std::vector<_type>>::sName.c_str()));	 \
	std::vector<_type>& vec = *obj->mObject;																														 \
	LuaBind* bind = static_cast<LuaBind*>(lua_touserdata(L, lua_upvalueindex(1)));																					 \
	vec.push_back(bind->GetFromLua<_type>(2));																														 \
	return 0;																																						 \
}																																									 \
template<> static inline int VectorWrapper<_type>::Insert(lua_State* L)																											 \
{																																									 \
	LuaWrapper<std::vector<_type>>* obj = static_cast<LuaWrapper<std::vector<_type>>*>(luaL_checkudata(L, 1, LuaWrapper<std::vector<_type>>::sName.c_str()));	 \
	std::vector<_type>& vec = *obj->mObject;																														 \
	size_t index = luaL_checkinteger(L, 2);																															 \
	LUA_ASSERT(L, index <= vec.size());																																 \
																																									 \
	LuaBind* bind = static_cast<LuaBind*>(lua_touserdata(L, lua_upvalueindex(1)));																					 \
	vec.insert(vec.begin() + index, bind->GetFromLua<_type>(3));																									 \
	return 0;																																						 \
}

// If you want to use std::vector of this type, you must use this macro to declare the vector wrapper
// The class must not be abstract and must have valid copy constructor
#define DECLARE_LUA_VECTOR_WRAPPER(_type, _name)  \
const std::string GameEngine::Lua::LuaWrapper<std::vector<_type>>::sName = VECTOR_PREFIX ## _name;  \
const uint64_t GameEngine::Lua::LuaWrapper<std::vector<_type>>::sTypeId = reinterpret_cast<uint64_t>(&GameEngine::Lua::LuaWrapper<std::vector<_type>>::sTypeId);  \
const bool GameEngine::Lua::LuaWrapper<std::vector<_type>>::sLuaAuthority = true;  \
template<> static inline void LuaBind::_RegisterVector<_type>(LuaBind& bind)  \
{  \
	lua_State* mLuaState = bind.mLuaState;  \
	LuaWrapper<std::vector<_type>>::Register(mLuaState);  \
	int newTable = luaL_newmetatable(mLuaState, LuaWrapper<std::vector<_type>>::sName.c_str());  \
	newTable;  \
	assert(!newTable);																			  \
	bind.SetConstructor<std::vector<_type>>();															  \
	lua_pushstring(mLuaState, "Get");															  \
	lua_pushlightuserdata(mLuaState, &bind);														  \
	lua_pushcclosure(mLuaState, VectorWrapper<_type>::Get, 1);										  \
	lua_rawset(mLuaState, -3);																	  \
	lua_pushstring(mLuaState, "Set");															  \
	lua_pushlightuserdata(mLuaState, &bind);														  \
	lua_pushcclosure(mLuaState, VectorWrapper<_type>::Set, 1);										  \
	lua_rawset(mLuaState, -3);																	  \
	lua_pushstring(mLuaState, "Size");															  \
	lua_pushcfunction(mLuaState, VectorWrapper<_type>::Size);										  \
	lua_rawset(mLuaState, -3);																	  \
	lua_pushstring(mLuaState, "Clear");															  \
	lua_pushcfunction(mLuaState, VectorWrapper<_type>::Clear);										  \
	lua_rawset(mLuaState, -3);																	  \
	lua_pushstring(mLuaState, "Append");														  \
	lua_pushlightuserdata(mLuaState, &bind);														  \
	lua_pushcclosure(mLuaState, VectorWrapper<_type>::PushBack, 1);									  \
	lua_rawset(mLuaState, -3);																	  \
	lua_pushstring(mLuaState, "Insert");														  \
	lua_pushlightuserdata(mLuaState, &bind);														  \
	lua_pushcclosure(mLuaState, VectorWrapper<_type>::Insert, 1);									  \
	lua_rawset(mLuaState, -3);																	  \
	lua_pushstring(mLuaState, "RemoveAt");														  \
	lua_pushlightuserdata(mLuaState, &bind);														  \
	lua_pushcclosure(mLuaState, VectorWrapper<_type>::RemoveAt, 1);									  \
	lua_rawset(mLuaState, -3);																	  \
	lua_pop(mLuaState, 1);																		   \
}  \
LUA_DEFINE_POINTER_TYPE(std::vector<_type>)  \
LUA_DEFINE_CUSTOM_OBJECT_TYPE(std::vector<_type>);  \
LUA_DEFINE_CUSTOM_COPY_TYPE(std::vector<_type>);

// Declare the vector types for object type, pointer type and const pointer type
// A handy macro to replace calling the same macro three times
#define DECLARE_LUA_VECTOR_WRAPPER_ALL(_type, _name)			\
DECLARE_LUA_VECTOR_WRAPPER(_type, _name)						\
VECTOR_PASS_BY_VALUE(_type*);									 \
DECLARE_LUA_VECTOR_WRAPPER(_type*, _name##"*")					\
//VECTOR_PASS_BY_VALUE(const _type*);								 \
//DECLARE_LUA_VECTOR_WRAPPER(const _type*, "const "##_name##"*")

#define CLASS(...)
#define PROPERTY(...)
#define FUNCTION(...)
#define CONSTRUCTOR(...)
#define ENUM(...)

#define EVENT_MESSAGE EventMessage
#define NO_LUA_AUTHORITY NoLuaAuthority
