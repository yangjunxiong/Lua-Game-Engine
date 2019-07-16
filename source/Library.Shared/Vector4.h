#pragma once
#include "Macro.h"
#ifdef WITH_OPENGL
#include <glm/glm.hpp>
#else
#include <d3d11.h>
#include <DirectXMath.h>
#endif

namespace GameEngine
{
	CLASS();
	class Vector4
	{
	public:
		Vector4();
		explicit Vector4(float v);
		Vector4(float x, float y, float z, float w);
		Vector4(float x, float y, float z);
		Vector4(const Vector4& other) = default;
		Vector4(Vector4&& other) = default;
		Vector4& operator=(const Vector4& other) = default;
		Vector4& operator=(Vector4&& other) = default;
		virtual ~Vector4() = default;

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

		PROPERTY();
		static const Vector4 One;

		PROPERTY();
		static const Vector4 Zero;

		PROPERTY();
		static const Vector4 Forward;

		PROPERTY();
		static const Vector4 Up;

		PROPERTY();
		static const Vector4 Right;

#ifdef WITH_OPENGL
		inline glm::vec4& RawVector();
		inline const glm::vec4& RawVector() const;
#else
		inline DirectX::XMFLOAT4& RawVector();
		inline const DirectX::XMFLOAT4& RawVector() const;
		static const DirectX::XMVECTOR OneVector;
		static const DirectX::XMVECTOR ZeroVector;
		static const DirectX::XMVECTOR ForwardVector;
		static const DirectX::XMVECTOR UpVector;
		static const DirectX::XMVECTOR RightVector;
#endif

	protected:
#ifdef WITH_OPENGL
		glm::vec4 mVector;
#else
		DirectX::XMFLOAT4 mVector;
#endif
	};

	CLASS();
	class Vector3 : public Vector4
	{
	public:
		Vector3();
		Vector3(const Vector3& other) = default;
		Vector3(Vector3&& other) = default;
		Vector3& operator=(const Vector3& other) = default;
		Vector3& operator=(Vector3&& other) = default;
		Vector3(const Vector4& vec);
		explicit Vector3(float v);
		virtual ~Vector3() = default;

		CONSTRUCTOR();
		Vector3(float x, float y, float z);

		FUNCTION();
		inline float Length() const;

		FUNCTION();
		inline float LengthSquare() const;

		FUNCTION();
		inline void Normalize();
	};
}

#include "Vector4.inl"
