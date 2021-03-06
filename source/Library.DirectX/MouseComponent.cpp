#include "pch.h"
#include "MouseComponent.h"
#include "Game.h"
#include "Event.h"
#include "LuaBind.h"
#include "Camera.h"
#include "Entity.h"
#include "Sector.h"
#include "CollisionComponent.h"
#include <imgui.h>

using namespace std;
using namespace DirectX;
using namespace GameEngine::Lua;

namespace GameEngine
{
	RTTI_DEFINITIONS(MouseEntity)

	unique_ptr<Mouse> MouseEntity::sMouse(new DirectX::Mouse);
	const std::vector<bool(*)(const DirectX::Mouse::State&)> MouseEntity::sButtonGetters =
	{
		[](const DirectX::Mouse::State& state) { return state.leftButton; },
		[](const DirectX::Mouse::State& state) { return state.rightButton; },
		[](const DirectX::Mouse::State& state) { return state.middleButton; },
		[](const DirectX::Mouse::State& state) { return state.xButton1; },
		[](const DirectX::Mouse::State& state) { return state.xButton2; },
	};

	Mouse* MouseEntity::Mouse()
	{
		return sMouse.get();
	}

	MouseEntity::MouseEntity(MouseModes mode) :
		Entity(MouseEntity::TypeIdClass())
	{
		mGame = Game::GetInstance();
		auto getWindow = mGame->GetWindowCallback();
		sMouse->SetWindow(reinterpret_cast<HWND>(getWindow()));
		sMouse->SetMode(static_cast<Mouse::Mode>(mode));
		sMouse->SetVisible(true);
		ShowCursor(true);
	}

	const Mouse::State& MouseEntity::CurrentState() const
	{
		return mCurrentState;
	}

	const Mouse::State& MouseEntity::LastState() const
	{
		return mLastState;
	}

	void MouseEntity::Start(WorldState&)
	{
		mCamera = static_cast<Camera*>(mGame->Services().GetService(Camera::TypeIdClass()));
		mCurrentState = sMouse->GetState();
		mLastState = mCurrentState;

		// Create mouse time map
		assert(static_cast<int>(MouseButtons::Begin) == 0);
		for (int i = static_cast<int>(MouseButtons::Begin); i <= static_cast<int>(MouseButtons::End); ++i)
		{
			mMousePressTime.emplace_back(0.f);
		}
	}

	void MouseEntity::Update(WorldState& state)
	{
		mLastState = mCurrentState;
		mCurrentState = sMouse->GetState();
		UpdateMouseEvent(state);
	}

	void MouseEntity::SetWindow(HWND window)
	{
		sMouse->SetWindow(window);
	}

	Vector3 MouseEntity::PixelPosition() const
	{
		return Vector3(static_cast<float>(mCurrentState.x), static_cast<float>(mCurrentState.y), 0.f);
	}

	Vector3 MouseEntity::ViewportPosition() const
	{
		SIZE viewportSize = mGame->RenderTargetSize();
		float x = (static_cast<float>(mCurrentState.x) / static_cast<float>(viewportSize.cx)) * 2.f - 1.f;
		float y = (static_cast<float>(mCurrentState.y) / static_cast<float>(viewportSize.cy)) * 2.f - 1.f;
		return Vector3(x, y, 0.f);
	}

	Vector3 MouseEntity::WorldPosition(float z) const
	{
		return mCamera->Unproject(Vector3(static_cast<float>(mCurrentState.x), static_cast<float>(mCurrentState.y), z));
	}

	int MouseEntity::Wheel() const
	{
		return mCurrentState.scrollWheelValue;
	}

	int MouseEntity::WheelDelta() const
	{
		return mCurrentState.scrollWheelValue - mLastState.scrollWheelValue;
	}

	const std::vector<CollisionComponent*>& MouseEntity::RayHitComponents() const
	{
		return mRayHitComponents;
	}

	bool MouseEntity::IsButtonUp(MouseButtons button) const
	{
		return GetButtonState(mCurrentState, button) == false;
	}

	bool MouseEntity::IsButtonDown(MouseButtons button) const
	{
		return GetButtonState(mCurrentState, button);
	}

	bool MouseEntity::WasButtonUp(MouseButtons button) const
	{
		return GetButtonState(mLastState, button) == false;
	}

	bool MouseEntity::WasButtonDown(MouseButtons button) const
	{
		return GetButtonState(mLastState, button);
	}

	bool MouseEntity::WasButtonPressedThisFrame(MouseButtons button) const
	{
		return (IsButtonDown(button) && WasButtonUp(button));
	}

	bool MouseEntity::WasButtonReleasedThisFrame(MouseButtons button) const
	{
		return (IsButtonUp(button) && WasButtonDown(button));
	}

	bool MouseEntity::IsButtonHeldDown(MouseButtons button) const
	{
		return (IsButtonDown(button) && WasButtonDown(button));
	}

	MouseModes MouseEntity::Mode() const
	{
		auto state = sMouse->GetState();
		return static_cast<MouseModes>(state.positionMode);
	}

	void MouseEntity::SetMode(MouseModes mode)
	{
		sMouse->SetMode(static_cast<Mouse::Mode>(mode));
	}

	bool MouseEntity::GetButtonState(const Mouse::State& state, MouseButtons button) const
	{
		return sButtonGetters[static_cast<int>(button)](state);
	}

	void MouseEntity::UpdateMouseEvent(WorldState& state)
	{
		if (ImGui::IsAnyItemHovered() || ImGui::IsAnyItemActive())
		{
			return;
		}

		float deltaTime = state.GetGameTime().ElapsedGameTimeSeconds().count();

		// Check collision objects
		mRayHitComponents = RaycastMouse();

		for (int i = static_cast<int>(MouseButtons::Begin); i <= static_cast<int>(MouseButtons::End); ++i)
		{
			auto getter = sButtonGetters[i];
			bool oldPress = getter(mLastState);
			bool newPress = getter(mCurrentState);

			if (!oldPress)
			{
				if (newPress)
				{
					mMousePressTime[i] = 0.f;
					SendMouseEvent(state, MouseEventType::Press, static_cast<MouseButtons>(i));
				}
			}
			else
			{
				mMousePressTime[i] += deltaTime;
				if (!newPress)
				{
					SendMouseEvent(state, MouseEventType::Release, static_cast<MouseButtons>(i));
					if (mMousePressTime[i] <= sClickThreshold)
					{
						SendMouseEvent(state, MouseEventType::Click, static_cast<MouseButtons>(i));
					}
				}
			}
		}
	}

	void MouseEntity::SendMouseEvent(WorldState& state, MouseEventType type, MouseButtons button)
	{
		MouseEvent message;
		message.Type = type;
		message.Button = button;
		auto event = make_shared<Event<MouseEvent>>(message);
		state.GetWorld()->EnqueueEvent(event);
	}

	std::vector<class CollisionComponent*> MouseEntity::RaycastMouse()
	{
		std::vector<CollisionComponent*> result;
		Vector3 pixelPosition = PixelPosition();
		Vector3 start = mCamera->Unproject(pixelPosition);
		Vector3 end = mCamera->Unproject(Vector3(pixelPosition.x, pixelPosition.y, 1.f));
		Ray ray(start, end);

		Datum& sectors = mGame->GetWorld()->Sectors();
		for (size_t s = 0; s < sectors.Size(); ++s)
		{
			Sector& sector = static_cast<Sector&>(sectors.AsTable(s));
			if (sector.IsActive())
			{
				Datum& entities = sector.Entities();
				for (size_t i = 0; i < entities.Size(); ++i)
				{
					Entity& entity = static_cast<Entity&>(entities.AsTable(i));
					if (entity.IsActive())
					{
						Datum& actions = entity.Actions();
						for (size_t j = 0; j < actions.Size(); ++j)
						{
							Action& action = static_cast<Action&>(actions.AsTable(j));
							if (action.Is(CollisionComponent::TypeIdClass()))
							{
								if (static_cast<CollisionComponent&>(action).IntersectsRay(ray))
								{
									result.emplace_back(static_cast<CollisionComponent*>(&action));
								}
							}
						}
					}
				}
			}
		}

		return result;
	}
}