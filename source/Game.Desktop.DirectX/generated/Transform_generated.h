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
		bind.SetFunction<Transform, Vector3&>("GetWorldPosition", &Transform::GetWorldPosition);
		bind.SetFunction<Transform, Quaternion&>("GetWorldRotation", &Transform::GetWorldRotation);
		bind.SetFunction<Transform, Vector3&>("GetWorldScale", &Transform::GetWorldScale);
		bind.SetFunction<Transform, void,  const Vector3&>("SetWorldPosition", &Transform::SetWorldPosition);
		bind.SetFunction<Transform, void,  const Quaternion&>("SetWorldRotation", &Transform::SetWorldRotation);
		bind.SetFunction<Transform, void,  const Vector3&>("SetWorldScale", &Transform::SetWorldScale);
		bind.SetFunction<Transform, Vector3&>("GetLocalPosition", &Transform::GetLocalPosition);
		bind.SetFunction<Transform, Quaternion&>("GetLocalRotation", &Transform::GetLocalRotation);
		bind.SetFunction<Transform, Vector3&>("GetLocalScale", &Transform::GetLocalScale);
		bind.SetFunction<Transform, void,  const Vector3&>("SetLocalPosition", &Transform::SetLocalPosition);
		bind.SetFunction<Transform, void,  const Quaternion&>("SetLocalRotation", &Transform::SetLocalRotation);
		bind.SetFunction<Transform, void,  const Vector3&>("SetLocalScale", &Transform::SetLocalScale);
		bind.SetFunction<Transform, Vector3&>("Forward", &Transform::Forward);
		bind.SetFunction<Transform, Vector3&>("Up", &Transform::Up);
		bind.SetFunction<Transform, Vector3&>("Right", &Transform::Right);
		bind.SetFunction<Transform, Matrix&>("GetWorldMatrix", &Transform::GetWorldMatrix);
		bind.SetFunction<Transform, Matrix&>("GetLocalMatrix", &Transform::GetLocalMatrix);
		bind.SetFunction<Transform, Matrix&>("GetWorldMatrixInverse", &Transform::GetWorldMatrixInverse);
	};
};
}
