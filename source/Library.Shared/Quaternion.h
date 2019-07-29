#pragma once
#include "Macro.h"
#include "Vector4.h"
#include "Matrix.h"
#ifdef WITH_OPENGL
#include <glm/glm.hpp>
#else
#include <d3d11.h>
#include <DirectXMath.h>
#endif

namespace GameEngine
{
	CLASS();
	class Quaternion final
	{
	public:
		CONSTRUCTOR();
		Quaternion();
		Quaternion(float x, float y, float z, float w);
		Quaternion(const Quaternion& other) = default;
		Quaternion(Quaternion&& other) = default;
		Quaternion& operator=(const Quaternion& other) = default;
		Quaternion& operator=(Quaternion&& other) = default;
		~Quaternion() = default;

		FUNCTION();
		inline float GetX() const;

		FUNCTION();
		inline float GetY() const;

		FUNCTION();
		inline float GetZ() const;

		FUNCTION();
		inline float GetW() const;

		FUNCTION();
		inline void SetX(float x);

		FUNCTION();
		inline void SetY(float y);

		FUNCTION();
		inline void SetZ(float z);

		FUNCTION();
		inline void SetW(float w);

		FUNCTION();
		inline Vector3 Forward() const;

		FUNCTION();
		inline Vector3 Up() const;

		FUNCTION();
		inline Vector3 Right() const;

		PROPERTY();
		static const Quaternion Identity;

		FUNCTION();
		static Quaternion AddRotation(const Quaternion& quat1, const Quaternion& quat2);

		FUNCTION();
		static Quaternion RotationAroundAxis(const Vector3& axis, float angle);

		FUNCTION();
		static Quaternion Slerp(const Quaternion& quat1, const Quaternion& quat2, float alpha);

		FUNCTION();
		static Quaternion FromEulerAngles(const Vector3& euler);
		
		FUNCTION();
		static Vector3 ToEulerAngles(const Quaternion& quat);

		FUNCTION();
		static Vector3 ToUnitVector(const Quaternion& quat);

		FUNCTION();
		static Quaternion FromUnitVector(const Vector3& vector);

		FUNCTION();
		inline Matrix ToMatrix() const;

#ifdef WITH_OPENGL
		inline glm::vec4& RawQuaternion();
		inline const glm::vec4& RawQuaternion() const;
#else
		Quaternion(const DirectX::XMVECTOR& vec);
		inline DirectX::XMFLOAT4& RawQuaternion();
		inline const DirectX::XMFLOAT4& RawQuaternion() const;
#endif

	private:
#ifdef WITH_OPENGL
		glm::vec4 mQuaternion;
#else
		DirectX::XMFLOAT4 mQuaternion;
#endif
	};
}

#include "Quaternion.inl"