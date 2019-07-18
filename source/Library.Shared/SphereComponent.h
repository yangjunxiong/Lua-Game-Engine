#pragma once
#include "CollisionComponent.h"

namespace GameEngine
{
	CLASS(NO_LUA_AUTHORITY);
	class SphereComponent : public CollisionComponent
	{
	public:
		CONSTRUCTOR();
		SphereComponent(Entity* parent, const Vector3& center, float radius);
		virtual ~SphereComponent() = default;

		FUNCTION();
		virtual bool IntersectsRay(const Ray& ray) const override;

		FUNCTION();
		virtual bool IntersectsSphere(const Sphere& other) const override;

		virtual gsl::owner<SphereComponent*> Clone() const override { return new SphereComponent(*this); };

		FUNCTION();
		const Vector3& GetCenter() const;

		FUNCTION();
		void SetCenter(const Vector3& center);

		FUNCTION();
		float GetRadius() const;

		FUNCTION();
		void SetRadius(float radius);

	protected:
		inline Sphere FinalSphere() const;

		Sphere mSphere;
	};
}