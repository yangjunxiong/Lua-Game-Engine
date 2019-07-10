// Genereated binding class
// Don't change this file manually

#include "PerspectiveCamera.h"
#include "LuaBind.h"

namespace GameEngine
{
class PerspectiveCamera_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<PerspectiveCamera, Camera>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
	};
};
}
