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
#include "ActionEvent.h"
#include "EventMessageAttributed.h"
#include "ReactionAttributed.h"
#include "ActionAdd.h"
#include "JsonParseMaster.h"
#include "JsonTableParseHelper.h"
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

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<> inline std::wstring ToString<Datum::DatumType>(const Datum::DatumType& type)
	{
		RETURN_WIDE_STRING(static_cast<int>(type));
	}

	template<> inline std::wstring ToString<glm::vec4>(const glm::vec4& vec)
	{
		RETURN_WIDE_STRING(vec.x);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ReactionTest)
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

		TEST_METHOD(TestEventMessageAttributed)
		{
			GameTime time;
			WorldState state(time);
			WorldState state2(time);

			EventMessageAttributed message("Type", state);
			Assert::AreEqual("Type"s, message.GetSubtype());
			Assert::IsTrue(message.GetWorldState() == &state);
			message.SetSubtype("NewType");
			message.SetWorldState(state2);
			Assert::AreEqual("NewType"s, message["Subtype"].AsString());
			Assert::IsTrue(message["WorldState"].AsPointer()->As<WorldState>() == &state2);

			EventMessageAttributed* copy = static_cast<EventMessageAttributed*>(message.Clone());
			Assert::AreEqual("NewType"s, copy->GetSubtype());
			Assert::IsTrue(copy->GetWorldState() == &state2);
			delete copy;
		}

		TEST_METHOD(TestActionEvent)
		{
			EntityFactory ef;
			SectorFactory sf;
			ActionEventFactory aef;

			GameTime time;
			World world(time);
			ActionEvent& action = static_cast<ActionEvent&>(*world.CreateSector("Sector")->CreateEntity("Entity", "Entity")->CreateAction("ActionEvent", "Action"));
			action.SetSubtype("Type");
			action.SetDelay(100);
			action.AppendAuxiliaryAttribute("Health") = 10;
			action.AppendAuxiliaryAttribute("Velocity") = vec4(1.f, 2.f, 3.f, 0.f);
			Assert::AreEqual("Type"s, action.GetSubtype());
			Assert::AreEqual(100, action.GetDelay());
			ActionEvent* clone = static_cast<ActionEvent*>(action.Clone());
			Assert::AreEqual("Type"s, (*clone)["Subtype"].AsString());
			Assert::AreEqual(100, (*clone)["Delay"].AsInt());
			delete clone;

			FooSubscriber foo;
			Event<EventMessageAttributed>::Subscribe(foo);
			world.Update();
			Assert::AreEqual(0_z, foo.mGenericEventCount);
			time.SetCurrentTime(time.CurrentTime() + chrono::milliseconds(150));
			world.Update();
			Assert::AreEqual(1_z, foo.mGenericEventCount);
			Assert::AreEqual("Type"s, foo.mLastMessage.GetSubtype());
			Assert::IsTrue(foo.mLastMessage.GetWorldState()->GetWorld() == &world);
			Assert::AreEqual(10, foo.mLastMessage["Health"].AsInt());
			Assert::AreEqual(vec4(1.f, 2.f, 3.f, 0.f), foo.mLastMessage["Velocity"].AsVector());

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(TestReaction)
		{
			ActionAddFactory aaf;
			GameTime time;
			WorldState state(time);
			
			// Create reaction with add behavior
			ReactionAttributed reaction;
			Assert::IsTrue(reaction.Is("ReactionAttributed"));
			Reaction& parent = reaction;
			Assert::IsTrue(parent.Is(Reaction::TypeIdClass()));
			reaction.SetSubtype("Add");
			ActionAdd* actionAdd = static_cast<ActionAdd*>(reaction.CreateAction("ActionAdd", "Add"));
			Scope& operand1 = actionAdd->Operands().AsTable(0);
			Scope& operand2 = actionAdd->Operands().AsTable(1);
			operand1["Type"] = 0;
			operand1["Value"] = 5.f;
			operand2["Type"] = 1;
			operand2["Value"] = "Health";

			// Update
			reaction.Update(state);
			Assert::IsFalse(actionAdd->IsAttribute("Result"));

			// Create event
			EventMessageAttributed message;
			message.SetSubtype("Add");
			message.SetWorldState(state);
			message.AppendAuxiliaryAttribute("Health") = 10.f;
			Event<EventMessageAttributed> event(message);

			// Broadcast event
			event.Deliver();
			Assert::AreEqual(10.f, message["Health"].AsFloat());
			Assert::IsTrue(!reaction.IsAttribute("Health"));
			Assert::AreEqual(15.f, actionAdd->Result().AsFloat());

			// Copy / Move constructor
			{
				ReactionAttributed copy = reaction;
				Assert::AreEqual("Add"s, copy.GetSubtype());
				Assert::AreEqual(15.f, copy.GetActionList().AsTable(0)["Result"].AsFloat());
				ReactionAttributed moved = move(copy);
				Assert::AreEqual("Add"s, moved.GetSubtype());
				Assert::AreEqual(15.f, moved.GetActionList().AsTable(0)["Result"].AsFloat());
			}

			// Copy / Move assignment
			{
				ReactionAttributed copy;
				copy = reaction;
				Assert::AreEqual("Add"s, copy.GetSubtype());
				Assert::AreEqual(15.f, copy.GetActionList().AsTable(0)["Result"].AsFloat());
				ReactionAttributed moved;
				moved = move(copy);
				Assert::AreEqual("Add"s, moved.GetSubtype());
				Assert::AreEqual(15.f, moved.GetActionList().AsTable(0)["Result"].AsFloat());
			}

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		void TestIntegrationReaction(GameTime& time, World* world)
		{
			// Get and check the actions from json
			ReactionAttributed& reaction = static_cast<ReactionAttributed&>((*world)["Reactions"].AsTable(0));
			ReactionAttributed& reaction2 = static_cast<ReactionAttributed&>((*world)["Reactions"].AsTable(1));
			ActionAdd& intAdd = static_cast<ActionAdd&>(reaction.GetActionList().AsTable(0));
			ActionAdd& floatAdd = static_cast<ActionAdd&>(reaction.GetActionList().AsTable(1));
			ActionAdd& stringAdd = static_cast<ActionAdd&>(reaction.GetActionList().AsTable(2));
			const ActionAdd& noAdd = static_cast<const ActionAdd&>(reaction2.GetActionList().AsTable(0));

			Assert::AreEqual(2_z, noAdd.Operands().Size());
			Assert::AreEqual(5, noAdd.Operands().AsTable(0)["Value"].AsInt());
			Assert::AreEqual("IntOperand"s, noAdd.Operands().AsTable(1)["Value"].AsString());

			// By default the add actions should have no result
			Assert::IsFalse(intAdd.IsAttribute("Result"));
			Assert::IsFalse(floatAdd.IsAttribute("Result"));
			Assert::IsFalse(stringAdd.IsAttribute("Result"));
			Assert::IsFalse(noAdd.IsAttribute("Result"));

			// First update enqueus the event, but with the delay it's not delivered
			world->Update();
			Assert::IsFalse(intAdd.IsAttribute("Result"));
			Assert::IsFalse(floatAdd.IsAttribute("Result"));
			Assert::IsFalse(stringAdd.IsAttribute("Result"));
			Assert::IsFalse(noAdd.IsAttribute("Result"));

			// Second update delieveries the event, check the add actions are notified and with right parameters,
			// and the one with wrong subtype is not
			time.SetCurrentTime(time.CurrentTime() + chrono::milliseconds(1000));
			world->Update();
			Assert::AreEqual(15, intAdd.Result().AsInt());
			Assert::AreEqual(25.f, floatAdd.Result().AsFloat());
			Assert::AreEqual("Hello World!"s, stringAdd.Result().AsString());
			Assert::IsFalse(noAdd.IsAttribute("Result"));
		}

		TEST_METHOD(TestIntegration)
		{
			// Prepare tools
			EntityFactory ef;
			SectorFactory sf;
			ActionEventFactory aef;
			ActionAddFactory aaf;
			ReactionAttributedFactory raf;
			GameClock clock;
			GameTime time;
			shared_ptr<World> world = make_shared<World>(time);
			JsonTableParseHelper::SharedData data(world);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			// Parse from json and test original and copy
			Assert::IsTrue(master.ParseFromFile("content/ReactionJsonTest.json"));
			clock.Reset();
			clock.UpdateGameTime(time);
			TestIntegrationReaction(time, &*world);

			// Test copy, use a fresh new world from json because the first one is already polluted
			clock.Reset();
			clock.UpdateGameTime(time);
			shared_ptr<World> world2 = make_shared<World>(time);
			JsonTableParseHelper::SharedData data2(world2);
			master.SetSharedData(data2);
			Assert::IsTrue(master.ParseFromFile("content/ReactionJsonTest.json"));
			World* copy = new World(*world2);
			TestIntegrationReaction(time, copy);
			delete copy;

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ReactionTest::sStartMemState;
}