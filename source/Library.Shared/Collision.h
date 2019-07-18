#pragma once
#include "Macro.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix.h"

#ifdef WITH_OPENGL
#include <glm/glm.hpp>
#else
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#endif

namespace GameEngine
{
	CLASS();
	class Sphere final
	{
	public:
		CONSTRUCTOR();
		Sphere(const Vector3& center, float radius);

		PROPERTY();
		Vector3 Center;

		PROPERTY();
		float Radius;

#ifdef WITH_OPENGL
#else
		inline DirectX::BoundingSphere Raw() const { return DirectX::BoundingSphere(DirectX::XMFLOAT3(Center.x, Center.y, Center.z), Radius); };
#endif
	};

	CLASS();
	class Ray final
	{
	public:
		CONSTRUCTOR();
		Ray(const Vector3& start, const Vector3& end);

		FUNCTION();
		Vector3 Direction() const;

		FUNCTION();
		float Distance() const;

		PROPERTY();
		Vector3 Start;

		PROPERTY();
		Vector3 End;
	};

	CLASS()
	class Collision final
	{
	public:
		FUNCTION();
		static bool RayIntersectsSphere(const Ray& ray, const Sphere& sphere);

		FUNCTION();
		static bool SphereIntersectsSphere(const Sphere& one, const Sphere& two);
	};
}