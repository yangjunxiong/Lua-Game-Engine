#pragma once
#include "Macro.h"
#include <map>

#define MOUSE_CLICK_THRESHOLD 0.2f

namespace GameEngine
{
	class World;

	class InputManager final
	{
	public:
		InputManager();
		void Update(GLFWwindow* window, World* world);

	private:
		inline void UpdateMouse(GLFWwindow* window, World* world);
		inline void SendMouseEvent(World* world, int button, int type) const;

		std::vector<float> mMouseButtonState;
		static inline const int MOUSE_BUTTON_COUNT = 3;
	};
}