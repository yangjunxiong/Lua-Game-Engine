#include "pch.h"
#include "Collision.h"

using namespace GameEngine;
using namespace DirectX;

#pragma region CollisionShape
Sphere::Sphere(const Vector3& center, float radius) :
	Center(center),
	Radius(radius)
{}

Ray::Ray(const Vector3& start, const Vector3& end) :
	Start(start),
	End(end)
{}

Vector3 Ray::Direction() const
{
	Vector3 direction = End - Start;
	direction.Normalize();
	return direction;
}

float Ray::Distance() const
{
	return (End - Start).Length();
}
#pragma endregion

#pragma region CollisionImpl
bool Collision::RayIntersectsSphere(const Ray& ray, const Sphere& sphere)
{
	Vector3 direction = ray.End - ray.Start;
	float distance = direction.Length();
	direction.Normalize();
	return sphere.Raw().Intersects(XMLoadFloat4(&ray.Start.RawVector()), XMLoadFloat4(&direction.RawVector()), distance);
}

bool Collision::SphereIntersectsSphere(const Sphere& one, const Sphere& two)
{
	return (one.Center - two.Center).LengthSquare() < (one.Radius + two.Radius) * (one.Radius + two.Radius);
}
#pragma endregion
