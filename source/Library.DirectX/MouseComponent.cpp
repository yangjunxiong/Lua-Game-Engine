#include "pch.h"
#include "MouseComponent.h"
#include "Game.h"

using namespace std;
using namespace DirectX;

namespace GameEngine
{
	RTTI_DEFINITIONS(MouseEntity)

	unique_ptr<Mouse> MouseEntity::sMouse(new DirectX::Mouse);

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
		mCurrentState = sMouse->GetState();
		mLastState = mCurrentState;
	}

	void MouseEntity::Update(WorldState&)
	{
		mLastState = mCurrentState;
		mCurrentState = sMouse->GetState();
	}

	void MouseEntity::SetWindow(HWND window)
	{
		sMouse->SetWindow(window);
	}

	int MouseEntity::X() const
	{
		return mCurrentState.x;
	}

	int MouseEntity::Y() const
	{
		return mCurrentState.y;
	}

	int MouseEntity::Wheel() const
	{
		return mCurrentState.scrollWheelValue;
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
		switch (button)
		{
			case GameEngine::MouseButtons::Left:
				return state.leftButton;

			case GameEngine::MouseButtons::Right:
				return state.rightButton;

			case GameEngine::MouseButtons::Middle:
				return state.middleButton;				

			case GameEngine::MouseButtons::X1:
				return state.xButton1;

			case GameEngine::MouseButtons::X2:
				return state.xButton2;

			default:
				throw exception("Invalid MouseButtons.");
		}
	}
}