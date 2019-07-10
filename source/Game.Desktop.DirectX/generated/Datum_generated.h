// Genereated binding class
// Don't change this file manually

#include "Datum.h"
#include "LuaBind.h"

namespace GameEngine
{
class Datum_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Datum>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
	};
};
}
