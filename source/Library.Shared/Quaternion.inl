#pragma once

namespace GameEngine
{
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
#endif
}