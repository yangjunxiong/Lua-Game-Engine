// Genereated register code
// Don't change this file manually

#include "pch.h"
#include "LuaBind.h"
#include "Action.h"
#include "ActionRender.h"
#include "Attributed.h"
#include "Datum.h"
#include "Entity.h"
#include "RTTI.h"
#include "Scope.h"
#include "Sector.h"
#include "Transform.h"
#include "World.h"
#include "WorldState.h"
using namespace GameEngine;
using namespace GameEngine::Lua;

DECLARE_LUA_WRAPPER(Action, "Action");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Action);
DECLARE_LUA_WRAPPER(ActionRender, "ActionRender");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(ActionRender);
DECLARE_LUA_WRAPPER(Attributed, "Attributed");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Attributed);
DECLARE_LUA_WRAPPER(Datum, "Datum");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Datum);
DECLARE_LUA_WRAPPER(Entity, "Entity");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Entity);
LUA_DEFINE_CUSTOM_COPY_TYPE(Entity);
DECLARE_LUA_VECTOR_WRAPPER_ALL(Entity, "Entity");
DECLARE_LUA_WRAPPER(RTTI, "RTTI");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(RTTI);
DECLARE_LUA_WRAPPER(Scope, "Scope");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Scope);
DECLARE_LUA_WRAPPER(Sector, "Sector");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Sector);
LUA_DEFINE_CUSTOM_COPY_TYPE(Sector);
DECLARE_LUA_VECTOR_WRAPPER_ALL(Sector, "Sector");
DECLARE_LUA_WRAPPER(Transform, "Transform");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Transform);
LUA_DEFINE_CUSTOM_COPY_TYPE(Transform);
DECLARE_LUA_VECTOR_WRAPPER_ALL(Transform, "Transform");
DECLARE_LUA_WRAPPER(World, "World");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(World);
DECLARE_LUA_WRAPPER(WorldState, "WorldState");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(WorldState);
#include "./generated/Action_generated.h"
#include "./generated/ActionRender_generated.h"
#include "./generated/Attributed_generated.h"
#include "./generated/Datum_generated.h"
#include "./generated/Entity_generated.h"
#include "./generated/RTTI_generated.h"
#include "./generated/Scope_generated.h"
#include "./generated/Sector_generated.h"
#include "./generated/Transform_generated.h"
#include "./generated/World_generated.h"
#include "./generated/WorldState_generated.h"
void RegisterLua(LuaBind& bind)
{
bind;
RTTI_generated::Lua_RegisterClass(bind);
Scope_generated::Lua_RegisterClass(bind);
Attributed_generated::Lua_RegisterClass(bind);
Action_generated::Lua_RegisterClass(bind);
ActionRender_generated::Lua_RegisterClass(bind);
Datum_generated::Lua_RegisterClass(bind);
Entity_generated::Lua_RegisterClass(bind);
Sector_generated::Lua_RegisterClass(bind);
Transform_generated::Lua_RegisterClass(bind);
World_generated::Lua_RegisterClass(bind);
WorldState_generated::Lua_RegisterClass(bind);
Action_generated::Lua_RegisterMember(bind);
ActionRender_generated::Lua_RegisterMember(bind);
Attributed_generated::Lua_RegisterMember(bind);
Datum_generated::Lua_RegisterMember(bind);
Entity_generated::Lua_RegisterMember(bind);
RTTI_generated::Lua_RegisterMember(bind);
Scope_generated::Lua_RegisterMember(bind);
Sector_generated::Lua_RegisterMember(bind);
Transform_generated::Lua_RegisterMember(bind);
World_generated::Lua_RegisterMember(bind);
WorldState_generated::Lua_RegisterMember(bind);
}
