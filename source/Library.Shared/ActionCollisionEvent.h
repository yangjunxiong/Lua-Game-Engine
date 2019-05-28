#pragma once
#include "ActionEvent.h"
#include "HashMap.h"

namespace GameEngine
{
	class ActionCollisionEvent : public ActionEvent
	{
		RTTI_DECLARATIONS(ActionCollisionEvent, ActionEvent);

	public:
		ActionCollisionEvent(RTTI::IdType type = ActionCollisionEvent::TypeIdClass());

		virtual void Update(class WorldState& state) override;

		void UpdateTransform(const std::chrono::milliseconds& timeStep);

		virtual ~ActionCollisionEvent() override;

		virtual gsl::owner<Scope*> Clone() const override;

		static const Vector<Attributed::Signature> Signatures();
		
	private:
		HashMap<Entity*, glm::vec4> mPositionMap;

	};

	DECLARE_FACTORY(ActionCollisionEvent, Scope);
}