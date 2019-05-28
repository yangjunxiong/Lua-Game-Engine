#include "pch.h"
#include "ActionUpdateHealthUI.h"

namespace GameEngine
{
	ActionUpdateHealthUI::ActionUpdateHealthUI()
	{
	}

	ActionUpdateHealthUI::~ActionUpdateHealthUI()
	{
	}

	void ActionUpdateHealthUI::Update(WorldState& state)
	{
		Action::Update(state);
		

	}

	gsl::owner<Scope*> ActionUpdateHealthUI::Clone() const
	{
		return new ActionUpdateHealthUI(*this);
	}

	const Vector<Attributed::Signature> ActionUpdateHealthUI::Signatures()
	{
		return Vector<Attributed::Signature>();
	}

}