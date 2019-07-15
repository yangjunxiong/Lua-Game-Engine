// Genereated binding class
// Don't change this file manually

#include "Transform.h"
#include "LuaBind.h"

namespace GameEngine
{
class Transform_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Transform>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetConstructor<Transform>();
		bind.SetFunction<Transform,  const Vector3&>("GetWorldPosition", &Transform::GetWorldPosition);
		bind.SetFunction<Transform,  const Quaternion&>("GetWorldRotation", &Transform::GetWorldRotation);
		bind.SetFunction<Transform,  const Vector3&>("GetWorldScale", &Transform::GetWorldScale);
		bind.SetFunction<Transform, void,  const Vector3&>("SetWorldPosition", &Transform::SetWorldPosition);
		bind.SetFunction<Transform, void,  const Quaternion&>("SetWorldRotation", &Transform::SetWorldRotation);
		bind.SetFunction<Transform, void,  const Vector3&>("SetWorldScale", &Transform::SetWorldScale);
		bind.SetFunction<Transform,  const Vector3&>("GetLocalPosition", &Transform::GetLocalPosition);
		bind.SetFunction<Transform,  const Quaternion&>("GetLocalRotation", &Transform::GetLocalRotation);
		bind.SetFunction<Transform,  const Vector3&>("GetLocalScale", &Transform::GetLocalScale);
		bind.SetFunction<Transform, void,  const Vector3&>("SetLocalPosition", &Transform::SetLocalPosition);
		bind.SetFunction<Transform, void,  const Quaternion&>("SetLocalRotation", &Transform::SetLocalRotation);
		bind.SetFunction<Transform, void,  const Vector3&>("SetLocalScale", &Transform::SetLocalScale);
	};
};
}
