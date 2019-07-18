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
			Vector3 movementAmount(gamePadState.thumbSticks.leftX, gamePadState.thumbSticks.leftY, 0.f);
			Vector3 rotationAmount(-gamePadState.thumbSticks.rightX, gamePadState.thumbSticks.rightY, 0.f);

			if (movementAmount.x != 0 || movementAmount.y != 0 || rotationAmount.z != 0 || rotationAmount.y != 0)
			{
				UpdatePosition(movementAmount, rotationAmount, gameTime);
			}
		}
		else
		{
			bool positionChanged = false;
			Vector3 movementAmount;
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

			Vector3 rotationAmount;
			//if (mMouse != nullptr && mMouse->Mode() == MouseModes::Relative)
			//{				
			//	if (mMouse->IsButtonHeldDown(MouseButtons::Left))
			//	{
			//		rotationAmount = mMouse->MousePosition() * -mMouseSensitivity;
			//		positionChanged = true;
			//	}
			//}

			if (positionChanged)
			{
				UpdatePosition(movementAmount, rotationAmount, gameTime);
			}
		}

        Camera::Update(state);
    }

	void FirstPersonCamera::UpdatePosition(const Vector3& movementAmount, const Vector3& rotationAmount, const GameEngine::GameTime& gameTime)
	{
		float elapsedTime = gameTime.ElapsedGameTimeSeconds().count();
		Vector3 rotationVector = rotationAmount * mRotationRate * elapsedTime;
		Vector3 translationVector = movementAmount * mMovementRate * elapsedTime;

		// Update rotation
		Quaternion deltaRotation = Quaternion::FromEulerAngles(Vector3(rotationVector.y, rotationVector.x, 0.f));
		Quaternion newRotation = Quaternion::AddRotation(mTransform.GetWorldRotation(), deltaRotation);
		mTransform.SetWorldRotation(newRotation);

		// Update position
		Vector3 deltaTranslation = mTransform.Forward() * translationVector.y + mTransform.Right() * translationVector.x;
		mTransform.SetWorldPosition(mTransform.GetWorldPosition() + deltaTranslation);

		mViewMatrixDataDirty = true;
	}
}