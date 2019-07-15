#include "pch.h"
#include "Transform.h"

using namespace GameEngine;
using namespace DirectX;

void Transform::SetParent(Transform* parent)
{
	if (parent != mParent)
	{
		mParent = parent;
		mWorldPositionDirty = true;
		mWorldRotationDirty = true;
		mWorldScaleDirty = true;
	}
}

Transform* Transform::GetParent() const
{
	return mParent;
}

const Vector3& Transform::GetWorldPosition()
{
	if (LocalTransformDirty() || (mParent != nullptr && mParent->TransformDirty()))
	{
		UpdateMatrix();
	}
	return mWorldPosition;
}

const Quaternion& Transform::GetWorldRotation()
{
	if (LocalTransformDirty() || (mParent != nullptr && mParent->TransformDirty()))
	{
		UpdateMatrix();
	}
	return mWorldRotation;
}

const Vector3& Transform::GetWorldScale()
{
	if (LocalTransformDirty() || (mParent != nullptr && mParent->TransformDirty()))
	{
		UpdateMatrix();
	}
	return mWorldScale;
}

void Transform::SetWorldPosition(const Vector3& position)
{
	mWorldPosition = position;
	mLocalPositionDirty = false;
	mWorldPositionDirty = true;
}

void Transform::SetWorldRotation(const Quaternion& rotation)
{
	mWorldRotation = rotation;
	mLocalRotationDirty = false;
	mWorldRotationDirty = true;
}

void Transform::SetWorldScale(const Vector3& scale)
{
	mWorldScale = scale;
	mLocalScaleDirty = false;
	mWorldScaleDirty = true;
}

const Vector3& Transform::GetLocalPosition()
{
	if (WorldTransformDirty())
	{
		UpdateMatrix();
	}
	return mLocalPosition;
}

const Quaternion& Transform::GetLocalRotation()
{
	if (WorldTransformDirty())
	{
		UpdateMatrix();
	}
	return mLocalRotation;
}

const Vector3& Transform::GetLocalScale()
{
	if (WorldTransformDirty())
	{
		UpdateMatrix();
	}
	return mLocalScale;
}

void Transform::SetLocalPosition(const Vector3& position)
{
	mLocalPosition = position;
	mLocalPositionDirty = true;
	mWorldPositionDirty = false;
}

void Transform::SetLocalRotation(const Quaternion& rotation)
{
	mLocalRotation = rotation;
	mLocalRotationDirty = true;
	mWorldRotationDirty = false;
}

void Transform::SetLocalScale(const Vector3& scale)
{
	mLocalScale = scale;
	mLocalScaleDirty = true;
	mWorldScaleDirty = false;
}

const Matrix& Transform::GetWorldMatrix()
{
	if (TransformDirty())
	{
		UpdateMatrix();
	}
	return mWorldMatrix;
}

const Matrix& Transform::GetLocalMatrix()
{
	if (TransformDirty())
	{
		UpdateMatrix();
	}
	return mLocalMatrix;
}

const Matrix& Transform::GetWorldMatrixInverse()
{
	if (TransformDirty())
	{
		UpdateMatrix();
	}
	return mWorldMatrixInverse;
}

bool Transform::LocalTransformDirty() const
{
	return mLocalPositionDirty || mLocalRotationDirty || mLocalScaleDirty;
}

bool Transform::WorldTransformDirty() const
{
	return mWorldPositionDirty || mWorldRotationDirty || mWorldScaleDirty;
}

bool Transform::TransformDirty() const
{
	return LocalTransformDirty() || WorldTransformDirty() || (mParent != nullptr && mParent->TransformDirty());
}

void Transform::UpdateMatrix()
{
	const Matrix& parentMatrix = mParent == nullptr ? Matrix::Identity : mParent->GetWorldMatrix();
	const Matrix& parentMatrixInverse = mParent == nullptr ? Matrix::Identity : mParent->mWorldMatrixInverse;

#ifdef WITH_OPENGL
	parentMatrix;
	parentMatrixInverse;
	// No implementation yet...
#else
	// Load DirectX matrix
	const XMMATRIX loadedParent = XMLoadFloat4x4(&parentMatrix.RawMatrix());
	const XMMATRIX loadedParentInverse = XMLoadFloat4x4(&parentMatrixInverse.RawMatrix());

	// Convert all to world vector
	XMVECTOR sourcePosition = XMLoadFloat4(&mWorldPosition.RawVector());
	XMVECTOR sourceRotation = XMLoadFloat4(&mWorldRotation.RawQuaternion());
	XMVECTOR sourceScale = XMLoadFloat4(&mWorldScale.RawVector());
	if (mLocalPositionDirty)
	{
		if (mParent == nullptr)
		{
			sourcePosition = XMLoadFloat4(&mLocalPosition.RawVector());
		}
		else
		{
			sourcePosition = XMVector4Transform(XMLoadFloat4(&mLocalPosition.RawVector()), loadedParent);
		}
	}
	if (mLocalRotationDirty)
	{
		if (mParent == nullptr)
		{
			sourceRotation = XMLoadFloat4(&mLocalRotation.RawQuaternion());
		}
		else
		{
			sourceRotation = XMQuaternionMultiply(XMLoadFloat4(&mParent->mWorldRotation.RawQuaternion()), XMLoadFloat4(&mLocalRotation.RawQuaternion()));
		}
	}
	if (mLocalScaleDirty)
	{
		if (mParent == nullptr)
		{
			sourceScale = XMLoadFloat4(&mLocalScale.RawVector());
		}
		else
		{
			sourceScale = XMVector4Transform(XMLoadFloat4(&mLocalScale.RawVector()), loadedParent);
		}
	}

	// Combine transform
	XMMATRIX newWorldMatrix = XMMatrixScalingFromVector(sourceScale) * XMMatrixRotationQuaternion(sourceRotation) * XMMatrixTranslationFromVector(sourcePosition);
	XMMATRIX newLocalMatrix = XMMatrixMultiply(newWorldMatrix, loadedParentInverse);
	XMStoreFloat4x4(&mLocalMatrix.RawMatrix(), newLocalMatrix);
	XMStoreFloat4x4(&mWorldMatrix.RawMatrix(), newWorldMatrix);
	XMStoreFloat4x4(&mWorldMatrixInverse.RawMatrix(), XMMatrixInverse(nullptr, newWorldMatrix));

	// Update individule components
	XMVECTOR translation;
	XMVECTOR rotation;
	XMVECTOR scale;
	if (WorldTransformDirty())
	{
		XMMatrixDecompose(&scale, &rotation, &translation, newLocalMatrix);
		XMStoreFloat4(&mLocalPosition.RawVector(), translation);
		XMStoreFloat4(&mLocalRotation.RawQuaternion(), rotation);
		XMStoreFloat4(&mLocalScale.RawVector(), scale);
	}
	XMMatrixDecompose(&scale, &rotation, &translation, newWorldMatrix);
	XMStoreFloat4(&mWorldPosition.RawVector(), translation);
	XMStoreFloat4(&mWorldRotation.RawQuaternion(), rotation);
	XMStoreFloat4(&mWorldScale.RawVector(), scale);
#endif
	
	// Reset dirty flag
	mLocalPositionDirty = false;
	mLocalRotationDirty = false;
	mLocalScaleDirty = false;
	mWorldPositionDirty = false;
	mWorldRotationDirty = false;
	mWorldScaleDirty = false;

	for (auto& callback : mCallbacks)
	{
		callback();
	}
}

void Transform::AddTransformUpdateCallback(UpdateCallback callback)
{
	mCallbacks.push_back(callback);
}

void Transform::RemoveTransformUpdateCallback(UpdateCallback callback)
{
	auto it = std::find_if(mCallbacks.begin(), mCallbacks.end(), [&callback](const UpdateCallback& c) { return c.target_type() == callback.target_type(); });
	mCallbacks.erase(it);
}
