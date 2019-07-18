// Genereated register code
// Don't change this file manually

#include "pch.h"
#include "LuaRegister.h"
#include "LuaBind.h"
#include "Event.h"
#include "IEventSubscriber.h"
#include "InputData.h"
#include "Camera.h"
#include "FirstPersonCamera.h"
#include "KeyboardComponent.h"
#include "Material.h"
#include "MouseComponent.h"
#include "PerspectiveCamera.h"
#include "RenderTarget.h"
#include "StaticMeshMaterial.h"
#include "StaticMeshRenderComponent.h"
#include "Action.h"
#include "ActionRender.h"
#include "Attributed.h"
#include "Collision.h"
#include "CollisionComponent.h"
#include "Datum.h"
#include "Entity.h"
#include "GameTime.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "RTTI.h"
#include "Scope.h"
#include "Sector.h"
#include "SphereComponent.h"
#include "Transform.h"
#include "Vector4.h"
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
DECLARE_LUA_WRAPPER(MouseInput, "MouseInput", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(MouseInput);
LUA_DEFINE_CUSTOM_COPY_TYPE(MouseInput);
DECLARE_LUA_VECTOR_WRAPPER_ALL(MouseInput, "MouseInput");
DECLARE_LUA_WRAPPER(Camera, "Camera", false);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Camera);
DECLARE_LUA_WRAPPER(FirstPersonCamera, "FirstPersonCamera", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(FirstPersonCamera);
DECLARE_LUA_WRAPPER(KeyboardEntity, "KeyboardEntity", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(KeyboardEntity);
DECLARE_LUA_WRAPPER(Material, "Material", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Material);
DECLARE_LUA_WRAPPER(MouseButtons, "MouseButtons", true);
LUA_DEFINE_ENUM(MouseButtons);
DECLARE_LUA_VECTOR_WRAPPER_ALL(MouseButtons, "MouseButtons");
DECLARE_LUA_WRAPPER(MouseEventType, "MouseEventType", true);
LUA_DEFINE_ENUM(MouseEventType);
DECLARE_LUA_VECTOR_WRAPPER_ALL(MouseEventType, "MouseEventType");
DECLARE_LUA_WRAPPER(MouseEntity, "MouseEntity", false);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(MouseEntity);
DECLARE_LUA_WRAPPER(MouseEvent, "MouseEvent", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(MouseEvent);
DECLARE_LUA_WRAPPER(PerspectiveCamera, "PerspectiveCamera", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(PerspectiveCamera);
DECLARE_LUA_WRAPPER(RenderTarget, "RenderTarget", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(RenderTarget);
DECLARE_LUA_WRAPPER(StaticMeshMaterial, "StaticMeshMaterial", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(StaticMeshMaterial);
DECLARE_LUA_WRAPPER(StaticMeshRenderComponent, "StaticMeshRenderComponent", false);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(StaticMeshRenderComponent);
LUA_DEFINE_CUSTOM_COPY_TYPE(StaticMeshRenderComponent);
DECLARE_LUA_VECTOR_WRAPPER_ALL(StaticMeshRenderComponent, "StaticMeshRenderComponent");
DECLARE_LUA_WRAPPER(Action, "Action", false);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Action);
DECLARE_LUA_WRAPPER(ActionRender, "ActionRender", false);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(ActionRender);
DECLARE_LUA_WRAPPER(Attributed, "Attributed", false);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Attributed);
DECLARE_LUA_WRAPPER(Sphere, "Sphere", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Sphere);
LUA_DEFINE_CUSTOM_COPY_TYPE(Sphere);
DECLARE_LUA_VECTOR_WRAPPER_ALL(Sphere, "Sphere");
DECLARE_LUA_WRAPPER(Ray, "Ray", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Ray);
LUA_DEFINE_CUSTOM_COPY_TYPE(Ray);
DECLARE_LUA_VECTOR_WRAPPER_ALL(Ray, "Ray");
DECLARE_LUA_WRAPPER(Collision, "Collision", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Collision);
DECLARE_LUA_WRAPPER(CollisionComponent, "CollisionComponent", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(CollisionComponent);
DECLARE_LUA_WRAPPER(Datum, "Datum", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Datum);
DECLARE_LUA_WRAPPER(Entity, "Entity", false);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Entity);
LUA_DEFINE_CUSTOM_COPY_TYPE(Entity);
DECLARE_LUA_VECTOR_WRAPPER_ALL(Entity, "Entity");
DECLARE_LUA_WRAPPER(GameTime, "GameTime", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(GameTime);
DECLARE_LUA_WRAPPER(Matrix, "Matrix", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Matrix);
LUA_DEFINE_CUSTOM_COPY_TYPE(Matrix);
DECLARE_LUA_VECTOR_WRAPPER_ALL(Matrix, "Matrix");
DECLARE_LUA_WRAPPER(Quaternion, "Quaternion", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Quaternion);
LUA_DEFINE_CUSTOM_COPY_TYPE(Quaternion);
DECLARE_LUA_VECTOR_WRAPPER_ALL(Quaternion, "Quaternion");
DECLARE_LUA_WRAPPER(RTTI, "RTTI", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(RTTI);
DECLARE_LUA_WRAPPER(Scope, "Scope", false);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Scope);
DECLARE_LUA_WRAPPER(Sector, "Sector", false);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Sector);
LUA_DEFINE_CUSTOM_COPY_TYPE(Sector);
DECLARE_LUA_VECTOR_WRAPPER_ALL(Sector, "Sector");
DECLARE_LUA_WRAPPER(SphereComponent, "SphereComponent", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(SphereComponent);
LUA_DEFINE_CUSTOM_COPY_TYPE(SphereComponent);
DECLARE_LUA_VECTOR_WRAPPER_ALL(SphereComponent, "SphereComponent");
DECLARE_LUA_WRAPPER(Transform, "Transform", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Transform);
LUA_DEFINE_CUSTOM_COPY_TYPE(Transform);
DECLARE_LUA_VECTOR_WRAPPER_ALL(Transform, "Transform");
DECLARE_LUA_WRAPPER(Vector4, "Vector4", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Vector4);
DECLARE_LUA_WRAPPER(Vector3, "Vector3", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Vector3);
LUA_DEFINE_CUSTOM_COPY_TYPE(Vector3);
DECLARE_LUA_VECTOR_WRAPPER_ALL(Vector3, "Vector3");
DECLARE_LUA_WRAPPER(World, "World", false);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(World);
DECLARE_LUA_WRAPPER(WorldState, "WorldState", true);
LUA_DEFINE_CUSTOM_OBJECT_TYPE(WorldState);
#include "./generated/InputData_generated.h"
#include "./generated/Camera_generated.h"
#include "./generated/FirstPersonCamera_generated.h"
#include "./generated/KeyboardComponent_generated.h"
#include "./generated/Material_generated.h"
#include "./generated/MouseComponent_generated.h"
#include "./generated/PerspectiveCamera_generated.h"
#include "./generated/RenderTarget_generated.h"
#include "./generated/StaticMeshMaterial_generated.h"
#include "./generated/StaticMeshRenderComponent_generated.h"
#include "./generated/Action_generated.h"
#include "./generated/ActionRender_generated.h"
#include "./generated/Attributed_generated.h"
#include "./generated/Collision_generated.h"
#include "./generated/CollisionComponent_generated.h"
#include "./generated/Datum_generated.h"
#include "./generated/Entity_generated.h"
#include "./generated/GameTime_generated.h"
#include "./generated/Matrix_generated.h"
#include "./generated/Quaternion_generated.h"
#include "./generated/RTTI_generated.h"
#include "./generated/Scope_generated.h"
#include "./generated/Sector_generated.h"
#include "./generated/SphereComponent_generated.h"
#include "./generated/Transform_generated.h"
#include "./generated/Vector4_generated.h"
#include "./generated/World_generated.h"
#include "./generated/WorldState_generated.h"
void LuaRegister::RegisterLua(LuaBind& bind)
{
bind;
MouseInput_generated::Lua_RegisterClass(bind);
RTTI_generated::Lua_RegisterClass(bind);
Scope_generated::Lua_RegisterClass(bind);
Attributed_generated::Lua_RegisterClass(bind);
Entity_generated::Lua_RegisterClass(bind);
Camera_generated::Lua_RegisterClass(bind);
PerspectiveCamera_generated::Lua_RegisterClass(bind);
FirstPersonCamera_generated::Lua_RegisterClass(bind);
KeyboardEntity_generated::Lua_RegisterClass(bind);
Material_generated::Lua_RegisterClass(bind);
MouseButtons_generated::Lua_RegisterClass(bind);
MouseEventType_generated::Lua_RegisterClass(bind);
MouseEntity_generated::Lua_RegisterClass(bind);
MouseEvent_generated::Lua_RegisterClass(bind);
RenderTarget_generated::Lua_RegisterClass(bind);
StaticMeshMaterial_generated::Lua_RegisterClass(bind);
Action_generated::Lua_RegisterClass(bind);
ActionRender_generated::Lua_RegisterClass(bind);
StaticMeshRenderComponent_generated::Lua_RegisterClass(bind);
Sphere_generated::Lua_RegisterClass(bind);
Ray_generated::Lua_RegisterClass(bind);
Collision_generated::Lua_RegisterClass(bind);
CollisionComponent_generated::Lua_RegisterClass(bind);
Datum_generated::Lua_RegisterClass(bind);
GameTime_generated::Lua_RegisterClass(bind);
Matrix_generated::Lua_RegisterClass(bind);
Quaternion_generated::Lua_RegisterClass(bind);
Sector_generated::Lua_RegisterClass(bind);
SphereComponent_generated::Lua_RegisterClass(bind);
Transform_generated::Lua_RegisterClass(bind);
Vector4_generated::Lua_RegisterClass(bind);
Vector3_generated::Lua_RegisterClass(bind);
World_generated::Lua_RegisterClass(bind);
WorldState_generated::Lua_RegisterClass(bind);

LuaMonitor* monitor = new LuaMonitor();
gMonitors[&bind] = monitor;
MouseInput_generated::Lua_RegisterMember(bind);
monitor->subscribers.emplace_back(new MouseInput_event_generated(bind));
Camera_generated::Lua_RegisterMember(bind);
FirstPersonCamera_generated::Lua_RegisterMember(bind);
KeyboardEntity_generated::Lua_RegisterMember(bind);
Material_generated::Lua_RegisterMember(bind);
MouseButtons_generated::Lua_RegisterMember(bind);
MouseEventType_generated::Lua_RegisterMember(bind);
MouseEntity_generated::Lua_RegisterMember(bind);
MouseEvent_generated::Lua_RegisterMember(bind);
monitor->subscribers.emplace_back(new MouseEvent_event_generated(bind));
PerspectiveCamera_generated::Lua_RegisterMember(bind);
RenderTarget_generated::Lua_RegisterMember(bind);
StaticMeshMaterial_generated::Lua_RegisterMember(bind);
StaticMeshRenderComponent_generated::Lua_RegisterMember(bind);
Action_generated::Lua_RegisterMember(bind);
ActionRender_generated::Lua_RegisterMember(bind);
Attributed_generated::Lua_RegisterMember(bind);
Sphere_generated::Lua_RegisterMember(bind);
Ray_generated::Lua_RegisterMember(bind);
Collision_generated::Lua_RegisterMember(bind);
CollisionComponent_generated::Lua_RegisterMember(bind);
Datum_generated::Lua_RegisterMember(bind);
Entity_generated::Lua_RegisterMember(bind);
GameTime_generated::Lua_RegisterMember(bind);
Matrix_generated::Lua_RegisterMember(bind);
Quaternion_generated::Lua_RegisterMember(bind);
RTTI_generated::Lua_RegisterMember(bind);
Scope_generated::Lua_RegisterMember(bind);
Sector_generated::Lua_RegisterMember(bind);
SphereComponent_generated::Lua_RegisterMember(bind);
Transform_generated::Lua_RegisterMember(bind);
Vector4_generated::Lua_RegisterMember(bind);
Vector3_generated::Lua_RegisterMember(bind);
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
Event<MouseEvent>::UnsubscribeAll();
}
}
