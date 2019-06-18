#pragma once

namespace GameEngine::Lua
{
	int MetaFunction::Index(lua_State* L)
	{
		int hasMetatable = lua_getmetatable(L, 1);  // Userdata pointer is at 1, get its metatable and push to 3
		assert(hasMetatable);

		int result = 0;
		while (result == 0)                         // Loop while can't find the required key
		{
			lua_pushvalue(L, 2);                        // Key is at 2, copy it and push to 4
			lua_rawget(L, 3);                           // Metatable for userdata is at 3, look for the key in metatable
			if (lua_isnil(L, -1))                       // Can't find the key in metatable, which means it's not a function, might be a property
			{
				lua_pushstring(L, "__propget");
				lua_rawget(L, 3);                       // Get __propget table from metatable
				lua_pushvalue(L, 2);                    // Copy key to stack top
				lua_rawget(L, -2);                      // Look up the key in __propget table
				if (!lua_isnil(L, -1))                  // Find the key in __propget table
				{
					assert(lua_iscfunction(L, -1));          // Must be a LuaWrapper getter function
					lua_pushvalue(L, 1);                     // Copy the user object to stack top, it serves as getter function argument
					lua_call(L, 1, 1);                       // Call lua function with 1 argument and 1 return value
					result = 1;
				}
				else                                    // Can't find key, go to parent table
				{
					lua_pop(L, 3);                      // Pop nil, __propget table, and nil, now metatable is at top
					lua_pushstring(L, "__parent");
					int type = lua_rawget(L, 3);        // Get parent table, it sits at 4
					if (type != LUA_TTABLE)             // No parent metatable, end of search
					{
						break;
					}
					lua_remove(L, 3);                   // Remove current metatable, so now parent metatable becomes new metatable for next iteration
					assert(lua_gettop(L) == 3 && lua_istable(L, 3));
				}
			}
			else if (lua_iscfunction(L, -1))
			{
				result = 1;
			}
			else
			{
				assert(("__index metatable should only contain function", false));
			}
		}

		return result;
	}

	int MetaFunction::NewIndex(lua_State* L)
	{
		int hasMetatable = lua_getmetatable(L, 1);  // Userdata pointer is at 1, get its metatable and push to 4
		assert(hasMetatable);

		while (true)
		{
			lua_pushvalue(L, 2);                        // Key is at 2, value is at 3, copy key and push to 5
			lua_rawget(L, 4);                           // Metatable for userdata is at 4, look for the key in metatable
			if (lua_isnil(L, -1))
			{
				lua_pushstring(L, "__propset");
				lua_rawget(L, 4);                       // Get __propset table from metatable
				lua_pushvalue(L, 2);                    // Copy key to stack top
				lua_rawget(L, -2);                      // Look up the key in __propset table
				if (!lua_isnil(L, -1))
				{
					assert(lua_iscfunction(L, -1));          // Must be a LuaWrapper setter function
					lua_pushvalue(L, 1);                     // Copy the user object to stack top, it serves as first setter function argument
					lua_pushvalue(L, 3);                     // Copy new value to stack top, it serves as second setter function argument
					lua_call(L, 2, 0);                       // Call lua function with 2 arguments and no return value
					break;
				}
				else                                    // Can't find in current table, go to parent table
				{
					lua_pop(L, 3);                      // Pop nil, __propset table, and nil, now metatable is at top
					lua_pushstring(L, "__parent");
					int type = lua_rawget(L, 4);         // Get parent table, it now sits at 5
					if (type != LUA_TTABLE)              // No parent metatable, can't find target member, error
					{
						lua_pushstring(L, "Name");
						lua_rawget(L, 4);
						lua_call(L, 0, 1);
						lua_pushfstring(L, "Attemp to modify non-existant variable \"%s\" in class \"%s\"", lua_tostring(L, 2), lua_tostring(L, -1));
						lua_error(L);
					}
					lua_remove(L, 4);                   // Remove current metatable, so now parent metatable becomes new metatable for next iteration
					assert(lua_gettop(L) == 4 && lua_istable(L, 4));
				}
			}
			else
			{
				lua_pushstring(L, "Name");
				lua_rawget(L, 4);
				lua_call(L, 0, 1);
				lua_pushfstring(L, "Attemp to modify function \"%s\" in class \"%s\"", lua_tostring(L, 2), lua_tostring(L, -1));
				lua_error(L);
			}
		}

		return 0;
	}

	int MetaFunction::StaticIndex(lua_State* L)
	{
		lua_getmetatable(L, 1);
		assert(lua_rawequal(L, 1, 3));  // Static table is its metatable

		int result = 0;
		lua_pushstring(L, "__propget");
		lua_rawget(L, 3);                       // Get __propget table from metatable
		lua_pushvalue(L, 2);                    // Copy key to stack top
		lua_rawget(L, -2);                      // Look up the key in __propget table
		if (!lua_isnil(L, -1))
		{
			assert(lua_iscfunction(L, -1));          // Must be a Lua static member variable getter function
			lua_call(L, 0, 1);                       // Call lua function, this will push the result to stack
			result = 1;
		}

		return result;
	}

	int MetaFunction::StaticNewIndex(lua_State* L)
	{
		lua_getmetatable(L, 1);
		assert(lua_rawequal(L, 1, 4));  // Static table is its metatable

		lua_pushstring(L, "__propset");
		lua_rawget(L, 4);                       // Get __propset table from metatable
		lua_pushvalue(L, 2);                    // Copy key to stack top
		lua_rawget(L, -2);                      // Look up the key in __propset table
		if (!lua_isnil(L, -1))
		{
			assert(lua_iscfunction(L, -1));          // Must be a Lua static member variable getter function
			lua_pushvalue(L, 3);                     // Copy value from argument place to top of stack, as the argument of static member setter function
			lua_call(L, 1, 0);                       // Call lua function, this will push the result to stack
		}

		return 0;
	}

	template <typename T>
	LuaWrapper<T>::LuaWrapper(bool luaObject, T* ptr) :
		mLuaObject(luaObject),
		mObject(ptr)
	{};

	template <typename T>
	LuaWrapper<T>::~LuaWrapper()
	{
		assert(mObject != nullptr);
		if (mLuaObject)
		{
			delete mObject;
		}
	}

	template <typename T>
	void LuaWrapper<T>::Register(lua_State* L)
	{
		assert(!sName.empty());

		// Create member metatable
		int newTable = luaL_newmetatable(L, sName.c_str());
		assert(("Each class mapped to Lua must have unique name", newTable));

		// Set __index metafunction
		lua_pushcfunction(L, &MetaFunction::Index);
		lua_setfield(L, -2, "__index");

		// Set __newindex metafunction
		lua_pushcfunction(L, &MetaFunction::NewIndex);
		lua_setfield(L, -2, "__newindex");

		// Set __propget metatable, for member variable get
		lua_pushstring(L, "__propget");
		lua_newtable(L);
		lua_rawset(L, -3);

		// Set __progset metatable, for member variable set
		lua_pushstring(L, "__propset");
		lua_newtable(L);
		lua_rawset(L, -3);

		// Set __gc metafunction
		lua_pushstring(L, "__gc");
		lua_pushcfunction(L, &LuaWrapper<T>::__gc);
		lua_rawset(L, -3);

		// Set __eq metafunction
		lua_pushstring(L, "__eq");
		lua_pushcfunction(L, &LuaWrapper<T>::__eq);
		lua_rawset(L, -3);

		// Set __tostring metafunction
		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, &LuaWrapper<T>::__tostring);
		lua_rawset(L, -3);

		// Set name function
		lua_pushstring(L, "TypeName");
		lua_pushcfunction(L, &LuaWrapper<T>::__name);
		lua_rawset(L, -3);

		// Set set value function
		lua_pushstring(L, "Set");
		lua_pushcfunction(L, &LuaWrapper<T>::__set);
		lua_rawset(L, -3);

		// Set get value function
		lua_pushstring(L, "Get");
		lua_pushcfunction(L, &LuaWrapper<T>::__get);
		lua_rawset(L, -3);

		// Pop metatable
		lua_pop(L, 1);

		// Create static member metatable
		lua_newtable(L);
		lua_pushvalue(L, -1);
		lua_setmetatable(L, -2);

		// Set static member variable index function
		lua_pushcfunction(L, &MetaFunction::StaticIndex);
		lua_setfield(L, -2, "__index");

		// Set static member variable newindex function
		lua_pushcfunction(L, &MetaFunction::StaticNewIndex);
		lua_setfield(L, -2, "__newindex");

		// Set static member variable get function
		lua_pushstring(L, "__propget");
		lua_newtable(L);
		lua_rawset(L, -3);

		// Set static member variable set function
		lua_pushstring(L, "__propset");
		lua_newtable(L);
		lua_rawset(L, -3);

		// Set default constructor as invalid
		lua_pushstring(L, "New");
		lua_pushcfunction(L, &LuaWrapper<T>::__new);
		lua_rawset(L, -3);

		// Set global static table
		lua_setglobal(L, sName.c_str());

		// Add type info to registry
		const std::string& name = LuaWrapper<T>::sName;
		Registry::sTypeRegistry.Insert(std::make_pair(name, Registry::TypeNode(name, LuaWrapper<T>::sTypeId)));
	}

	template <typename T>
	template <typename Parent>
	static void LuaWrapper<T>::Register(lua_State* L)
	{
		// Register child class
		LuaWrapper<T>::Register(L);

		// Set parent metatable
		assert(("Hey you can't parent yourself", sName != LuaWrapper<Parent>::sName));
		luaL_newmetatable(L, sName.c_str());
		lua_pushstring(L, "__parent");
		int newParentTable = luaL_newmetatable(L, LuaWrapper<Parent>::sName.c_str());
		assert(("Must register parent type before registering child type", !newParentTable));
		lua_rawset(L, -3);

		// Build type link in registry
		Registry::TypeNode& node = Registry::sTypeRegistry.Find(LuaWrapper<T>::sName)->second;
		Registry::TypeNode& parentNode = Registry::sTypeRegistry.Find(LuaWrapper<Parent>::sName)->second;
		node.mUpLink.PushBack(&parentNode);
		parentNode.mDownLink.PushBack(&node);
	}

	template <typename T>
	void LuaWrapper<T>::Unregister(lua_State* L)
	{
		// Remove metatable
		if (L != nullptr)
		{
			lua_pushnil(L);
			lua_setfield(L, LUA_REGISTRYINDEX, sName.c_str());
		}

		// Remove static table
		if (L != nullptr)
		{
			lua_pushnil(L);
			lua_setglobal(L, LuaWrapper<T>::sName.c_str());
		}

		// Fix type link in registry
		auto it = Registry::sTypeRegistry.Find(LuaWrapper<T>::sName)->second;
		auto& node = it->second;
		for (const auto* parentNode : node.mUpLink)
		{
			for (const auto* childNode : node.mDownLink)
			{
				if (parentNode->mDownLink.Find(childNode) != parentNode->mDownLink.end())
				{
					parentNode->mDownLink.PushBack(childNode);
				}
			}
		}
		for (const auto* childNode : node.mDownLink)
		{
			for (const auto* parentNode : node.mUpLink)
			{
				if (childNode->mUpLink.Find(parentNode) != childNode->mUpLink.end())
				{
					childNode->mUpLink.PushBack(parentNode);
				}
			}
		}
		Registry::sTypeRegistry.Remove(it);
	}

	template <typename T>
	int LuaWrapper<T>::__new(lua_State* L)
	{
		luaL_error(L, "Class \"%s\" has no registered constructor", LuaWrapper<T>::sName.c_str());
		return 0;
	}

	template <typename T>
	int LuaWrapper<T>::__gc(lua_State* L)
	{
		auto* wrap = static_cast<LuaWrapper<T>*>(luaL_checkudata(L, 1, sName.c_str()));
		wrap->~LuaWrapper();
		return 0;
	}

	template <typename T>
	int LuaWrapper<T>::__eq(lua_State* L)
	{
		if (!Registry::Is(L, 1, LuaWrapper<T>::sTypeId) || !Registry::Is(L, 2, LuaWrapper<T>::sTypeId))
		{
			return false;
		}

		auto* wrap1 = static_cast<LuaWrapper<T>*>(lua_touserdata(L, 1));
		auto* wrap2 = static_cast<LuaWrapper<T>*>(lua_touserdata(L, 2));
		lua_pushboolean(L, wrap1->mObject == wrap2->mObject);
		return 1;
	}

	template <typename T>
	int LuaWrapper<T>::__name(lua_State* L)
	{
		lua_pushstring(L, sName.c_str());
		return 1;
	}

	template <typename T>
	int LuaWrapper<T>::__set(lua_State* L)
	{
		lua_error(L);
		return 0;
	}

	template <typename T>
	int LuaWrapper<T>::__get(lua_State* L)
	{
		lua_error(L);
		return 0;
	}

	template <typename T>
	int LuaWrapper<T>::__tostring(lua_State* L)
	{
		lua_error(L);
		return 0;
	}

#pragma region Registry
	Registry::TypeNode::TypeNode(const std::string& name, uint64_t id) :
		mName(name),
		mTypeId(id)
	{}

	bool Registry::TypeNode::IsAncestor(uint64_t expectType)
	{
		return BFS([](TypeNode* node) { return &node->mDownLink; }, expectType);
	}

	bool Registry::TypeNode::IsDescedant(uint64_t expectType)
	{
		return BFS([](TypeNode* node) { return &node->mUpLink; }, expectType);
	}

	bool Registry::TypeNode::BFS(std::function<Link*(TypeNode*)> getLinkFunc, uint64_t expectType)
	{
		SList<TypeNode*> queue = { this };
		while (!queue.IsEmpty())
		{
			// Check the first item in queue
			TypeNode* node = queue.Front();
			queue.PopFront();
			if (node->mTypeId == expectType)
			{
				return true;
			}

			// Push this node's next list to queue
			Link& nextLink = *getLinkFunc(node);
			for (TypeNode* childNode : nextLink)
			{
				queue.PushBack(childNode);
			}
		}

		return false;
	}

	bool Registry::Is(lua_State* L, int index, uint64_t expectType)
	{
		int type = lua_type(L, index);
		if (type == LUA_TUSERDATA)
		{
			lua_getmetatable(L, index);
			lua_pushstring(L, "__name");
			type = lua_rawget(L, -2);
			if (type == LUA_TSTRING)
			{
				std::string name = lua_tostring(L, -1);
				auto it = sTypeRegistry.Find(name);
				if (it != sTypeRegistry.end())
				{
					if (it->second.IsDescedant(expectType))
					{
						lua_pop(L, 2);  // Remove string and metatable
						return true;
					}
				}
			}
		}

		return false;
	}
#pragma endregion

}