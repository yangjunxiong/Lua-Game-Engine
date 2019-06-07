#include "pch.h"
#include "LuaBind.h"
#include "LuaWrapper.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

using namespace GameEngine;
using namespace GameEngine::Lua;
using namespace std;

LuaBind::LuaBind() :
	mLuaState(luaL_newstate())
{
	assert(mLuaState != nullptr);
	luaL_openlibs(mLuaState);
	_RegisterPrimitiveType();
}

LuaBind::~LuaBind()
{
	assert(mLuaState != nullptr);
	lua_close(mLuaState);
	Registry::sTypeRegistry.Clear();
}

lua_State* LuaBind::LuaState() const
{
	return mLuaState;
}

void LuaBind::LoadFile(const std::string& fileName)
{
	int error = luaL_dofile(mLuaState, fileName.c_str());
	_ProcessError(error);
}

void LuaBind::LoadString(const std::string& str)
{
	int error = luaL_dostring(mLuaState, str.c_str());
	_ProcessError(error);
}

void LuaBind::ClearStack()
{
	lua_settop(mLuaState, 0);
	mTableStack.Clear();
}

void LuaBind::CreateTable(const std::string& name)
{
	lua_newtable(mLuaState);
	if (mTableStack.IsEmpty())
	{
		lua_setglobal(mLuaState, name.c_str());
		lua_getglobal(mLuaState, name.c_str());  // Retreive the table back
	}
	else
	{
		lua_setfield(mLuaState, mTableStack.Peak().mIndex, name.c_str());
		lua_getfield(mLuaState, mTableStack.Peak().mIndex, name.c_str());  // Retreive the table back
	}
	assert(lua_istable(mLuaState, -1));
	mTableStack.Push(LuaTable(lua_gettop(mLuaState), name));
}

void LuaBind::OpenTable(const std::string& path)
{
	string front;
	string back = path;
	while (!back.empty())
	{
		// Split one delimeter
		size_t index = back.find('.');
		if (index != string::npos)
		{
			front = back.substr(0, index);
			back = back.substr(index + 1, back.size() - index - 1);
		}
		else
		{
			front = back;
			back.clear();
		}

		// Push this table to stack
		if (mTableStack.IsEmpty())
		{
			lua_getglobal(mLuaState, front.c_str());
			assert(lua_gettop(mLuaState) > 0);
		}
		else
		{
			int oldSize = lua_gettop(mLuaState);
			int tableIndex = mTableStack.Peak().mIndex;
			lua_getfield(mLuaState, tableIndex, front.c_str());
			assert(lua_gettop(mLuaState) == oldSize + 1);
		}

		// Record table stack state
		mTableStack.Push(LuaTable(lua_gettop(mLuaState), front));
	}
}

void LuaBind::CloseTable()
{
	assert(lua_gettop(mLuaState) > 0 && lua_istable(mLuaState, -1));
	lua_pop(mLuaState, 1);
	mTableStack.Pop();
}

std::string LuaBind::CurrentTableName() const
{
	if (mTableStack.IsEmpty())
	{
		return "";
	}
	return mTableStack.Peak().mName;
}

int LuaBind::CurrentTableIndex() const
{
	if (mTableStack.IsEmpty())
	{
		return 0;
	}
	return mTableStack.Peak().mIndex;
}

void LuaBind::_ProcessError(int error)
{
	if (error != 0 && !lua_isnil(mLuaState, -1))
	{
		if (lua_isstring(mLuaState, -1))
		{
			const char* msg = lua_tostring(mLuaState, -1);
			std::time_t time = std::time(0);
			std::tm now;
			localtime_s(&now, &time);
			std::fstream stream;
			stream.open("temp.log", ios_base::out | ios_base::app);
			stream << (now.tm_year + 1900) << "-" << (now.tm_mon + 1) << "-" << now.tm_mday << " " << now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec << "    " << msg << endl;
			stream.close();
			assert(("Lua runtime error", false));
		}
	}
}

void LuaBind::_SetLuaValue(std::function<void()> func, const std::string& key)
{
	assert(!key.empty() && key.find_last_of('.') == string::npos);

	// Determine global or nested table
	if (lua_gettop(mLuaState) > 0 && lua_istable(mLuaState, -1))
	{
		lua_pushstring(mLuaState, key.c_str());
		func();
		lua_rawset(mLuaState, -3);
	}
	else
	{
		func();
		lua_setglobal(mLuaState, key.c_str());
	}
}

void LuaBind::_RegisterPrimitiveType()
{
	LuaWrapper<std::string>::Register(mLuaState);
	LuaWrapper<int>::Register(mLuaState);
	LuaWrapper<long long>::Register(mLuaState);
	LuaWrapper<float>::Register(mLuaState);
	LuaWrapper<double>::Register(mLuaState);
	LuaWrapper<bool>::Register(mLuaState);
	LuaWrapper<char>::Register(mLuaState);
}
