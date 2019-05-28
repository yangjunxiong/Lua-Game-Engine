#pragma once
#include "ActionCollision.h"

namespace GameEngine
{
	/// <summary>
	/// ActionBoxCollision creates and enqueus an Event<EventMessageAttributed> to the eventqueue.
	/// It will pass the subtype to the payload, and use the delay as enqueue delay
	/// </summary>
	class ActionBoxCollision final : public ActionCollision
	{
		RTTI_DECLARATIONS(ActionBoxCollision, ActionCollision);

	public:
		ActionBoxCollision(RTTI::IdType type = ActionBoxCollision::TypeIdClass());

		virtual void Update(class WorldState& state) override;

		virtual void InitBody() override;

		float GetWidth() const;

		float GetLength() const;

		virtual ~ActionBoxCollision() override = default;

		virtual gsl::owner<Scope*> Clone() const override;

		static const Vector<Attributed::Signature> Signatures();

	private:
		float mWidth = 0.0f;
		float mLength = 0.0f;
	};

	DECLARE_FACTORY(ActionBoxCollision, Scope);
}
