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
		bind.RegisterType<Camera, GameComponent>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
	};
};
}
