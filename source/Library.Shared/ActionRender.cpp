#include "pch.h"
#include "ActionRender.h"
#include "Entity.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionRender);

ActionRender::ActionRender(Entity* parent) :
	mParent(parent)
{
	AttachToParent();
}

ActionRender::ActionRender(RTTI::IdType type, Entity* parent) :
	Action(type),
	mParent(parent)
{
	AttachToParent();
}

const Vector<Attributed::Signature> ActionRender::Signatures()
{
	return Vector<Attributed::Signature>();
}

void ActionRender::AttachToParent()
{
	assert(mParent != nullptr);
	mParent->Adopt(*this, Entity::ACTION_TABLE_KEY);
}