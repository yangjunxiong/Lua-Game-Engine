#include "pch.h"
#include "LuaBind.h"
#include "./generated/Attributed_generated.h"
#include "./generated/Entity_generated.h"
#include "./generated/RTTI_generated.h"
#include "./generated/Scope_generated.h"
using namespace GameEngine::Lua;
void RegisterLua(LuaBind& bind)
{
bind;
RTTI_generated::Lua_RegisterClass(bind.LuaState());
Scope_generated::Lua_RegisterClass(bind.LuaState());
Attributed_generated::Lua_RegisterClass(bind.LuaState());
Entity_generated::Lua_RegisterClass(bind.LuaState());
Attributed_generated::Lua_RegisterMember(bind);
Entity_generated::Lua_RegisterMember(bind);
RTTI_generated::Lua_RegisterMember(bind);
Scope_generated::Lua_RegisterMember(bind);
}
