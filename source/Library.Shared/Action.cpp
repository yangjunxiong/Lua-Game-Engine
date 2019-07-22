#include "pch.h"
#include "Action.h"
#include "Entity.h"
#include "WorldState.h"

using namespace GameEngine;
using namespace std;

RTTI_DEFINITIONS(Action);

Action::Action(Entity* parent) :
	Attributed(Action::TypeIdClass()),
	mParent(parent)
{
	parent->Adopt(*this, Entity::ACTION_TABLE_KEY);
}

Action::Action(Entity* parent, RTTI::IdType type) :
	Attributed(type),
	mParent(parent)
{
	parent->Adopt(*this, Entity::ACTION_TABLE_KEY);
}

void Action::Update(WorldState& state)
{
	state.mAction = this;
}

const std::string& Action::Name() const
{
	return mName;
}

void Action::SetName(const std::string& name)
{
	mName = name;
}

const Vector<Attributed::Signature> Action::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Name", Datum::DatumType::String, 1, offsetof(Action, mName))
	});
}
