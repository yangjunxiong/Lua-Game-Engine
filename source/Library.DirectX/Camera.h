#pragma once

#include "Entity.h"
#include <DirectXMath.h>
#include <vector>
#include <functional>

namespace GameEngine
{
	class GameTime;
	class Game;

	CLASS();
	class Camera : public Entity
	{
		RTTI_DECLARATIONS(Camera, Entity);

	public:
		explicit Camera(float nearPlaneDistance = DefaultNearPlaneDistance, float farPlaneDistance = DefaultFarPlaneDistance);
		Camera(const Camera&) = default;
		Camera& operator=(const Camera&) = default;
		Camera(Camera&&) = default;
		Camera& operator=(Camera&&) = default;
		virtual ~Camera() = default;		

		const DirectX::XMFLOAT3& Position() const;
		const DirectX::XMFLOAT3& Direction() const;
		const DirectX::XMFLOAT3& Up() const;
		const DirectX::XMFLOAT3& Right() const;

		DirectX::XMVECTOR PositionVector() const;
		DirectX::XMVECTOR DirectionVector() const;
		DirectX::XMVECTOR UpVector() const;
		DirectX::XMVECTOR RightVector() const;

		float NearPlaneDistance() const;
		void SetNearPlaneDistance(float distance);

		float FarPlaneDistance() const;
		void SetFarPlaneDistance(float distance);

		DirectX::XMMATRIX ViewMatrix() const;
		DirectX::XMMATRIX ProjectionMatrix() const;
		DirectX::XMMATRIX ViewProjectionMatrix() const;

		const std::vector<std::function<void()>>& ViewMatrixUpdatedCallbacks() const;
		void AddViewMatrixUpdatedCallback(std::function<void()> callback);

		const std::vector<std::function<void()>>& ProjectionMatrixUpdatedCallbacks() const;
		void AddProjectionMatrixUpdatedCallback(std::function<void()> callback);

		virtual void SetPosition(float x, float y, float z);
		virtual void SetPosition(DirectX::FXMVECTOR position);
		virtual void SetPosition(const DirectX::XMFLOAT3& position);

		virtual void Reset();
		virtual void Start(WorldState& state) override;
		virtual void Update(WorldState& state) override;
		virtual void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix() = 0;
		virtual void ApplyRotation(DirectX::CXMMATRIX transform);
		virtual void ApplyRotation(const DirectX::XMFLOAT4X4& transform);

		inline static const float DefaultNearPlaneDistance{ 0.01f };
		inline static const float DefaultFarPlaneDistance{ 10000.0f };

	protected:
		Game* mGame = nullptr;
		float mNearPlaneDistance;
		float mFarPlaneDistance;

		DirectX::XMFLOAT3 mPosition;
		DirectX::XMFLOAT3 mDirection;
		DirectX::XMFLOAT3 mUp;
		DirectX::XMFLOAT3 mRight;

		DirectX::XMFLOAT4X4 mViewMatrix;
		DirectX::XMFLOAT4X4 mProjectionMatrix;

		bool mViewMatrixDataDirty{ true };
		bool mProjectionMatrixDataDirty{ true };

		std::vector<std::function<void()>> mViewMatrixUpdatedCallbacks;
		std::vector<std::function<void()>> mProjectionMatrixUpdatedCallbacks;
	};
}