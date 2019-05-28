#pragma once
#include "ActionCollision.h"

namespace GameEngine
{
	/// <summary>
	/// ActionCircleCollision creates and enqueus an Event<EventMessageAttributed> to the eventqueue.
	/// It will pass the subtype to the payload, and use the delay as enqueue delay
	/// </summary>
	class ActionCircleCollision final : public ActionCollision
	{
		RTTI_DECLARATIONS(ActionCircleCollision, ActionCollision);

	public:
		ActionCircleCollision(RTTI::IdType type = ActionCircleCollision::TypeIdClass());

		virtual void Update(class WorldState& state) override;

		virtual void InitBody() override;
		
		float GetRadius() const;

		virtual ~ActionCircleCollision() override = default;

		virtual gsl::owner<Scope*> Clone() const override;

		static const Vector<Attributed::Signature> Signatures();

	private:
		float mRadius = 0.2f;
	};

	DECLARE_FACTORY(ActionCircleCollision, Scope);
}
