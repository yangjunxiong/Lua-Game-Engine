#pragma once
#include "Action.h"
#include "Collision.h"
#include "Transform.h"

namespace GameEngine
{
	CLASS(NO_LUA_AUTHORITY);
	class CollisionComponent : public Action
	{
		RTTI_DECLARATIONS(CollisionComponent, Action);

	public:
		CONSTRUCTOR();
		CollisionComponent(Entity* parent);
		virtual ~CollisionComponent() = default;
		virtual void Start(WorldState&) override {};
		virtual void Update(WorldState&) override {};
		virtual gsl::owner<CollisionComponent*> Clone() const override { return new CollisionComponent(*this); };

		FUNCTION();
		virtual bool IntersectsRay(const Ray&) const { return false; };

		FUNCTION();
		virtual bool IntersectsSphere(const Sphere&) const { return false; };

	protected:
		CollisionComponent(Entity* parent, RTTI::IdType type);
		Transform mTransform;
	};
}