#pragma once
using namespace GameEngine;
using namespace std;

template <typename T>
RTTI_DEFINITIONS(Event<T>);

template <typename T>
BaseEvent::SubscriberList Event<T>::sSubscriberList;

template <typename T>
Event<T>::Event(const T& message) :
	BaseEvent(sSubscriberList),
	mMessage(message)
{}

template <typename T>
Event<T>::Event(T&& message) :
	BaseEvent(sSubscriberList),
	mMessage(move(message))
{}

template <typename T>
const T& Event<T>::Message() const
{
	return mMessage;
}

template <typename T>
void Event<T>::Subscribe(IEventSubscriber& subscriber)
{
	if (sSubscriberList.mUpdateCount > 0)
	{
		sSubscriberList.mPendingList.PushBack(make_tuple(&subscriber, true));
	}
	else
	{
		if (sSubscriberList.mSubscribers.Find(&subscriber) == sSubscriberList.mSubscribers.end())
		{
			sSubscriberList.mSubscribers.PushBack(&subscriber);
		}
	}
}

template <typename T>
void Event<T>::Unsubscribe(IEventSubscriber& subscriber)
{
	if (sSubscriberList.mUpdateCount > 0)
	{
		sSubscriberList.mPendingList.PushBack(make_tuple(&subscriber, false));
	}
	else
	{
		sSubscriberList.mSubscribers.Remove(sSubscriberList.mSubscribers.Find(&subscriber));
	}
}

template <typename T>
void Event<T>::UnsubscribeAll()
{
	if (sSubscriberList.mUpdateCount > 0)
	{
		sSubscriberList.mNeedClearAll = true;
	}
	else
	{
		sSubscriberList.mSubscribers.Destroy();
		sSubscriberList.mPendingList.Destroy();
	}
}