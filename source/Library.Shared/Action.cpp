#include "pch.h"
#include "Action.h"
#include "Entity.h"
#include "WorldState.h"

using namespace GameEngine;
using namespace std;

RTTI_DEFINITIONS(Action);
RTTI_DEFINITIONS(ActionList);

Action::Action(RTTI::IdType type) : Attributed(type)
{}

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

Action* Action::SearchAction(const std::string& name)
{
	if (mName == name)
	{
		return this;
	}

	Action* current = this;
	while (current->GetParent() != nullptr)
	{
		// Get datum storing action list in parent
		Scope* parent = current->GetParent();
		Datum* actions = nullptr;
		if (parent->Is(ActionList::TypeIdClass()))
		{
			actions = &static_cast<ActionList*>(parent)->GetActionList();
		}
		else if (parent->Is(Entity::TypeIdClass()))
		{
			actions = &static_cast<Entity*>(parent)->Actions();
		}

		// Go over all actions in parent
		if (actions != nullptr)
		{
			for (size_t i = 0; i < actions->Size(); ++i)
			{
				Action& action = static_cast<Action&>(actions->AsTable(i));
				if (action.Name() == name)
				{
					return &action;
				}
			}
		}

		// We end the search when parent is Entity
		if (parent->Is(ActionList::TypeIdClass()))
		{
			current = static_cast<ActionList*>(parent);
		}
		else
		{
			break;
		}
	}

	return nullptr;
}

const Vector<Attributed::Signature> Action::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Name", Datum::DatumType::String, 1, offsetof(Action, mName))
	});
}

ActionList::ActionList(RTTI::IdType type) : Action(type)
{
	assert(mDatumPointers[ACTION_LIST_TABLE_INDEX]->first == ACTION_LIST_TABLE_KEY);
}

Datum& ActionList::GetActionList()
{
	return (*this)[ACTION_LIST_TABLE_INDEX];
}

const Datum& ActionList::GetActionList() const
{
	return (*this)[ACTION_LIST_TABLE_INDEX];
}

const Vector<Attributed::Signature> ActionList::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature(ACTION_LIST_TABLE_KEY, Datum::DatumType::Table, 0)
	});
}

void ActionList::Update(WorldState& state)
{
	Action::Update(state);

	Datum& actionList = GetActionList();
	for (size_t i = 0; i < actionList.Size(); ++i)
	{
		assert(actionList.AsTable(i).Is("Action"));
		Action& action = static_cast<Action&>(actionList.AsTable(i));
		action.Update(state);
	}
}

Action* ActionList::CreateAction(const std::string& className, const std::string& instanceName)
{
	Scope* product = Factory<Scope>::Create(className);
	assert(product != nullptr && product->Is(Action::TypeIdClass()));
	Action* action = static_cast<Action*>(product);
	action->SetName(instanceName);
	Adopt(*action, ACTION_LIST_TABLE_KEY);
	return action;
}

gsl::owner<Scope*> ActionList::Clone() const
{
	return new ActionList(*this);
}
