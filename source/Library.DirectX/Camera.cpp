#include "pch.h"
#include "Camera.h"
#include "VectorHelper.h"
#include "Game.h"

using namespace std;
using namespace DirectX;
using namespace GameEngine;

namespace GameEngine
{
	RTTI_DEFINITIONS(Camera)

	Camera::Camera(float nearPlaneDistance, float farPlaneDistance) :
		Entity(Camera::TypeIdClass()),
		mNearPlaneDistance(nearPlaneDistance), mFarPlaneDistance(farPlaneDistance)
	{
		mGame = Game::GetInstance();
		assert(mGame != nullptr);
	}

	float Camera::NearPlaneDistance() const
	{
		return mNearPlaneDistance;
	}

	void Camera::SetNearPlaneDistance(float distance)
	{
		mNearPlaneDistance = distance;
		mProjectionMatrixDataDirty = true;
	}

	float Camera::FarPlaneDistance() const
	{
		return mFarPlaneDistance;
	}

	void Camera::SetFarPlaneDistance(float distance)
	{
		mFarPlaneDistance = distance;
		mProjectionMatrixDataDirty = true;
	}

	Matrix Camera::ViewMatrix() const
	{
		return mViewMatrix;
	}

	Matrix Camera::ProjectionMatrix() const
	{
		return mProjectionMatrix;
	}

	Matrix Camera::ViewProjectionMatrix() const
	{
		XMMATRIX viewMatrix = XMLoadFloat4x4(&mViewMatrix.RawMatrix());
		XMMATRIX projectionMatrix = XMLoadFloat4x4(&mProjectionMatrix.RawMatrix());

		return XMMatrixMultiply(viewMatrix, projectionMatrix);
	}

	Vector3 Camera::Unproject(const Vector3& screenPosition) const
	{
		return Vector3::Unproject(screenPosition, mProjectionMatrix, mViewMatrix,
			Matrix::Identity, static_cast<float>(mGame->RenderTargetSize().cx), static_cast<float>(mGame->RenderTargetSize().cy));
	}

	const vector<function<void()>>& Camera::ViewMatrixUpdatedCallbacks() const
	{
		return mViewMatrixUpdatedCallbacks;
	}

	void Camera::AddViewMatrixUpdatedCallback(function<void()> callback)
	{
		mViewMatrixUpdatedCallbacks.push_back(callback);
	}

	const vector<function<void()>>& Camera::ProjectionMatrixUpdatedCallbacks() const
	{
		return mProjectionMatrixUpdatedCallbacks;
	}

	void Camera::AddProjectionMatrixUpdatedCallback(function<void()> callback)
	{
		mProjectionMatrixUpdatedCallbacks.push_back(callback);
	}

	void Camera::Reset()
	{
		mViewMatrixDataDirty = true;
		UpdateViewMatrix();
	}

	void Camera::Start(WorldState&)
	{
		UpdateProjectionMatrix();
		Reset();
	}

	void Camera::Update(WorldState&)
	{
		if (mViewMatrixDataDirty)
		{
			UpdateViewMatrix();
		}
	}

	void Camera::UpdateViewMatrix()
	{
		XMVECTOR eyePosition = XMLoadFloat4(&mTransform.GetWorldPosition().RawVector());
		XMVECTOR direction = XMLoadFloat4(&mTransform.GetWorldRotation().Forward().RawVector());
		XMVECTOR upDirection = XMLoadFloat4(&mTransform.GetWorldRotation().Up().RawVector());

		XMMATRIX viewMatrix = XMMatrixLookToRH(eyePosition, direction, upDirection);
		XMStoreFloat4x4(&mViewMatrix.RawMatrix(), viewMatrix);

		for (auto& callback : mViewMatrixUpdatedCallbacks)
		{
			callback();
		}

		mViewMatrixDataDirty = false;
	}
}
