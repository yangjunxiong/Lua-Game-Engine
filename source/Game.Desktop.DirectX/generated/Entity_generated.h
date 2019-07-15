// Genereated binding class
// Don't change this file manually

#include "Entity.h"
#include "LuaBind.h"

namespace GameEngine
{
class Entity_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Entity, Attributed>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetConstructor<Entity>();
		bind.SetFunction<Entity,  const std::string&>("Name", &Entity::Name);
		bind.SetFunction<Entity, void,  const std::string&>("SetName", &Entity::SetName);
		bind.SetFunction<Entity, void, Sector&>("SetSector", &Entity::SetSector);
		bind.SetFunction<Entity, Transform*>("GetTransform", &Entity::GetTransform);
		bind.SetFunction<Entity, bool>("IsActive", &Entity::IsActive);
		bind.SetFunction<Entity, void, bool>("SetActive", &Entity::SetActive);
		bind.SetFunction<Entity, Action*, std::string>("CreateAction", &Entity::CreateAction);
		bind.SetFunction<Entity, void>("Destroy", &Entity::Destroy);
		bind.SetFunction<Entity, Entity*>("GetTransformParent", &Entity::GetTransformParent);
		bind.SetFunction<Entity, void, Entity*>("SetTransformParent", &Entity::SetTransformParent);
		bind.SetProperty("Tags", &Entity::Tags);
	};
};
}
