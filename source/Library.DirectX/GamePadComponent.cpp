#include "pch.h"
#include "GamePadComponent.h"

using namespace std;
using namespace DirectX;

namespace GameEngine
{
	RTTI_DEFINITIONS(GamePadEntity)

	unique_ptr<GamePad> GamePadEntity::sGamePad(new DirectX::GamePad);

	GamePad* GamePadEntity::GamePad()
	{
		return sGamePad.get();
	}

	GamePadEntity::GamePadEntity(int player) :
		Entity(GamePadEntity::TypeIdClass()), mPlayer(player)
	{
	}

	int GamePadEntity::Player() const
	{
		return mPlayer;
	}

	const GamePad::State& GamePadEntity::CurrentState() const
	{
		return mCurrentState;
	}

	const GamePad::State& GamePadEntity::LastState() const
	{
		return mLastState;
	}

	void GamePadEntity::Start(WorldState&)
	{
		mCurrentState = sGamePad->GetState(mPlayer);
		mLastState = mCurrentState;
	}

	void GamePadEntity::Update(WorldState&)
	{
		mLastState = mCurrentState;
		mCurrentState = sGamePad->GetState(mPlayer);
	}

	bool GamePadEntity::IsButtonUp(GamePadButtons button) const
	{
		return GetButtonState(mCurrentState, button) == false;
	}

	bool GamePadEntity::IsButtonDown(GamePadButtons button) const
	{
		return GetButtonState(mCurrentState, button);
	}

	bool GamePadEntity::WasButtonUp(GamePadButtons button) const
	{
		return GetButtonState(mLastState, button) == false;
	}

	bool GamePadEntity::WasButtonDown(GamePadButtons button) const
	{
		return GetButtonState(mLastState, button);
	}

	bool GamePadEntity::WasButtonPressedThisFrame(GamePadButtons button) const
	{
		return (IsButtonDown(button) && WasButtonUp(button));
	}

	bool GamePadEntity::WasButtonReleasedThisFrame(GamePadButtons button) const
	{
		return (IsButtonUp(button) && WasButtonDown(button));
	}

	bool GamePadEntity::IsButtonHeldDown(GamePadButtons button) const
	{
		return (IsButtonDown(button) && WasButtonDown(button));
	}

	bool GamePadEntity::GetButtonState(const GamePad::State& state, GamePadButtons button) const
	{
		switch (button)
		{
			case GamePadButtons::A:
				return state.buttons.a;

			case GamePadButtons::B:
				return state.buttons.b;

			case GamePadButtons::X:
				return state.buttons.x;

			case GamePadButtons::Y:
				return state.buttons.y;

			case GamePadButtons::LeftStick:
				return state.buttons.leftStick;

			case GamePadButtons::RightStick:
				return state.buttons.rightStick;

			case GamePadButtons::LeftShoulder:
				return state.buttons.leftShoulder;

			case GamePadButtons::RightShoulder:
				return state.buttons.rightShoulder;

			case GamePadButtons::Back:
				return state.buttons.back;

			case GamePadButtons::Start:
				return state.buttons.start;

			case GamePadButtons::DPadUp:
				return state.dpad.up;

			case GamePadButtons::DPadDown:
				return state.dpad.down;

			case GamePadButtons::DPadLeft:
				return state.dpad.left;

			case GamePadButtons::DPadRight:
				return state.dpad.right;

			default:
				throw exception("Invalid GamePadButtons.");
		}
	}
}