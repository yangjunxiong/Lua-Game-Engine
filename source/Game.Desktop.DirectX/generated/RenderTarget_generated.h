// Genereated binding class
// Don't change this file manually

#include "RenderTarget.h"
#include "LuaBind.h"

namespace GameEngine
{
class RenderTarget_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<RenderTarget, RTTI>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
	};
};
}
