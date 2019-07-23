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
			Button,
			Slider
		};
		
		const static inline char* TYPE_FIELD = "Type";
		const static inline char* TEXT_FIELD = "Text";
		const static inline char* CLICK_FIELD = "Click";
		const static inline char* SLIDER_MIN_FIELD = "Min";
		const static inline char* SLIDER_MAX_FIELD = "Max";
		const static inline char* SLIDER_VALUE_GETTER = "Value";
		const static inline char* SLIDER_VALUE_CALLBACK_FIELD = "ValueChange";

		using WidgetFunction = std::function<void()>(*)(lua_State* L, int index);
		static std::vector<WidgetFunction> sWidgetFunctions;
		static std::map<std::string, WidgetType> sWidgetTypeMap;
		static std::function<void()> TextState(lua_State* L, int index);
		static std::function<void()> ButtonState(lua_State* L, int index);
		static std::function<void()> SliderState(lua_State* L, int index);
	};
}