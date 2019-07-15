#include "pch.h"
#include "KeyboardComponent.h"

using namespace std;
using namespace DirectX;

namespace GameEngine
{
	RTTI_DEFINITIONS(KeyboardEntity)

	unique_ptr<Keyboard> KeyboardEntity::sKeyboard(new DirectX::Keyboard);

	Keyboard* KeyboardEntity::Keyboard()
	{
		return sKeyboard.get();
	}

	KeyboardEntity::KeyboardEntity() :
		Entity(KeyboardEntity::TypeIdClass())
	{}

	const Keyboard::State& KeyboardEntity::CurrentState() const
	{
		return mCurrentState;
	}

	const Keyboard::State& KeyboardEntity::LastState() const
	{
		return mLastState;
	}

	void KeyboardEntity::Start(WorldState&)
	{
		mCurrentState = sKeyboard->GetState();
		mLastState = mCurrentState;
	}

	void KeyboardEntity::Update(WorldState&)
	{
		mLastState = mCurrentState;
		mCurrentState = sKeyboard->GetState();
	}

	bool KeyboardEntity::IsKeyUp(Keys key) const
	{
		return mCurrentState.IsKeyUp(static_cast<Keyboard::Keys>(key));
	}

	bool KeyboardEntity::IsKeyDown(Keys key) const
	{
		return mCurrentState.IsKeyDown(static_cast<Keyboard::Keys>(key));
	}

	bool KeyboardEntity::WasKeyUp(Keys key) const
	{
		return mLastState.IsKeyUp(static_cast<Keyboard::Keys>(key));
	}

	bool KeyboardEntity::WasKeyDown(Keys key) const
	{
		return mLastState.IsKeyDown(static_cast<Keyboard::Keys>(key));
	}

	bool KeyboardEntity::WasKeyPressedThisFrame(Keys key) const
	{
		return (IsKeyDown(key) && WasKeyUp(key));
	}

	bool KeyboardEntity::WasKeyReleasedThisFrame(Keys key) const
	{
		return (IsKeyUp(key) && WasKeyDown(key));
	}

	bool KeyboardEntity::IsKeyHeldDown(Keys key) const
	{
		return (IsKeyDown(key) && WasKeyDown(key));
	}
}