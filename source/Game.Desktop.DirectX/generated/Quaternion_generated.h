// Genereated binding class
// Don't change this file manually

#include "Quaternion.h"
#include "LuaBind.h"

namespace GameEngine
{
class Quaternion_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Quaternion>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetConstructor<Quaternion>();
	};
};
}
