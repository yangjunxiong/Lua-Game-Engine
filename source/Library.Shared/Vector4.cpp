#include "pch.h"
#include "Vector4.h"

using namespace GameEngine;

const Vector4 Vector4::One { 1.f, 1.f, 1.f, 1.f };
const Vector4 Vector4::Zero;
const Vector4 Vector4::Forward { 0.f, 0.f, 1.f, 1.f };
const Vector4 Vector4::Up { 0.f, 1.f, 0.f, 1.f };
const Vector4 Vector4::Right { 1.f, 0.f, 0.f, 1.f };

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

Vector3::Vector3() :
	Vector4(0.f, 0.f, 0.f, 1.f)
{}

Vector3::Vector3(const Vector4& vec) :
	Vector4(vec)
{}

Vector3::Vector3(float v) :
	Vector4(v)
{}

Vector3::Vector3(float x, float y, float z) :
	Vector4(x, y, z, 1.f)
{}
