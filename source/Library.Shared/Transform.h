#pragma once
#include <winrt\Windows.Foundation.h>
#include <gsl\gsl>
#include "Matrix.h"
#include "Vector4.h"
#include "Quaternion.h"

namespace GameEngine
{
	CLASS();
	class Transform
	{
		friend class Entity;

	public:
		CONSTRUCTOR();
		Transform() = default;
		Transform(const Transform&) = default;
		Transform& operator=(const Transform&) = default;
		Transform(Transform&&) = default;
		Transform& operator=(Transform&&) = default;
		virtual ~Transform() = default;

		FUNCTION();
		Vector3& GetWorldPosition();
		const Vector3& GetWorldPosition() const;

		FUNCTION();
		Quaternion& GetWorldRotation();
		const Quaternion& GetWorldRotation() const;

		FUNCTION();
		Vector3& GetWorldScale();
		const Vector3& GetWorldScale() const;

		FUNCTION();
		void SetWorldPosition(const Vector3& position);

		FUNCTION();
		void SetWorldRotation(const Quaternion& rotation);

		FUNCTION();
		void SetWorldScale(const Vector3& scale);

		FUNCTION();
		Vector3& GetLocalPosition();
		const Vector3& GetLocalPosition() const;

		FUNCTION();
		Quaternion& GetLocalRotation();
		const Quaternion& GetLocalRotation() const;

		FUNCTION();
		Vector3& GetLocalScale();
		const Vector3& GetLocalScale() const;

		FUNCTION();
		void SetLocalPosition(const Vector3& position);

		FUNCTION();
		void SetLocalRotation(const Quaternion& rotation);

		FUNCTION();
		void SetLocalScale(const Vector3& scale);

		FUNCTION();
		Vector3& Forward();
		const Vector3& Forward() const;

		FUNCTION();
		Vector3& Up();
		const Vector3& Up() const;

		FUNCTION();
		Vector3& Right();
		const Vector3& Right() const;

		using UpdateCallback = std::function<void()>;
		void AddTransformUpdateCallback(UpdateCallback callback);
		void RemoveTransformUpdateCallback(UpdateCallback callback);

		FUNCTION();
		Matrix& GetWorldMatrix();
		const Matrix& GetWorldMatrix() const;

		FUNCTION();
		Matrix& GetLocalMatrix();
		const Matrix& GetLocalMatrix() const;

		FUNCTION();
		Matrix& GetWorldMatrixInverse();
		const Matrix& GetWorldMatrixInverse() const;

		FUNCTION();
		void RefreshTransform();

		void SetParent(Transform* parent);
		Transform* GetParent() const;

	protected:
		inline bool LocalTransformDirty() const;
		inline bool WorldTransformDirty() const;
		inline bool TransformDirty() const;
		inline void UpdateMatrix();

		Transform* mParent;
		std::vector<Transform*> mChildren;
		Matrix mWorldMatrix;
		Matrix mWorldMatrixInverse;
		Matrix mLocalMatrix;
		Vector3 mWorldPosition { 0.f, 0.f, 0.f, 1.f } ;
		Quaternion mWorldRotation;
		Vector3 mWorldScale { 1.f, 1.f, 1.f, 0.f };
		Vector3 mLocalPosition { 0.f, 0.f, 0.f, 1.f };
		Quaternion mLocalRotation;
		Vector3 mLocalScale { 1.f, 1.f, 1.f, 0.f };
		Vector3 mForward = Vector3::Forward;
		Vector3 mUp = Vector3::Up;
		Vector3 mRight = Vector3::Right;

		bool mLocalPositionDirty = false;
		bool mLocalRotationDirty = false;
		bool mLocalScaleDirty = false;
		bool mWorldPositionDirty = false;
		bool mWorldRotationDirty = false;
		bool mWorldScaleDirty = false;

		std::vector<UpdateCallback> mCallbacks;
	};
}
