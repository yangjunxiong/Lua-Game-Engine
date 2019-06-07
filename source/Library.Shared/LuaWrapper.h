#pragma once
#include "Macro.h"
#include "vector.h"
#include "HashMap.h"

namespace GameEngine::Lua
{
	class MetaFunction final
	{
	public:
		static inline int Index(lua_State* L);
		static inline int NewIndex(lua_State* L);
		static inline int StaticIndex(lua_State* L);
		static inline int StaticNewIndex(lua_State* L);
	};

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
		template <typename Parent>
		static void Register(lua_State* L);
		static void Unregister(lua_State* L);

		T* const mObject = nullptr;
		const static std::string sName;
		const static uint64_t sTypeId;

	protected:
		const bool mLuaObject = false;
		
		static int __new(lua_State* L);
		static int __gc(lua_State* L);
		static int __eq(lua_State* L);
		static int __name(lua_State* L);
		static int __set(lua_State* L);
		static int __get(lua_State* L);
		static int __tostring(lua_State* L);
	};

	class Registry final
	{
	public:
		struct TypeNode
		{
			using Link = Vector<TypeNode*>;

			inline TypeNode(const std::string& name, uint64_t id);
			inline bool IsAncestor(uint64_t expectType);
			inline bool IsDescedant(uint64_t expectType);

			const std::string mName;
			const uint64_t mTypeId = 0;
			Link mUpLink;
			Link mDownLink;

		private:
			inline bool BFS(std::function<Link*(TypeNode*)> getLinkFunc, uint64_t expectType);
		};

		static inline HashMap<std::string, TypeNode> sTypeRegistry;
		static inline bool Is(lua_State* L, int index, uint64_t expectType);
	};
}

#include "LuaWrapper.inl"
