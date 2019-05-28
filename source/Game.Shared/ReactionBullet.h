#pragma once
#include "Reaction.h"

namespace GameEngine
{
	/// <summary>
	/// ReactionSword is a generic Reaction that is scriptable from json config file.
	/// This type of object always subscribe to Event<EventMessageAttributed>.
	/// It has "Subtype" as prescribed attribute, so when the event payload also has same subtype, it executes the action list
	/// </summary>
	class ReactionBullet : public Reaction
	{
		RTTI_DECLARATIONS(ReactionBullet, Reaction);

	public:
		ReactionBullet(RTTI::IdType type = ReactionBullet::TypeIdClass());
		ReactionBullet(const ReactionBullet& other);
		ReactionBullet(ReactionBullet&& other);
		ReactionBullet& operator=(const ReactionBullet& other) = default;
		ReactionBullet& operator=(ReactionBullet&& other) = default;
		~ReactionBullet();

		virtual gsl::owner<Scope*> Clone() const override;

		virtual void Notify(const BaseEvent& event) override;

		static const Vector<Attributed::Signature> Signatures();
	};

	DECLARE_FACTORY(ReactionBullet, Scope);
}
