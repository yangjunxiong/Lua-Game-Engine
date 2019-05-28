#include "pch.h"
#include "ActionCollisionResult.h"
#include "CollisionManager.h"
#include "Box2D/Box2D.h"
#include "EventMessageAttributed.h"
#include "ActionCollision.h"
#include "Event.h"
#include "Entity.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionCollisionResult);

ActionCollisionResult::ActionCollisionResult(RTTI::IdType type) :
	Action(type)
{}

void GameEngine::ActionCollisionResult::Update(WorldState & /*state*/)
{
	assert(mOnHitEntity1->Is(Entity::TypeIdClass()));
	assert(mOnHitEntity2->Is(Entity::TypeIdClass()));
	Entity* onHitEntity1 = static_cast<Entity*>(mOnHitEntity1);
	Entity* onHitEntity2 = static_cast<Entity*>(mOnHitEntity2);
	onHitEntity1;
	onHitEntity2;

	//TO-DO: Damage, Power up, Game end
}

gsl::owner<Scope*> ActionCollisionResult::Clone() const
{
	return new ActionCollisionResult(*this);
}

const Vector<Attributed::Signature> ActionCollisionResult::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("OnHitEntity1", Datum::DatumType::Pointer, 1, offsetof(ActionCollisionResult, mOnHitEntity1)),
		Signature("OnHitEntity2", Datum::DatumType::Pointer, 1, offsetof(ActionCollisionResult, mOnHitEntity2))
		});
}