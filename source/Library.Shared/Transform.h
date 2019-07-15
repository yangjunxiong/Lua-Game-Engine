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
		const Vector3& GetWorldPosition();

		FUNCTION();
		const Quaternion& GetWorldRotation();

		FUNCTION();
		const Vector3& GetWorldScale();

		FUNCTION();
		void SetWorldPosition(const Vector3& position);

		FUNCTION();
		void SetWorldRotation(const Quaternion& rotation);

		FUNCTION();
		void SetWorldScale(const Vector3& scale);

		FUNCTION();
		const Vector3& GetLocalPosition();

		FUNCTION();
		const Quaternion& GetLocalRotation();

		FUNCTION();
		const Vector3& GetLocalScale();

		FUNCTION();
		void SetLocalPosition(const Vector3& position);

		FUNCTION();
		void SetLocalRotation(const Quaternion& rotation);

		FUNCTION();
		void SetLocalScale(const Vector3& scale);

		using UpdateCallback = std::function<void()>;
		void AddTransformUpdateCallback(UpdateCallback callback);
		void RemoveTransformUpdateCallback(UpdateCallback callback);

		const Matrix& GetWorldMatrix();
		const Matrix& GetLocalMatrix();
		const Matrix& GetWorldMatrixInverse();

	protected:
		void SetParent(Transform* parent);
		Transform* GetParent() const;

		inline bool LocalTransformDirty() const;
		inline bool WorldTransformDirty() const;
		inline bool TransformDirty() const;
		inline void UpdateMatrix();

		Transform* mParent;
		Matrix mWorldMatrix;
		Matrix mWorldMatrixInverse;
		Matrix mLocalMatrix;
		Vector3 mWorldPosition;
		Quaternion mWorldRotation;
		Vector3 mWorldScale { 1.f };
		Vector3 mLocalPosition;
		Quaternion mLocalRotation;
		Vector3 mLocalScale { 1.f };

		bool mLocalPositionDirty = false;
		bool mLocalRotationDirty = false;
		bool mLocalScaleDirty = false;
		bool mWorldPositionDirty = false;
		bool mWorldRotationDirty = false;
		bool mWorldScaleDirty = false;

		std::vector<UpdateCallback> mCallbacks;
	};
}
