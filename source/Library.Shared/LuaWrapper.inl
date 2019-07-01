#pragma once

namespace GameEngine::Lua
{
	int MetaFunction::Index(lua_State* L)
	{
		int hasMetatable = lua_getmetatable(L, 1);  // Userdata pointer is at 1, get its metatable and push to 3
		(hasMetatable);
		assert(hasMetatable);

		assert(lua_isuserdata(L, 1) || lua_istable(L, 1));
		bool staticMember = lua_istable(L, 1);

		int result = 0;
		while (result == 0)                         // Loop while can't find the required key
		{
			lua_pushvalue(L, 2);                        // Key is at 2, copy it and push to 4
			lua_rawget(L, 3);                           // Metatable for userdata is at 3, look for the key in metatable
			if (lua_isnil(L, -1))                       // Can't find the key in metatable, which means it's not a function, might be a property
			{
				lua_pushstring(L, staticMember ? STATIC_MEMBER_GETTER : MEMBER_GETTER);
				lua_rawget(L, 3);                       // Get __propget table from metatable
				lua_pushvalue(L, 2);                    // Copy key to stack top
				lua_rawget(L, -2);                      // Look up the key in __propget table
				if (!lua_isnil(L, -1))                  // Find the key in __propget table
				{
					assert(lua_iscfunction(L, -1));          // Must be a LuaWrapper getter function
					if (staticMember)
					{
						lua_call(L, 0, 1);
					}
					else
					{
						lua_pushvalue(L, 1);                     // Copy the user object to stack top, it serves as getter function argument
						lua_call(L, 1, 1);                       // Call lua function with 1 argument and 1 return value
					}
					result = 1;
				}
				else if (!staticMember)                 // Can't find key, go to parent table
				{
					lua_pop(L, 3);                      // Pop nil, __propget table, and nil, now metatable is at top
					lua_pushstring(L, PARENT);
					int type = lua_rawget(L, 3);        // Get parent table, it sits at 4
					if (type != LUA_TTABLE)             // No parent metatable, end of search
					{
						break;
					}
					lua_remove(L, 3);                   // Remove current metatable, so now parent metatable becomes new metatable for next iteration
					assert(lua_gettop(L) == 3 && lua_istable(L, 3));
				}
				else                                    // For static member search, don't go to parent table
				{
					break;
				}
			}
			else
			{
				result = 1;
			}
		}

		return result;
	}

	int MetaFunction::NewIndex(lua_State* L)
	{
		int hasMetatable = lua_getmetatable(L, 1);  // Userdata pointer is at 1, get its metatable and push to 4
		(hasMetatable);
		assert(hasMetatable);

		assert(lua_isuserdata(L, 1) || lua_istable(L, 1));
		bool staticMember = lua_istable(L, 1);

		bool fromC = true;
		while (true)
		{
			lua_pushvalue(L, 2);                        // Key is at 2, value is at 3, copy key and push to 5
			lua_rawget(L, 4);                           // Metatable for userdata is at 4, look for the key in metatable
			if (lua_isnil(L, -1))
			{
				lua_pushstring(L, staticMember ? STATIC_MEMBER_SETTER : MEMBER_SETTER);
				lua_rawget(L, 4);                       // Get __propset table from metatable
				lua_pushvalue(L, 2);                    // Copy key to stack top
				lua_rawget(L, -2);                      // Look up the key in __propset table
				if (!lua_isnil(L, -1))
				{
					assert(lua_iscfunction(L, -1));          // Must be a LuaWrapper setter function
					if (staticMember)
					{
						lua_pushvalue(L, 3);                     // Copy value from argument place to top of stack, as the argument of static member setter function
						lua_call(L, 1, 0);                       // Call lua function, this will push the result to stack
					}
					else
					{
						lua_pushvalue(L, 1);                     // Copy the user object to stack top, it serves as first setter function argument
						lua_pushvalue(L, 3);                     // Copy new value to stack top, it serves as second setter function argument
						lua_call(L, 2, 0);                       // Call lua function with 2 arguments and no return value
					}
					break;
				}
				else if (!staticMember)                                   // Can't find in current table, go to parent table
				{
					lua_pop(L, 3);                      // Pop nil, __propset table, and nil, now metatable is at top
					lua_pushstring(L, PARENT);
					int type = lua_rawget(L, 4);         // Get parent table, it now sits at 5
					if (type != LUA_TTABLE)              // No parent metatable, can't find target member, error
					{
						fromC = false;
						break;
					}
					lua_remove(L, 4);                   // Remove current metatable, so now parent metatable becomes new metatable for next iteration
					assert(lua_gettop(L) == 4 && lua_istable(L, 4));
				}
				else
				{
					fromC = false;
					break;
				}
			}
			else if (lua_iscfunction(L, -1))
			{
				lua_pushstring(L, NAME_METHOD);
				lua_rawget(L, 4);
				lua_call(L, 0, 1);
				lua_pushfstring(L, "Attemp to modify function \"%s\" in class \"%s\"", lua_tostring(L, 2), lua_tostring(L, -1));
				lua_error(L);
			}
			else
			{
				fromC = false;
				break;
			}
		}

		if (!fromC)
		{
			lua_pushvalue(L, 2);
			lua_pushvalue(L, 3);
			lua_rawset(L, 4);
		}

		return 0;
	}

	template <typename T>
	LuaWrapper<T>::LuaWrapper(bool luaObject, T* ptr) :
		mLuaObject(luaObject),
		mObject(ptr)
	{
		// Add pointer counter
		if (mLuaObject)
		{
			Registry::IncrementObjectCounter(ptr);
		}
	};

	template <typename T>
	LuaWrapper<T>::~LuaWrapper()
	{
		assert(mObject != nullptr);
		if (mLuaObject)
		{
			Registry::DecrementObjectCounter(mObject);
			if (!Registry::IsValid(mObject))
			{
				delete mObject;
			}
		}
	}

	template <typename T>
	void LuaWrapper<T>::Register(lua_State* L)
	{
		assert(!sName.empty());
		assert(lua_gettop(L) == 0);

		// Create class metatable and set it as itself's metatable
		int newTable = luaL_newmetatable(L, sName.c_str());
		(newTable);
		assert(("Each class mapped to Lua must have unique name", newTable));
		lua_pushvalue(L, -1);
		lua_setmetatable(L, -2);
		assert(lua_gettop(L) == 1);

		// Set __index metafunction
		lua_pushcfunction(L, &MetaFunction::Index);
		lua_setfield(L, -2, "__index");

		// Set __newindex metafunction
		lua_pushcfunction(L, &MetaFunction::NewIndex);
		lua_setfield(L, -2, "__newindex");

		// Set __propget metatable, for member variable get
		lua_pushstring(L, MEMBER_GETTER);
		lua_newtable(L);
		lua_rawset(L, -3);

		// Set __progset metatable, for member variable set
		lua_pushstring(L, MEMBER_SETTER);
		lua_newtable(L);
		lua_rawset(L, -3);

		// Set static member getter table
		lua_pushstring(L, STATIC_MEMBER_GETTER);
		lua_newtable(L);
		lua_rawset(L, -3);

		// Set static member setter table
		lua_pushstring(L, STATIC_MEMBER_SETTER);
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
		lua_pushstring(L, NAME_METHOD);
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

		// Set cast type function
		lua_pushstring(L, "Cast");
		lua_pushcfunction(L, &LuaWrapper<T>::__cast);
		lua_rawset(L, -3);

		// Set default constructor as invalid
		lua_pushstring(L, NEW_METHOD);
		lua_pushcfunction(L, &LuaWrapper<T>::__new);
		lua_rawset(L, -3);

		// Set the name of class as alias of metatable as global table
		lua_setglobal(L, sName.c_str());
		assert(lua_gettop(L) == 0);

		// Add type info to registry
		const std::string& name = LuaWrapper<T>::sName;
		Registry::sTypeRegistry.insert(std::make_pair(name, Registry::TypeNode(name, LuaWrapper<T>::sTypeId)));
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
		lua_pushstring(L, PARENT);
		int newParentTable = luaL_newmetatable(L, LuaWrapper<Parent>::sName.c_str());
		(newParentTable);
		assert(("Must register parent type before registering child type", !newParentTable));
		lua_rawset(L, -3);
		lua_pop(L, 1);

		// Build type link in registry
		Registry::TypeNode& node = Registry::sTypeRegistry.find(LuaWrapper<T>::sName)->second;
		Registry::TypeNode& parentNode = Registry::sTypeRegistry.find(LuaWrapper<Parent>::sName)->second;
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
		auto it = Registry::sTypeRegistry.find(LuaWrapper<T>::sName)->second;
		auto& node = it->second;
		for (const auto* parentNode : node.mUpLink)
		{
			for (const auto* childNode : node.mDownLink)
			{
				if (parentNode->mDownLink.find(childNode) != parentNode->mDownLink.end())
				{
					parentNode->mDownLink.PushBack(childNode);
				}
			}
		}
		for (const auto* childNode : node.mDownLink)
		{
			for (const auto* parentNode : node.mUpLink)
			{
				if (childNode->mUpLink.find(parentNode) != childNode->mUpLink.end())
				{
					childNode->mUpLink.PushBack(parentNode);
				}
			}
		}
		Registry::sTypeRegistry.erase(it);
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

	template <typename T>
	int LuaWrapper<T>::__cast(lua_State* L)
	{
#ifdef RUN_TIME_TYPE_CHECK
		if (!Registry::Is(L, 1, sTypeId))
		{
			luaL_checkudata(L, 1, sName.c_str());  // Gurantee to fail, so will trigger lua runtime error
		}
#endif // RUN_TIME_TYPE_CHECK

		// Cast to this type and return a new wrapper of it
		LuaWrapper* pointer = static_cast<LuaWrapper*>(lua_touserdata(L, 1));
		T* obj = static_cast<T*>(pointer->mObject);
		new(pointer) LuaWrapper(true, obj);
		int newTable = luaL_newmetatable(L, sName.c_str());
		(newTable);
		assert(!newTable);
		lua_setmetatable(L, -2);

		return 1;
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
				auto it = sTypeRegistry.find(name);
				if (it != sTypeRegistry.end())
				{
					if (it->second.IsDescedant(expectType) || it->second.IsAncestor(expectType))
					{
						lua_pop(L, 2);  // Remove string and metatable
						return true;
					}
				}
			}
		}
		else
		{
			switch (type)
			{
			case LUA_TNUMBER:
				if (expectType == LuaWrapper<int>::sTypeId || expectType == LuaWrapper<unsigned int>::sTypeId
					|| expectType == LuaWrapper<float>::sTypeId || expectType == LuaWrapper<double>::sTypeId
					|| expectType == LuaWrapper<long long>::sTypeId || expectType == LuaWrapper<unsigned long long>::sTypeId
					|| expectType == LuaWrapper<char>::sTypeId || expectType == LuaWrapper<unsigned char>::sTypeId)
				{
					return true;
				}
			case LUA_TSTRING:
				if (expectType == LuaWrapper<char>::sTypeId || expectType == LuaWrapper<std::string>::sTypeId)
				{
					return true;
				}
			}
		}

		return false;
	}

	void Registry::IncrementObjectCounter(void* address)
	{
		++sObjectCounter[address];
	}

	void Registry::DecrementObjectCounter(void* address)
	{
		assert(sObjectCounter.find(address) != sObjectCounter.end());
		--sObjectCounter[address];
		if (sObjectCounter[address] == 0)
		{
			sObjectCounter.erase(address);
		}
	}

	bool Registry::IsValid(void* address)
	{
		return sObjectCounter.find(address) != sObjectCounter.end();
	}
#pragma endregion

}