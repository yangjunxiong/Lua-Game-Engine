// Genereated binding class
// Don't change this file manually

#include "GameComponent.h"
#include "LuaBind.h"

namespace GameEngine
{
class GameComponent_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<GameComponent, RTTI>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetFunction<GameComponent, Game*>("GetGame", &GameComponent::GetGame);
		bind.SetFunction<GameComponent, void, Game&>("SetGame", &GameComponent::SetGame);
		bind.SetFunction<GameComponent, bool>("Enabled", &GameComponent::Enabled);
		bind.SetFunction<GameComponent, void, bool>("SetEnabled", &GameComponent::SetEnabled);
		bind.SetFunction<GameComponent, void,  const GameEngine::GameTime&>("Update", &GameComponent::Update);
	};
};
}
