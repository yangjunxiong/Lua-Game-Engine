#pragma once
#include "Entity.h"
#include "IEventSubscriber.h"

namespace GameEngine
{
	class HealthUI final : public Entity, public IEventSubscriber
	{
		RTTI_DECLARATIONS(HealthUI, Entity);

	public:
		HealthUI();
		~HealthUI();
		virtual void Start(WorldState& state) override;
		virtual void Notify(const BaseEvent& event) override;
		virtual gsl::owner<Scope*> Clone() const override;
		static const Vector<Attributed::Signature> Signatures();

	private:
		int mHealthPoint = 0;
		int mPlayerId = 0;
		SList<class ActionRender*> mRenders;
	};

	DECLARE_FACTORY(HealthUI, Scope);
}
