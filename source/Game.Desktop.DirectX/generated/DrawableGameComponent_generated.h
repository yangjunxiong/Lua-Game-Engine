// Genereated binding class
// Don't change this file manually

#include "DrawableGameComponent.h"
#include "LuaBind.h"

namespace GameEngine
{
class DrawableGameComponent_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<DrawableGameComponent, TransformComponent>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetFunction<DrawableGameComponent, bool>("Visible", &DrawableGameComponent::Visible);
		bind.SetFunction<DrawableGameComponent, void, bool>("SetVisible", &DrawableGameComponent::SetVisible);
	};
};
}
