#pragma once
#include "lua.hpp"
#include "Stack.h"
#include "vector.h"
#include "HashMap.h"
#include "LuaWrapper.h"

namespace GameEngine::Lua
{
	class LuaBind final
	{
	public:
		LuaBind();
		~LuaBind();
		LuaBind(const LuaBind& other) = delete;
		LuaBind(LuaBind&& other) = delete;
		LuaBind& operator=(const LuaBind& other) = delete;
		LuaBind& operator==(LuaBind&& other) = delete;
		lua_State* LuaState() const;

		void ClearStack();
		void CreateTable(const std::string& name);
		void OpenTable(const std::string& path);
		void CloseTable();
		std::string CurrentTableName() const;
		int CurrentTableIndex() const;

		template <typename T>
		void SetLuaValue(const std::string& key, const T& value) { static_assert(false, "Unsupported value type, only int32, int64, float, double, std::string are supported"); };

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

		void LoadFile(const std::string& fileName);
		void LoadString(const std::string& str);

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

		template <typename Ret>
		static inline int _CallCFunction(lua_State* L, const std::function<Ret(lua_State*)>& wrap);

		template <typename T>
		static inline T _FromLuaStack(lua_State* L, int index) { static_assert(false, "Unsupported type, please register your type"); };

		template <typename... Args>
		static void _ToLuaStack(lua_State* L, Args&&... args);
		template <typename T, typename... Args>
		static void _ToLuaStackStep(lua_State* L, T value, Args&&... args);
		static void _ToLuaStackStep(lua_State*) {};
		template <typename T>
		static inline void _ToLuaStackTerminal(lua_State* L, T value) { static_assert(false, "Unsupported type, please register your type"); };
	};
}

#define LUA_DEFINE_POINTER_TYPE(_type)														   \
template<> static inline _type* GameEngine::Lua::LuaBind::_FromLuaStack(lua_State* L, int index)                   \
{																					   \
	if (lua_isuserdata(L, index))														   \
	{																				   \
		void* pointer = lua_touserdata(L, index);										   \
		if (pointer == nullptr)														   \
		{																			   \
			throw std::exception("Pointer is nullptr");								   \
		}																			   \
		return static_cast<LuaWrapper<_type>*>(pointer)->mObject;										   \
	}																				   \
	throw std::exception("Unknown pointer type");									   \
}																					   \
template<> static inline const _type* GameEngine::Lua::LuaBind::_FromLuaStack(lua_State* L, int index)			   \
{																					   \
	if (lua_isuserdata(L, index))														   \
	{																				   \
		void* pointer = lua_touserdata(L, index);										   \
		if (pointer == nullptr)														   \
		{																			   \
			throw std::exception("Pointer is nullptr");								   \
		}																			   \
		return static_cast<const LuaWrapper<_type>*>(pointer)->mObject;									   \
	}																				   \
	throw std::exception("Unknown pointer type");									   \
}																					   \
template<> static inline _type& GameEngine::Lua::LuaBind::_FromLuaStack(lua_State* L, int index)				   \
{																					   \
	if (lua_isuserdata(L, index))														   \
	{																				   \
		void* pointer = lua_touserdata(L, index);										   \
		if (pointer == nullptr)														   \
		{																			   \
			throw std::exception("Pointer is nullptr");								   \
		}																			   \
		return *static_cast<LuaWrapper<_type>*>(pointer)->mObject;										   \
	}																				   \
	throw std::exception("Unknown pointer type");									   \
}																					   \
template<> static inline const _type& GameEngine::Lua::LuaBind::_FromLuaStack(lua_State* L, int index)			   \
{																					   \
	if (lua_isuserdata(L, index))														   \
	{																				   \
		void* pointer = lua_touserdata(L, index);										   \
		if (pointer == nullptr)														   \
		{																			   \
			throw std::exception("Pointer is nullptr");								   \
		}																			   \
		return *static_cast<const LuaWrapper<_type>*>(pointer)->mObject;									   \
	}																				   \
	throw std::exception("Unknown pointer type");									   \
}																									   \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStackTerminal(lua_State* L, _type* value)                 \
{																									   \
	LuaWrapper<_type>::Create(L, value);															   \
}																									   \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStackTerminal(lua_State* L, const _type* value)		   \
{																									   \
	LuaWrapper<_type>::Create(L, const_cast<_type*>(value));										   \
}																									   \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStackTerminal(lua_State* L, _type& value)				   \
{																									   \
	LuaWrapper<_type>::Create(L, &value);															   \
}																									   \
template<> static inline void GameEngine::Lua::LuaBind::_ToLuaStackTerminal(lua_State* L, const _type& value)		   \
{																									   \
	LuaWrapper<_type>::Create(L, &const_cast<_type&>(value));										   \
}

#include "LuaBind.inl"