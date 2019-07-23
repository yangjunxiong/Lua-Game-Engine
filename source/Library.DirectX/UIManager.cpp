#include "pch.h"
#include "UIManager.h"
#include "LuaBind.h"
#include <imgui.h>

using namespace GameEngine;

#define CALL_FUNCTION(_required, _funcName, _paramCount, _params, _paramsPush, ...)                          \
if (lua_isfunction(L, -1))														   \
{																				   \
	int uniqueIndex = luaL_ref(L, LUA_REGISTRYINDEX);							   \
	_funcName = [uniqueIndex, L](_params)										   \
	{																			   \
		lua_rawgeti(L, LUA_REGISTRYINDEX, uniqueIndex);							   \
		assert(lua_isfunction(L, -1));											   \
		_paramsPush;  \
		lua_pcall(L, _paramCount, 1, 0);                                                     \
		__VA_ARGS__;															   \
	};																			   \
}																				   \
else if (lua_istable(L, -1))													   \
{																				   \
	size_t length = lua_rawlen(L, -1);											   \
	size_t paramLength = length - 1;											   \
																				   \
	lua_rawgeti(L, -1, 1);														   \
	int functionIndex = luaL_ref(L, LUA_REGISTRYINDEX);							   \
																				   \
	std::vector<int> paramIndexes;												   \
	for (size_t i = 0; i < paramLength; ++i)									   \
	{																			   \
		lua_rawgeti(L, -1, i + 2);												   \
		paramIndexes.emplace_back(luaL_ref(L, LUA_REGISTRYINDEX));				   \
	}																			   \
																				   \
	_funcName = [functionIndex, paramIndexes, L](_params)						   \
	{																			   \
		lua_rawgeti(L, LUA_REGISTRYINDEX, functionIndex);						   \
		assert(lua_isfunction(L, -1));											   \
		for (int index : paramIndexes)											   \
		{																		   \
			lua_rawgeti(L, LUA_REGISTRYINDEX, index);							   \
		}																		   \
		_paramsPush;  \
		lua_pcall(L, static_cast<int>(paramIndexes.size()) + _paramCount, 1, 0);				   \
		__VA_ARGS__;															   \
	};																			   \
}																				   \
else if (_required)                                                                \
{																				   \
	lua_error(L);																   \
}

#define CALL_FUNCTION_BRIEF(_required, _funcName, ...) CALL_FUNCTION(_required, _funcName, 0, EXP(), EXP(), __VA_ARGS__)

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
	&UILuaAdapter::ButtonState,
	&UILuaAdapter::SliderState
};
std::map<std::string, UILuaAdapter::WidgetType> UILuaAdapter::sWidgetTypeMap =
{
	{ "Text", WidgetType::Text },
	{ "Button", WidgetType::Button },
	{ "Slider", WidgetType::Slider }
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
		lua_getfield(L, 3, TYPE_FIELD);  // Get "Type" field
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
	std::function<void()> func;
	lua_getfield(L, index, TEXT_FIELD);
	if (lua_isstring(L, -1))
	{
		const char* str = lua_tostring(L, -1);
		func = [str]() { ImGui::Text(str); };
	}
	else
	{
		CALL_FUNCTION_BRIEF(true, func,
			const char* str = luaL_checkstring(L, -1);
			lua_pop(L, 1);
			ImGui::Text(str);
		);
	}

	return func;
}

std::function<void()> UILuaAdapter::ButtonState(lua_State* L, int index)
{
	// Label function
	lua_getfield(L, index, TEXT_FIELD);
	std::function<const char*()> label;
	std::string labelString;
	if (lua_isstring(L, -1))
	{
		labelString = lua_tostring(L, -1);
	}
	else
	{
		CALL_FUNCTION_BRIEF(true, label,
			const char* str = luaL_checkstring(L, -1);
			lua_pop(L, 1);
			return str;
		);
	}

	// Click callback function
	lua_getfield(L, index, CLICK_FIELD);
	std::function<void()> callback;
	CALL_FUNCTION_BRIEF(false, callback);

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
			if (ImGui::Button(label()))
			{
				callback();
			}
		};
	}
}

std::function<void()> UILuaAdapter::SliderState(lua_State* L, int index)
{
	lua_getfield(L, index, TEXT_FIELD);
	std::function<const char*()> label;
	if (lua_isstring(L, -1))
	{
		const char* str = lua_tostring(L, -1);
		label = [str]() { return str; };
	}
	else
	{
		CALL_FUNCTION_BRIEF(true, label,
			const char* str = luaL_checkstring(L, -1);
			lua_pop(L, 1);
			return str;
		);
	}

	lua_getfield(L, index, SLIDER_MIN_FIELD);
	std::function<float()> min;
	if (lua_isnumber(L, -1))
	{
		float value = static_cast<float>(lua_tonumber(L, -1));
		min = [value]() { return value; };
	}
	else
	{
		CALL_FUNCTION_BRIEF(true, min,
			float value = static_cast<float>(luaL_checknumber(L, -1));
			lua_pop(L, 1);
			return value;
		);
	}

	lua_getfield(L, index, SLIDER_MAX_FIELD);
	std::function<float()> max;
	if (lua_isnumber(L, -1))
	{
		float value = static_cast<float>(lua_tonumber(L, -1));
		max = [value]() { return value; };
	}
	else
	{
		CALL_FUNCTION_BRIEF(true, max,
			float value = static_cast<float>(luaL_checknumber(L, -1));
			lua_pop(L, 1);
			return value;
		);
	}

	lua_getfield(L, index, SLIDER_VALUE_GETTER);
	std::function<float()> getter;
	CALL_FUNCTION_BRIEF(true, getter,
		float value = static_cast<float>(luaL_checknumber(L, -1));
		lua_pop(L, 1);
		return value;
	);

	lua_getfield(L, index, SLIDER_VALUE_CALLBACK_FIELD);
	std::function<void(float)> callback;
	CALL_FUNCTION(false, callback, 1, EXP(float value), EXP(lua_pushnumber(L, value);));

	return [label, min, max, getter, callback]()
	{
		float value = getter();
		if (ImGui::SliderFloat(label(), &value, min(), max()))
		{
			callback(value);
		}
	};
}
#pragma endregion
