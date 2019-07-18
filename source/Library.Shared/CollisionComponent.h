#pragma once
#include "Action.h"
#include "Collision.h"
#include "Transform.h"

namespace GameEngine
{
	CLASS(NO_LUA_AUTHORITY);
	class CollisionComponent : public Action
	{
	public:
		CollisionComponent(Entity* parent);
		virtual ~CollisionComponent() = default;
		virtual void Start(WorldState&) override {};
		virtual void Update(WorldState&) override {};
		virtual gsl::owner<CollisionComponent*> Clone() const override = 0;

		FUNCTION();
		virtual bool IntersectsRay(const Ray& ray) const = 0;

		FUNCTION();
		virtual bool IntersectsSphere(const Sphere& sphere) const = 0;

	protected:
		Entity* mParent = nullptr;
		Transform mTransform;
	};
}