// Genereated binding class
// Don't change this file manually

#include "Collision.h"
#include "LuaBind.h"

namespace GameEngine
{
class Sphere_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Sphere>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetConstructor<Sphere,  const Vector3&, float>();
		bind.SetProperty("Center", &Sphere::Center);
		bind.SetProperty("Radius", &Sphere::Radius);
	};
};
class Ray_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Ray>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetConstructor<Ray,  const Vector3&,  const Vector3&>();
		bind.SetFunction<Ray, Vector3>("Direction", &Ray::Direction);
		bind.SetFunction<Ray, float>("Distance", &Ray::Distance);
		bind.SetProperty("Start", &Ray::Start);
		bind.SetProperty("End", &Ray::End);
	};
};
class Collision_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Collision>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetFunction<Collision, bool(*)( const Ray&,  const Sphere&)>("RayIntersectsSphere", &Collision::RayIntersectsSphere);
		bind.SetFunction<Collision, bool(*)( const Sphere&,  const Sphere&)>("SphereIntersectsSphere", &Collision::SphereIntersectsSphere);
	};
};
}
