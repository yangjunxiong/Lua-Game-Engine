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
		~Vector4() = default;

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

#ifdef WITH_OPENGL
		inline glm::vec4& RawVector();
		inline const glm::vec4& RawVector() const;
#else
		inline DirectX::XMFLOAT4& RawVector();
		inline const DirectX::XMFLOAT4& RawVector() const;
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
		explicit Vector3(float v);

		CONSTRUCTOR();
		Vector3(float x, float y, float z);

		FUNCTION();
		inline float Length() const;

		FUNCTION();
		inline float LengthSquare() const;
	};
}

#include "Vector4.inl"
