#pragma once
#include <functional>
#include "lua.h"

namespace GameEngine
{
	class UIManager final
	{
	public:
		struct RenderBlock
		{
			RenderBlock(const std::string& name, bool active, std::function<void()> func);
			std::string Name;
			bool Active = true;
			std::function<void()> RenderFunc;
		};

		static void AddRenderBlock(RenderBlock block);
		static void RemoveRenderBlock(const std::string& name);
		static const std::vector<RenderBlock>& GetAllBlocks();
		static void ClearAllBlocks();
		static bool GetWindowActive(const std::string& name);
		static void SetWindowActive(const std::string& name, bool active);

	private:
		static std::vector<RenderBlock> sRenderBlocks;
	};

	class UILuaAdapter final
	{
	public:
		static void Init(lua_State* L);
		static int AddWindow(lua_State* L);
		static int RemoveWindow(lua_State* L);
		static int SetWindowActive(lua_State* L);

	private:
		enum class WidgetType
		{
			Text = 0,
			Button
		};

		using WidgetFunction = std::function<void()>(*)(lua_State* L, int index);
		static std::vector<WidgetFunction> sWidgetFunctions;
		static std::map<std::string, WidgetType> sWidgetTypeMap;
		static std::function<void()> TextState(lua_State* L, int index);
		static std::function<void()> ButtonState(lua_State* L, int index);
	};
}