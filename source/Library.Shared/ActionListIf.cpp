#include "pch.h"
#include "ActionListIf.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionListIf);

ActionListIf::ActionListIf(RTTI::IdType type) : ActionList(type)
{}

void ActionListIf::Update(WorldState& state)
{
	Action::Update(state);

	Datum& actionList = GetActionList();
	assert(actionList.Size() > 0 && actionList.Size() < 3);
	if (mCondition != 0)
	{
		assert(actionList.AsTable(0).Is(Action::TypeIdClass()));
		Action& action = static_cast<Action&>(actionList.AsTable(0));
		action.Update(state);
	}
	else if (actionList.Size() > 1)
	{
		assert(actionList.AsTable(1).Is(Action::TypeIdClass()));
		Action& action = static_cast<Action&>(actionList.AsTable(1));
		action.Update(state);
	}
}

gsl::owner<Scope*> ActionListIf::Clone() const
{
	return new ActionListIf(*this);
}

const Vector<Attributed::Signature> ActionListIf::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature(CONDITION_KEY, Datum::DatumType::Integer, 1, offsetof(ActionListIf, mCondition))
	});
}