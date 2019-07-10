// Genereated binding class
// Don't change this file manually

#include "WorldState.h"
#include "LuaBind.h"

namespace GameEngine
{
class WorldState_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<WorldState, RTTI>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetFunction<WorldState, World*>("GetWorld", &WorldState::GetWorld);
		bind.SetFunction<WorldState, Sector*>("GetSector", &WorldState::GetSector);
		bind.SetFunction<WorldState, Entity*>("GetEntity", &WorldState::GetEntity);
		bind.SetFunction<WorldState, Action*>("GetAction", &WorldState::GetAction);
	};
};
}
