#pragma once
#include "Action.h"
#include "IEventSubscriber.h"

namespace GameEngine
{
	/// <summary>
	/// A Reaction is both an ActionList and an IEventSubscriber, so that it can subscribe to event and execute a list of actions
	/// in response to the Notify() method
	/// </summary>
	class Reaction : public ActionList, public IEventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList);

	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		/// <param name="type">Type to look for signature list</param>
		Reaction(RTTI::IdType type = Reaction::TypeIdClass());

		/// <summary>
		/// Override Update method of ActionList so this one does nothing
		/// </summary>
		/// <param name="state">WorldState object</param>
		virtual void Update(WorldState& state) override;

		/// <summary>
		/// Interface for Clone. To be implemented in derived class
		/// </summary>
		/// <returns>A clone of this reaction object</returns>
		virtual gsl::owner<Scope*> Clone() const override = 0;

		/// <summary>
		/// Interface for event notify. To be implemented in derived class
		/// </summary>
		/// <param name="event">The event object that triggers the notify</param>
		virtual void Notify(const BaseEvent& event) override = 0;

		/// <summary>
		/// List of signatures for this type. This just returns the signature list of ActionList
		/// </summary>
		/// <returns>Signature list of this type</returns>
		static const Vector<Attributed::Signature> Signatures();
	};
}
