#include "pch.h"
#include "CollisionComponent.h"
#include "Entity.h"

using namespace GameEngine;
RTTI_DEFINITIONS(CollisionComponent);

CollisionComponent::CollisionComponent(Entity* parent) :
	Action(parent, CollisionComponent::TypeIdClass())
{
	mTransform.SetParent(parent->GetTransform());
}

CollisionComponent::CollisionComponent(Entity* parent, RTTI::IdType type) :
	Action(parent, type)
{
	mTransform.SetParent(parent->GetTransform());
}
