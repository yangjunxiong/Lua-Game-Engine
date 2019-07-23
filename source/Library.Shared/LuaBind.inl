#pragma once

#pragma region Function
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

#define SET_MEMBER_FUNCTION_BODY(_params, ...)																				   \
auto wrapper = [](lua_State* L) -> int																						   \
{																															   \
	auto* func = static_cast<Ret(Class::**)(__VA_ARGS__)>(lua_touserdata(L, lua_upvalueindex(1)));							   \
	return _CallCFunction<Ret>(L, [func](lua_State* L)->Ret																	   \
	{																														   \
		TYPE_CHECK(CheckArgType<Class>(L, 1));																					   \
		LuaWrapper<Class>* wrap = static_cast<LuaWrapper<Class>*>(lua_touserdata(L, 1));                                       \
		Class* obj = wrap->mObject;																								\
		return (obj->**func)(_params);																							\
	});																															\
};																																\
																																 \
int newTable = luaL_newmetatable(mLuaState, LuaWrapper<Class>::sName.c_str());													 \
		(newTable);\
assert(("Must register the class type before binding its member function", !newTable));											 \
lua_pushstring(mLuaState, key.c_str());																							 \
new (lua_newuserdata(mLuaState, sizeof(value))) (Ret(Class::*)(__VA_ARGS__))(value);											 \
lua_pushcclosure(mLuaState, wrapper, 1);																						 \
lua_rawset(mLuaState, -3);																										 \
lua_pop(mLuaState, 1);

#define SET_CONST_MEMBER_FUNCTION_BODY(_params, ...)																		\
auto wrapper = [](lua_State* L) -> int																						   \
{																															   \
	auto* func = static_cast<Ret(Class:: * *)(__VA_ARGS__) const>(lua_touserdata(L, lua_upvalueindex(1)));							   \
	return _CallCFunction<Ret>(L, [func](lua_State* L)->Ret																	   \
	{																														   \
		TYPE_CHECK(CheckArgType<Class>(L, 1));																					   \
		LuaWrapper<Class>* wrap = static_cast<LuaWrapper<Class>*>(lua_touserdata(L, 1));                                       \
		Class* obj = wrap->mObject;																								\
		return (obj->* * func)(_params);																							\
	});																															\
};																																\
\
int newTable = luaL_newmetatable(mLuaState, LuaWrapper<Class>::sName.c_str());													 \
		(newTable);\
assert(("Must register the class type before binding its member function", !newTable));											 \
lua_pushstring(mLuaState, key.c_str());																							 \
new (lua_newuserdata(mLuaState, sizeof(value))) (Ret(Class::*)(__VA_ARGS__)const)(value);											 \
lua_pushcclosure(mLuaState, wrapper, 1);																						 \
lua_rawset(mLuaState, -3);																										 \
lua_pop(mLuaState, 1);

#define SET_CONSTRUCTOR_BODY(...)                                                                                                \
auto factory = [](lua_State* L) -> int																						     \
{																																 \
	LuaWrapper<Class>* pointer = static_cast<LuaWrapper<Class>*>(lua_newuserdata(L, sizeof(LuaWrapper<Class>)));				 \
	new(pointer) LuaWrapper<Class>(true, new Class(__VA_ARGS__));																 \
																																 \
	int newTable = luaL_newmetatable(L, LuaWrapper<Class>::sName.c_str());														 \
		(newTable);\
	assert(!newTable);																											 \
	lua_setmetatable(L, -2);																									 \
																																 \
	return 1;																													 \
};																																 \
lua_getglobal(mLuaState, LuaWrapper<Class>::sName.c_str());																		  \
assert(lua_istable(mLuaState, -1));																								  \
lua_getmetatable(mLuaState, -1);																								  \
assert(lua_rawequal(mLuaState, -1, -2));																						  \
lua_pushstring(mLuaState, NEW_METHOD);																								  \
lua_pushcfunction(mLuaState, factory);																							  \
lua_rawset(mLuaState, -3);																										  \
lua_pop(mLuaState, 2);
																																									 
namespace GameEngine::Lua																																			 
{																																									 
	template <typename Ret>																																			 
	void LuaBind::SetFunction(const std::string& key, const std::function<Ret()>& value)
	{
		SET_FUNCTION_BODY(EXP());
	}

	template <typename Ret, typename Param1>
	void LuaBind::SetFunction(const std::string& key, const std::function<Ret(Param1)>& value)
	{
		SET_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 1)), Param1);
	}

	template <typename Ret, typename Param1, typename Param2>
	void LuaBind::SetFunction(const std::string& key, const std::function<Ret(Param1, Param2)>& value)
	{
		SET_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 1), _FromLuaStack<Param2>(L, 2)), Param1, Param2);
	}

	template <typename Ret, typename Param1, typename Param2, typename Param3>
	void LuaBind::SetFunction(const std::string& key, const std::function<Ret(Param1, Param2, Param3)>& value)
	{
		SET_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 1), _FromLuaStack<Param2>(L, 2), _FromLuaStack<Param3>(L, 3)), Param1, Param2, Param3);
	}

	template <typename Ret, typename Param1, typename Param2, typename Param3, typename Param4>
	void LuaBind::SetFunction(const std::string& key, const std::function<Ret(Param1, Param2, Param3, Param4)>& value)
	{
		SET_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 1), _FromLuaStack<Param2>(L, 2), _FromLuaStack<Param3>(L, 3), _FromLuaStack<Param4>(L, 4)), Param1, Param2, Param3, Param4);
	}

	template <typename Ret, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
	void LuaBind::SetFunction(const std::string& key, const std::function<Ret(Param1, Param2, Param3, Param4, Param5)>& value)
	{
		SET_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 1), _FromLuaStack<Param2>(L, 2), _FromLuaStack<Param3>(L, 3), _FromLuaStack<Param4>(L, 4), _FromLuaStack<Param5>(L, 5)), Param1, Param2, Param3, Param4, Param5);
	}

	template <typename Ret, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6>
	void LuaBind::SetFunction(const std::string& key, const std::function<Ret(Param1, Param2, Param3, Param4, Param5, Param6)>& value)
	{
		SET_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 1), _FromLuaStack<Param2>(L, 2), _FromLuaStack<Param3>(L, 3), _FromLuaStack<Param4>(L, 4), _FromLuaStack<Param5>(L, 5), _FromLuaStack<Param6>(L, 6)), Param1, Param2, Param3, Param4, Param5, Param6);
	}

	template <typename Class, typename Ret>
	void LuaBind::SetFunction(const std::string& key, Ret(Class::* value)())
	{
		SET_MEMBER_FUNCTION_BODY(EXP());
	}

	template <typename Class, typename Ret, typename Param1>
	void LuaBind::SetFunction(const std::string& key, Ret(Class::* value)(Param1))
	{
		SET_MEMBER_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 2)), Param1);
	}

	template <typename Class, typename Ret, typename Param1, typename Param2>
	void LuaBind::SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2))
	{
		SET_MEMBER_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 2), _FromLuaStack<Param2>(L, 3)), Param1, Param2);
	}

	template <typename Class, typename Ret, typename Param1, typename Param2, typename Param3>
	void LuaBind::SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2, Param3))
	{
		SET_MEMBER_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 2), _FromLuaStack<Param2>(L, 3), _FromLuaStack<Param3>(L, 4)), Param1, Param2, Param3);
	}

	template <typename Class, typename Ret, typename Param1, typename Param2, typename Param3, typename Param4>
	void LuaBind::SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2, Param3, Param4))
	{
		SET_MEMBER_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 2), _FromLuaStack<Param2>(L, 3), _FromLuaStack<Param3>(L, 4), _FromLuaStack<Param4>(L, 5)), Param1, Param2, Param3, Param4);
	}

	template <typename Class, typename Ret, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
	void LuaBind::SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2, Param3, Param4, Param5))
	{
		SET_MEMBER_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 2), _FromLuaStack<Param2>(L, 3), _FromLuaStack<Param3>(L, 4), _FromLuaStack<Param4>(L, 5), _FromLuaStack<Param5>(L, 6)), Param1, Param2, Param3, Param4, Param5);
	}

	template <typename Class, typename Ret, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6>
	void LuaBind::SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2, Param3, Param4, Param5, Param6))
	{
		SET_MEMBER_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 2), _FromLuaStack<Param2>(L, 3), _FromLuaStack<Param3>(L, 4), _FromLuaStack<Param4>(L, 5), _FromLuaStack<Param5>(L, 6), _FromLuaStack<Param6>(L, 7)), Param1, Param2, Param3, Param4, Param5, Param6);
	}

	template <typename Class, typename Ret>
	void LuaBind::SetFunction(const std::string& key, Ret(Class::* value)() const)
	{
		SET_CONST_MEMBER_FUNCTION_BODY(EXP());
	}

	template <typename Class, typename Ret, typename Param1>
	void LuaBind::SetFunction(const std::string& key, Ret(Class::* value)(Param1) const)
	{
		SET_CONST_MEMBER_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 2)), Param1);
	}

	template <typename Class, typename Ret, typename Param1, typename Param2>
	void LuaBind::SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2) const)
	{
		SET_CONST_MEMBER_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 2), _FromLuaStack<Param2>(L, 3)), Param1, Param2);
	}

	template <typename Class, typename Ret, typename Param1, typename Param2, typename Param3>
	void LuaBind::SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2, Param3) const)
	{
		SET_CONST_MEMBER_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 2), _FromLuaStack<Param2>(L, 3), _FromLuaStack<Param3>(L, 4)), Param1, Param2, Param3);
	}

	template <typename Class, typename Ret, typename Param1, typename Param2, typename Param3, typename Param4>
	void LuaBind::SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2, Param3, Param4) const)
	{
		SET_CONST_MEMBER_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 2), _FromLuaStack<Param2>(L, 3), _FromLuaStack<Param3>(L, 4), _FromLuaStack<Param4>(L, 5)), Param1, Param2, Param3, Param4);
	}

	template <typename Class, typename Ret, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
	void LuaBind::SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2, Param3, Param4, Param5) const)
	{
		SET_CONST_MEMBER_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 2), _FromLuaStack<Param2>(L, 3), _FromLuaStack<Param3>(L, 4), _FromLuaStack<Param4>(L, 5), _FromLuaStack<Param5>(L, 6)), Param1, Param2, Param3, Param4, Param5);
	}

	template <typename Class, typename Ret, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6>
	void LuaBind::SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2, Param3, Param4, Param5, Param6) const)
	{
		SET_CONST_MEMBER_FUNCTION_BODY(EXP(_FromLuaStack<Param1>(L, 2), _FromLuaStack<Param2>(L, 3), _FromLuaStack<Param3>(L, 4), _FromLuaStack<Param4>(L, 5), _FromLuaStack<Param5>(L, 6), _FromLuaStack<Param6>(L, 7)), Param1, Param2, Param3, Param4, Param5, Param6);
	}

	template <typename Class, typename T>
	void LuaBind::SetFunction(const std::string& key, T func)
	{
		lua_getglobal(mLuaState, LuaWrapper<Class>::sName.c_str());
		assert(lua_istable(mLuaState, -1));
		lua_getmetatable(mLuaState, -1);
		assert(lua_rawequal(mLuaState, -1, -2));
		SetFunction(key, std::function(func));
		lua_pop(mLuaState, 2);
	}

	template <typename Class>
	void LuaBind::SetConstructor()
	{
		SET_CONSTRUCTOR_BODY(EXP());
	}

	template <typename Class, typename Param1>
	void LuaBind::SetConstructor()
	{
		SET_CONSTRUCTOR_BODY(EXP(_FromLuaStack<Param1>(L, 1)));
	}

	template <typename Class, typename Param1, typename Param2>
	void LuaBind::SetConstructor()
	{
		SET_CONSTRUCTOR_BODY(EXP(_FromLuaStack<Param1>(L, 1), _FromLuaStack<Param2>(L, 2)));
	}

	template <typename Class, typename Param1, typename Param2, typename Param3>
	void LuaBind::SetConstructor()
	{
		SET_CONSTRUCTOR_BODY(EXP(_FromLuaStack<Param1>(L, 1), _FromLuaStack<Param2>(L, 2), _FromLuaStack<Param3>(L, 3)));
	}

	template <typename Class, typename Param1, typename Param2, typename Param3, typename Param4>
	void LuaBind::SetConstructor()
	{
		SET_CONSTRUCTOR_BODY(EXP(_FromLuaStack<Param1>(L, 1), _FromLuaStack<Param2>(L, 2), _FromLuaStack<Param3>(L, 3), _FromLuaStack<Param4>(L, 4)));
	}

	template <typename Class, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
	void LuaBind::SetConstructor()
	{
		SET_CONSTRUCTOR_BODY(EXP(_FromLuaStack<Param1>(L, 1), _FromLuaStack<Param2>(L, 2), _FromLuaStack<Param3>(L, 3), _FromLuaStack<Param4>(L, 4), _FromLuaStack<Param5>(L, 5)));
	}

	template <typename Class, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6>
	void LuaBind::SetConstructor()
	{
		SET_CONSTRUCTOR_BODY(EXP(_FromLuaStack<Param1>(L, 1), _FromLuaStack<Param2>(L, 2), _FromLuaStack<Param3>(L, 3), _FromLuaStack<Param4>(L, 4), _FromLuaStack<Param5>(L, 5), _FromLuaStack<Param6>(L, 6)));
	}

	template <typename Enum>
	void LuaBind::SetEnumValue(const std::string& enumName, int value)
	{
		int newTable = luaL_newmetatable(mLuaState, LuaWrapper<Enum>::sName.c_str());
		newTable;
		assert(!newTable);
		lua_pushstring(mLuaState, enumName.c_str());
		lua_pushinteger(mLuaState, value);
		lua_rawset(mLuaState, -3);
		lua_pop(mLuaState, 1);
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

	template <typename T>
	static inline bool LuaBind::CheckArgType(lua_State* L, int index)
	{
		if (!Registry::Is(L, index, LuaWrapper<T>::sTypeId))
		{
			luaL_checkudata(L, index, LuaWrapper<T>::sName.c_str());  // Gurantee to fail, so will trigger lua runtime error
		}

		return true;
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

#pragma region SetLuaProperty
	template <typename T>
	void LuaBind::SetProperty(const std::string& key, T* value, int index)
	{
		_SetProperty(mLuaState, value, key, index);
	}

	template <typename Class, typename T>
	void LuaBind::SetProperty(const std::string& key, T Class::* address, bool writable)
	{
		// Get metatable for target class
		int newTable = luaL_newmetatable(mLuaState, LuaWrapper<Class>::sName.c_str());
		(newTable);
		assert(("Must register class type before setting an object as property", !newTable));

		// Set getter and getter depends on writability
		_SetPropertyFunction(key, address, MEMBER_GETTER, &_PropertyGetter<Class, T>);
		if (writable)
		{
			_SetPropertyFunction(key, address, MEMBER_SETTER, &_PropertySetter<Class, T>);
		}
		else
		{
			_SetPropertyFunction(key, address, MEMBER_SETTER, &_ConstPropertySetter<Class>);
		}

		// Pop class metatable, restore stack
		lua_pop(mLuaState, 1);
	}

	template <typename Class, typename T>
	void LuaBind::SetProperty(const std::string& key, const T Class::* address)
	{
		// Get metatable for target class
		int newTable = luaL_newmetatable(mLuaState, LuaWrapper<Class>::sName.c_str());
		assert(("Must register class type before setting an object as property", !newTable));

		// Set const version getter and const error setter
		_SetPropertyFunction(key, address, MEMBER_GETTER, &_PropertyGetter<Class, T, int>);
		_SetPropertyFunction(key, address, MEMBER_SETTER, &_ConstPropertySetter<Class>);

		// Pop class metatable, restore stack
		lua_pop(mLuaState, 1);
	}

	template <typename Class, typename T>
	void LuaBind::SetProperty(const std::string& key, T* address, bool writable)
	{
		// Get metatable for target class
		lua_getglobal(mLuaState, LuaWrapper<Class>::sName.c_str());
		assert(("Must register class type before setting an object as property", lua_istable(mLuaState, -1)));

		// Set getter and setter
		_SetStaticPropertyFunction(key, address, STATIC_MEMBER_GETTER, &LuaBind::_StaticPropertyGetter<T>);
		if (writable)
		{
			_SetStaticPropertyFunction(key, address, STATIC_MEMBER_SETTER, &LuaBind::_StaticPropertySetter<T>);
		}
		else
		{
			_SetStaticPropertyFunction(key, address, STATIC_MEMBER_SETTER, &LuaBind::_ConstPropertySetter<Class>);
		}

		// Pop class metatable, restore stack
		lua_pop(mLuaState, 1);
	}

	template <typename Class, typename T>
	void LuaBind::SetProperty(const std::string& key, const T* address)
	{
		// Get metatable for target class
		lua_getglobal(mLuaState, LuaWrapper<Class>::sName.c_str());
		assert(("Must register class type before setting an object as property", lua_istable(mLuaState, -1)));

		// Set getter and setter
		_SetStaticPropertyFunction(key, const_cast<T*>(address), STATIC_MEMBER_GETTER, &LuaBind::_StaticPropertyGetter<T, int>);
		_SetStaticPropertyFunction(key, const_cast<T*>(address), STATIC_MEMBER_SETTER, &LuaBind::_ConstPropertySetter<Class>);

		// Pop class metatable, restore stack
		lua_pop(mLuaState, 1);
	}

	template <typename T>
	static inline LuaWrapper<T>* LuaBind::_SetProperty(lua_State* L, T* value, const std::string& key, int index)
	{
		// Create wrapper for object
		LuaWrapper<T>* pointer = static_cast<LuaWrapper<T>*>(lua_newuserdata(L, sizeof(LuaWrapper<T>)));
		new(pointer) LuaWrapper<T>(false, value);

		// Set metatable for this wrapper
		int newTable = luaL_newmetatable(L, LuaWrapper<T>::sName.c_str());
		(newTable);
		assert(("Must register class type before setting an object as property", !newTable));
		lua_setmetatable(L, -2);

		// Push to global or a table if key name is provided
		if (!key.empty())
		{
			if (index == 0)
			{
				lua_setglobal(L, key.c_str());
			}
			else
			{
				lua_setfield(L, index > 0 ? index : index - 1, key.c_str());
			}
		}

		return pointer;
	}

	template <typename Class, typename T>
	void LuaBind::_SetPropertyFunction(const std::string& key, T Class::* address, const char* tableName, int(*func)(lua_State*))
	{
		lua_pushstring(mLuaState, tableName);
		lua_rawget(mLuaState, -2);                                         // Push required table to the stack
		lua_pushstring(mLuaState, key.c_str());                            // Push name of variable
		new (lua_newuserdata(mLuaState, sizeof(T Class::*))) (T Class::*)(address);   // Push variable memory offset as first upvalue
		lua_pushstring(mLuaState, key.c_str());                                       // Push member name as second upvalue for error report purpose
		lua_pushlightuserdata(mLuaState, this);                            // Push LuaBind object pointer as third upvalue
		lua_pushcclosure(mLuaState, func, 3);                              // Push function with upvalue
		lua_rawset(mLuaState, -3);                                         // Set getter function to __propget table
		lua_pop(mLuaState, 1);											   // Pop __propget table
	}

	void LuaBind::_SetStaticPropertyFunction(const std::string& key, void* address, const char* tableName, int(*func)(lua_State*))
	{
		lua_pushstring(mLuaState, tableName);
		lua_rawget(mLuaState, -2);                                         // Push __propset table to the stack
		lua_pushstring(mLuaState, key.c_str());                            // Push name of variable
		lua_pushlightuserdata(mLuaState, (address));                       // Push variable memory offset as first upvalue
		lua_pushstring(mLuaState, key.c_str());                            // Push member name as second upvalue for error report purpose
		lua_pushlightuserdata(mLuaState, this);                            // Push LuaBind object pointer as third upvalue
		lua_pushcclosure(mLuaState, func, 3);                              // Push function with upvalue
		lua_rawset(mLuaState, -3);                                         // Set getter function to __propget table
		lua_pop(mLuaState, 1);											   // Pop __propget table
	}

	template <typename Class, typename T>
	static inline int LuaBind::_PropertyGetter(lua_State* L)
	{
		auto* wrapper = static_cast<LuaWrapper<Class>*>(lua_touserdata(L, 1));
		T Class::* address = *static_cast<T Class:: * *>(lua_touserdata(L, lua_upvalueindex(1)));
		LuaBind* bind = static_cast<LuaBind*>(lua_touserdata(L, lua_upvalueindex(3)));
		Class* obj = wrapper->mObject;
		bind->_ToLuaStack(L, (T*)&(obj->*address), 0);
		return 1;
	}

	template <typename Class, typename T, typename Flag>
	static inline int LuaBind::_PropertyGetter(lua_State* L)
	{
		auto* wrapper = static_cast<LuaWrapper<Class>*>(lua_touserdata(L, 1));
		const T Class::* address = *static_cast<const T Class::**>(lua_touserdata(L, lua_upvalueindex(1)));
		LuaBind* bind = static_cast<LuaBind*>(lua_touserdata(L, lua_upvalueindex(3)));
		Class* obj = wrapper->mObject;
		bind->_ToLuaStack(L, const_cast<T*>((const T*) & (obj->*address)), 0);
		return 1;
	}

	template <typename Class, typename T>
	static inline int LuaBind::_PropertySetter(lua_State* L)
	{
		auto* wrapper = static_cast<LuaWrapper<Class>*>(lua_touserdata(L, 1));
		T Class::* address = *static_cast<T Class:: * *>(lua_touserdata(L, lua_upvalueindex(1)));
		LuaBind* bind = static_cast<LuaBind*>(lua_touserdata(L, lua_upvalueindex(3)));
		Class* obj = wrapper->mObject;
		obj->*address = bind->_FromLuaStack<T>(L, 2);
		return 0;
	}

	template <typename T>
	static inline int LuaBind::_StaticPropertyGetter(lua_State* L)
	{
		T* address = static_cast<T*>(lua_touserdata(L, lua_upvalueindex(1)));
		LuaBind* bind = static_cast<LuaBind*>(lua_touserdata(L, lua_upvalueindex(3)));
		bind->_ToLuaStack(L, address, 0);
		return 1;
	}

	template <typename T, typename Flag>
	static inline int LuaBind::_StaticPropertyGetter(lua_State* L)
	{
		const T* address = static_cast<const T*>(lua_touserdata(L, lua_upvalueindex(1)));
		LuaBind* bind = static_cast<LuaBind*>(lua_touserdata(L, lua_upvalueindex(3)));
		bind->_ToLuaStack(L, const_cast<T*>(address), 0);
		return 1;
	}

	template <typename T>
	static inline int LuaBind::_StaticPropertySetter(lua_State* L)
	{
		T* address = static_cast<T*>(lua_touserdata(L, lua_upvalueindex(1)));
		LuaBind* bind = static_cast<LuaBind*>(lua_touserdata(L, lua_upvalueindex(3)));
		*address = bind->_FromLuaStack<T>(L, 1);
		return 0;
	}

	template <typename Class>
	int LuaBind::_ConstPropertySetter(lua_State* L)
	{
		return luaL_error(L, "Member variable \"%s\" of class \"%s\" is read-only",
			lua_tostring(L, lua_upvalueindex(2)),
			LuaWrapper<Class>::sName.c_str());
	}
#pragma endregion

#pragma region CallLuaFunction
	template <typename Ret, typename... Args>
	Ret LuaBind::CallFunction(const std::string& name, Args&&... args)
	{
		// Get Lua function to stack
		if (mTableStack.IsEmpty())
		{
			lua_getglobal(mLuaState, name.c_str());
		}
		else
		{
			lua_getfield(mLuaState, mTableStack.Peak().mIndex, name.c_str());
		}
		if (!lua_isfunction(mLuaState, -1))
		{
			throw std::runtime_error("Lua function can't be found");
		}

		// Call function
		int argCount = 0;
		_PushFunctionArgument(argCount, std::forward<Args>(args)...);
		int error = lua_pcall(mLuaState, argCount, 1, 0);
		_ProcessError(error);
		return _FromLuaStack<Ret>(mLuaState, -1);
	}

	template <typename... Args>
	void LuaBind::CallFunctionNoReturn(const std::string& name, Args&&... args)
	{
		// Get Lua function to stack
		if (mTableStack.IsEmpty())
		{
			lua_getglobal(mLuaState, name.c_str());
		}
		else
		{
			lua_getfield(mLuaState, mTableStack.Peak().mIndex, name.c_str());
		}
		if (!lua_isfunction(mLuaState, -1))
		{
			throw std::runtime_error("Lua function can't be found");
		}

		// Call function
		int argCount = 0;
		_PushFunctionArgument(argCount, std::forward<Args>(args)...);
		int error = lua_pcall(mLuaState, argCount, 0, 0);
		_ProcessError(error);
	}

	template <typename T, typename... Args>
	void LuaBind::_PushFunctionArgument(int& count, T&& t, Args&&... args)
	{
		++count;
		_ToLuaStack(mLuaState, t);
		_PushFunctionArgument(count, std::forward<Args>(args)...);
	}

	void LuaBind::_PushFunctionArgument(int& /*count*/)
	{}
#pragma endregion


#pragma region FromLuaStack
	template <typename T>
	T LuaBind::GetFromLua(size_t index)
	{
		return _FromLuaStack<T>(mLuaState, static_cast<int>(index));
	}

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

	template<> static inline unsigned int LuaBind::_FromLuaStack(lua_State* L, int index)
	{
		return static_cast<unsigned int>(luaL_checknumber(L, index));
	}

	template<> static inline unsigned long long LuaBind::_FromLuaStack(lua_State* L, int index)
	{
		return static_cast<unsigned long long>(luaL_checknumber(L, index));
	}

	template<> static inline unsigned char LuaBind::_FromLuaStack(lua_State* L, int index)
	{
		return static_cast<unsigned char>(luaL_checknumber(L, index));
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
	template <typename T>
	void LuaBind::PushToLua(T value)
	{
		_ToLuaStack(mLuaState, value);
	}

	template <typename T>
	void LuaBind::PushRefToLua(T* value)
	{
		_ToLuaStack(mLuaState, value, 0);
	}

	template<> static inline void LuaBind::_ToLuaStack(lua_State* L, int value)
	{
		lua_pushinteger(L, static_cast<lua_Integer>(value));
	}

	template<> static inline void LuaBind::_ToLuaStack(lua_State* L, long long value)
	{
		lua_pushinteger(L, value);
	}

	template<> static inline void LuaBind::_ToLuaStack(lua_State* L, float value)
	{
		lua_pushnumber(L, static_cast<lua_Number>(value));
	}

	template<> static inline void LuaBind::_ToLuaStack(lua_State* L, double value)
	{
		lua_pushnumber(L, value);
	}

	template<> static inline void LuaBind::_ToLuaStack(lua_State* L, bool value)
	{
		lua_pushboolean(L, value);
	}

	template<> static inline void LuaBind::_ToLuaStack(lua_State* L, char value)
	{
		lua_pushlstring(L, &value, 1);
	}

	template<> static inline void LuaBind::_ToLuaStack(lua_State* L, unsigned int value)
	{
		lua_pushinteger(L, static_cast<lua_Integer>(value));
	}

	template<> static inline void LuaBind::_ToLuaStack(lua_State* L, unsigned long long value)
	{
		lua_pushinteger(L, static_cast<lua_Integer>(value));
	}

	template<> static inline void LuaBind::_ToLuaStack(lua_State* L, unsigned char value)
	{
		lua_pushinteger(L, static_cast<lua_Integer>(value));
	}

	template<> static inline void LuaBind::_ToLuaStack(lua_State* L, char* value)
	{
		lua_pushstring(L, value);
	}

	template<> static inline void LuaBind::_ToLuaStack(lua_State* L, const char* value)
	{
		lua_pushstring(L, value);
	}

	template<> static inline void LuaBind::_ToLuaStack(lua_State* L, std::string value)
	{
		lua_pushstring(L, value.c_str());
	}
#pragma endregion

#pragma region VectorWrapper
	template <typename T>
	static inline void LuaBind::_RegisterVector(LuaBind& bind) { bind; }

	template <typename T>
	int VectorWrapper<T>::Size(lua_State* L)
	{
		LuaWrapper<std::vector<T>>* obj = static_cast<LuaWrapper<std::vector<T>>*>(luaL_checkudata(L, 1, LuaWrapper<std::vector<T>>::sName.c_str()));
		std::vector<T>& vec = *obj->mObject;
		lua_pushinteger(L, vec.size());
		return 1;
	}

	template <typename T>
	int VectorWrapper<T>::Get(lua_State* L)
	{
		LuaWrapper<std::vector<T>>* obj = static_cast<LuaWrapper<std::vector<T>>*>(luaL_checkudata(L, 1, LuaWrapper<std::vector<T>>::sName.c_str()));
		std::vector<T>& vec = *obj->mObject;
		size_t index = luaL_checkinteger(L, 2);
		LUA_ASSERT(L, index < vec.size());

		LuaBind* bind = static_cast<LuaBind*>(lua_touserdata(L, lua_upvalueindex(1)));
		bind->PushRefToLua<T>(&vec[index]);
		return 1;
	}

	template <typename T>
	int VectorWrapper<T>::Set(lua_State* L)
	{
		LuaWrapper<std::vector<T>>* obj = static_cast<LuaWrapper<std::vector<T>>*>(luaL_checkudata(L, 1, LuaWrapper<std::vector<T>>::sName.c_str()));
		std::vector<T>& vec = *obj->mObject;
		size_t index = luaL_checkinteger(L, 2);
		LUA_ASSERT(L, index < vec.size());

		LuaBind* bind = static_cast<LuaBind*>(lua_touserdata(L, lua_upvalueindex(1)));
		TYPE_CHECK(bind->CheckArgType<T>(L, 3));
		vec[index] = *bind->GetFromLua<T*>(3);
		return 0;
	}

	template <typename T>
	int VectorWrapper<T>::Clear(lua_State* L)
	{
		LuaWrapper<std::vector<T>>* obj = static_cast<LuaWrapper<std::vector<T>>*>(luaL_checkudata(L, 1, LuaWrapper<std::vector<T>>::sName.c_str()));
		std::vector<T>& vec = *obj->mObject;
		vec.clear();
		return 0;
	}

	template <typename T>
	int VectorWrapper<T>::PushBack(lua_State* L)
	{
		LuaWrapper<std::vector<T>>* obj = static_cast<LuaWrapper<std::vector<T>>*>(luaL_checkudata(L, 1, LuaWrapper<std::vector<T>>::sName.c_str()));
		std::vector<T>& vec = *obj->mObject;
		LuaBind* bind = static_cast<LuaBind*>(lua_touserdata(L, lua_upvalueindex(1)));
		TYPE_CHECK(bind->CheckArgType<T>(L, 2));
		vec.push_back(*bind->GetFromLua<T*>(2));
		return 0;
	}

	template <typename T>
	int VectorWrapper<T>::Insert(lua_State* L)
	{
		LuaWrapper<std::vector<T>>* obj = static_cast<LuaWrapper<std::vector<T>>*>(luaL_checkudata(L, 1, LuaWrapper<std::vector<T>>::sName.c_str()));
		std::vector<T>& vec = *obj->mObject;
		size_t index = luaL_checkinteger(L, 2);
		LUA_ASSERT(L, index <= vec.size());

		LuaBind* bind = static_cast<LuaBind*>(lua_touserdata(L, lua_upvalueindex(1)));
		TYPE_CHECK(bind->CheckArgType<T>(L, 3));
		vec.insert(vec.begin() + index, *bind->GetFromLua<T*>(3));
		return 0;
	}

	template <typename T>
	int VectorWrapper<T>::RemoveAt(lua_State* L)
	{
		LuaWrapper<std::vector<T>>* obj = static_cast<LuaWrapper<std::vector<T>>*>(luaL_checkudata(L, 1, LuaWrapper<std::vector<T>>::sName.c_str()));
		std::vector<T>& vec = *obj->mObject;
		size_t index = luaL_checkinteger(L, 2);
		vec.erase(vec.begin() + index);
		return 0;
	}

	VECTOR_PASS_BY_VALUE(int);
	VECTOR_PASS_BY_VALUE(unsigned int);
	VECTOR_PASS_BY_VALUE(long long);
	VECTOR_PASS_BY_VALUE(unsigned long long);
	VECTOR_PASS_BY_VALUE(float);
	VECTOR_PASS_BY_VALUE(double);
	VECTOR_PASS_BY_VALUE(bool);
	VECTOR_PASS_BY_VALUE(char);
	VECTOR_PASS_BY_VALUE(unsigned char);
	VECTOR_PASS_BY_VALUE(std::string);
	VECTOR_PASS_BY_VALUE(glm::vec4);
#pragma endregion

	template <typename T>
	void LuaBind::RegisterType()
	{
		LuaWrapper<T>::Register(mLuaState);
		_AdditionalRegister<T>(*this);

		// Register vector type
		_RegisterVector<T>(*this);
		_RegisterVector<T*>(*this);
		_RegisterVector<const T*>(*this);
	}

	template <typename T, typename Parent>
	void LuaBind::RegisterType()
	{
		LuaWrapper<T>::template Register<Parent>(mLuaState);
		_AdditionalRegister<T>(*this);

		// Register vector type
		_RegisterVector<T>(*this);
		_RegisterVector<T*>(*this);
		_RegisterVector<const T*>(*this);
	}
}
