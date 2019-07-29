// Genereated binding class
// Don't change this file manually

#include "Skybox.h"
#include "LuaBind.h"

namespace GameEngine
{
class Skybox_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Skybox, ActionRender>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetConstructor<Skybox, Entity*,  const std::string&, float>();
	};
};
}
