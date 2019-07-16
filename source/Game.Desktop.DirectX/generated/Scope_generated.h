// Genereated binding class
// Don't change this file manually

#include "Scope.h"
#include "LuaBind.h"

namespace GameEngine
{
class Scope_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Scope, RTTI>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetFunction<Scope, Datum*,  const std::string&>("Find", &Scope::Find);
		bind.SetFunction<Scope, void, Scope&,  const std::string&>("Adopt", &Scope::Adopt);
		bind.SetFunction<Scope, void, Scope&>("Orphan", &Scope::Orphan);
		bind.SetFunction<Scope, Scope*>("GetParent", &Scope::GetParent);
		bind.SetFunction<Scope, size_t>("Size", &Scope::Size);
		bind.SetFunction<Scope, void>("Clear", &Scope::Clear);
	};
};
}
