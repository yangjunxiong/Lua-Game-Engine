#pragma once
#include "ActionCollision.h"

namespace GameEngine
{
	/// <summary>
	/// ActionTriangleCollision creates and enqueus an Event<EventMessageAttributed> to the eventqueue.
	/// It will pass the subtype to the payload, and use the delay as enqueue delay
	/// </summary>
	class ActionTriangleCollision final : public ActionCollision
	{
		RTTI_DECLARATIONS(ActionTriangleCollision, ActionCollision);

	public:
		ActionTriangleCollision(RTTI::IdType type = ActionTriangleCollision::TypeIdClass());

		virtual void Update(class WorldState& state) override;

		virtual void InitBody() override;

		virtual ~ActionTriangleCollision() override = default;

		virtual gsl::owner<Scope*> Clone() const override;

		static const Vector<Attributed::Signature> Signatures();

	private:
		glm::vec4 mVertices[3];
	};

	DECLARE_FACTORY(ActionTriangleCollision, Scope);
}
