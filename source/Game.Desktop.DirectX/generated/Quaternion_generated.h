// Genereated binding class
// Don't change this file manually

#include "Quaternion.h"
#include "LuaBind.h"

namespace GameEngine
{
class Quaternion_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Quaternion>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetConstructor<Quaternion>();
		bind.SetFunction<Quaternion, float>("GetX", &Quaternion::GetX);
		bind.SetFunction<Quaternion, float>("GetY", &Quaternion::GetY);
		bind.SetFunction<Quaternion, float>("GetZ", &Quaternion::GetZ);
		bind.SetFunction<Quaternion, float>("GetW", &Quaternion::GetW);
		bind.SetFunction<Quaternion, void, float>("SetX", &Quaternion::SetX);
		bind.SetFunction<Quaternion, void, float>("SetY", &Quaternion::SetY);
		bind.SetFunction<Quaternion, void, float>("SetZ", &Quaternion::SetZ);
		bind.SetFunction<Quaternion, void, float>("SetW", &Quaternion::SetW);
		bind.SetFunction<Quaternion, Vector3>("Forward", &Quaternion::Forward);
		bind.SetFunction<Quaternion, Vector3>("Up", &Quaternion::Up);
		bind.SetFunction<Quaternion, Vector3>("Right", &Quaternion::Right);
		bind.SetProperty<Quaternion>("Identity", &Quaternion::Identity);
		bind.SetFunction<Quaternion, Quaternion(*)( const Quaternion&,  const Quaternion&)>("AddRotation", &Quaternion::AddRotation);
		bind.SetFunction<Quaternion, Quaternion(*)( const Vector3&, float)>("RotationAroundAxis", &Quaternion::RotationAroundAxis);
		bind.SetFunction<Quaternion, Quaternion(*)( const Quaternion&,  const Quaternion&, float)>("Slerp", &Quaternion::Slerp);
		bind.SetFunction<Quaternion, Quaternion(*)( const Vector3&)>("FromEulerAngles", &Quaternion::FromEulerAngles);
		bind.SetFunction<Quaternion, Vector3(*)( const Quaternion&)>("ToEulerAngles", &Quaternion::ToEulerAngles);
	};
};
}
