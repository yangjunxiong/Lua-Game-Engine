// Genereated binding class
// Don't change this file manually

#include "ActionRender.h"
#include "LuaBind.h"

namespace GameEngine
{
class ActionRender_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<ActionRender, Action>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetProperty("Visible", &ActionRender::Visible);
	};
};
}
