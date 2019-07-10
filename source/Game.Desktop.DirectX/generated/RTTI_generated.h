// Genereated binding class
// Don't change this file manually

#include "RTTI.h"
#include "LuaBind.h"

namespace GameEngine
{
class RTTI_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<RTTI>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
	};
};
}
