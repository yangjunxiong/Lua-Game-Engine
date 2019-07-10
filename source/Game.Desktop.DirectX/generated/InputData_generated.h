// Genereated binding class
// Don't change this file manually

#include "InputData.h"
#include "LuaBind.h"

namespace GameEngine
{
class MouseInput_event_generated final : public IEventSubscriber
{
public:
LuaBind* bind;
MouseInput_event_generated(LuaBind& b)
{
bind = &b;
Event<MouseInput>::Subscribe(*this);
}
~MouseInput_event_generated()
{
Event<MouseInput>::Unsubscribe(*this);
}
virtual void Notify(const BaseEvent& event) override
{
const MouseInput* msg = &static_cast<const Event<MouseInput>&>(event).Message();
bind->CallFunctionNoReturn("EventNotify", msg, "MouseInput");
}
};

class MouseInput_generated final
{
private:
	using LuaBind = GameEngine::Lua::LuaBind;
public:
	static void Lua_RegisterClass(LuaBind& bind)
	{
		bind.RegisterType<MouseInput>();
	};
	static void Lua_RegisterMember(LuaBind& bind)
	{
		bind;
		bind.SetConstructor<MouseInput>();
		bind.SetProperty("Button", &MouseInput::Button);
		bind.SetProperty("Type", &MouseInput::Type);
	};
};
}
