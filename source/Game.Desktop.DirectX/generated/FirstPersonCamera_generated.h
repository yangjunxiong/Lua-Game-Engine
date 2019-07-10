// Genereated binding class
// Don't change this file manually

#include "FirstPersonCamera.h"
#include "LuaBind.h"

namespace GameEngine
{
class FirstPersonCamera_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<FirstPersonCamera, PerspectiveCamera>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetFunction<FirstPersonCamera, void, float>("SetPosition", &FirstPersonCamera::SetPosition);
	};
};
}
