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

		GamePadEntity* GetGamePad() const;
		void SetGamePad(GamePadEntity* gamePad);

		KeyboardEntity* GetKeyboard() const;
		void SetKeyboard(KeyboardEntity* keyboard);

		MouseEntity* GetMouse() const;
		void SetMouse(MouseEntity* mouse);

		float& MouseSensitivity();
        float& RotationRate();
        float& MovementRate();
        
		const std::vector<std::function<void()>>& PositionUpdatedCallbacks() const;
		void AddPositionUpdatedCallback(std::function<void()> callback);

		FUNCTION();
		virtual void SetPosition(float x, float y, float z) override;
		virtual void SetPosition(DirectX::FXMVECTOR position) override;
		virtual void SetPosition(const DirectX::XMFLOAT3& position) override;

		virtual void Start(WorldState&) override;
        virtual void Update(WorldState&) override;

		inline static const float DefaultMouseSensitivity{ 0.1f };
		inline static const float DefaultRotationRate{ DirectX::XMConvertToRadians(100.0f) };
		inline static const float DefaultMovementRate{ 100.0f };

	private:
		void UpdatePosition(const DirectX::XMFLOAT2& movementAmount, const DirectX::XMFLOAT2& rotationAmount, const GameEngine::GameTime& gameTime);
		void InvokePositionUpdatedCallbacks();
		
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
		std::vector<std::function<void()>> mPositionUpdatedCallbacks;
    };
}