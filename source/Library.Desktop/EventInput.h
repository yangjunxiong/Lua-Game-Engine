#pragma once
#include "Attributed.h"
#include "vector.h"
#include "InputManager.h"

namespace GameEngine
{
	class EventInput final : public Attributed
	{
		RTTI_DECLARATIONS(EventInput, Attributed)
	public:
		EventInput(int player, InputManager::Inputs inputs);
		~EventInput();

		/// <summary>
		/// Get the list signatures for this type
		/// </summary>
		/// <returns>The list signatures for this type</returns>
		static const Vector<Attributed::Signature> Signatures();

		const InputManager::Inputs& GetInputs() const;
		const int GetPlayerID() const;

		WorldState* m_WorldState = nullptr;

	private:
		int playerID = 0;
		InputManager::Inputs mInputs;
		
	};
}