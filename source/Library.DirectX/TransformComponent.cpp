#include "pch.h"
#include "TransformComponent.h"

using namespace GameEngine;
using namespace DirectX;
RTTI_DEFINITIONS(TransformComponent)

TransformComponent::TransformComponent(Game& game) :
	GameComponent(game)
{}

void TransformComponent::SetParent(std::shared_ptr<TransformComponent> parent)
{
	if (mParent != nullptr)
	{
		auto list = mParent->mChildren;
		mParent->mChildren.erase(std::find_if(list.begin(), list.end(), [this](const std::shared_ptr<TransformComponent> comp) { return &*comp == this; }));
	}

	mParent = parent;
	mWorldPositionDirty = true;
	mWorldRotationDirty = true;
	mWorldScaleDirty = true;
}

std::shared_ptr<TransformComponent> TransformComponent::GetParent() const
{
	return mParent;
}

const std::vector<std::shared_ptr<TransformComponent>>& TransformComponent::GetChildren() const
{
	return mChildren;
}

const DirectX::XMFLOAT4& TransformComponent::GetWorldPosition()
{
	if (LocalTransformDirty() || (mParent != nullptr && mParent->TransformDirty()))
	{
		UpdateMatrix();
	}
	return mWorldPosition;
}

const DirectX::XMFLOAT4& TransformComponent::GetWorldRotation()
{
	if (LocalTransformDirty() || (mParent != nullptr && mParent->TransformDirty()))
	{
		UpdateMatrix();
	}
	return mWorldRotation;
}

const DirectX::XMFLOAT4& TransformComponent::GetWorldScale()
{
	if (LocalTransformDirty() || (mParent != nullptr && mParent->TransformDirty()))
	{
		UpdateMatrix();
	}
	return mWorldScale;
}

void TransformComponent::SetWorldPosition(const DirectX::XMFLOAT4& position)
{
	mWorldPosition = position;
	mLocalPositionDirty = false;
	mWorldPositionDirty = true;
}

void TransformComponent::SetWorldPosition(const DirectX::XMVECTOR& position)
{
	XMFLOAT4 pos;
	XMStoreFloat4(&pos, position);
	SetWorldPosition(pos);
}

void TransformComponent::SetWorldRotation(const DirectX::XMFLOAT4& rotation)
{
	mWorldRotation = rotation;
	mLocalRotationDirty = false;
	mWorldRotationDirty = true;
}

void TransformComponent::SetWorldRotation(const DirectX::XMVECTOR& rotation)
{
	XMFLOAT4 rot;
	XMStoreFloat4(&rot, rotation);
	SetWorldRotation(rot);
}

void TransformComponent::SetWorldScale(const DirectX::XMFLOAT4& scale)
{
	mWorldScale = scale;
	mLocalScaleDirty = false;
	mWorldScaleDirty = true;
}

void TransformComponent::SetWorldScale(const DirectX::XMVECTOR& scale)
{
	XMFLOAT4 sca;
	XMStoreFloat4(&sca, scale);
	SetWorldScale(sca);
}

const DirectX::XMFLOAT4& TransformComponent::GetLocalPosition()
{
	if (WorldTransformDirty())
	{
		UpdateMatrix();
	}
	return mLocalPosition;
}

const DirectX::XMFLOAT4& TransformComponent::GetLocalRotation()
{
	if (WorldTransformDirty())
	{
		UpdateMatrix();
	}
	return mLocalRotation;
}

const DirectX::XMFLOAT4& TransformComponent::GetLocalScale()
{
	if (WorldTransformDirty())
	{
		UpdateMatrix();
	}
	return mLocalScale;
}

void TransformComponent::SetLocalPosition(const DirectX::XMFLOAT4& position)
{
	mLocalPosition = position;
	mLocalPositionDirty = true;
	mWorldPositionDirty = false;
}

void TransformComponent::SetLocalPosition(const DirectX::XMVECTOR& position)
{
	XMFLOAT4 pos;
	XMStoreFloat4(&pos, position);
	SetLocalPosition(pos);
}

void TransformComponent::SetLocalRotation(const DirectX::XMFLOAT4& rotation)
{
	mLocalRotation = rotation;
	mLocalRotationDirty = true;
	mWorldRotationDirty = false;
}

void TransformComponent::SetLocalRotation(const DirectX::XMVECTOR& rotation)
{
	XMFLOAT4 rot;
	XMStoreFloat4(&rot, rotation);
	SetLocalRotation(rot);
}

void TransformComponent::SetLocalScale(const DirectX::XMFLOAT4& scale)
{
	mLocalScale = scale;
	mLocalScaleDirty = true;
	mWorldScaleDirty = false;
}

void TransformComponent::SetLocalScale(const DirectX::XMVECTOR& scale)
{
	XMFLOAT4 sca;
	XMStoreFloat4(&sca, scale);
	SetLocalScale(sca);
}

const DirectX::XMFLOAT4X4& TransformComponent::GetWorldMatrix()
{
	if (TransformDirty())
	{
		UpdateMatrix();
	}
	return mWorldMatrix;
}

const DirectX::XMFLOAT4X4& TransformComponent::GetLocalMatrix()
{
	if (TransformDirty())
	{
		UpdateMatrix();
	}
	return mLocalMatrix;
}

const DirectX::XMFLOAT4X4& TransformComponent::GetWorldInverseMatrix()
{
	if (TransformDirty())
	{
		UpdateMatrix();
	}
	return mWorldMatrixInverse;
}

bool TransformComponent::LocalTransformDirty() const
{
	return mLocalPositionDirty || mLocalRotationDirty || mLocalScaleDirty;
}

bool TransformComponent::WorldTransformDirty() const
{
	return mWorldPositionDirty || mWorldRotationDirty || mWorldScaleDirty;
}

bool TransformComponent::TransformDirty() const
{
	return LocalTransformDirty() || WorldTransformDirty() || (mParent != nullptr && mParent->TransformDirty());
}

void TransformComponent::UpdateMatrix()
{
	DirectX::XMFLOAT4X4 parentMatrix = mParent == nullptr ? MatrixHelper::Identity : mParent->GetWorldMatrix();
	DirectX::XMFLOAT4X4 parentMatrixInverse = mParent == nullptr ? MatrixHelper::Identity : mParent->mWorldMatrixInverse;
	DirectX::XMMATRIX parent_Matrix = XMLoadFloat4x4(&parentMatrix);
	DirectX::XMMATRIX parent_Matrix_Inverse = XMLoadFloat4x4(&parentMatrixInverse);

	// Convert all to world vector
	XMVECTOR sourcePosition = XMLoadFloat4(&mWorldPosition);
	XMVECTOR sourceRotation = XMLoadFloat4(&mWorldRotation);
	XMVECTOR sourceScale = XMLoadFloat4(&mWorldScale);
	if (mLocalPositionDirty)
	{
		if (mParent == nullptr)
		{
			sourcePosition = XMLoadFloat4(&mLocalPosition);
		}
		else
		{
			sourcePosition = XMVector4Transform(XMLoadFloat4(&mLocalPosition), parent_Matrix);
		}
	}
	if (mLocalRotationDirty)
	{
		if (mParent == nullptr)
		{
			sourceRotation = XMLoadFloat4(&mLocalRotation);
		}
		else
		{
			sourceRotation = XMQuaternionMultiply(XMLoadFloat4(&mParent->mWorldRotation), XMLoadFloat4(&mLocalRotation));
		}
	}
	if (mLocalScaleDirty)
	{
		if (mParent == nullptr)
		{
			sourceScale = XMLoadFloat4(&mLocalScale);
		}
		else
		{
			sourceScale = XMVector4Transform(XMLoadFloat4(&mLocalScale), parent_Matrix);
		}
	}

	// Combine transform
	XMMATRIX newWorldMatrix = XMMatrixScalingFromVector(sourceScale) * XMMatrixRotationQuaternion(sourceRotation) * XMMatrixTranslationFromVector(sourcePosition);
	XMMATRIX newLocalMatrix = XMMatrixMultiply(newWorldMatrix, parent_Matrix_Inverse);
	XMStoreFloat4x4(&mLocalMatrix, newLocalMatrix);
	XMStoreFloat4x4(&mWorldMatrix, newWorldMatrix);
	XMStoreFloat4x4(&mWorldMatrixInverse, XMMatrixInverse(nullptr, newWorldMatrix));

	// Update individule components
	XMVECTOR translation;
	XMVECTOR rotation;
	XMVECTOR scale;
	if (WorldTransformDirty())
	{
		XMMatrixDecompose(&scale, &rotation, &translation, newLocalMatrix);
		XMStoreFloat4(&mLocalPosition, translation);
		XMStoreFloat4(&mLocalRotation, rotation);
		XMStoreFloat4(&mLocalScale, scale);
	}
	XMMatrixDecompose(&scale, &rotation, &translation, newWorldMatrix);
	XMStoreFloat4(&mWorldPosition, translation);
	XMStoreFloat4(&mWorldRotation, rotation);
	XMStoreFloat4(&mWorldScale, scale);
	
	// Reset dirty flag
	mLocalPositionDirty = false;
	mLocalRotationDirty = false;
	mLocalScaleDirty = false;
	mWorldPositionDirty = false;
	mWorldRotationDirty = false;
	mWorldScaleDirty = false;
}
