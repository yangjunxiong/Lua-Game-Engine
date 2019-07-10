#pragma once
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <gsl\gsl>
#include "GameComponent.h"
#include "MatrixHelper.h"
#include "VectorHelper.h"

namespace GameEngine
{
	CLASS();
	class TransformComponent : public GameComponent
	{
		RTTI_DECLARATIONS(TransformComponent, GameComponent)

	public:
		TransformComponent() = default;
		TransformComponent(Game& game);
		TransformComponent(const TransformComponent&) = default;
		TransformComponent& operator=(const TransformComponent&) = default;
		TransformComponent(TransformComponent&&) = default;
		TransformComponent& operator=(TransformComponent&&) = default;
		virtual ~TransformComponent() = default;

		void SetParent(std::shared_ptr<TransformComponent> parent);
		std::shared_ptr<TransformComponent> GetParent() const;
		const std::vector<std::shared_ptr<TransformComponent>>& GetChildren() const;

		const DirectX::XMFLOAT4& GetWorldPosition();
		const DirectX::XMFLOAT4& GetWorldRotation();
		const DirectX::XMFLOAT4& GetWorldScale();
		void SetWorldPosition(const DirectX::XMFLOAT4& position);
		void SetWorldPosition(const DirectX::XMVECTOR& position);
		void SetWorldRotation(const DirectX::XMFLOAT4& rotation);
		void SetWorldRotation(const DirectX::XMVECTOR& rotation);
		void SetWorldScale(const DirectX::XMFLOAT4& scale);
		void SetWorldScale(const DirectX::XMVECTOR& scale);

		const DirectX::XMFLOAT4& GetLocalPosition();
		const DirectX::XMFLOAT4& GetLocalRotation();
		const DirectX::XMFLOAT4& GetLocalScale();
		void SetLocalPosition(const DirectX::XMFLOAT4& position);
		void SetLocalPosition(const DirectX::XMVECTOR& position);
		void SetLocalRotation(const DirectX::XMFLOAT4& rotation);
		void SetLocalRotation(const DirectX::XMVECTOR& rotation);
		void SetLocalScale(const DirectX::XMFLOAT4& scale);
		void SetLocalScale(const DirectX::XMVECTOR& scale);

		const DirectX::XMFLOAT4X4& GetWorldMatrix();
		const DirectX::XMFLOAT4X4& GetLocalMatrix();
		const DirectX::XMFLOAT4X4& GetWorldInverseMatrix();

	protected:
		inline bool LocalTransformDirty() const;
		inline bool WorldTransformDirty() const;
		inline bool TransformDirty() const;
		inline void UpdateMatrix();

		std::shared_ptr<TransformComponent> mParent;
		std::vector<std::shared_ptr<TransformComponent>> mChildren;
		DirectX::XMFLOAT4X4 mWorldMatrix = MatrixHelper::Identity;
		DirectX::XMFLOAT4X4 mWorldMatrixInverse = MatrixHelper::Identity;
		DirectX::XMFLOAT4 mWorldPosition = Vector4Helper::Zero;
		DirectX::XMFLOAT4 mWorldRotation = Vector4Helper::Zero;
		DirectX::XMFLOAT4 mWorldScale = Vector4Helper::One;
		DirectX::XMFLOAT4X4 mLocalMatrix = MatrixHelper::Identity;
		DirectX::XMFLOAT4 mLocalPosition = Vector4Helper::Zero;
		DirectX::XMFLOAT4 mLocalRotation = Vector4Helper::Zero;
		DirectX::XMFLOAT4 mLocalScale = Vector4Helper::One;
		bool mLocalPositionDirty = false;
		bool mLocalRotationDirty = false;
		bool mLocalScaleDirty = false;
		bool mWorldPositionDirty = false;
		bool mWorldRotationDirty = false;
		bool mWorldScaleDirty = false;
	};
}
