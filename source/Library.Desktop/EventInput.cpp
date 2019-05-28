#include "pch.h"
#include "EventInput.h"
#include <Windows.h>
#include <Xinput.h>

namespace GameEngine
{
	RTTI_DEFINITIONS(EventInput)

	EventInput::EventInput(int player, InputManager::Inputs input) :
		Attributed(EventInput::TypeIdClass()), playerID(player), mInputs(input)
	{
	}


	EventInput::~EventInput()
	{
	}

	const Vector<Attributed::Signature> EventInput::Signatures()
	{
		return Vector<Attributed::Signature>({
			Signature("PlayerID", Datum::DatumType::String, 1, offsetof(EventInput, playerID)),
			Signature("Inputs", Datum::DatumType::Pointer, 1, offsetof(EventInput, mInputs))
			});
	}

	const InputManager::Inputs & EventInput::GetInputs() const
	{
		return mInputs;
	}
	const int EventInput::GetPlayerID() const
	{
		return playerID;
	}
}