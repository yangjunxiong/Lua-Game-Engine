#include "pch.h"
#include "SphereComponent.h"
#include "Entity.h"

using namespace GameEngine;

SphereComponent::SphereComponent(Entity* parent, const Vector3& center, float radius) :
	CollisionComponent(parent),
	mSphere(center, radius)
{
	mTransform.SetLocalPosition(center);
}

bool SphereComponent::IntersectsRay(const Ray& ray) const
{
	return Collision::RayIntersectsSphere(ray, FinalSphere());
}

bool SphereComponent::IntersectsSphere(const Sphere& other) const
{
	return Collision::SphereIntersectsSphere(FinalSphere(), other);
}

const Vector3& SphereComponent::GetCenter() const
{
	return mSphere.Center;
}

void SphereComponent::SetCenter(const Vector3& center)
{
	mSphere.Center = center;
	mTransform.SetLocalPosition(center);
}

float SphereComponent::GetRadius() const
{
	return mSphere.Radius;
}

void SphereComponent::SetRadius(float radius)
{
	mSphere.Radius = radius;
}

Sphere SphereComponent::FinalSphere() const
{
	Transform& trans = const_cast<Transform&>(mTransform);
	
	return Sphere(trans.GetWorldPosition(), trans.GetWorldScale().x * mSphere.Radius);
}
