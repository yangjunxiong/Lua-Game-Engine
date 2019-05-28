#pragma once
#include "Reaction.h"

namespace GameEngine
{
	/// <summary>
	/// ReactionSword is a generic Reaction that is scriptable from json config file.
	/// This type of object always subscribe to Event<EventMessageAttributed>.
	/// It has "Subtype" as prescribed attribute, so when the event payload also has same subtype, it executes the action list
	/// </summary>
	class ReactionSword : public Reaction
	{
		RTTI_DECLARATIONS(ReactionSword, Reaction);

	public:
		ReactionSword(RTTI::IdType type = ReactionSword::TypeIdClass());
		ReactionSword(const ReactionSword& other);
		ReactionSword(ReactionSword&& other);
		ReactionSword& operator=(const ReactionSword& other) = default;
		ReactionSword& operator=(ReactionSword&& other) = default;
		~ReactionSword();

		virtual gsl::owner<Scope*> Clone() const override;

		virtual void Notify(const BaseEvent& event) override;

		static const Vector<Attributed::Signature> Signatures();

	private:
		std::chrono::time_point<std::chrono::steady_clock> mLastTouchTime;
	};

	DECLARE_FACTORY(ReactionSword, Scope);
}
