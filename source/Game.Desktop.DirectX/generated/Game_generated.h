// Genereated binding class
// Don't change this file manually

#include "Game.h"
#include "LuaBind.h"

namespace GameEngine
{
class Game_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Game, RenderTarget>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
	};
};
}
