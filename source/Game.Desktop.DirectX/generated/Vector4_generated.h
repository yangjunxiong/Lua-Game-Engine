// Genereated binding class
// Don't change this file manually

#include "Vector4.h"
#include "LuaBind.h"

namespace GameEngine
{
class Vector4_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Vector4>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetFunction<Vector4, float>("GetX", &Vector4::GetX);
		bind.SetFunction<Vector4, float>("GetY", &Vector4::GetY);
		bind.SetFunction<Vector4, float>("GetZ", &Vector4::GetZ);
		bind.SetFunction<Vector4, float>("GetW", &Vector4::GetW);
		bind.SetFunction<Vector4, void, float>("SetX", &Vector4::SetX);
		bind.SetFunction<Vector4, void, float>("SetY", &Vector4::SetY);
		bind.SetFunction<Vector4, void, float>("SetZ", &Vector4::SetZ);
		bind.SetFunction<Vector4, void, float>("SetW", &Vector4::SetW);
		bind.SetProperty<Vector4>("One", &Vector4::One);
		bind.SetProperty<Vector4>("Zero", &Vector4::Zero);
		bind.SetProperty<Vector4>("Forward", &Vector4::Forward);
		bind.SetProperty<Vector4>("Up", &Vector4::Up);
		bind.SetProperty<Vector4>("Right", &Vector4::Right);
	};
};
class Vector3_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Vector3, Vector4>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetConstructor<Vector3, float, float, float>();
		bind.SetFunction<Vector3, float>("Length", &Vector3::Length);
		bind.SetFunction<Vector3, float>("LengthSquare", &Vector3::LengthSquare);
		bind.SetFunction<Vector3, void>("Normalize", &Vector3::Normalize);
		bind.SetProperty<Vector3>("One", &Vector3::One);
		bind.SetProperty<Vector3>("Zero", &Vector3::Zero);
		bind.SetProperty<Vector3>("Forward", &Vector3::Forward);
		bind.SetProperty<Vector3>("Up", &Vector3::Up);
		bind.SetProperty<Vector3>("Right", &Vector3::Right);
	};
};
}
