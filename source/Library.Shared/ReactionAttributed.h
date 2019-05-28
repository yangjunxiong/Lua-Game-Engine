#pragma once
#include "Reaction.h"

namespace GameEngine
{
	/// <summary>
	/// ReactionAttributed is a generic Reaction that is scriptable from json config file.
	/// This type of object always subscribe to Event<EventMessageAttributed>.
	/// It has "Subtype" as prescribed attribute, so when the event payload also has same subtype, it executes the action list
	/// </summary>
	class ReactionAttributed : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);

	public:
		/// <summary>
		/// Default constructor. Will subscribe this object to the event
		/// </summary>
		/// <param name="type">Type to find signature list</param>
		ReactionAttributed(RTTI::IdType type = ReactionAttributed::TypeIdClass());

		/// <summary>
		/// Copy contructor. Does the same work as default one, and then subscribe this object to the event
		/// </summary>
		/// <param name="other">Other ReactionAttributed to copy from</param>
		ReactionAttributed(const ReactionAttributed& other);

		/// <summary>
		/// Move contructor. Does the same work as default one, and then subscribe this object to the event
		/// </summary>
		/// <param name="other">Other ReactionAttributed to move from</param>
		ReactionAttributed(ReactionAttributed&& other);

		/// <summary>
		/// Copy assignment. Does the same work as default one, and then subscribe this object to the event
		/// </summary>
		/// <param name="other">Other ReactionAttributed to copy from</param>
		ReactionAttributed& operator=(const ReactionAttributed& other) = default;

		/// <summary>
		/// Move assignment. Does the same work as default one, and then subscribe this object to the event
		/// </summary>
		/// <param name="other">Other ReactionAttributed to move from</param>
		ReactionAttributed& operator=(ReactionAttributed&& other) = default;

		/// <summary>
		/// Destructor, unsubscribe this object from event
		/// </summary>
		~ReactionAttributed();

		/// <summary>
		/// Set the subtype of this object
		/// </summary>
		/// <param name="type">New subtype</param>
		void SetSubtype(const std::string& type);

		/// <summary>
		/// Get the subtype of this object
		/// </summary>
		/// <returns>Subtype of this object</returns>
		const std::string& GetSubtype() const;

		/// <summary>
		/// Get a clone of this object
		/// </summary>
		/// <returns>A clone of this object</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Implement IEventSubsriber interface.
		/// Check the incoming event is Event<EventMessageAttributed>.
		/// If two subtypes match, then copy the parameters from payload and invoke action list of this object
		/// </summary>
		/// <param name="event">Incoming event</param>
		virtual void Notify(const BaseEvent& event) override;

		/// <summary>
		/// Get the signature list of this type
		/// </summary>
		/// <returns>Signature list of this type</returns>
		static const Vector<Attributed::Signature> Signatures();

	protected:
		/// <summary>
		/// Subtype to match with event payload
		/// </summary>
		std::string mSubtype;
	};

	DECLARE_FACTORY(ReactionAttributed, Scope);
}
