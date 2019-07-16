#include "pch.h"
#include "Matrix.h"

using namespace GameEngine;

const Matrix Matrix::Identity
{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

const Matrix Matrix::Zero
{
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f
};

Matrix::Matrix() :
	Matrix(Matrix::Identity)
{}

Matrix::Matrix(float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44) :
	mMatrix(m11, m12, m13, m14,
		m21, m22, m23, m24,
		m31, m32, m33, m34,
		m41, m42, m43, m44)
{}

#ifdef WITH_OPENGL
#else
using namespace DirectX;

Matrix::Matrix(const XMMATRIX& matrix)
{
	XMStoreFloat4x4(&mMatrix, matrix);
}

Vector3 Matrix::Forward() const
{
	auto vector = Vector3(-mMatrix._31, -mMatrix._32, -mMatrix._33);
	vector.Normalize();
	return vector;
}

Vector3 Matrix::Up() const
{
	auto vector = Vector3(mMatrix._21, mMatrix._22, mMatrix._23);
	vector.Normalize();
	return vector;
}

Vector3 Matrix::Right() const
{
	auto vector = Vector3(mMatrix._11, mMatrix._12, mMatrix._13);
	vector.Normalize();
	return vector;
}
#endif