#include "pch.h"
#include "ActionCreateAction.h"
#include "Entity.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionCreateAction);

ActionCreateAction::ActionCreateAction(RTTI::IdType type) : Action(type)
{}

void ActionCreateAction::Update(WorldState& state)
{
	Action::Update(state);

	Scope* parent = GetParent();
	assert(parent != nullptr);
	if (parent->Is(Entity::TypeIdClass()))
	{
		Entity* entity = static_cast<Entity*>(parent);
		entity->CreateAction(mClass, mInstanceName);
	}
	else if (parent->Is(ActionList::TypeIdClass()))
	{
		ActionList* list = static_cast<ActionList*>(parent);
		list->CreateAction(mClass, mInstanceName);
	}
	else
	{
		assert(false);  // Parent type is not correct
	}
}

gsl::owner<Scope*> ActionCreateAction::Clone() const
{
	return new ActionCreateAction(*this);
}

const Vector<Attributed::Signature> ActionCreateAction::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature(CLASS_KEY, Datum::DatumType::String, 1, offsetof(ActionCreateAction, mClass)),
		Signature(INSTANCE_NAME_KEY, Datum::DatumType::String, 1, offsetof(ActionCreateAction, mInstanceName))
	});
}
