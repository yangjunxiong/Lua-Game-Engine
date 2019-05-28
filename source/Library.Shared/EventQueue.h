#pragma once
#include "vector.h"
#include "HashMap.h"
#include <chrono>
#include <memory>

namespace GameEngine
{
	class BaseEvent;
	class GameTime;

	/// <summary>
	/// EventQueue is a queue that exists in world object, in each update EventQueue will process events and dispatch them
	/// </summary>
	class EventQueue final
	{
	public:
		/// <summary>
		/// Constructor that takes a GameTime object for delay event purpose
		/// </summary>
		/// <param name="time">A GameTime object reference</param>
		EventQueue(const GameTime& time);

		/// <summary>
		/// Add an event to the queue.
		/// If the EventQueue is not updating, this is a synchronous call and will enqueue the event immediately.
		/// If the EventQueue is updating, this will add the incoming event to a pending list and enqueue it after updating finishes.
		/// </summary>
		/// <param name="event">Shared pointer to the evnet object</param>
		/// <param name="delay">Delay in milliseconds for the event to trigger</param>
		void Enqueue(const std::shared_ptr<BaseEvent>& event, const std::chrono::milliseconds& delay = std::chrono::milliseconds(0));
		
		/// <summary>
		/// Remove an existing event from the queue.
		/// If the EventQueue is not updating, this is a synchronous call and will dequeue the event immediately.
		/// If the EventQueue is updating, this will add the incoming event to a pending list and dequeue it after updating finishes.
		/// </summary>
		/// <param name="event">Shared pointer to the event object</param>
		void Dequeue(const std::shared_ptr<BaseEvent>& event);

		/// <summary>
		/// Immediately dispatch the event
		/// </summary>
		/// <param name="event">The event to dispatch</param>
		void Send(const std::shared_ptr<BaseEvent>& event) const;

		/// <summary>
		/// Iterate through the queue, dispatch all events which finished delay and remove them from the queue
		/// </summary>
		void Update();

		/// <summary>
		/// Remove all events from the queue.
		/// This doesn't handle the situation where the subscriber calls Clear during updating, as EventQueue object is supposed to live
		/// in World object and the outside code can only have the const version of it
		/// </summary>
		void Clear();

		/// <summary>
		/// Check if the queue is empty
		/// </summary>
		/// <returns>True if the queue is empty, False otherwise</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Get the number of events in the queue
		/// </summary>
		/// <returns>Number of events in the queue</returns>
		size_t Size() const;
		
	private:
		/// <summary>
		/// Global game time, use this to check event delay
		/// </summary>
		const GameTime* mTime;

		/// <summary>
		/// Event queue
		/// </summary>
		Vector<std::shared_ptr<BaseEvent>> mQueue;

		/// <summary>
		/// Temporary queue for delivering expired events
		/// </summary>
		Vector<std::shared_ptr<BaseEvent>> mTempQueue;
	};
}
