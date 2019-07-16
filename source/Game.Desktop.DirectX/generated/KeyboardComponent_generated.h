// Genereated binding class
// Don't change this file manually

#include "KeyboardComponent.h"
#include "LuaBind.h"

namespace GameEngine
{
class KeyboardEntity_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<KeyboardEntity, Entity>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
	};
};
}
