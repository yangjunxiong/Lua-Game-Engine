// Genereated binding class
// Don't change this file manually

#include "Material.h"
#include "LuaBind.h"

namespace GameEngine
{
class Material_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Material, RTTI>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
	};
};
}
