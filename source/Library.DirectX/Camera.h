#pragma once

#include "Entity.h"
#include <DirectXMath.h>
#include <vector>
#include <functional>

namespace GameEngine
{
	class GameTime;
	class Game;

	CLASS(NoLuaAuthority);
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

		FUNCTION();
		float NearPlaneDistance() const;

		FUNCTION();
		void SetNearPlaneDistance(float distance);

		FUNCTION();
		float FarPlaneDistance() const;

		FUNCTION();
		void SetFarPlaneDistance(float distance);

		FUNCTION();
		Matrix ViewMatrix() const;

		FUNCTION();
		Matrix ProjectionMatrix() const;

		FUNCTION();
		Matrix ViewProjectionMatrix() const;

		FUNCTION();
		Vector3 Unproject(const Vector3& screenPosition) const;

		const std::vector<std::function<void()>>& ViewMatrixUpdatedCallbacks() const;
		void AddViewMatrixUpdatedCallback(std::function<void()> callback);

		const std::vector<std::function<void()>>& ProjectionMatrixUpdatedCallbacks() const;
		void AddProjectionMatrixUpdatedCallback(std::function<void()> callback);

		virtual void Reset();
		virtual void Start(WorldState& state) override;
		virtual void Update(WorldState& state) override;
		virtual void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix() = 0;

		PROPERTY();
		inline static const float DefaultNearPlaneDistance{ 0.01f };

		PROPERTY();
		inline static const float DefaultFarPlaneDistance{ 10000.0f };

	protected:
		Game* mGame = nullptr;
		float mNearPlaneDistance;
		float mFarPlaneDistance;

		Matrix mViewMatrix;
		Matrix mProjectionMatrix;

		bool mViewMatrixDataDirty{ true };
		bool mProjectionMatrixDataDirty{ true };

		std::vector<std::function<void()>> mViewMatrixUpdatedCallbacks;
		std::vector<std::function<void()>> mProjectionMatrixUpdatedCallbacks;
	};
}