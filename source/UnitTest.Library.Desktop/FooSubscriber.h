#pragma once
#include "IEventSubscriber.h"
#include "Foo.h"
#include "EventMessageAttributed.h"
#include <chrono>

namespace GameEngine
{
	class BaseEvent;

	class FooSubscriber : public IEventSubscriber
	{
		RTTI_DECLARATIONS(FooSubscriber, IEventSubscriber);

	public:
		~FooSubscriber();
		virtual void Notify(const BaseEvent& event) override;
		size_t mIntEventCount = 0;
		size_t mFloatEventCount = 0;
		size_t mStringEventCount = 0;
		size_t mFooEventCount = 0;
		int mLastInt;
		float mLastFloat;
		std::string mLastString;
		Foo mLastFoo;
		size_t mGenericEventCount = 0;
		EventMessageAttributed mLastMessage;
		
		class EventQueue* mEventQueue = nullptr;
		std::shared_ptr<BaseEvent> mEnqueueEvent;
		std::chrono::milliseconds mEnqueueDelay;
		std::shared_ptr<BaseEvent> mDequeueEvent;
		IEventSubscriber* mAddSubscriber = nullptr;
		IEventSubscriber* mRemoveSubscriber = nullptr;
		bool mNeedUnsubscribeAll = false;
	};
}
