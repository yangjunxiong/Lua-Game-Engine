#include "pch.h"
#include "FirstPersonCamera.h"
#include "MouseComponent.h"
#include "KeyboardComponent.h"
#include "Game.h"
#include "VectorHelper.h"

using namespace std;
using namespace DirectX;

namespace GameEngine
{
    RTTI_DEFINITIONS(FirstPersonCamera)

    FirstPersonCamera::FirstPersonCamera(float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance) :
		PerspectiveCamera(fieldOfView, aspectRatio, nearPlaneDistance, farPlaneDistance)
    {
    }

	GamePadEntity* FirstPersonCamera::GetGamePad() const
	{
		return mGamePad;
	}

	void FirstPersonCamera::SetGamePad(GamePadEntity* gamePad)
	{
		mGamePad = gamePad;
	}

	KeyboardEntity* FirstPersonCamera::GetKeyboard() const
	{
		return mKeyboard;
	}

	void FirstPersonCamera::SetKeyboard(KeyboardEntity* keyboard)
	{
		mKeyboard = keyboard;
	}

	MouseEntity* FirstPersonCamera::GetMouse() const
	{
		return mMouse;
	}

	void FirstPersonCamera::SetMouse(MouseEntity* mouse)
	{
		mMouse = mouse;
	}

	float& FirstPersonCamera::MouseSensitivity()
	{
		return mMouseSensitivity;
	}

    float& FirstPersonCamera::RotationRate()
    {
        return mRotationRate;
    }

    float& FirstPersonCamera::MovementRate()
    {
        return mMovementRate;
    }

	const vector<function<void()>>& FirstPersonCamera::PositionUpdatedCallbacks() const
	{
		return mPositionUpdatedCallbacks;
	}

	void FirstPersonCamera::AddPositionUpdatedCallback(function<void()> callback)
	{
		mPositionUpdatedCallbacks.push_back(callback);
	}

	void FirstPersonCamera::SetPosition(float x, float y, float z)
	{
		Camera::SetPosition(x, y, z);
		InvokePositionUpdatedCallbacks();
	}

	void FirstPersonCamera::SetPosition(FXMVECTOR position)
	{
		Camera::SetPosition(position);
		InvokePositionUpdatedCallbacks();
	}

	void FirstPersonCamera::SetPosition(const XMFLOAT3& position)
	{
		Camera::SetPosition(position);
		InvokePositionUpdatedCallbacks();
	}

	void FirstPersonCamera::Start(WorldState& state)
	{
		mGamePad = reinterpret_cast<GamePadEntity*>(mGame->Services().GetService(GamePadEntity::TypeIdClass()));
		mKeyboard = (KeyboardEntity*)mGame->Services().GetService(KeyboardEntity::TypeIdClass());
		mMouse = (MouseEntity*)mGame->Services().GetService(MouseEntity::TypeIdClass());

		Camera::Start(state);
	}

    void FirstPersonCamera::Update(WorldState& state)
    {
		auto& gameTime = state.GetGameTime();
		GamePad::State gamePadState;
		if (IsGamePadConnected(gamePadState))
		{
			XMFLOAT2 movementAmount(gamePadState.thumbSticks.leftX, gamePadState.thumbSticks.leftY);
			XMFLOAT2 rotationAmount(-gamePadState.thumbSticks.rightX, gamePadState.thumbSticks.rightY);

			if (movementAmount.x != 0 || movementAmount.y != 0 || rotationAmount.x != 0 || rotationAmount.y != 0)
			{
				UpdatePosition(movementAmount, rotationAmount, gameTime);
			}
		}
		else
		{
			bool positionChanged = false;
			XMFLOAT2 movementAmount = Vector2Helper::Zero;
			if (mKeyboard != nullptr)
			{				
				if (mKeyboard->IsKeyDown(Keys::W))
				{
					movementAmount.y = 1.0f;
					positionChanged = true;
				}
				if (mKeyboard->IsKeyDown(Keys::S))
				{
					movementAmount.y = -1.0f;
					positionChanged = true;
				}
				if (mKeyboard->IsKeyDown(Keys::A))
				{
					movementAmount.x = -1.0f;
					positionChanged = true;
				}
				if (mKeyboard->IsKeyDown(Keys::D))
				{
					movementAmount.x = 1.0f;
					positionChanged = true;
				}
			}

			XMFLOAT2 rotationAmount = Vector2Helper::Zero;
			if (mMouse != nullptr && mMouse->Mode() == MouseModes::Relative)
			{				
				if (mMouse->IsButtonHeldDown(MouseButtons::Left))
				{
					rotationAmount.x = -mMouse->X() * mMouseSensitivity;
					rotationAmount.y = -mMouse->Y() * mMouseSensitivity;
					positionChanged = true;
				}
			}

			if (positionChanged)
			{
				UpdatePosition(movementAmount, rotationAmount, gameTime);
			}
		}

        Camera::Update(state);
    }

	void FirstPersonCamera::UpdatePosition(const XMFLOAT2& movementAmount, const XMFLOAT2& rotationAmount, const GameEngine::GameTime& gameTime)
	{
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();
		XMVECTOR rotationVector = XMLoadFloat2(&rotationAmount) * mRotationRate * elapsedTime;
		XMVECTOR right = XMLoadFloat3(&mRight);

		XMMATRIX pitchMatrix = XMMatrixRotationAxis(right, XMVectorGetY(rotationVector));
		XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotationVector));

		ApplyRotation(XMMatrixMultiply(pitchMatrix, yawMatrix));

		XMVECTOR position = XMLoadFloat3(&mPosition);
		XMVECTOR movement = XMLoadFloat2(&movementAmount) * mMovementRate * elapsedTime;

		XMVECTOR strafe = right * XMVectorGetX(movement);
		position += strafe;

		XMVECTOR forward = XMLoadFloat3(&mDirection) * XMVectorGetY(movement);
		position += forward;

		XMStoreFloat3(&mPosition, position);

		InvokePositionUpdatedCallbacks();
		mViewMatrixDataDirty = true;
	}

	inline void FirstPersonCamera::InvokePositionUpdatedCallbacks()
	{
		for (auto& callback : mPositionUpdatedCallbacks)
		{
			callback();
		}
	}
}