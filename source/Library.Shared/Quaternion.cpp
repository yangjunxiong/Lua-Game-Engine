#include "pch.h"
#include "Quaternion.h"
#include <DirectXMath.h>

namespace DirectX {};

using namespace GameEngine;
using namespace DirectX;

const Quaternion Quaternion::Identity;

Quaternion::Quaternion() :
	mQuaternion(0.f, 0.f, 0.f, 1.f)
{}

Quaternion::Quaternion(float x, float y, float z, float w) :
	mQuaternion(x, y, z, w)
{}

#ifdef WITH_OPENGL
#else
Quaternion::Quaternion(const XMVECTOR& vec)
{
	XMStoreFloat4(&mQuaternion, vec);
}

Quaternion Quaternion::AddRotation(const Quaternion& quat1, const Quaternion& quat2)
{
	return XMQuaternionMultiply(XMLoadFloat4(&quat1.RawQuaternion()), XMLoadFloat4(&quat2.RawQuaternion()));
}

Quaternion Quaternion::RotationAroundAxis(const Vector3& axis, float angle)
{
	return XMQuaternionRotationAxis(XMLoadFloat4(&axis.RawVector()), angle);
}

Quaternion Quaternion::Slerp(const Quaternion& quat1, const Quaternion& quat2, float alpha)
{
	return XMQuaternionSlerp(XMLoadFloat4(&quat1.RawQuaternion()), XMLoadFloat4(&quat2.RawQuaternion()), alpha);
}

Quaternion Quaternion::FromEulerAngles(const Vector3& euler)
{
	return XMQuaternionRotationRollPitchYaw(euler.GetX(), euler.GetY(), euler.GetZ());
}

Vector3 Quaternion::ToEulerAngles(const Quaternion& quat)
{
	float pitch, yaw, roll;

	// roll (x-axis rotation)
	float sinr_cosp = 2.f * (quat.GetW() * quat.GetX() + quat.GetY() * quat.GetZ());
	float cosr_cosp = 1.f - 2.f * (quat.GetX() * quat.GetX() + quat.GetY() * quat.GetY());
	pitch = atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	float sinp = 2.f * (quat.GetW() * quat.GetY() - quat.GetZ() * quat.GetX());
	if (fabs(sinp) >= 1)
		yaw = copysign(XM_PI / 2, sinp); // use 90 degrees if out of range
	else
		yaw = asin(sinp);

	// yaw (z-axis rotation)
	float siny_cosp = 2.f * (quat.GetW() * quat.GetZ() + quat.GetX() * quat.GetY());
	float cosy_cosp = 1.f - 2.f * (quat.GetY() * quat.GetY() + quat.GetZ() * quat.GetZ());
	roll = atan2(siny_cosp, cosy_cosp);

	return Vector3(pitch, yaw, roll);
}

Vector3 Quaternion::ToUnitVector(const Quaternion& quat)
{
	return XMVector3Rotate(Vector4::ForwardVector, XMLoadFloat4(&quat.RawQuaternion()));
}

Quaternion Quaternion::FromUnitVector(const Vector3& vector)
{
	auto dotResult = XMVector3Dot(Vector3::ForwardVector, XMLoadFloat4(&vector.RawVector()));
	XMFLOAT4 dotFloat;
	XMStoreFloat4(&dotFloat, dotResult);
	float dot = dotFloat.x;

	if (fabs(dot - (-1.0f)) < 0.000001f)
	{
		return Quaternion::RotationAroundAxis(Vector3::Up, 180.f);
	}
	if (fabs(dot - (1.0f)) < 0.000001f)
	{
		return Quaternion::Identity;
	}

	float rotAngle = *XMVector3AngleBetweenVectors(Vector3::ForwardVector, XMLoadFloat4(&vector.RawVector())).m128_f32;
	auto rotAxis = XMVector3Cross(Vector3::ForwardVector, XMLoadFloat4(&vector.RawVector()));
	rotAxis = XMVector3Normalize(rotAxis);
	return Quaternion::RotationAroundAxis(rotAxis, rotAngle);
}
#endif
