#pragma once
#include "RTTI.h"
#include "vector.h"
#include <chrono>

namespace GameEngine
{
	class IEventSubscriber;

	/// <summary>
	/// Abstract base event class. This class should not be inherited directly by classes other than templated Event
	/// </summary>
	class BaseEvent : public RTTI
	{
		RTTI_DECLARATIONS(BaseEvent, RTTI);

	public:
		using TimePoint = std::chrono::high_resolution_clock::time_point;
		using TimeDuration = std::chrono::milliseconds;

		/// <summary>
		/// Broadcast this event and call Notify method on all its subscribers
		/// </summary>
		void Deliver();

		/// <summary>
		/// Get the time point when this event is enqueued to the world event queue
		/// </summary>
		/// <returns>The time point when this event is enqueued</returns>
		const TimePoint& TimeEnqueued() const;

		/// <summary>
		/// Get the delay in milliseconds for the event to trigger in event queue
		/// </summary>
		/// <returns>The delay to trigger the event in milliseconds</returns>
		const TimeDuration& Delay() const;

		/// <summary>
		/// Check if given time has passed the trigger time for the event (enqueuedTime + delay)
		/// </summary>
		/// <param name="now">The time point to check against</param>
		/// <returns>True if now has passed the trigger time, False otherwise</returns>
		bool IsExpired(const TimePoint& now) const;

		/// <summary>
		/// Set the enqueue time and delay for this event
		/// </summary>
		/// <param name="time">Enqueue time point</param>
		/// <param name="delay">Delay time duration</param>
		void SetTime(const TimePoint& time, const TimeDuration& delay = TimeDuration(0));

		/// <summary>
		/// Set the delay of this event
		/// </summary>
		/// <param name="newDelay">New delay duration</param>
		void SetDelay(const TimeDuration& newDelay);

	protected:
		/// <summary>
		/// A helper structure to keep track of the subscriber list and especially handle add/remove during subscriber Notify()
		/// </summary>
		struct SubscriberList
		{
			/// <summary>
			/// Current working list of subscribers
			/// </summary>
			Vector<IEventSubscriber*> mSubscribers;

			/// <summary>
			/// If Subscribe/Unsubscribe method is invoked by some subscribers' Notify method, the subscriber goes in here
			/// True indicates pending add, false indicates pending remove
			/// </summary>
			Vector<std::tuple<IEventSubscriber*, bool>> mPendingList;

			/// <summary>
			/// Whether the event is iterating through all subscribers.
			/// 0 means the event is not iterating
			/// </summary>
			size_t mUpdateCount = 0;

			/// <summary>
			/// Whether there is a call to UnsubscribeAll during iterating through all subscribers
			/// </summary>
			bool mNeedClearAll = false;
		};

		/// <summary>
		/// Constructor that takes a reference to a subscriber list. Templated event will pass in its static subsriber list so BaseEvent can store it.
		/// </summary>
		/// <param name="subsriberList">static Subscriber list in templated event</param>
		BaseEvent(SubscriberList& subsriberList);

		/// <summary>
		/// Pointer to the subscriber list for this event object
		/// </summary>
		SubscriberList* mSubscriberList = nullptr;

		/// <summary>
		/// Time point when this event is enqueued to the world event queue
		/// </summary>
		TimePoint mEnqueueTime;

		/// <summary>
		/// Delay in milliseconds before actually dispatch the event in queue
		/// </summary>
		TimeDuration mDelay;
	};

	/// <summary>
	/// Actual event type. Type argument is the payload data that the event object carries to its subsribers
	/// </summary>
	template <typename T>
	class Event final : public BaseEvent
	{
		RTTI_DECLARATIONS(Event<T>, BaseEvent);

	public:
		/// <summary>
		/// Constructor to copy from a message object
		/// </summary>
		/// <param name="message">The message object to copy from</param>
		Event(const T& message);

		/// <summary>
		/// Constructor to move from a message object
		/// </summary>
		/// <param name="message">The message object to move from</param>
		Event(T&& message);

		/// <summary>
		/// Default copy constructor
		/// </summary>
		/// <param name="other">Other event object to copy from</param>
		Event(const Event& other) = default;

		/// <summary>
		/// Default move constructor
		/// </summary>
		/// <param name="other">Other event object to move from</param>
		Event(Event&& other) = default;

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="other">Other event object to copy from</param>
		/// <returns>This event after copying</returns>
		Event& operator=(const Event& other) = default;

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="other">Other event object to move from</param>
		/// <returns>This event after moving</returns>
		Event& operator=(Event&& other) = default;

		/// <summary>
		/// Get the message object (i.e. payload data)
		/// </summary>
		/// <returns>The message object</returns>
		const T& Message() const;

		/// <summary>
		/// Add an event subscriber to the subscriber list for this event type.
		/// If this is called when the event is iterating through all of its subscribers, it will be asynchronous operation.
		/// The new subscriber will be added to a pending list and added after the iteration ends
		/// </summary>
		/// <param name="subscriber">Subscriber instance</param>
		static void Subscribe(IEventSubscriber& subscriber);

		/// <summary>
		/// Remove an event subscriber to the subscriber list for this event type.
		/// If this is called when the event is iterating through all of its subscribers, it will be asynchronous operation.
		/// The new subscriber will be added to a pending list and removed after the iteration ends
		/// </summary>
		/// <param name="subscriber">Subscriber instance</param>
		static void Unsubscribe(IEventSubscriber& subscriber);

		/// <summary>
		/// Remove all subscribers from the subscriber list for this event type.
		/// If this is called when the event is iterating through all of its subscribers, it will be asynchronous operation.
		/// The subscriber list will be cleared after the iteration ends
		/// </summary>
		static void UnsubscribeAll();

	protected:
		/// <summary>
		/// Payload data
		/// </summary>
		T mMessage;

		/// <summary>
		/// Subscriber list for this event type
		/// </summary>
		static SubscriberList sSubscriberList;
	};
}

#include "Event.inl"
