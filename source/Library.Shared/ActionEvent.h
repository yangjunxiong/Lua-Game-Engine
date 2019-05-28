#pragma once
#include "Action.h"
#include "Factory.h"

namespace GameEngine
{
	/// <summary>
	/// ActionEvent creates and enqueus an Event<EventMessageAttributed> to the eventqueue.
	/// It will pass the subtype to the payload, and use the delay as enqueue delay
	/// </summary>
	class ActionEvent : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action);

	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		/// <param name="type">Type to look for signature list</param>
		ActionEvent(RTTI::IdType type = ActionEvent::TypeIdClass());

		/// <summary>
		/// Create and enqueue the event with given subtype and delay.
		/// Will also copy all auxliary attributes in this action to the message
		/// </summary>
		/// <param name="state">WorldState object</param>
		virtual void Update(class WorldState& state) override;

		/// <summary>
		/// Get a clone of this object
		/// </summary>
		/// <returns>A clone of this object</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Get the subtype of event message
		/// </summary>
		/// <returns>Subtype of event message</returns>
		const std::string& GetSubtype() const;

		/// <summary>
		/// Get the delay when enqueueing the event
		/// </summary>
		/// <returns>The delay to enqueue the event</returns>
		const int& GetDelay() const;

		/// <summary>
		/// Set the subtype of event message
		/// </summary>
		/// <param name="subtype">New subtype of event message</param>
		void SetSubtype(const std::string& subtype);

		/// <summary>
		/// Set the delay to enqueue the message
		/// </summary>
		/// <param name="delay">New delay to enqueue the message</param>
		void SetDelay(int delay);

		/// <summary>
		/// Get the list of signatures
		/// </summary>
		/// <returns>List of signatures</returns>
		static const Vector<Attributed::Signature> Signatures();

	protected:
		/// <summary>
		/// Subtype attribute of event message
		/// </summary>
		std::string mSubtype;

		/// <summary>
		/// Delay to enqueue the event
		/// </summary>
		int mDelay = 0;
	};

	DECLARE_FACTORY(ActionEvent, Scope);
}
