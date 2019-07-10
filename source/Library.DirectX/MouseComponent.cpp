#include "pch.h"
#include "MouseComponent.h"
#include "Game.h"

using namespace std;
using namespace DirectX;

namespace GameEngine
{
	RTTI_DEFINITIONS(MouseComponent)

	unique_ptr<Mouse> MouseComponent::sMouse(new DirectX::Mouse);

	Mouse* MouseComponent::Mouse()
	{
		return sMouse.get();
	}

	MouseComponent::MouseComponent(Game& game, MouseModes mode) :
		GameComponent(game)
	{
		auto getWindow = mGame->GetWindowCallback();
		sMouse->SetWindow(reinterpret_cast<HWND>(getWindow()));
		sMouse->SetMode(static_cast<Mouse::Mode>(mode));
	}

	const Mouse::State& MouseComponent::CurrentState() const
	{
		return mCurrentState;
	}

	const Mouse::State& MouseComponent::LastState() const
	{
		return mLastState;
	}

	void MouseComponent::Initialize()
	{
		mCurrentState = sMouse->GetState();
		mLastState = mCurrentState;
	}

	void MouseComponent::Update(const GameTime&)
	{
		mLastState = mCurrentState;
		mCurrentState = sMouse->GetState();
	}

	void MouseComponent::SetWindow(HWND window)
	{
		sMouse->SetWindow(window);
	}

	int MouseComponent::X() const
	{
		return mCurrentState.x;
	}

	int MouseComponent::Y() const
	{
		return mCurrentState.y;
	}

	int MouseComponent::Wheel() const
	{
		return mCurrentState.scrollWheelValue;
	}

	bool MouseComponent::IsButtonUp(MouseButtons button) const
	{
		return GetButtonState(mCurrentState, button) == false;
	}

	bool MouseComponent::IsButtonDown(MouseButtons button) const
	{
		return GetButtonState(mCurrentState, button);
	}

	bool MouseComponent::WasButtonUp(MouseButtons button) const
	{
		return GetButtonState(mLastState, button) == false;
	}

	bool MouseComponent::WasButtonDown(MouseButtons button) const
	{
		return GetButtonState(mLastState, button);
	}

	bool MouseComponent::WasButtonPressedThisFrame(MouseButtons button) const
	{
		return (IsButtonDown(button) && WasButtonUp(button));
	}

	bool MouseComponent::WasButtonReleasedThisFrame(MouseButtons button) const
	{
		return (IsButtonUp(button) && WasButtonDown(button));
	}

	bool MouseComponent::IsButtonHeldDown(MouseButtons button) const
	{
		return (IsButtonDown(button) && WasButtonDown(button));
	}

	MouseModes MouseComponent::Mode() const
	{
		auto state = sMouse->GetState();
		return static_cast<MouseModes>(state.positionMode);
	}

	void MouseComponent::SetMode(MouseModes mode)
	{
		sMouse->SetMode(static_cast<Mouse::Mode>(mode));
	}

	bool MouseComponent::GetButtonState(const Mouse::State& state, MouseButtons button) const
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