#pragma once

namespace GameEngine
{
	float Vector4::GetX() const
	{
		return mVector.x;
	}

	float Vector4::GetY() const
	{
		return mVector.y;
	}

	float Vector4::GetZ() const
	{
		return mVector.z;
	}

	float Vector4::GetW() const
	{
		return mVector.w;
	}

	void Vector4::SetX(float x)
	{
		mVector.x = x;
	}

	void Vector4::SetY(float y)
	{
		mVector.y = y;
	}

	void Vector4::SetZ(float z)
	{
		mVector.z = z;
	}

	void Vector4::SetW(float w)
	{
		mVector.w = w;
	}

#ifdef WITH_OPENGL
	glm::vec4& Vector4::RawVector()
	{
		return mVector;
	}

	const glm::vec4& Vector4::RawVector() const
	{
		return mVector;
	}
#else
	DirectX::XMFLOAT4& Vector4::RawVector()
	{
		return mVector;
	}

	const DirectX::XMFLOAT4& Vector4::RawVector() const
	{
		return mVector;
	}
#endif

	float Vector3::Length() const
	{
#ifdef WITH_OPENGL
		return sqrtf(LengthSquare());
#else
		DirectX::XMFLOAT4 ret;
		DirectX::XMStoreFloat4(&ret, DirectX::XMVector3Length(DirectX::XMLoadFloat4(&mVector)));
		return ret.x;
#endif
	}

	float Vector3::LengthSquare() const
	{
#ifdef WITH_OPENGL
		return mVector.x * mVector.x + mVector.y * mVector.y + mVector.z * mVector.z;
#else
		DirectX::XMFLOAT4 ret;
		DirectX::XMStoreFloat4(&ret, DirectX::XMVector3LengthSq(DirectX::XMLoadFloat4(&mVector)));
		return ret.x;
#endif
	}
}