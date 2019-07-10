// Genereated binding class
// Don't change this file manually

#include "Planet.h"
#include "LuaBind.h"

namespace GameEngine
{
class Planet_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Planet, DrawableGameComponent>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetProperty("mSpinSpeed", &Planet::mSpinSpeed);
		bind.SetProperty("mRotationSpeed", &Planet::mRotationSpeed);
		bind.SetProperty("mOrbitRadius", &Planet::mOrbitRadius);
	};
};
}
