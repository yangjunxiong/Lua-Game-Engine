#pragma once
#include "Action.h"
#include "Box2D/Box2D.h"

namespace GameEngine
{
	class ActionCollision : public Action
	{
		RTTI_DECLARATIONS(ActionCollision, Action);

	public:
		ActionCollision(RTTI::IdType type = ActionCollision::TypeIdClass());

		virtual void Update(class WorldState& state) override;

		virtual void InitBody() = 0;

		virtual ~ActionCollision() override;

		virtual gsl::owner<Scope*> Clone() const override = 0;

		static const Vector<Attributed::Signature> Signatures();

	protected:
		b2Body* mBody = nullptr;
		int32_t mIsStatic = 1;
		int32_t mIsSensor = 0;
		bool mIsInitialized = false;
	};
}