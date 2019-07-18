#include "pch.h"
#include "CollisionComponent.h"
#include "Entity.h"

using namespace GameEngine;

CollisionComponent::CollisionComponent(Entity* parent) :
	mParent(parent)
{
	mTransform.SetParent(parent->GetTransform());
}
