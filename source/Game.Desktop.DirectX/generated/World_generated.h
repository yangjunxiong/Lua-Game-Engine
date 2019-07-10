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
		bind.SetFunction<World, Sector*,  const std::string&>("CreateSector", &World::CreateSector);
		bind.SetFunction<World, void, Attributed&>("Destroy", &World::Destroy);
		bind.SetProperty<World>("SECTOR_TABLE_KEY", &World::SECTOR_TABLE_KEY);
		bind.SetProperty<World>("ACTION_TABLE_KEY", &World::ACTION_TABLE_KEY);
		bind.SetFunction<World, WorldState*>("GetWorldState", &World::GetWorldState);
	};
};
}
