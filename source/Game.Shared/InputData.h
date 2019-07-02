#pragma once
#include "Event.h"

namespace GameEngine
{
	CLASS(EventMessage)
	class MouseInput
	{
	public:
		CONSTRUCTOR();
		MouseInput() = default;

		PROPERTY();
		int Button = 0;

		PROPERTY();
		bool Press = true;
	};
}