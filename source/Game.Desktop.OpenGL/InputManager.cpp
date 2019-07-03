#include "pch.h"
#include "InputManager.h"
#include "Event.h"
#include "World.h"
#include "GameTime.h"
#include "InputData.h"

using namespace std;
using namespace GameEngine;

InputManager::InputManager()
{
	for (size_t i = 0; i < MOUSE_BUTTON_COUNT; ++i)
	{
		mMouseButtonState.emplace_back(0.f);
	}
}

void InputManager::Update(GLFWwindow* window, World* world)
{
	UpdateMouse(window, world);
}

void InputManager::UpdateMouse(GLFWwindow* window, World* world)
{
	// Update pressing state
	float gameTime = world->GetWorldState()->GetGameTime().ElapsedGameTimeSeconds().count();
	for (int i = 0; i < MOUSE_BUTTON_COUNT; ++i)
	{
		int buttonId = GLFW_MOUSE_BUTTON_1 + i;
		if (glfwGetMouseButton(window, buttonId) == GLFW_PRESS)
		{
			mMouseButtonState[i] = gameTime;
			SendMouseEvent(world, buttonId, (int)MouseInputType::Press);
		}
		else
		{
			SendMouseEvent(world, buttonId, (int)MouseInputType::Release);
			if (gameTime - mMouseButtonState[i] >= MOUSE_CLICK_THRESHOLD)
			{
				SendMouseEvent(world, buttonId, (int)MouseInputType::Click);
			}
			mMouseButtonState[i] = 0.f;
		}
	}

	// Broadcasting holding event
	for (int i = 0; i < MOUSE_BUTTON_COUNT; ++i)
	{
		int buttonId = GLFW_MOUSE_BUTTON_1 + i;
		if (mMouseButtonState[i] != gameTime)
		{
			SendMouseEvent(world, buttonId, (int)MouseInputType::Hold);
		}
	}
}

void InputManager::SendMouseEvent(World* world, int button, int type) const
{
	MouseInput message;
	message.Button = button;
	message.Type = type;
	auto event = make_shared<Event<MouseInput>>(message);
	world->EnqueueEvent(event);
}
