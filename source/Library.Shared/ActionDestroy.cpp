#include "pch.h"
#include "ActionDestroy.h"
#include "Entity.h"
#include "Sector.h"
#include "Action.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionDestroy);

ActionDestroy::ActionDestroy() :
	Action(ActionDestroy::TypeIdClass())
{}

void ActionDestroy::Update(WorldState& state)
{
	assert(("ActionDestroy: World is nullptr", state.GetWorld() != nullptr));

	// Handle the last [] to differentiate whether to use index or the name of Entity/Sector/Action
	size_t end = mTarget.find_last_of(']');
	bool useIndex = true;
	size_t index = 0;
	string name;
	string searchString;
	if (end != string::npos)
	{
		size_t start = mTarget.find_last_of('[');
		string sub = mTarget.substr(start + 1, end - start - 1);
		searchString = mTarget.substr(0, start);
		try
		{
			index = stoi(sub);
		}
		catch (const invalid_argument&)
		{
			name = sub;
			useIndex = false;
		}
	}
	else
	{
		searchString = mTarget;
	}

	// Find the datum and remove the scope from the datum
	auto[datum, scope] = Search(searchString);
	assert(("ActionDestroy: can't find the containing datum", datum != nullptr && scope != nullptr));
	if (useIndex)
	{
		assert(("ActionDestroy: Item index out of range", datum->Size() > index));
		state.GetWorld()->Destroy(static_cast<Attributed&>(datum->AsTable(index)));
	}
	else
	{
		for (size_t i = 0; i < datum->Size(); ++i)
		{
			Scope& target = datum->AsTable(i);
			if (target.Is(Sector::TypeIdClass()))
			{
				Sector& sector = static_cast<Sector&>(target);
				if (sector.Name() == name)
				{
					state.GetWorld()->Destroy(sector);
					break;
				}
			}
			else if (target.Is(Entity::TypeIdClass()))
			{
				Entity& entity = static_cast<Entity&>(target);
				if (entity.Name() == name)
				{
					state.GetWorld()->Destroy(entity);
					break;
				}
			}
			else if (target.Is(Action::TypeIdClass()))
			{
				Action& action = static_cast<Action&>(target);
				if (action.Name() == name)
				{
					state.GetWorld()->Destroy(action);
					break;
				}
			}
		}
	}
}

gsl::owner<ActionDestroy*> ActionDestroy::Clone() const
{
	return new ActionDestroy(*this);
}

const Vector<Attributed::Signature> ActionDestroy::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Target", Datum::DatumType::String, 1, offsetof(ActionDestroy, mTarget))
	});
}