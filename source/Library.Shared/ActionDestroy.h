#pragma once
#include "Action.h"

namespace GameEngine
{
	class ActionDestroy final : public Action
	{
		RTTI_DECLARATIONS(ActionDestroy, Action);

	public:
		ActionDestroy();
		virtual void Update(WorldState& state) override;
		virtual gsl::owner<ActionDestroy*> Clone() const override;
		static const Vector<Attributed::Signature> Signatures();

	private:
		std::string mTarget;
	};

	DECLARE_FACTORY(ActionDestroy, Scope);
}