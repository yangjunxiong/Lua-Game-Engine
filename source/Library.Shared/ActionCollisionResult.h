#pragma once
#include "Action.h"

namespace GameEngine
{
	class ActionCollisionResult : public Action
	{
		RTTI_DECLARATIONS(ActionCollisionResult, Action);

	public:
		ActionCollisionResult(RTTI::IdType type = ActionCollisionResult::TypeIdClass());

		virtual void Update(class WorldState& state) override;

		virtual ~ActionCollisionResult() override = default;

		virtual gsl::owner<Scope*> Clone() const override;

		static const Vector<Attributed::Signature> Signatures();

	private:
		RTTI* mOnHitEntity1;
		RTTI* mOnHitEntity2;

	};

	DECLARE_FACTORY(ActionCollisionResult, Scope);
}