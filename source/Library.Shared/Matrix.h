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
	class Matrix final
	{
	public:
		Matrix();
		Matrix(float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44);
		Matrix(const Matrix& other) = default;
		Matrix(Matrix&& other) = default;
		Matrix& operator=(const Matrix& other) = default;
		Matrix& operator=(Matrix&& other) = default;
		~Matrix() = default;

		static const Matrix Identity;
		static const Matrix Zero;

#ifdef WITH_OPENGL
		inline glm::mat4& RawMatrix();
		inline const glm::mat4& RawMatrix() const;
#else
		inline DirectX::XMFLOAT4X4& RawMatrix();
		inline const DirectX::XMFLOAT4X4& RawMatrix() const;
#endif

	private:
#ifdef WITH_OPENGL
		glm::mat4 mMatrix;
#else
		DirectX::XMFLOAT4X4 mMatrix;
#endif
	};
}

#include "Matrix.inl"
