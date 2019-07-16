#pragma once

namespace GameEngine
{
	float Quaternion::GetX() const
	{
		return mQuaternion.x;
	}

	float Quaternion::GetY() const
	{
		return mQuaternion.y;
	}

	float Quaternion::GetZ() const
	{
		return mQuaternion.z;
	}

	float Quaternion::GetW() const
	{
		return mQuaternion.w;
	}

	void Quaternion::SetX(float x)
	{
		mQuaternion.x = x;
	}

	void Quaternion::SetY(float y)
	{
		mQuaternion.y = y;
	}

	void Quaternion::SetZ(float z)
	{
		mQuaternion.z = z;
	}

	void Quaternion::SetW(float w)
	{
		mQuaternion.w = w;
	}

	Vector3 Quaternion::Forward() const
	{
		return ToMatrix().Forward();
	}

	Vector3 Quaternion::Up() const
	{
		return ToMatrix().Up();
	}

	Vector3 Quaternion::Right() const
	{
		return ToMatrix().Right();
	}

#ifdef WITH_OPENGL
	glm::vec4& Quaternion::RawQuaternion()
	{
		return mQuaternion;
	}

	const glm::vec4& Quaternion::RawQuaternion() const
	{
		return mQuaternion;
	}
#else
	DirectX::XMFLOAT4& Quaternion::RawQuaternion()
	{
		return mQuaternion;
	}

	const DirectX::XMFLOAT4& Quaternion::RawQuaternion() const
	{
		return mQuaternion;
	}

	Matrix Quaternion::ToMatrix() const
	{
		return DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&mQuaternion));
	}
#endif
}