#include "pch.h"
#include "FooSubscriber.h"
#include "Event.h"
#include "EventQueue.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(FooSubscriber);

FooSubscriber::~FooSubscriber()
{
	Event<int>::Unsubscribe(*this);
	Event<float>::Unsubscribe(*this);
	Event<string>::Unsubscribe(*this);
	Event<Foo>::Unsubscribe(*this);
}

void FooSubscriber::Notify(const BaseEvent& event)
{
	// Do the default counting
	RTTI::IdType type = event.TypeIdInstance();
	if (type == Event<int>::TypeIdClass())
	{
		++mIntEventCount;
		mLastInt = static_cast<const Event<int>&>(event).Message();
	}
	else if (type == Event<float>::TypeIdClass())
	{
		++mFloatEventCount;
		mLastFloat = static_cast<const Event<float>&>(event).Message();
	}
	else if (type == Event<string>::TypeIdClass())
	{
		++mStringEventCount;
		mLastString = static_cast<const Event<string>&>(event).Message();
	}
	else if (type == Event<Foo>::TypeIdClass())
	{
		++mFooEventCount;
		mLastFoo = static_cast<const Event<Foo>&>(event).Message();
	}
	else if (type == Event<EventMessageAttributed>::TypeIdClass())
	{
		++mGenericEventCount;
		mLastMessage = static_cast<const Event<EventMessageAttributed>&>(event).Message();
	}

	// Change the event queue during update
	if (mEventQueue != nullptr)
	{
		if (mEnqueueEvent)
		{
			mEventQueue->Enqueue(mEnqueueEvent, mEnqueueDelay);
			mEventQueue->Enqueue(mEnqueueEvent, mEnqueueDelay);
			mEventQueue->Enqueue(mEnqueueEvent, mEnqueueDelay);
		}
		if (mDequeueEvent)
		{
			mEventQueue->Dequeue(mDequeueEvent);
			mEventQueue->Dequeue(mDequeueEvent);
			mEventQueue->Dequeue(mDequeueEvent);
		}
	}
	
	// Change the event during update
	if (mAddSubscriber != nullptr)
	{
		Event<int>::Subscribe(*mAddSubscriber);
		Event<int>::Subscribe(*mAddSubscriber);
		Event<int>::Subscribe(*mAddSubscriber);
	}
	if (mRemoveSubscriber != nullptr)
	{
		Event<int>::Unsubscribe(*mRemoveSubscriber);
		Event<int>::Unsubscribe(*mRemoveSubscriber);
		Event<int>::Unsubscribe(*mRemoveSubscriber);
	}
	if (mNeedUnsubscribeAll)
	{
		Event<int>::UnsubscribeAll();
		Event<int>::UnsubscribeAll();
		Event<int>::UnsubscribeAll();
	}
}
