#include "pch.h"
#include "ActionRender.h"
#include "Entity.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionRender);

ActionRender::ActionRender(Entity* parent) :
	Action(parent, ActionRender::TypeIdClass())
{}

ActionRender::ActionRender(RTTI::IdType type, Entity* parent) :
	Action(parent, type)
{}

const Vector<Attributed::Signature> ActionRender::Signatures()
{
	return Vector<Attributed::Signature>();
}
