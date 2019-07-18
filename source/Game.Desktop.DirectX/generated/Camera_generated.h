// Genereated binding class
// Don't change this file manually

#include "Camera.h"
#include "LuaBind.h"

namespace GameEngine
{
class Camera_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Camera, Entity>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetFunction<Camera, float>("NearPlaneDistance", &Camera::NearPlaneDistance);
		bind.SetFunction<Camera, void, float>("SetNearPlaneDistance", &Camera::SetNearPlaneDistance);
		bind.SetFunction<Camera, float>("FarPlaneDistance", &Camera::FarPlaneDistance);
		bind.SetFunction<Camera, void, float>("SetFarPlaneDistance", &Camera::SetFarPlaneDistance);
		bind.SetFunction<Camera, Matrix>("ViewMatrix", &Camera::ViewMatrix);
		bind.SetFunction<Camera, Matrix>("ProjectionMatrix", &Camera::ProjectionMatrix);
		bind.SetFunction<Camera, Matrix>("ViewProjectionMatrix", &Camera::ViewProjectionMatrix);
		bind.SetFunction<Camera, Vector3,  const Vector3&>("Unproject", &Camera::Unproject);
		bind.SetProperty<Camera>("DefaultNearPlaneDistance", &Camera::DefaultNearPlaneDistance);
		bind.SetProperty<Camera>("DefaultFarPlaneDistance", &Camera::DefaultFarPlaneDistance);
	};
};
}
