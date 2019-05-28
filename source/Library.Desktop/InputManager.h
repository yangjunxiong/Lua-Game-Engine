#pragma once
#include "Entity.h"
#include "vector.h"
#include "RTTI.h"
#include <Windows.h>
#include <Xinput.h>

namespace GameEngine
{
	class InputManager : public Entity
	{
		RTTI_DECLARATIONS(InputManager, Entity)

	public:
		/// <summary>
		/// A struct containing processed inputs from keyboard or mouse
		/// </summary>
		struct Inputs
		{
			glm::vec4 LS;
			float LSMagnitude = 0.0f;
			glm::vec4 RS;
			float RSMagnitude = 0.0f;
			float LT = 0.0f;
			float RT = 0.0f;
			bool A = false;
			bool B = false;
			bool X = false;
			bool Y = false;
			bool DUp = false;
			bool DDown = false;
			bool DLeft = false;
			bool DRight = false;
			bool Back = false;
			bool Start = false;
			bool LB = false;
			bool RB = false;
			bool Home = false;
			bool LSClick = false;
			bool RSClick = false;
		};

		InputManager();
		~InputManager();

		void Update(WorldState& worldState) override;

		/// <summary>
		/// Gets the inputs for the given player
		/// </summary>
		/// <exception cref="Out of range exception">Throws an out of range exception if player is out of bounds of the number of players</exception>
		/// <param name="player">The ID for the player to get inputs for</param>
		const Inputs& GetInputs(int player) const;

		static const Vector<Attributed::Signature> Signatures();

	private:
		bool mUsingController[2];
		XINPUT_GAMEPAD mControllers[2];
		Inputs mInputs[2];
	};

	DECLARE_FACTORY(InputManager, Scope)

#define INPUT_DEADZONE XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
#define INPUT_MAX 32767
#define GUNMAN_PLAYER_ID 0
}