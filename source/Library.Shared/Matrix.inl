#pragma once

namespace GameEngine
{
#ifdef WITH_OPENGL
	glm::mat4& Matrix::RawMatrix()
	{
		return mMatrix;
	}

	const glm::mat4& Matrix::RawMatrix() const
	{
		return mMatrix;
	}
#else
	DirectX::XMFLOAT4X4& Matrix::RawMatrix()
	{
		return mMatrix;
	}

	const DirectX::XMFLOAT4X4& Matrix::RawMatrix() const
	{
		return mMatrix;
	}

	DirectX::XMMATRIX Matrix::SIMDMatrix() const
	{
		return XMLoadFloat4x4(&mMatrix);
	}
#endif
}