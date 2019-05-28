#include "pch.h"
#include "ActionIncrement.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionIncrement);

ActionIncrement::ActionIncrement(RTTI::IdType type) : Action(type)
{}

void ActionIncrement::Update(WorldState& state)
{
	Action::Update(state);

	if (!mTargetName.empty())
	{
		auto [datum, scope] = Search(mTargetName);
		if (datum != nullptr)
		{
			if (datum->Type() == Datum::DatumType::Integer)
			{
				int& value = datum->AsInt();
				value += static_cast<int>(mStep);
			}
			else if (datum->Type() == Datum::DatumType::Float)
			{
				float& value = datum->AsFloat();
				value += mStep;
			}
		}
	}
}

gsl::owner<Scope*> ActionIncrement::Clone() const
{
	return new ActionIncrement(*this);
}

const Vector<Attributed::Signature> ActionIncrement::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature(TARGET_KEY, Datum::DatumType::String, 1, offsetof(ActionIncrement, mTargetName)),
		Signature(STEP_KEY, Datum::DatumType::Float, 1, offsetof(ActionIncrement, mStep))
	});
}
