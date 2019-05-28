#include "pch.h"
#include "ActionDestroyAction.h"
#include "WorldState.h"
#include "World.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionDestroyAction);

ActionDestroyAction::ActionDestroyAction(RTTI::IdType type) : Action(type)
{}

void ActionDestroyAction::Update(WorldState& state)
{
	Action::Update(state);

	Action* target = SearchAction(mTarget);
	if (target != nullptr)
	{
		World* world = state.GetWorld();
		assert(world != nullptr);
		world->Destroy(*target);
	}
}

gsl::owner<Scope*> ActionDestroyAction::Clone() const
{
	return new ActionDestroyAction(*this);
}

const Vector<Attributed::Signature> ActionDestroyAction::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature(TARGET_KEY, Datum::DatumType::String, 1, offsetof(ActionDestroyAction, mTarget))
	});
}

