// Genereated binding class
// Don't change this file manually

#include "StaticMeshRenderComponent.h"
#include "LuaBind.h"

namespace GameEngine
{
class StaticMeshRenderComponent_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<StaticMeshRenderComponent, ActionRender>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetConstructor<StaticMeshRenderComponent, Entity*>();
		bind.SetFunction<StaticMeshRenderComponent, void,  const std::string&>("SetModel", &StaticMeshRenderComponent::SetModel);
		bind.SetFunction<StaticMeshRenderComponent, StaticMeshMaterial*>("GetMaterial", &StaticMeshRenderComponent::GetMaterial);
	};
};
}
