// Genereated binding class
// Don't change this file manually

#include "Action.h"
#include "LuaBind.h"

namespace GameEngine
{
class Action_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<Action, Attributed>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetFunction<Action,  const std::string&>("Name", &Action::Name);
		bind.SetFunction<Action, void,  const std::string&>("SetName", &Action::SetName);
	};
};
}
