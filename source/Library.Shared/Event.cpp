#include "pch.h"
#include "Event.h"
#include "IEventSubscriber.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(BaseEvent);

BaseEvent::BaseEvent(BaseEvent::SubscriberList& subsriberList) :
	mSubscriberList(&subsriberList)
{}

void BaseEvent::Deliver()
{
	// Notify all subscribers
	++mSubscriberList->mUpdateCount;
	for (const auto& subscriber : mSubscriberList->mSubscribers)
	{
		subscriber->Notify(*this);
	}
	--mSubscriberList->mUpdateCount;

	// Handle subscribe/unsubscribe during Notify
	if (mSubscriberList->mUpdateCount == 0)
	{
		if (mSubscriberList->mNeedClearAll)
		{
			mSubscriberList->mSubscribers.Destroy();
			mSubscriberList->mPendingList.Destroy();
			mSubscriberList->mNeedClearAll = false;
		}
		else
		{
			auto& list = mSubscriberList->mSubscribers;
			for (auto& [subscriber, add] : mSubscriberList->mPendingList)
			{
				if (!add)
				{
					list.Remove(mSubscriberList->mSubscribers.Find(subscriber));
				}
				else if (list.Find(subscriber) == list.end())
				{
					list.PushBack(subscriber);
				}
			}
			mSubscriberList->mPendingList.Clear();
		}
	}
}

const BaseEvent::TimePoint& BaseEvent::TimeEnqueued() const
{
	return mEnqueueTime;
}

const BaseEvent::TimeDuration& BaseEvent::Delay() const
{
	return mDelay;
}

bool BaseEvent::IsExpired(const TimePoint& now) const
{
	auto expireTime = mEnqueueTime + mDelay;
	return now >= expireTime;
}

void BaseEvent::SetTime(const TimePoint& time, const TimeDuration& delay)
{
	mEnqueueTime = time;
	mDelay = delay;
}

void BaseEvent::SetDelay(const TimeDuration& newDelay)
{
	mDelay = newDelay;
}
