#include "pch.h"
#include "InputManager.h"
#include "Event.h"
#include "EventInput.h"
#include "WorldState.h"
#include "World.h"

namespace GameEngine
{
	RTTI_DEFINITIONS(InputManager)

	InputManager::InputManager() :
		Entity(InputManager::TypeIdClass())
	{
	}

	InputManager::~InputManager()
	{
	}

	void InputManager::Update(WorldState& worldState)
	{
		DWORD dwResult;
		for (DWORD i = 0; i < 2U; i++)
		{
			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));

			dwResult = XInputGetState(i, &state);

			//Get left stick
			float LX = mControllers[i].sThumbLX;
			float LY = mControllers[i].sThumbLY;

			//determine how far the controller is pushed
			float magnitude = sqrt(LX*LX + LY * LY);

			if (dwResult == ERROR_SUCCESS)
			{
				mUsingController[i] = true;
				mControllers[i] = state.Gamepad;
			}
			else
			{
				mUsingController[i] = false;
				magnitude = 1.f;
				break;
			}

			//determine the direction the controller is pushed
			float normalizedLX = (magnitude == 0.0f) ? 0 : LX / magnitude;
			float normalizedLY = (magnitude == 0.0f) ? 0 : LY / magnitude;

			float normalizedMagnitude = 0;

			//check if the controller is outside a circular dead zone
			if (magnitude > INPUT_DEADZONE)
			{
				//clip the magnitude at its expected maximum value
				if (magnitude > INPUT_MAX) magnitude = INPUT_MAX;

				//adjust magnitude relative to the end of the dead zone
				magnitude -= INPUT_DEADZONE;

				//optionally normalize the magnitude with respect to its expected range
				//giving a magnitude value of 0.0 to 1.0
				normalizedMagnitude = magnitude / (INPUT_MAX - INPUT_DEADZONE);
			}
			else //if the controller is in the deadzone zero out the magnitude
			{
				magnitude = 0.0;
				normalizedMagnitude = 0.0;
				normalizedLX = 0.0f;
				normalizedLY = 0.0f;
			}

			mInputs[i].LS[0] = normalizedLX;
			mInputs[i].LS[1] = normalizedLY;
			mInputs[i].LSMagnitude = normalizedMagnitude;

			if (i == GUNMAN_PLAYER_ID)
			{
				//Get right stick
				float RX = mControllers[i].sThumbRX;
				float RY = mControllers[i].sThumbRY;

				//determine how far the controller is pushed
				magnitude = sqrt(RX*RX + RY * RY);

				//determine the direction the controller is pushed
				float normalizedRX = RX / magnitude;
				float normalizedRY = RY / magnitude;

				normalizedMagnitude = 0;

				//check if the controller is outside a circular dead zone
				if (magnitude > INPUT_DEADZONE)
				{
					//clip the magnitude at its expected maximum value
					if (magnitude > INPUT_MAX) magnitude = INPUT_MAX;

					//adjust magnitude relative to the end of the dead zone
					magnitude -= INPUT_DEADZONE;

					//optionally normalize the magnitude with respect to its expected range
					//giving a magnitude value of 0.0 to 1.0
					normalizedMagnitude = magnitude / (INPUT_MAX - INPUT_DEADZONE);
				}
				else //if the controller is in the deadzone zero out the magnitude
				{
					magnitude = 0.0;
					normalizedMagnitude = 0.0;
					normalizedRX = 0.0f;
					normalizedRY = 0.0f;
				}

				mInputs[i].RS[0] = normalizedRX;
				mInputs[i].RS[1] = normalizedRY;
				mInputs[i].RSMagnitude = normalizedMagnitude;

				//Get trigger
				float RT = mControllers[i].bRightTrigger;
				if (RT > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
				{
					RT -= XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
					RT = RT / (255 - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
				}

				mInputs[i].RT = RT;
			}

			mInputs[i].DUp = mControllers[i].wButtons & XINPUT_GAMEPAD_DPAD_UP;
			mInputs[i].DDown = mControllers[i].wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			mInputs[i].DLeft = mControllers[i].wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			mInputs[i].DRight = mControllers[i].wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
			mInputs[i].Start = mControllers[i].wButtons & XINPUT_GAMEPAD_START;
			mInputs[i].Back = mControllers[i].wButtons & XINPUT_GAMEPAD_BACK;
			mInputs[i].LB = mControllers[i].wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			mInputs[i].RB = mControllers[i].wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
			mInputs[i].A = mControllers[i].wButtons & XINPUT_GAMEPAD_A;
			mInputs[i].B = mControllers[i].wButtons & XINPUT_GAMEPAD_B;
			mInputs[i].X = mControllers[i].wButtons & XINPUT_GAMEPAD_X;
			mInputs[i].Y = mControllers[i].wButtons & XINPUT_GAMEPAD_Y;
			mInputs[i].LSClick = mControllers[i].wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			mInputs[i].RSClick = mControllers[i].wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			EventInput e(0, mInputs[i]);

			e.m_WorldState = &worldState;

			auto ptr = std::make_shared<Event<EventInput>>(Event<EventInput>(e));

			worldState.GetWorld()->EnqueueEvent(ptr);
		}

		//If not using a controller, process keyboard input
		for (int i = 0; i < 2; ++i)
		{
			if (!mUsingController[i])
			{
				if (i == GUNMAN_PLAYER_ID)
				{
					float LY = 0.0f;
					if (GetKeyState('W') & 0x8000)
					{
						LY += 1.0f;
					}
					if (GetKeyState('S') & 0x8000)
					{
						LY -= 1.0f;
					}

					float LX = 0.0f;
					if (GetKeyState('A') & 0x8000)
					{
						LX -= 1.0f;
					}
					if (GetKeyState('D') & 0x8000)
					{
						LX += 1.0f;
					}

					if (LX != 0.0f && LY != 0.0f)
					{
						LX /= 1.41421356237f;
						LY /= 1.41421356237f;
					}

					mInputs[i].LS[0] = LX;
					mInputs[i].LS[1] = LY;

					float RY = 0.0f;
					float RX = 0.0f;
					if (GetKeyState('Q') & 0x8000)
					{
						RX = 0.5f;
						RY = 0.5f;
					}

					if (GetKeyState('E') & 0x8000)
					{
						RX = -0.5f;
						RY = -0.5f;
					}

					if (RX != 0.0f && RY != 0.0f)
					{
						RX /= 1.41421356237f;
						RY /= 1.41421356237f;
					}

					//mInputs[i].RS[0] = RX;
					mInputs[i].RS[2] = RY;

					if (GetKeyState(VK_SPACE) & 0x8000)
					{
						mInputs[i].RT = 1.0f;
					}
					else
					{
						mInputs[i].RT = 0.f;
					}

					EventInput e(0, mInputs[i]);

					e.m_WorldState = &worldState;

					auto ptr = std::make_shared<Event<EventInput>>(Event<EventInput>(e));

					worldState.GetWorld()->EnqueueEvent(ptr);
				}
				else
				{
					float LY = 0.0f;
					if (GetKeyState('I') & 0x8000)
					{
						LY += 1.0f;
					}
					if (GetKeyState('K') & 0x8000)
					{
						LY -= 1.0f;
					}

					float LX = 0.0f;
					if (GetKeyState('J') & 0x8000)
					{
						LX -= 1.0f;
					}
					if (GetKeyState('L') & 0x8000)
					{
						LX += 1.0f;
					}

					if (LX != 0.0f && LY != 0.0f)
					{
						LX /= 1.41421356237f;
						LY /= 1.41421356237f;
					}

					mInputs[i].LS[0] = LX;
					mInputs[i].LS[1] = LY;

					float RY = 0.0f;
					float RX = 0.0f;
					if (GetKeyState('U') & 0x8000)
					{
						RX = 0.5f;
						RY = 0.5f;
					}

					if (GetKeyState('O') & 0x8000)
					{
						RX = -0.5f;
						RY = -0.5f;
					}

					if (RX != 0.0f && RY != 0.0f)
					{
						RX /= 1.41421356237f;
						RY /= 1.41421356237f;
					}

					//mInputs[i].RS[0] = RX;
					mInputs[i].RS[2] = RY;

					EventInput e(1, mInputs[i]);

					e.m_WorldState = &worldState;

					auto ptr = std::make_shared<Event<EventInput>>(Event<EventInput>(e));

					worldState.GetWorld()->EnqueueEvent(ptr);
				}
			}
		}
	}

	const InputManager::Inputs& InputManager::GetInputs(int player) const
	{
		if (player > 2)
		{
			return mInputs[player];
		}
		else
		{
			throw std::out_of_range::exception("Out of bounds");
		}
	}

	const Vector<Attributed::Signature> InputManager::Signatures()
	{
		return Vector<Attributed::Signature>();
	}

}