#pragma once

#include "PerspectiveCamera.h"
#include "GamePadComponent.h"
#include <functional>
#include <vector>

namespace GameEngine
{	
	class KeyboardEntity;
	class MouseEntity;

	CLASS();
    class FirstPersonCamera : public PerspectiveCamera
    {
		RTTI_DECLARATIONS(FirstPersonCamera, PerspectiveCamera)

    public:
        explicit FirstPersonCamera(float fieldOfView = DefaultFieldOfView, float aspectRatio = DefaultAspectRatio, float nearPlaneDistance = DefaultNearPlaneDistance, float farPlaneDistance = DefaultFarPlaneDistance);
		FirstPersonCamera(const FirstPersonCamera&) = default;
		FirstPersonCamera& operator=(const FirstPersonCamera&) = default;
		FirstPersonCamera(FirstPersonCamera&&) = default;
		FirstPersonCamera& operator=(FirstPersonCamera&&) = default;
		virtual ~FirstPersonCamera() = default;

		float& MouseSensitivity();
        float& RotationRate();
        float& MovementRate();

		virtual void Start(WorldState&) override;
        virtual void Update(WorldState&) override;

		inline static const float DefaultMouseSensitivity{ 0.1f };
		inline static const float DefaultRotationRate{ DirectX::XMConvertToRadians(100.0f) };
		inline static const float DefaultMovementRate{ 100.0f };

	private:
		void UpdatePosition(const Vector3& movementAmount, const Vector3& rotationAmount, const GameEngine::GameTime& gameTime);
		
		inline bool IsGamePadConnected(DirectX::GamePad::State& gamePadState)
		{
			if (mGamePad != nullptr)
			{
				gamePadState = mGamePad->CurrentState();
				return gamePadState.IsConnected();
			}

			return false;
		}

    protected:
		GamePadEntity* mGamePad{ nullptr };
		KeyboardEntity* mKeyboard{ nullptr };
		MouseEntity* mMouse{ nullptr };
		float mMouseSensitivity{ DefaultMouseSensitivity };
		float mRotationRate{ DefaultRotationRate };
        float mMovementRate{ DefaultMovementRate };
    };
}