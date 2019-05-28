#include "pch.h"
#include "Foo.h"
#include "CppUnitTest.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "WorldState.h"
#include "GameClock.h"
#include "GameTime.h"
#include "Event.h"
#include "FooSubscriber.h"
#include "EventQueue.h"
#include <json/json.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngine;
using namespace std;
using namespace std::string_literals;
using namespace glm;

inline std::size_t operator "" _z(unsigned long long int x)
{
	return static_cast<size_t>(x);
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(TestEventRTTI)
		{
			Event<int> intEvent(10);
			Event<float> floatEvent(5.f);
			Event<string> stringEvent("FIEA"s);

			Assert::IsTrue(intEvent.Is(Event<int>::TypeIdClass()));
			Assert::IsTrue(intEvent.Is(BaseEvent::TypeIdClass()));
			Assert::IsTrue(floatEvent.Is(Event<float>::TypeIdClass()));
			Assert::IsTrue(floatEvent.Is(BaseEvent::TypeIdClass()));
			Assert::IsTrue(stringEvent.Is(Event<string>::TypeIdClass()));
			Assert::IsTrue(stringEvent.Is(BaseEvent::TypeIdClass()));

			FooSubscriber foo;
			IEventSubscriber& subscriber = foo;
			Assert::IsTrue(subscriber.Is(IEventSubscriber::TypeIdClass()));
		}

		TEST_METHOD(TestEventCopyMove)
		{
			Event<Foo> e(Foo(10));
			const Foo* foo = &e.Message();

			{
				Event<Foo> copy = e;
				Assert::AreEqual(10, copy.Message().Data());
				Assert::IsFalse(foo == &copy.Message());
				Event<Foo> moved = move(copy);
				Assert::AreEqual(10, moved.Message().Data());
				Assert::IsFalse(&moved.Message() == &copy.Message());
			}

			{
				Event<Foo> copy(Foo(5));
				copy = e;
				Assert::AreEqual(10, copy.Message().Data());
				Assert::IsFalse(foo == &copy.Message());
				Event<Foo> moved(Foo(1));
				moved = move(copy);
				Assert::AreEqual(10, moved.Message().Data());
				Assert::IsFalse(&moved.Message() == &copy.Message());
			}
		}

		TEST_METHOD(TestEventSubscribe)
		{
			FooSubscriber foos[3];
			Event<int>::Subscribe(foos[0]);
			Event<float>::Subscribe(foos[1]);
			Event<string>::Subscribe(foos[2]);
			Event<int> intEvent(10);
			Event<float> floatEvent(5.f);
			Event<string> stringEvent("FIEA"s);

			intEvent.Deliver();
			Assert::AreEqual(1_z, foos[0].mIntEventCount);
			Assert::AreEqual(0_z, foos[0].mFloatEventCount);
			Assert::AreEqual(0_z, foos[0].mStringEventCount);
			Assert::AreEqual(0_z, foos[1].mIntEventCount);
			Assert::AreEqual(0_z, foos[1].mFloatEventCount);
			Assert::AreEqual(0_z, foos[1].mStringEventCount);
			Assert::AreEqual(0_z, foos[2].mIntEventCount);
			Assert::AreEqual(0_z, foos[2].mFloatEventCount);
			Assert::AreEqual(0_z, foos[2].mStringEventCount);
			Assert::AreEqual(10, foos[0].mLastInt);

			floatEvent.Deliver();
			Assert::AreEqual(1_z, foos[0].mIntEventCount);
			Assert::AreEqual(0_z, foos[0].mFloatEventCount);
			Assert::AreEqual(0_z, foos[0].mStringEventCount);
			Assert::AreEqual(0_z, foos[1].mIntEventCount);
			Assert::AreEqual(1_z, foos[1].mFloatEventCount);
			Assert::AreEqual(0_z, foos[1].mStringEventCount);
			Assert::AreEqual(0_z, foos[2].mIntEventCount);
			Assert::AreEqual(0_z, foos[2].mFloatEventCount);
			Assert::AreEqual(0_z, foos[2].mStringEventCount);
			Assert::AreEqual(5.f, foos[1].mLastFloat);

			stringEvent.Deliver();
			Assert::AreEqual(1_z, foos[0].mIntEventCount);
			Assert::AreEqual(0_z, foos[0].mFloatEventCount);
			Assert::AreEqual(0_z, foos[0].mStringEventCount);
			Assert::AreEqual(0_z, foos[1].mIntEventCount);
			Assert::AreEqual(1_z, foos[1].mFloatEventCount);
			Assert::AreEqual(0_z, foos[1].mStringEventCount);
			Assert::AreEqual(0_z, foos[2].mIntEventCount);
			Assert::AreEqual(0_z, foos[2].mFloatEventCount);
			Assert::AreEqual(1_z, foos[2].mStringEventCount);
			Assert::AreEqual("FIEA"s, foos[2].mLastString);

			Event<int>::Subscribe(foos[0]);
			Event<int>::Subscribe(foos[0]);
			Event<int>::Subscribe(foos[0]);
			Event<int>::Subscribe(foos[1]);
			Event<float>::Unsubscribe(foos[0]);
			Event<float>::Unsubscribe(foos[1]);
			Event<float>::Unsubscribe(foos[2]);
			Event<string>::Subscribe(foos[0]);
			Event<string>::Unsubscribe(foos[2]);
			intEvent.Deliver();
			floatEvent.Deliver();
			stringEvent.Deliver();
			Assert::AreEqual(2_z, foos[0].mIntEventCount);
			Assert::AreEqual(0_z, foos[0].mFloatEventCount);
			Assert::AreEqual(1_z, foos[0].mStringEventCount);
			Assert::AreEqual(1_z, foos[1].mIntEventCount);
			Assert::AreEqual(1_z, foos[1].mFloatEventCount);
			Assert::AreEqual(0_z, foos[1].mStringEventCount);
			Assert::AreEqual(0_z, foos[2].mIntEventCount);
			Assert::AreEqual(0_z, foos[2].mFloatEventCount);
			Assert::AreEqual(1_z, foos[2].mStringEventCount);

			Foo foo(233);
			Event<Foo> fooEvent(foo);
			Event<Foo> fooEvent2(Foo(332));
			Event<Foo>::Subscribe(foos[0]);
			fooEvent.Deliver();
			Assert::AreEqual(1_z, foos[0].mFooEventCount);
			Assert::AreEqual(233, foos[0].mLastFoo.Data());
			fooEvent2.Deliver();
			Assert::AreEqual(2_z, foos[0].mFooEventCount);
			Assert::AreEqual(332, foos[0].mLastFoo.Data());

			Event<int>::UnsubscribeAll();
			Event<float>::UnsubscribeAll();
			Event<string>::UnsubscribeAll();
			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(TestEventQueue)
		{
			GameTime time;
			EventQueue queue(time);

			shared_ptr<Event<int>> intEvent = make_shared<Event<int>>(10);
			shared_ptr<Event<int>> intEvent2 = make_shared<Event<int>>(20);
			Assert::IsTrue(queue.IsEmpty());
			queue.Enqueue(intEvent);
			Assert::AreEqual(1_z, queue.Size());
			queue.Enqueue(intEvent);
			queue.Enqueue(intEvent);
			queue.Enqueue(intEvent);
			Assert::AreEqual(1_z, queue.Size());
			queue.Enqueue(intEvent2);
			Assert::AreEqual(2_z, queue.Size());
			queue.Clear();
			Assert::IsTrue(queue.IsEmpty());

			FooSubscriber foo;
			Event<int>::Subscribe(foo);
			queue.Send(intEvent);
			Assert::AreEqual(1_z, foo.mIntEventCount);
			Assert::AreEqual(10, foo.mLastInt);

			queue.Enqueue(intEvent);
			Assert::AreEqual(1_z, foo.mIntEventCount);
			Assert::AreEqual(1_z, queue.Size());
			queue.Update();
			Assert::AreEqual(2_z, foo.mIntEventCount);
			Assert::AreEqual(0_z, queue.Size());

			Assert::IsTrue(queue.IsEmpty());
			queue.Enqueue(intEvent);
			queue.Enqueue(intEvent2);
			Assert::AreEqual(2_z, queue.Size());
			queue.Dequeue(intEvent);
			queue.Dequeue(intEvent);
			queue.Dequeue(intEvent);
			Assert::AreEqual(1_z, queue.Size());
			queue.Update();
			Assert::AreEqual(3_z, foo.mIntEventCount);
			Assert::AreEqual(20, foo.mLastInt);

			Event<int>::UnsubscribeAll();
		}

		TEST_METHOD(TestEventQueueDelay)
		{
			GameClock clock;
			GameTime time;
			EventQueue queue(time);
			shared_ptr<Event<int>> intEvent = make_shared<Event<int>>(10);
			shared_ptr<Event<int>> intEvent2 = make_shared<Event<int>>(20);
			FooSubscriber foo;
			Event<int>::Subscribe(foo);

			clock.UpdateGameTime(time);
			auto now = time.CurrentTime();
			queue.Enqueue(intEvent, chrono::milliseconds(10));
			Assert::IsTrue(now == intEvent->TimeEnqueued());
			Assert::IsTrue(intEvent->Delay() == chrono::milliseconds(10));
			Assert::IsFalse(intEvent->IsExpired(now));
			intEvent->SetDelay(chrono::milliseconds(20));
			Assert::IsTrue(intEvent->Delay() == chrono::milliseconds(20));

			queue.Update();
			Assert::AreEqual(1_z, queue.Size());
			Assert::AreEqual(0_z, foo.mIntEventCount);

			while (!intEvent->IsExpired(time.CurrentTime()))
			{
				for (int i = 0; i < 100000; ++i)
				{
					intEvent->Delay();
				}
				clock.UpdateGameTime(time);
			}
			Assert::IsTrue(time.CurrentTime() - chrono::milliseconds(20) >= now);

			queue.Update();
			Assert::AreEqual(0_z, queue.Size());
			Assert::AreEqual(1_z, foo.mIntEventCount);

			Event<int>::UnsubscribeAll();
		}

		TEST_METHOD(TestIntegration)
		{
			GameClock clock;
			GameTime time;
			World world(time);
			const EventQueue& queue = world.GetEventQueue();
			shared_ptr<Event<int>> intEvent = make_shared<Event<int>>(10);
			shared_ptr<Event<int>> intEvent2 = make_shared<Event<int>>(20);
			shared_ptr<Event<Foo>> fooEvent = make_shared<Event<Foo>>(Foo(5));
			shared_ptr<Event<string>> stringEvent = make_shared<Event<string>>("ABC"s);
			FooSubscriber foos[2];
			Event<int>::Subscribe(foos[0]);
			Event<string>::Subscribe(foos[0]);
			Event<Foo>::Subscribe(foos[1]);
			Event<string>::Subscribe(foos[1]);

			clock.UpdateGameTime(time);
			auto start = time.CurrentTime();
			world.EnqueueEvent(intEvent, BaseEvent::TimeDuration(100));
			world.EnqueueEvent(intEvent2);
			world.EnqueueEvent(fooEvent, BaseEvent::TimeDuration(50));
			world.EnqueueEvent(stringEvent, BaseEvent::TimeDuration(10));
			Assert::AreEqual(4_z, queue.Size());

			world.Update();
			Assert::AreEqual(3_z, queue.Size());
			Assert::AreEqual(1_z, foos[0].mIntEventCount);
			Assert::AreEqual(0_z, foos[0].mStringEventCount);
			Assert::AreEqual(0_z, foos[0].mFooEventCount);
			Assert::AreEqual(0_z, foos[1].mIntEventCount);
			Assert::AreEqual(0_z, foos[1].mStringEventCount);
			Assert::AreEqual(0_z, foos[1].mFooEventCount);
			Assert::AreEqual(20, foos[0].mLastInt);

			time.SetCurrentTime(start + BaseEvent::TimeDuration(15));
			world.Update();
			Assert::AreEqual(2_z, queue.Size());
			Assert::AreEqual(1_z, foos[0].mIntEventCount);
			Assert::AreEqual(1_z, foos[0].mStringEventCount);
			Assert::AreEqual(0_z, foos[0].mFooEventCount);
			Assert::AreEqual(0_z, foos[1].mIntEventCount);
			Assert::AreEqual(1_z, foos[1].mStringEventCount);
			Assert::AreEqual(0_z, foos[1].mFooEventCount);

			fooEvent->SetDelay(fooEvent->Delay() + BaseEvent::TimeDuration(60));
			time.SetCurrentTime(start + BaseEvent::TimeDuration(100));
			world.Update();
			Assert::AreEqual(1_z, queue.Size());
			Assert::AreEqual(2_z, foos[0].mIntEventCount);
			Assert::AreEqual(1_z, foos[0].mStringEventCount);
			Assert::AreEqual(0_z, foos[0].mFooEventCount);
			Assert::AreEqual(0_z, foos[1].mIntEventCount);
			Assert::AreEqual(1_z, foos[1].mStringEventCount);
			Assert::AreEqual(0_z, foos[1].mFooEventCount);
			Assert::AreEqual(10, foos[0].mLastInt);

			time.SetCurrentTime(start + BaseEvent::TimeDuration(150));
			world.Update();
			Assert::IsTrue(queue.IsEmpty());
			Assert::AreEqual(2_z, foos[0].mIntEventCount);
			Assert::AreEqual(1_z, foos[0].mStringEventCount);
			Assert::AreEqual(0_z, foos[0].mFooEventCount);
			Assert::AreEqual(0_z, foos[1].mIntEventCount);
			Assert::AreEqual(1_z, foos[1].mStringEventCount);
			Assert::AreEqual(1_z, foos[1].mFooEventCount);

			world.EnqueueEvent(intEvent);
			Assert::AreEqual(1_z, queue.Size());
			world.DequeueEvent(intEvent);
			world.DequeueEvent(intEvent);
			world.DequeueEvent(intEvent);
			world.DequeueEvent(intEvent);
			Assert::AreEqual(0_z, queue.Size());

			Event<int>::UnsubscribeAll();
			Event<float>::UnsubscribeAll();
			Event<string>::UnsubscribeAll();
			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(TestChangeEventQueueUpdate)
		{
			// Prepare events
			shared_ptr<Event<int>> event1 = make_shared<Event<int>>(1);
			shared_ptr<Event<int>> event2 = make_shared<Event<int>>(2);
			shared_ptr<Event<int>> event3 = make_shared<Event<int>>(3);
			GameClock clock;
			GameTime time;
			EventQueue queue(time);
			queue.Enqueue(event1);
			queue.Enqueue(event2);

			// Prepare subscribers
			const size_t size = 10;
			FooSubscriber foos[size];
			for (size_t i = 0; i < size; ++i)
			{
				Event<int>::Subscribe(foos[i]);
				foos[i].mEventQueue = &queue;
				foos[i].mEnqueueEvent = event3;
				foos[i].mEnqueueDelay = chrono::milliseconds(100);
			}

			// First update, remove two existing events and add one during update
			queue.Update();
			Assert::AreEqual(1_z, queue.Size());
			Assert::IsTrue(event3->Delay() == chrono::milliseconds(100));
			for (size_t i = 0; i < size; ++i)
			{
				Assert::AreEqual(2_z, foos[i].mIntEventCount);
				Assert::AreEqual(2, foos[i].mLastInt);
				foos[i].mEnqueueEvent = shared_ptr<Event<int>>();
			}

			// Added one has delay
			queue.Update();
			Assert::AreEqual(1_z, queue.Size());

			// Run added one
			time.SetCurrentTime(time.CurrentTime() + chrono::milliseconds(100));
			queue.Update();
			Assert::AreEqual(0_z, queue.Size());
			for (size_t i = 0; i < size; ++i)
			{
				Assert::AreEqual(3_z, foos[i].mIntEventCount);
				Assert::AreEqual(3, foos[i].mLastInt);
			}

			// Add dequeue during update
			for (size_t i = 0; i < size; ++i)
			{
				foos[i].mEnqueueEvent = event3;
				foos[i].mEnqueueDelay = chrono::milliseconds(0);
				foos[i].mDequeueEvent = event2;
			}
			queue.Enqueue(event2, chrono::milliseconds(100));
			queue.Enqueue(event1);

			// Dequeue and enqueue during update
			queue.Update();
			Assert::AreEqual(1_z, queue.Size());
			for (size_t i = 0; i < size; ++i)
			{
				Assert::AreEqual(4_z, foos[i].mIntEventCount);
				Assert::AreEqual(1, foos[i].mLastInt);
				foos[i].mEnqueueEvent = shared_ptr<Event<int>>();
			}

			queue.Update();
			Assert::AreEqual(0_z, queue.Size());
			for (size_t i = 0; i < size; ++i)
			{
				Assert::AreEqual(5_z, foos[i].mIntEventCount);
				Assert::AreEqual(3, foos[i].mLastInt);
			}

			Event<int>::UnsubscribeAll();
		}

		TEST_METHOD(TestChangeSubscribeInNotify)
		{
			shared_ptr<Event<int>> event = make_shared<Event<int>>(1);
			FooSubscriber foos[10];
			for (size_t i = 0; i < 5; ++i)
			{
				Event<int>::Subscribe(foos[i]);
				foos[i].mAddSubscriber = &foos[i + 5];
			}

			event->Deliver();
			for (size_t i = 0; i < 5; ++i)
			{
				Assert::AreEqual(1_z, foos[i].mIntEventCount);
				foos[i].mAddSubscriber = nullptr;
			}
			for (size_t i = 5; i < 10; ++i)
			{
				Assert::AreEqual(0_z, foos[i].mIntEventCount);
				foos[i].mRemoveSubscriber = &foos[i - 5];
			}

			event->Deliver();
			for (size_t i = 0; i < 5; ++i)
			{
				Assert::AreEqual(2_z, foos[i].mIntEventCount);
			}
			for (size_t i = 5; i < 10; ++i)
			{
				Assert::AreEqual(1_z, foos[i].mIntEventCount);
				foos[i].mRemoveSubscriber = nullptr;
			}

			event->Deliver();
			for (size_t i = 0; i < 5; ++i)
			{
				Assert::AreEqual(2_z, foos[i].mIntEventCount);
			}
			for (size_t i = 5; i < 10; ++i)
			{
				Assert::AreEqual(2_z, foos[i].mIntEventCount);
				foos[i].mNeedUnsubscribeAll = true;
			}

			event->Deliver();
			for (size_t i = 5; i < 10; ++i)
			{
				Assert::AreEqual(3_z, foos[i].mIntEventCount);
			}

			event->Deliver();
			for (size_t i = 0; i < 5; ++i)
			{
				Assert::AreEqual(2_z, foos[i].mIntEventCount);
			}
			for (size_t i = 5; i < 10; ++i)
			{
				Assert::AreEqual(3_z, foos[i].mIntEventCount);
			}

			Event<int>::UnsubscribeAll();
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState EventTest::sStartMemState;
}