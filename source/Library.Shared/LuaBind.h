#pragma once
#include "Macro.h"
#include "lua.hpp"
#include "Stack.h"
#include "vector.h"
#include "HashMap.h"
#include "LuaWrapper.h"
#include <glm/glm.hpp>

namespace GameEngine::Lua
{
	class LuaBind final
	{
	public:
		/// <summary>
		/// Default and only constructor, creates a lua state and register primitive types
		/// </summary>
		LuaBind();

		/// <summary>
		/// Destructor, destroy the lua state
		/// </summary>
		~LuaBind();
		LuaBind(const LuaBind& other) = delete;
		LuaBind(LuaBind&& other) = delete;
		LuaBind& operator=(const LuaBind& other) = delete;
		LuaBind& operator==(LuaBind&& other) = delete;
		lua_State* LuaState() const;

		/// <summary>
		/// Register a type to lua binding
		/// </summary>
		template <typename T>
		void RegisterType();
		template <typename T, typename Parent>
		void RegisterType();

		/// <summary>
		/// Clear current lua stack
		/// </summary>
		void ClearStack();

		/// <summary>
		/// Create a new table on the top of current stack
		/// </summary>
		/// <param name="name">Name of new table</param>
		void CreateTable(const std::string& name);

		/// <summary>
		/// Open the table under current stack context. If current table stack is empty, will open global table, otherwise will open nested table belonging to current table on the top of stack
		/// </summary>
		/// <param name="path">The path to target table, delimited by .</param>
		void OpenTable(const std::string& path);

		/// <summary>
		/// Close current table
		/// </summary>
		void CloseTable();
		std::string CurrentTableName() const;
		int CurrentTableIndex() const;

		/// <summary>
		/// Set the value of a Lua variable. If current stack has table, will set the variable in the table, otherwise will set global variable
		/// Note this function doesn't pass reference to Lua, Lua variable only holds the copy of the data passed in.
		/// So any change in Lua to the variable will not influence C++ value
		/// </summary>
		/// <param name="key">The name of variable to set</param>
		/// <param name="value">The value to set to</param>
		template <typename T>
		void SetLuaValue(const std::string& key, const T& value) { static_assert(false, "Unsupported value type, only int32, int64, float, double, std::string are supported"); };

		/// <summary>
		/// Bind a C++ object to Lua, so both share the same memory location. Operation happended in Lua will also change the content in C++.
		/// Primitive types are supported natively, for custom classes, user needs to register the class with LuaWrapper before binding.
		/// This operation happens under current lua stack. If user doesn't provide index, it will set the property to global, otherwise it will set the property to the table whose index on stack is "index"
		/// </summary>
		/// <param name="key">The property name in lua</param>
		/// <param name="value">The pointer to the object</param>
		/// <param name="index">Index of target table on Lua stack. If index is 0, will set the property to global</param>
		template <typename T>
		void SetProperty(const std::string& key, T* value, int index = 0);

		/// <summary>
		/// Bind C++ class member variable to Lua. So Lua script can access and modify the member values as if in C++
		/// Before calling this function, be sure to register corresponding class type first
		/// </summary>
		/// <param name="key">Name of the member variable in Lua</param>
		/// <param name="address">The pointer to class member</param>
		/// <param name="writable">Whether this member is editable in Lua. If not, when Lua tries to write the value, it will trigger Lua error</param>
		template <typename Class, typename T>
		void SetProperty(const std::string& key, T Class::* address, bool writable = true);
		template <typename Class, typename T>
		void SetProperty(const std::string& key, const T Class::* address);

		/// <summary>
		/// Bind C++ class static member variables to Lua
		/// Lua can access the static member variables with ClassName.VariableName
		/// </summary>
		/// <param name="key">Name of the variable</param>
		/// <param name="address">Pointer to the static member variable</param>
		/// <param name="writable">Whehter this variable can be modified in Lua</param>
		template <typename Class, typename T>
		void SetProperty(const std::string& key, T* address, bool writable = true);
		template <typename Class, typename T>
		void SetProperty(const std::string& key, const T* address);

		/// <summary>
		/// Set free C function or lambda function to a Lua variable
		/// If current stack has table, will set the variable in the table, otherwise will set global variable
		/// </summary>
		/// <param name="key">Name of the function in Lua</param>
		/// <param name="value">function to set</param>
		template <typename Ret>
		void SetFunction(const std::string& key, const std::function<Ret()>& value);
		template <typename Ret, typename Param1>
		void SetFunction(const std::string& key, const std::function<Ret(Param1)>& value);
		template <typename Ret, typename Param1, typename Param2>
		void SetFunction(const std::string& key, const std::function<Ret(Param1, Param2)>& value);
		template <typename Ret, typename Param1, typename Param2, typename Param3>
		void SetFunction(const std::string& key, const std::function<Ret(Param1, Param2, Param3)>& value);
		template <typename Ret, typename Param1, typename Param2, typename Param3, typename Param4>
		void SetFunction(const std::string& key, const std::function<Ret(Param1, Param2, Param3, Param4)>& value);
		template <typename Ret, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
		void SetFunction(const std::string& key, const std::function<Ret(Param1, Param2, Param3, Param4, Param5)>& value);
		template <typename Ret, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6>
		void SetFunction(const std::string& key, const std::function<Ret(Param1, Param2, Param3, Param4, Param5, Param6)>& value);

		/// <summary>
		/// Set member function (static or non-static) to a Lua name.
		/// Before running this function, be sure to register corresponding class type with LuaWrapper<class>::Register().
		/// This will always change the global Lua registry regardless of current Lua stack state
		/// Member function in Lua can only be called against an object with same class type.
		/// To call a member function in Lua, use "obj:func(...)" or "obj.func(obj, ...)"
		/// </summary>
		/// <param name="key">Name of the member function in Lua</param>
		/// <param name="value">Pointer to member function</param>
		template <typename Class, typename Ret>
		void SetFunction(const std::string& key, Ret (Class::* value)());
		template <typename Class, typename Ret, typename Param1>
		void SetFunction(const std::string& key, Ret(Class::* value)(Param1));
		template <typename Class, typename Ret, typename Param1, typename Param2>
		void SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2));
		template <typename Class, typename Ret, typename Param1, typename Param2, typename Param3>
		void SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2, Param3));
		template <typename Class, typename Ret, typename Param1, typename Param2, typename Param3, typename Param4>
		void SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2, Param3, Param4));
		template <typename Class, typename Ret, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
		void SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2, Param3, Param4, Param5));
		template <typename Class, typename Ret, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6>
		void SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2, Param3, Param4, Param5, Param6));
		template <typename Class, typename Ret>
		void SetFunction(const std::string& key, Ret(Class::* value)() const);
		template <typename Class, typename Ret, typename Param1>
		void SetFunction(const std::string& key, Ret(Class::* value)(Param1) const);
		template <typename Class, typename Ret, typename Param1, typename Param2>
		void SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2) const);
		template <typename Class, typename Ret, typename Param1, typename Param2, typename Param3>
		void SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2, Param3) const);
		template <typename Class, typename Ret, typename Param1, typename Param2, typename Param3, typename Param4>
		void SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2, Param3, Param4) const);
		template <typename Class, typename Ret, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
		void SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2, Param3, Param4, Param5) const);
		template <typename Class, typename Ret, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6>
		void SetFunction(const std::string& key, Ret(Class::* value)(Param1, Param2, Param3, Param4, Param5, Param6) const);

		template <typename Class>
		void SetConstructor();
		template <typename Class, typename Param1>
		void SetConstructor();
		template <typename Class, typename Param1, typename Param2>
		void SetConstructor();
		template <typename Class, typename Param1, typename Param2, typename Param3>
		void SetConstructor();
		template <typename Class, typename Param1, typename Param2, typename Param3, typename Param4>
		void SetConstructor();
		template <typename Class, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
		void SetConstructor();
		template <typename Class, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6>
		void SetConstructor();

		template <typename Enum>
		void SetEnumValue(const std::string& enumName, int value);
		
		/// <summary>
		/// Set static member function. Lua can invoke the function by ClassName.FunctionName()
		/// </summary>
		/// <param name="key">Name of the function</param>
		/// <param name="func">Function pointer to static member function</param>
		template <typename Class, typename T>
		void SetFunction(const std::string& key, T func);

		template <typename Ret, typename... Args>
		Ret CallFunction(const std::string& name, Args&&... args);
		template <typename... Args>
		void CallFunctionNoReturn(const std::string& name, Args&&... args);

		void LoadFile(const std::string& fileName);
		void LoadString(const std::string& str);

		/// <summary>
		/// Debug function to check Lua run time userdata type. Expensive so turn it off in release mode
		/// </summary>
		template <typename T>
		static inline bool CheckArgType(lua_State* L, int index);

		template <typename T>
		void PushToLua(T value);
		template <typename T>
		void PushRefToLua(T* value);

		template <typename T>
		T GetFromLua(size_t index);

	private:
		struct LuaTable
		{
			LuaTable(int index, const std::string& name) : mIndex(index), mName(name) {}
			int mIndex;
			std::string mName;
		};

		lua_State* mLuaState = nullptr;
		Stack<LuaTable> mTableStack;

		void _RegisterPrimitiveType();
		void _ProcessError(int error);
		void _SetLuaValue(std::function<void()> func, const std::string& key);

		/// <summary>
		/// Register some functions and properties for native types
		/// </summary>
		template <typename T>
		static inline void _AdditionalRegister(LuaBind& bind) { bind; };

		/// <summary>
		/// Register std::vector of given type to Lua
		/// </summary>
		template <typename T>
		static inline void _RegisterVector(LuaBind& bind);

		/// <summary>
		/// Helper functions for set property
		/// </summary>
		template <typename T>
		static inline LuaWrapper<T>* _SetProperty(lua_State* L, T* value, const std::string& key = "", int index = 0);
		template <typename Class, typename T>
		inline void _SetPropertyFunction(const std::string& key, T Class::* address, const char* tableName, int(*func)(lua_State*));
		template <typename Class, typename T>
		static inline int _PropertyGetter(lua_State* L);
		template <typename Class, typename T, typename Flag>
		static inline int _PropertyGetter(lua_State* L);
		template <typename Class, typename T>
		static inline int _PropertySetter(lua_State* L);
		template <typename Class>
		static inline int _ConstPropertySetter(lua_State* L);
		inline void _SetStaticPropertyFunction(const std::string& key, void* address, const char* tableName, int(*func)(lua_State*));
		template <typename T>
		static inline int _StaticPropertyGetter(lua_State* L);
		template <typename T, typename Flag>
		static inline int _StaticPropertyGetter(lua_State* L);
		template <typename T>
		static inline int _StaticPropertySetter(lua_State* L);

		/// <summary>
		/// Helper wrapper that calls a user-provided function
		/// </summary>
		template <typename Ret>
		static inline int _CallCFunction(lua_State* L, const std::function<Ret(lua_State*)>& wrap);

		/// <summary>
		/// Get an argument from Lua stack, used to retreive arguments for function call from Lua
		/// </summary>
		template <typename T>
		static inline T _FromLuaStack(lua_State* L, int index) { static_assert(false, "Unsupported type, please register your type"); };

		/// <summary>
		/// Push argument to Lua stack, used to report return value for function call from Lua
		/// </summary>
		template <typename T>
		static inline void _ToLuaStack(lua_State* L, T value) { static_assert(false, "Unsupported type, please register your type"); };
		template <typename T>
		static inline void _ToLuaStack(lua_State* L, T* address, int flag) { static_assert(false, "Unsupported type, please register your type"); };

		template <typename T, typename... Args>
		inline void _PushFunctionArgument(int& count, T&& t, Args&&... args);
		inline void _PushFunctionArgument(int& count);
	};

	template <typename T>
	class VectorWrapper final
	{
	public:
		static int Size(lua_State* L);
		static int Get(lua_State* L);
		static int Set(lua_State* L);
		static int Clear(lua_State* L);
		static int PushBack(lua_State* L);
		static int Insert(lua_State* L);
		static int RemoveAt(lua_State* L);
	};
}


#include "LuaBind.inl"
#include "NativeType.inl"