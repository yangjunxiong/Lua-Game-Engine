#pragma once
#include "ActionCollision.h"

namespace GameEngine
{
	/// <summary>
	/// ActionPolygonCollision creates and enqueus an Event<EventMessageAttributed> to the eventqueue.
	/// It will pass the subtype to the payload, and use the delay as enqueue delay
	/// </summary>
	class ActionPolygonCollision final : public ActionCollision
	{
		RTTI_DECLARATIONS(ActionPolygonCollision, ActionCollision);

	public:
		ActionPolygonCollision(RTTI::IdType type = ActionPolygonCollision::TypeIdClass());

		//virtual void Update(class WorldState& state) override;

		virtual void InitBody() override;

		virtual ~ActionPolygonCollision() override = default;

		virtual gsl::owner<Scope*> Clone() const override;

		static const Vector<Attributed::Signature> Signatures();

	private:
		enum PolygonType
		{
			Triangle,
			Box
		};

		int32_t mType;
		glm::vec4 mVertices[4];
	};

	DECLARE_FACTORY(ActionPolygonCollision, Scope);
}
