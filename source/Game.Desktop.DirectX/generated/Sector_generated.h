// Genereated binding class
// Don't change this file manually

#include "Sector.h"
#include "LuaBind.h"

namespace GameEngine
{
class Sector_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Sector, Attributed>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetConstructor<Sector>();
		bind.SetFunction<Sector,  const std::string&>("Name", &Sector::Name);
		bind.SetFunction<Sector, void,  const std::string&>("SetName", &Sector::SetName);
		bind.SetFunction<Sector, void, bool>("SetActive", &Sector::SetActive);
		bind.SetFunction<Sector, bool>("IsActive", &Sector::IsActive);
		bind.SetFunction<Sector, Entity*,  const std::string&,  const std::string&>("CreateEntity", &Sector::CreateEntity);
	};
};
}
