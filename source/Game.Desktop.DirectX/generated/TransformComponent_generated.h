// Genereated binding class
// Don't change this file manually

#include "TransformComponent.h"
#include "LuaBind.h"

namespace GameEngine
{
class TransformComponent_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<TransformComponent, GameComponent>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
	};
};
}
