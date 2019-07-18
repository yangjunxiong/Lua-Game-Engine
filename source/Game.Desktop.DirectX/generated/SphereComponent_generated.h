// Genereated binding class
// Don't change this file manually

#include "SphereComponent.h"
#include "LuaBind.h"

namespace GameEngine
{
class SphereComponent_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<SphereComponent, CollisionComponent>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetConstructor<SphereComponent, Entity*,  const Vector3&, float>();
		bind.SetFunction<SphereComponent, bool,  const Ray&>("IntersectsRay", &SphereComponent::IntersectsRay);
		bind.SetFunction<SphereComponent, bool,  const Sphere&>("IntersectsSphere", &SphereComponent::IntersectsSphere);
		bind.SetFunction<SphereComponent,  const Vector3&>("GetCenter", &SphereComponent::GetCenter);
		bind.SetFunction<SphereComponent, void,  const Vector3&>("SetCenter", &SphereComponent::SetCenter);
		bind.SetFunction<SphereComponent, float>("GetRadius", &SphereComponent::GetRadius);
		bind.SetFunction<SphereComponent, void, float>("SetRadius", &SphereComponent::SetRadius);
	};
};
}
