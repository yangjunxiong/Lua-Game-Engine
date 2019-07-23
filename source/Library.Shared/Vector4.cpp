#include "pch.h"
#include "Vector4.h"
#include "Matrix.h"

using namespace GameEngine;

const Vector4 Vector4::One { 1.f, 1.f, 1.f, 1.f };
const Vector4 Vector4::Zero;
const Vector4 Vector4::Forward { 0.f, 0.f, 1.f, 1.f };
const Vector4 Vector4::Up { 0.f, 1.f, 0.f, 1.f };
const Vector4 Vector4::Right { 1.f, 0.f, 0.f, 1.f };

const Vector3 Vector3::One { 1.f, 1.f, 1.f };
const Vector3 Vector3::Zero;
const Vector3 Vector3::Forward { 0.f, 0.f, 1.f };
const Vector3 Vector3::Up { 0.f, 1.f, 0.f };
const Vector3 Vector3::Right { 1.f, 0.f, 0.f };

#ifdef WITH_OPENGL
#else
using namespace DirectX;
const DirectX::XMVECTOR Vector4::OneVector = XMLoadFloat4(&Vector4::One.RawVector());
const DirectX::XMVECTOR Vector4::ZeroVector = XMLoadFloat4(&Vector4::Zero.RawVector());
const DirectX::XMVECTOR Vector4::ForwardVector = XMLoadFloat4(&Vector4::Forward.RawVector());
const DirectX::XMVECTOR Vector4::UpVector = XMLoadFloat4(&Vector4::Up.RawVector());
const DirectX::XMVECTOR Vector4::RightVector = XMLoadFloat4(&Vector4::Right.RawVector());
#endif

Vector4::Vector4() :
	mVector(0.f, 0.f, 0.f, 0.f)
{}

Vector4::Vector4(float v) :
	mVector(v, v, v, v)
{}

Vector4::Vector4(float x, float y, float z, float w) :
	mVector(x, y, z, w)
{}

Vector4::Vector4(float x, float y, float z) :
	mVector(x, y, z, 1.f)
{}

Vector4::Vector4(const Vector4& other) :
	x(mVector.x),
	y(mVector.y),
	z(mVector.z),
	w(mVector.w),
	mVector(other.mVector)
{}

Vector4& Vector4::operator=(const Vector4& other)
{
	if (this != &other)
	{
		mVector = other.mVector;
	}
	return *this;
}

Vector3::Vector3() :
	Vector4(0.f, 0.f, 0.f, 1.f)
{}

Vector3::Vector3(const Vector4& vec) :
	Vector4(vec)
{}

Vector3::Vector3(float x, float y, float z, float w) :
	Vector4(x, y, z, w)
{}

Vector3::Vector3(float v) :
	Vector4(v)
{}

Vector3::Vector3(float x, float y, float z) :
	Vector4(x, y, z, 1.f)
{}

#ifdef WITH_OPENGL
#else
Vector3::Vector3(const DirectX::XMVECTOR& vec)
{
	XMStoreFloat4(&mVector, vec);
}

Vector3 Vector3::operator+(const Vector3 other) const
{
	return XMVectorAdd(XMLoadFloat4(&mVector), XMLoadFloat4(&other.RawVector()));
}

Vector3 Vector3::operator-(const Vector3 other) const
{
	return XMVectorSubtract(XMLoadFloat4(&mVector), XMLoadFloat4(&other.RawVector()));
}

Vector3 Vector3::operator*(float scale) const
{
	return XMVectorScale(XMLoadFloat4(&mVector), scale);
}

Vector3 Vector3::operator/(float scale) const
{
	return XMVectorScale(XMLoadFloat4(&mVector), 1.f / scale);
}

Vector3 Vector3::Unproject(Vector3 screenCoordinates,
	const Matrix& projection,
	const Matrix& view,
	const Matrix& world,
	float viewportWidth, float viewportHeight,
	float viewportX, float viewportY,
	float viewportMinZ, float viewportMaxZ
)
{
	return XMVector3Unproject(XMLoadFloat4(&screenCoordinates.RawVector()),
		viewportX, viewportY,
		viewportWidth, viewportHeight,
		viewportMinZ, viewportMaxZ,
		XMLoadFloat4x4(&projection.RawMatrix()), XMLoadFloat4x4(&view.RawMatrix()), XMLoadFloat4x4(&world.RawMatrix()));
}
#endif
