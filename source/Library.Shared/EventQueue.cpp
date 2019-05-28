#include "pch.h"
#include <algorithm>
#include "EventQueue.h"
#include "GameTime.h"
#include "WorldState.h"
#include "Event.h"
#include <vector>

using namespace GameEngine;
using namespace std;

EventQueue::EventQueue(const GameTime& time) :
	mTime(&time)
{}

void EventQueue::Enqueue(const shared_ptr<BaseEvent>& event, const chrono::milliseconds& delay)
{
	// If same event is already in queue, just return
	auto it = mQueue.Find(event);
	if (it != mQueue.end())
	{
		return;
	}

	// Enqueue the event
	event->SetTime(mTime->CurrentTime(), delay);
	mQueue.PushBack(event);
}

void EventQueue::Dequeue(const std::shared_ptr<BaseEvent>& event)
{
	mQueue.Remove(mQueue.Find(event));
}

void EventQueue::Send(const shared_ptr<BaseEvent>& event) const
{
	event->Deliver();
}

void EventQueue::Update()
{
	// Put all expired events to the last of queue
	// Notice this algorithm will change the content of item in place,
	// if there is a pointer to an item in the queue, that pointer might point to a different event after partition!
	// So never expose the queue item pointer to outside world.
	auto now = mTime->CurrentTime();
	auto bound = partition(mQueue.begin(), mQueue.end(), [&now](shared_ptr<BaseEvent> event)
	{
		return !event->IsExpired(now);
	});

	// Remove the expired events and place them to a temp vector
	for (auto it = bound; it != mQueue.end(); ++it)
	{
		mTempQueue.PushBack(*it);
	}
	mQueue.Remove(bound, mQueue.end());

	// Deliver all expired events
	for (auto it = mTempQueue.begin(); it != mTempQueue.end(); ++it)
	{
		(*it)->Deliver();
	}
	mTempQueue.Clear();
}

void EventQueue::Clear()
{
	mQueue.Clear();
}

bool EventQueue::IsEmpty() const
{
	return mQueue.Size() == 0;
}

size_t EventQueue::Size() const
{
	return mQueue.Size();
}
