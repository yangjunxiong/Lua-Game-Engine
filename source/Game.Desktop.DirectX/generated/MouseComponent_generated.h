// Genereated binding class
// Don't change this file manually

#include "MouseComponent.h"
#include "LuaBind.h"

namespace GameEngine
{
class MouseButtons_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<MouseButtons>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetEnumValue<MouseButtons>("Left", static_cast<int>(MouseButtons::Left));
		bind.SetEnumValue<MouseButtons>("Right", static_cast<int>(MouseButtons::Right));
		bind.SetEnumValue<MouseButtons>("Middle", static_cast<int>(MouseButtons::Middle));
		bind.SetEnumValue<MouseButtons>("X1", static_cast<int>(MouseButtons::X1));
		bind.SetEnumValue<MouseButtons>("X2", static_cast<int>(MouseButtons::X2));
		bind.SetEnumValue<MouseButtons>("Begin", static_cast<int>(MouseButtons::Begin));
		bind.SetEnumValue<MouseButtons>("End", static_cast<int>(MouseButtons::End));
	};
};
class MouseEventType_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<MouseEventType>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetEnumValue<MouseEventType>("Press", static_cast<int>(MouseEventType::Press));
		bind.SetEnumValue<MouseEventType>("Release", static_cast<int>(MouseEventType::Release));
		bind.SetEnumValue<MouseEventType>("Click", static_cast<int>(MouseEventType::Click));
		bind.SetEnumValue<MouseEventType>("Wheel", static_cast<int>(MouseEventType::Wheel));
		bind.SetEnumValue<MouseEventType>("Begin", static_cast<int>(MouseEventType::Begin));
		bind.SetEnumValue<MouseEventType>("End", static_cast<int>(MouseEventType::End));
	};
};
class MouseEntity_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<MouseEntity, Entity>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetFunction<MouseEntity, Vector3>("PixelPosition", &MouseEntity::PixelPosition);
		bind.SetFunction<MouseEntity, Vector3>("ViewportPosition", &MouseEntity::ViewportPosition);
		bind.SetFunction<MouseEntity, Vector3, float>("WorldPosition", &MouseEntity::WorldPosition);
		bind.SetFunction<MouseEntity, int>("Wheel", &MouseEntity::Wheel);
	};
};
class MouseEvent_event_generated final : public IEventSubscriber
{
public:
LuaBind* bind;
MouseEvent_event_generated(LuaBind& b)
{
bind = &b;
Event<MouseEvent>::Subscribe(*this);
}
~MouseEvent_event_generated()
{
Event<MouseEvent>::Unsubscribe(*this);
}
virtual void Notify(const BaseEvent& event) override
{
const MouseEvent* msg = &static_cast<const Event<MouseEvent>&>(event).Message();
bind->CallFunctionNoReturn("EventNotify", msg, "MouseEvent");
}
};

class MouseEvent_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<MouseEvent>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetProperty("Type", &MouseEvent::Type);
		bind.SetProperty("Button", &MouseEvent::Button);
	};
};
}
