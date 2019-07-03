#pragma once
#include "Macro.h"
#include "Event.h"

namespace GameEngine
{
	enum class MouseInputType
	{
		Press = 0,
		Release = 1,
		Click = 2,
		Hold = 3
	};
	
	CLASS(EventMessage)
	class MouseInput
	{
	public:
		CONSTRUCTOR();
		MouseInput() = default;

		PROPERTY();
		int Button = 0;

		PROPERTY();
		int Type = 0;
	};
}