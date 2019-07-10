// Genereated register code
// Don't change this file manually

#include "pch.h"
#include "LuaRegister.h"
#include "LuaBind.h"
#include "Event.h"
#include "IEventSubscriber.h"
#include "Planet.h"
#include "InputData.h"
#include "Camera.h"
#include "DrawableGameComponent.h"
#include "FirstPersonCamera.h"
#include "Game.h"
#include "GameComponent.h"
#include "PerspectiveCamera.h"
#include "RenderTarget.h"
#include "TransformComponent.h"
#include "Action.h"
#include "ActionRender.h"
#include "Attributed.h"
#include "Datum.h"
#include "Entity.h"
#include "GameTime.h"
#include "RTTI.h"
#include "Scope.h"
#include "Sector.h"
#include "Transform.h"
#include "World.h"
#include "WorldState.h"
namespace GameEngine {};
namespace GameEngine::Lua {};
namespace GameEngine {};
namespace Rendering {};
using namespace GameEngine;
using namespace GameEngine::Lua;
using namespace GameEngine;
using namespace Rendering;

class LuaMonitor final {
public:
std::vector<IEventSubscriber*> subscribers;
};
std::map<LuaBind*, LuaMonitor*> gMonitors;
DECLARE_LUA_WRAPPER(Planet, "Planet");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Planet);
DECLARE_LUA_WRAPPER(MouseInput, "MouseInput");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(MouseInput);
LUA_DEFINE_CUSTOM_COPY_TYPE(MouseInput);
DECLARE_LUA_VECTOR_WRAPPER_ALL(MouseInput, "MouseInput");
DECLARE_LUA_WRAPPER(Camera, "Camera");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Camera);
DECLARE_LUA_WRAPPER(DrawableGameComponent, "DrawableGameComponent");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(DrawableGameComponent);
DECLARE_LUA_WRAPPER(FirstPersonCamera, "FirstPersonCamera");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(FirstPersonCamera);
DECLARE_LUA_WRAPPER(Game, "Game");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Game);
DECLARE_LUA_WRAPPER(GameComponent, "GameComponent");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(GameComponent);
DECLARE_LUA_WRAPPER(PerspectiveCamera, "PerspectiveCamera");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(PerspectiveCamera);
DECLARE_LUA_WRAPPER(RenderTarget, "RenderTarget");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(RenderTarget);
DECLARE_LUA_WRAPPER(TransformComponent, "TransformComponent");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(TransformComponent);
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
DECLARE_LUA_WRAPPER(GameTime, "GameTime");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(GameTime);
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
#include "./generated/Planet_generated.h"
#include "./generated/InputData_generated.h"
#include "./generated/Camera_generated.h"
#include "./generated/DrawableGameComponent_generated.h"
#include "./generated/FirstPersonCamera_generated.h"
#include "./generated/Game_generated.h"
#include "./generated/GameComponent_generated.h"
#include "./generated/PerspectiveCamera_generated.h"
#include "./generated/RenderTarget_generated.h"
#include "./generated/TransformComponent_generated.h"
#include "./generated/Action_generated.h"
#include "./generated/ActionRender_generated.h"
#include "./generated/Attributed_generated.h"
#include "./generated/Datum_generated.h"
#include "./generated/Entity_generated.h"
#include "./generated/GameTime_generated.h"
#include "./generated/RTTI_generated.h"
#include "./generated/Scope_generated.h"
#include "./generated/Sector_generated.h"
#include "./generated/Transform_generated.h"
#include "./generated/World_generated.h"
#include "./generated/WorldState_generated.h"
void LuaRegister::RegisterLua(LuaBind& bind)
{
bind;
RTTI_generated::Lua_RegisterClass(bind);
GameComponent_generated::Lua_RegisterClass(bind);
TransformComponent_generated::Lua_RegisterClass(bind);
DrawableGameComponent_generated::Lua_RegisterClass(bind);
Planet_generated::Lua_RegisterClass(bind);
MouseInput_generated::Lua_RegisterClass(bind);
Camera_generated::Lua_RegisterClass(bind);
PerspectiveCamera_generated::Lua_RegisterClass(bind);
FirstPersonCamera_generated::Lua_RegisterClass(bind);
RenderTarget_generated::Lua_RegisterClass(bind);
Game_generated::Lua_RegisterClass(bind);
Scope_generated::Lua_RegisterClass(bind);
Attributed_generated::Lua_RegisterClass(bind);
Action_generated::Lua_RegisterClass(bind);
ActionRender_generated::Lua_RegisterClass(bind);
Datum_generated::Lua_RegisterClass(bind);
Entity_generated::Lua_RegisterClass(bind);
GameTime_generated::Lua_RegisterClass(bind);
Sector_generated::Lua_RegisterClass(bind);
Transform_generated::Lua_RegisterClass(bind);
World_generated::Lua_RegisterClass(bind);
WorldState_generated::Lua_RegisterClass(bind);

LuaMonitor* monitor = new LuaMonitor();
gMonitors[&bind] = monitor;
Planet_generated::Lua_RegisterMember(bind);
MouseInput_generated::Lua_RegisterMember(bind);
monitor->subscribers.emplace_back(new MouseInput_event_generated(bind));
Camera_generated::Lua_RegisterMember(bind);
DrawableGameComponent_generated::Lua_RegisterMember(bind);
FirstPersonCamera_generated::Lua_RegisterMember(bind);
Game_generated::Lua_RegisterMember(bind);
GameComponent_generated::Lua_RegisterMember(bind);
PerspectiveCamera_generated::Lua_RegisterMember(bind);
RenderTarget_generated::Lua_RegisterMember(bind);
TransformComponent_generated::Lua_RegisterMember(bind);
Action_generated::Lua_RegisterMember(bind);
ActionRender_generated::Lua_RegisterMember(bind);
Attributed_generated::Lua_RegisterMember(bind);
Datum_generated::Lua_RegisterMember(bind);
Entity_generated::Lua_RegisterMember(bind);
GameTime_generated::Lua_RegisterMember(bind);
RTTI_generated::Lua_RegisterMember(bind);
Scope_generated::Lua_RegisterMember(bind);
Sector_generated::Lua_RegisterMember(bind);
Transform_generated::Lua_RegisterMember(bind);
World_generated::Lua_RegisterMember(bind);
WorldState_generated::Lua_RegisterMember(bind);
}
void LuaRegister::UnregisterLua(LuaBind& bind, bool all)
{
LuaMonitor* monitor = gMonitors[&bind];
for (const auto sub : monitor->subscribers) {
delete sub;
}
delete monitor;
gMonitors.erase(&bind);
if (all) {
Event<MouseInput>::UnsubscribeAll();
}
}
