// Genereated binding class
// Don't change this file manually

#include "Attributed.h"
#include "LuaBind.h"

namespace GameEngine
{
class Attributed_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Attributed, Scope>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
	};
};
}
