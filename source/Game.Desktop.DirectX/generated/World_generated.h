// Genereated binding class
// Don't change this file manually

#include "World.h"
#include "LuaBind.h"

namespace GameEngine
{
class World_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<World, Attributed>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetFunction<World, void, Attributed&>("Destroy", &World::Destroy);
		bind.SetFunction<World, WorldState*>("GetWorldState", &World::GetWorldState);
	};
};
}
