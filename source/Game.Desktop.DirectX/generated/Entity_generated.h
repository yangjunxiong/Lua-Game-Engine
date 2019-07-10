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
		bind.SetFunction<Entity, Transform&>("GetTransform", &Entity::GetTransform);
		bind.SetFunction<Entity, void,  const Transform&>("SetTransform", &Entity::SetTransform);
		bind.SetFunction<Entity, glm::vec4>("GetPosition", &Entity::GetPosition);
		bind.SetFunction<Entity, glm::vec4>("GetRotation", &Entity::GetRotation);
		bind.SetFunction<Entity, glm::vec4>("GetScale", &Entity::GetScale);
		bind.SetFunction<Entity, bool>("IsActive", &Entity::IsActive);
		bind.SetFunction<Entity, void,  const glm::vec4&>("SetPosition", &Entity::SetPosition);
		bind.SetFunction<Entity, void,  const glm::vec4&>("SetRotation", &Entity::SetRotation);
		bind.SetFunction<Entity, void,  const glm::vec4&>("SetScale", &Entity::SetScale);
		bind.SetFunction<Entity, void, bool>("SetActive", &Entity::SetActive);
		bind.SetFunction<Entity, Action*, std::string>("CreateAction", &Entity::CreateAction);
		bind.SetProperty<Entity>("ACTION_TABLE_KEY", &Entity::ACTION_TABLE_KEY);
		bind.SetProperty<Entity>("ACTION_TABLE_INDEX", &Entity::ACTION_TABLE_INDEX);
		bind.SetProperty("Tags", &Entity::Tags);
		bind.SetProperty("Targets", &Entity::Targets);
	};
};
}
