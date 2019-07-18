#include "pch.h"
#include "UIManager.h"
#include "LuaBind.h"

using namespace GameEngine;

#pragma region UIManager

std::vector<UIManager::RenderBlock> UIManager::sRenderBlocks;

void UIManager::AddRenderBlock(RenderBlock block)
{
	for (const auto& b : sRenderBlocks)
	{
		if (block.Name == b.Name)
		{
			return;
		}
	}
	sRenderBlocks.emplace_back(block);
}

void UIManager::RemoveRenderBlock(const std::string& name)
{
	for (auto it = sRenderBlocks.begin(); it != sRenderBlocks.end(); ++it)
	{
		if (it->Name == name)
		{
			sRenderBlocks.erase(it);
			return;
		}
	}
}

const std::vector<UIManager::RenderBlock>& UIManager::GetAllBlocks()
{
	return sRenderBlocks;
}

void UIManager::ClearAllBlocks()
{
	sRenderBlocks.clear();
}

bool UIManager::GetWindowActive(const std::string& name)
{
	for (const auto& block : sRenderBlocks)
	{
		if (block.Name == name)
		{
			return block.Active;
		}
	}
	return false;
}

void UIManager::SetWindowActive(const std::string& name, bool active)
{
	for (auto& block : sRenderBlocks)
	{
		if (block.Name == name)
		{
			block.Active = active;
		}
	}
}

UIManager::RenderBlock::RenderBlock(const std::string& name, bool active, std::function<void()> func) :
	Name(name),
	Active(active),
	RenderFunc(func)
{}
#pragma endregion

#pragma region LuaAdapter
std::vector<UILuaAdapter::WidgetFunction> UILuaAdapter::sWidgetFunctions =
{
	&UILuaAdapter::TextState,
	&UILuaAdapter::ButtonState
};
std::map<std::string, UILuaAdapter::WidgetType> UILuaAdapter::sWidgetTypeMap =
{
	{ "Text", WidgetType::Text },
	{ "Button", WidgetType::Button }
};

void UILuaAdapter::Init(lua_State* L)
{
	// Expose functions to Lua
	lua_newtable(L);
	lua_pushvalue(L, -1);
	lua_setglobal(L, "G_UI");
	assert(lua_istable(L, -1));
	lua_pushcfunction(L, &UILuaAdapter::AddWindow);
	lua_setfield(L, -2, "AddWindow");
	lua_pushcfunction(L, &UILuaAdapter::RemoveWindow);
	lua_setfield(L, -2, "RemoveWindow");
	lua_pushcfunction(L, &UILuaAdapter::SetWindowActive);
	lua_setfield(L, -2, "SetWindowActive");
	lua_pop(L, 1);
}

int UILuaAdapter::AddWindow(lua_State* L)
{
	if (!lua_istable(L, 1))
	{
		lua_error(L);
	}

	// Get basic attributes
	lua_getfield(L, 1, "Name");
	std::string name = luaL_checkstring(L, -1);
	lua_getfield(L, 1, "Active");
	bool active = lua_toboolean(L, -1);
	lua_pop(L, 2);

	// Process each widget
	lua_getfield(L, 1, "Layout");
	if (!lua_istable(L, 2))
	{
		lua_error(L);
	}
	size_t length = lua_rawlen(L, 2);
	std::vector<std::function<void()>> widgets;
	for (size_t i = 1; i <= length; ++i)
	{
		lua_rawgeti(L, 2, i);
		if (!lua_istable(L, 3))
		{
			lua_error(L);
		}
		lua_rawgeti(L, 3, 1);
		std::string type = luaL_checkstring(L, -1);
		auto it = sWidgetTypeMap.find(type);
		if (it == sWidgetTypeMap.end())
		{
			lua_error(L);
		}
		auto func = sWidgetFunctions[static_cast<int>(it->second)];
		widgets.emplace_back(func(L, 3));
		lua_settop(L, 2);
	}

	// Set window with ImGui interface
	auto renderFunc = [widgets]()
	{
		for (auto& func : widgets)
		{
			func();
		}
	};
	UIManager::AddRenderBlock(UIManager::RenderBlock(name, active, renderFunc));

	return 0;
}

int UILuaAdapter::RemoveWindow(lua_State* L)
{
	std::string name = luaL_checkstring(L, 1);
	UIManager::RemoveRenderBlock(name);
	return 0;
}

int UILuaAdapter::SetWindowActive(lua_State* L)
{
	std::string name = luaL_checkstring(L, 1);
	bool active = lua_toboolean(L, 2);
	UIManager::SetWindowActive(name, active);
	return 0;
}

std::function<void()> UILuaAdapter::TextState(lua_State* L, int index)
{
	lua_rawgeti(L, index, 2);  // second config is label
	if (lua_isstring(L, -1))
	{
		const char* str = lua_tostring(L, -1);
		return [str]() { ImGui::Text(str); };
	}
	else if (lua_isfunction(L, -1))
	{
		int uniqueIndex = luaL_ref(L, LUA_REGISTRYINDEX);
		return [uniqueIndex, L]() {
			lua_rawgeti(L, LUA_REGISTRYINDEX, uniqueIndex);
			assert(lua_isfunction(L, -1));
			lua_call(L, 0, 1);
			ImGui::Text(luaL_checkstring(L, -1));
			lua_pop(L, 1);
		};
	}
	else
	{
		lua_error(L);
	}
	return []() {};
}

std::function<void()> UILuaAdapter::ButtonState(lua_State* L, int index)
{
	// Label function
	lua_rawgeti(L, index, 2);
	std::function<std::string()> label;
	std::string labelString;
	if (lua_isstring(L, -1))
	{
		labelString = lua_tostring(L, -1);
	}
	else if (lua_isfunction(L, -1))
	{
		int uniqueIndex = luaL_ref(L, LUA_REGISTRYINDEX);
		label = [uniqueIndex, L]()
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, uniqueIndex);
			assert(lua_isfunction(L, -1));
			lua_call(L, 0, 1);
			std::string str = luaL_checkstring(L, -1);
			lua_pop(L, 1);
			return str;
		};
	}
	else
	{
		lua_error(L);
	}

	// Click callback function
	lua_rawgeti(L, index, 3);
	std::function<void()> callback;
	if (lua_isfunction(L, -1))
	{
		int uniqueIndex = luaL_ref(L, LUA_REGISTRYINDEX);
		callback = [uniqueIndex, L]()
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, uniqueIndex);
			assert(lua_isfunction(L, -1));
			lua_call(L, 0, 0);
		};
	}
	else
	{
		callback = []() {};
	}

	if (!labelString.empty())
	{
		return [labelString, callback]()
		{
			if (ImGui::Button(labelString.c_str()))
			{
				callback();
			}
		};
	}
	else
	{
		return [label, callback]()
		{
			if (ImGui::Button(label().c_str()))
			{
				callback();
			}
		};
	}
}
#pragma endregion
