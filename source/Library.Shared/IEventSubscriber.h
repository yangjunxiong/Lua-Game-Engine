#pragma once
#include "RTTI.h"

namespace GameEngine
{
	class BaseEvent;

	/// <summary>
	/// Interface for event subscriber, all inherited class must implement Notify method
	/// </summary>
	class IEventSubscriber : public RTTI
	{
		RTTI_DECLARATIONS(IEventSubscriber, RTTI);

	public:
		/// <summary>
		/// Event will call this method to send notification to subscribers
		/// </summary>
		/// <param name="event">The event who calls this method. Concrete subscriber needs to check the actual event type to process the event</param>
		virtual void Notify(const BaseEvent& event) = 0;
	};
}
