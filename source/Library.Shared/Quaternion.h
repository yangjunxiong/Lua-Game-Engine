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
	class Quaternion final
	{
	public:
		CONSTRUCTOR();
		Quaternion();
		Quaternion(float x, float y, float z, float w);
		Quaternion(const Quaternion& other) = default;
		Quaternion(Quaternion&& other) = default;
		Quaternion& operator=(const Quaternion& other) = default;
		Quaternion& operator=(Quaternion&& other) = default;
		~Quaternion() = default;

		static const Quaternion Identity;

#ifdef WITH_OPENGL
		inline glm::vec4& RawQuaternion();
		inline const glm::vec4& RawQuaternion() const;
#else
		inline DirectX::XMFLOAT4& RawQuaternion();
		inline const DirectX::XMFLOAT4& RawQuaternion() const;
#endif

	private:
#ifdef WITH_OPENGL
		glm::vec4 mQuaternion;
#else
		DirectX::XMFLOAT4 mQuaternion;
#endif
	};
}

#include "Quaternion.inl"