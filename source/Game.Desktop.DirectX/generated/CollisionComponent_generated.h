// Genereated binding class
// Don't change this file manually

#include "CollisionComponent.h"
#include "LuaBind.h"

namespace GameEngine
{
class CollisionComponent_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<CollisionComponent, Action>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetConstructor<CollisionComponent, Entity*>();
		bind.SetFunction<CollisionComponent, bool,  const Ray&>("IntersectsRay", &CollisionComponent::IntersectsRay);
		bind.SetFunction<CollisionComponent, bool,  const Sphere&>("IntersectsSphere", &CollisionComponent::IntersectsSphere);
	};
};
}
