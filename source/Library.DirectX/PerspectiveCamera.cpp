#include "pch.h"
#include "PerspectiveCamera.h"

using namespace DirectX;

namespace GameEngine
{
    RTTI_DEFINITIONS(PerspectiveCamera)

    PerspectiveCamera::PerspectiveCamera(float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance) :
		Camera(nearPlaneDistance, farPlaneDistance),
        mFieldOfView(fieldOfView), mAspectRatio(aspectRatio)
    {
    }

    float PerspectiveCamera::AspectRatio() const
    {
        return mAspectRatio;
    }

	void PerspectiveCamera::SetAspectRatio(float aspectRatio)
	{
		mAspectRatio = aspectRatio;
		mProjectionMatrixDataDirty = true;
	}

    float PerspectiveCamera::FieldOfView() const
    {
        return mFieldOfView;
    }

	void PerspectiveCamera::SetFieldOfView(float fieldOfView)
	{
		mFieldOfView = fieldOfView;
		mProjectionMatrixDataDirty = true;
	}

    void PerspectiveCamera::UpdateProjectionMatrix()
    {
		if (mProjectionMatrixDataDirty)
		{
			XMMATRIX projectionMatrix = XMMatrixPerspectiveFovRH(mFieldOfView, mAspectRatio, mNearPlaneDistance, mFarPlaneDistance);
			XMStoreFloat4x4(&mProjectionMatrix, projectionMatrix);

			for (auto& callback : mProjectionMatrixUpdatedCallbacks)
			{
				callback();
			}
		}
    }
}
