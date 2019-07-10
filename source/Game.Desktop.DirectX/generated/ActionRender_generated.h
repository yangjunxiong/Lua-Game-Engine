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
		bind.SetFunction<ActionRender, void, WorldState&>("Update", &ActionRender::Update);
		bind.SetFunction<ActionRender, void, int>("SetTexture", &ActionRender::SetTexture);
		bind.SetFunction<ActionRender, void, int>("SetSprite", &ActionRender::SetSprite);
		bind.SetFunction<ActionRender, void, bool>("SetHorizontalFlip", &ActionRender::SetHorizontalFlip);
		bind.SetFunction<ActionRender, void, bool>("SetVerticalFlip", &ActionRender::SetVerticalFlip);
		bind.SetFunction<ActionRender, void,  const Transform&>("SetOffset", &ActionRender::SetOffset);
	};
};
}
