// Genereated binding class
// Don't change this file manually

#include "GameTime.h"
#include "LuaBind.h"

namespace GameEngine
{
class GameTime_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<GameTime>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
	};
};
}
