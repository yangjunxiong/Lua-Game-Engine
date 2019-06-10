#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseMaster.h"
#include "JsonTableParseHelper.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "Avatar.h"
#include "WorldState.h"
#include "GameTime.h"
#include "Action.h"
#include "ActionListIf.h"
#include "ActionIncrement.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include <json/json.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngine;
using namespace std;
using namespace std::string_literals;
using namespace glm;

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

	template<> inline std::wstring ToString<glm::mat4>(const glm::mat4& mat)
	{
		RETURN_WIDE_STRING(mat[0][0]);
	}
}

inline std::size_t operator "" _z(unsigned long long int x)
{
	return static_cast<size_t>(x);
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionTest)
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

#define INIT(_root_type)																   \
		EntityFactory ef;																   \
		SectorFactory sf;																   \
		AvatarFactory af;																   \
		ActionListFactory alf;															   \
		ActionListIfFactory alif;														   \
		ActionIncrementFactory aif;														   \
		ActionCreateActionFactory acaf;                                                    \
		ActionDestroyActionFactory adaf;                                                   \
		AttributedTypeManager::RegisterType(Avatar::TypeIdClass(), Avatar::Signatures(), Entity::TypeIdClass());  \
		AttributedTypeManager::FinalizeSignature();                                        \
		shared_ptr<_root_type> ptr = make_shared<_root_type>();							   \
		_root_type& root = *ptr;														   \
		root;																			   \
		JsonTableParseHelper::SharedData data(ptr);										   \
		JsonParseMaster master(data);													   \
		JsonTableParseHelper helper;													   \
		master.AddHelper(helper);

#define UNINIT()																		   \
		AttributedTypeManager::UnregisterType(Avatar::TypeIdClass());					   \
		if (root.GetParent() != nullptr)												   \
		{																				   \
			root.GetParent()->Orphan(root);												   \
		}

		void Check(World& world)
		{
			auto root = static_cast<Sector&>(world.Sectors().AsTable(0));
			Assert::AreEqual("Sector"s, root.Name());
			Assert::AreEqual(3_z, root.Entities().Size());
			Assert::IsTrue(root.Entities().AsTable(0).Is("Avatar"));
			Assert::AreEqual(""s, static_cast<Avatar&>(root.Entities().AsTable(0)).Name());
			Assert::IsTrue(root.Entities().AsTable(1).Is("Entity") && !root.Entities().AsTable(1).Is("Avatar"));
			Assert::AreEqual(""s, static_cast<Entity&>(root.Entities().AsTable(1)).Name());
			Assert::IsTrue(root.Entities().AsTable(2).Is("Entity") && !root.Entities().AsTable(2).Is("Avatar"));

			Entity& entity = static_cast<Entity&>(root.Entities().AsTable(2));
			Assert::AreEqual("Entity"s, entity.Name());
			Assert::AreEqual(10, entity["Health"].AsInt());
			Assert::AreEqual(4_z, entity.Actions().Size());

			Assert::AreEqual("Action1"s, static_cast<ActionIncrement&>(entity.Actions().AsTable(0)).Name());
			Assert::AreEqual("Health"s, static_cast<ActionIncrement&>(entity.Actions().AsTable(0))["Target"].AsString());
			Assert::AreEqual(5.f, static_cast<ActionIncrement&>(entity.Actions().AsTable(0))["Step"].AsFloat());

			Assert::AreEqual("Action2"s, static_cast<ActionCreateAction&>(entity.Actions().AsTable(1)).Name());
			Assert::AreEqual("ActionIncrement"s, static_cast<ActionCreateAction&>(entity.Actions().AsTable(1))["Class"].AsString());
			Assert::AreEqual("CreatedAction"s, static_cast<ActionCreateAction&>(entity.Actions().AsTable(1))["InstanceName"].AsString());

			ActionList& list = static_cast<ActionList&>(entity.Actions().AsTable(2));
			Assert::AreEqual(2_z, list.GetActionList().Size());
			ActionDestroyAction* destroy = static_cast<ActionDestroyAction*>(&list.GetActionList().AsTable(0));
			Assert::AreEqual("Action3"s, destroy->Name());
			Assert::AreEqual("Action2"s, (*destroy)["Target"].AsString());
			destroy = static_cast<ActionDestroyAction*>(&list.GetActionList().AsTable(1));
			Assert::AreEqual("Action4"s, destroy->Name());
			Assert::AreEqual("ActionList1"s, (*destroy)["Target"].AsString());

			ActionListIf& actionIf = static_cast<ActionListIf&> (entity.Actions().AsTable(3));
			Assert::AreEqual("ActionIf"s, actionIf.Name());
			Assert::AreEqual(0, actionIf["Condition"].AsInt());
			Assert::AreEqual(2_z, actionIf.GetActionList().Size());
			ActionIncrement& inc = static_cast<ActionIncrement&>(actionIf.GetActionList().AsTable(0));
			Assert::AreEqual("INC"s, inc.Name());
			Assert::AreEqual("Health"s, inc["Target"].AsString());
			Assert::AreEqual(-7.f, inc["Step"].AsFloat());
			ActionDestroyAction& des = static_cast<ActionDestroyAction&>(actionIf.GetActionList().AsTable(1));
			Assert::AreEqual("DES"s, des.Name());
			Assert::AreEqual("Action1"s, des["Target"].AsString());
		}

		TEST_METHOD(TestBaseAction)
		{
			INIT(ActionList);
			string json = R"#({
				"Name": {
					"type": "string",
					"value": "TestAction"
				},
				"ActionList": {
					"type": "table",
					"value": [
						{
							"class": "ActionList",
							"Name": {
								"type": "string",
								"value": "NestedActionList"
							},
							"ActionList": {
								"type": "table",
								"value": [
									{
										"class": "ActionIncrement",
										"Name": {
											"type": "string",
											"value": "Labor"
										},
										"Target": {
											"type": "string",
											"value": "Health"
										},
										"Step": {
											"type": "float",
											"value": 5.2
										}
									}
								]
							}
						}
					]
				}
			})#";
			Assert::IsTrue(master.Parse(json));

			Assert::AreEqual("TestAction"s, root.Name());
			root.SetName("ModifiedName");
			Assert::AreEqual("ModifiedName"s, root.Name());
			Assert::AreEqual("ModifiedName"s, root["Name"].AsString());
			Assert::AreEqual(1_z, root["ActionList"].Size());
			ActionList& childAction = static_cast<ActionList&>(root["ActionList"][0]);
			Assert::AreEqual("NestedActionList"s, childAction.Name());

			Entity entity;
			entity.Append("Health").first = 10.f;
			entity.Adopt(root, "Actions");
			GameTime time;
			WorldState state(time);
			entity.Update(state);
			Assert::AreEqual(15.2f, entity["Health"].AsFloat());

			UNINIT();
		}

		TEST_METHOD(TestSearchAction)
		{
			INIT(ActionList);

			{
				ActionList list;
				list.SetName("Name");
				Assert::IsTrue(list.SearchAction("Name") == &list);
			}

			{
				ActionList list;
				list.SetName("Name");
				Assert::IsTrue(list.SearchAction("NotName") == nullptr);
			}

			{
				ActionList list;
				list.SetName("Name");
				ActionList* nestedList = static_cast<ActionList*>(list.CreateAction("ActionList", "Name"));
				Assert::IsTrue(nestedList->SearchAction("Name") == nestedList);
				Assert::IsTrue(nestedList->SearchAction("NorName") == nullptr);
				Assert::IsTrue(list.SearchAction("Name") == &list);
			}

			{
				Entity entity;
				entity.SetName("Name");
				ActionList* firstAction = static_cast<ActionList*>(entity.CreateAction("ActionList", "NotName"));
				Assert::IsTrue(firstAction->SearchAction("Name") == nullptr);
				ActionIncrement* secondAction = static_cast<ActionIncrement*>(entity.CreateAction("ActionIncrement", "Name"));
				Assert::IsTrue(firstAction->SearchAction("Name") == secondAction);
				ActionList* firstActionFirstChild = static_cast<ActionList*>(firstAction->CreateAction("ActionList", "Name"));
				Assert::IsTrue(firstActionFirstChild->SearchAction("Name") == firstActionFirstChild);
				Assert::IsTrue(firstActionFirstChild->SearchAction("NotName") == firstAction);
				ActionList* thirdAction = static_cast<ActionList*>(entity.CreateAction("ActionList", "Action3"));

				Assert::IsTrue(firstActionFirstChild->SearchAction("Action3") == thirdAction);
				Assert::IsTrue(firstAction->SearchAction("Action3") == thirdAction);
				Assert::IsTrue(secondAction->SearchAction("Action3") == thirdAction);
				Assert::IsTrue(thirdAction->SearchAction("Action3") == thirdAction);

				ActionList* firstActionSecondChild = static_cast<ActionList*>(firstAction->CreateAction("ActionList", "Action4"));
				Assert::IsTrue(firstActionFirstChild->SearchAction("Action4") == firstActionSecondChild);
				Assert::IsTrue(firstActionSecondChild->SearchAction("Action4") == firstActionSecondChild);
				Assert::IsTrue(firstAction->SearchAction("Action4") == nullptr);
				Assert::IsTrue(secondAction->SearchAction("Action4") == nullptr);
				Assert::IsTrue(thirdAction->SearchAction("Action4") == nullptr);
			}

			UNINIT();
		}

		TEST_METHOD(TestActionIncrement)
		{
			INIT(Entity);

			Entity entity;
			entity.Append("Health").first = 10;
			ActionIncrement& action = *static_cast<ActionIncrement*>(entity.CreateAction("ActionIncrement", "ActionTest"));
			action["Target"] = "Health";
			action["Step"] = 2.f;

			GameTime time;
			WorldState state(time);
			action.Update(state);
			Assert::IsTrue(state.GetAction() == &action);
			Assert::AreEqual(12, entity["Health"].AsInt());
			
			entity.Append("HealthF").first = 100.f;
			action["Target"] = "HealthF";
			action["Step"] = 10.f;
			action.Update(state);
			Assert::AreEqual(110.f, entity["HealthF"].AsFloat());

			UNINIT();
		}

		TEST_METHOD(TestActionList)
		{
			INIT(ActionList);
			Entity entity;
			entity.Append("Health").first = 0;
			entity.Append("Value").first = 10.5f;
			ActionList& list = static_cast<ActionList&>(*entity.CreateAction("ActionList", "List"));
			ActionIncrement& action = *new ActionIncrement;
			ActionIncrement& action2 = *new ActionIncrement;
			ActionIncrement& action3 = *new ActionIncrement;
			action["Target"] = "Health";
			action["Step"] = 2.f;
			action2["Target"] = "Health";
			action3["Target"] = "Value";
			action3["Step"] = -5.f;
			list.Adopt(action, ActionList::ACTION_LIST_TABLE_KEY);
			list.Adopt(action2, ActionList::ACTION_LIST_TABLE_KEY);
			list.Adopt(action3, ActionList::ACTION_LIST_TABLE_KEY);

			Datum& actionList = list.GetActionList();
			const Datum& constActionList = const_cast<const ActionList&>(list).GetActionList();
			Assert::IsTrue(&actionList == &constActionList);
			Assert::IsTrue(&actionList[0] == &action);
			Assert::IsTrue(&actionList[1] == &action2);
			Assert::IsTrue(&actionList[2] == &action3);

			Assert::AreEqual(0, entity["Health"].AsInt());
			Assert::AreEqual(10.5f, entity["Value"].AsFloat());
			GameTime time;
			WorldState state(time);
			list.Update(state);
			Assert::AreEqual(3, entity["Health"].AsInt());
			Assert::AreEqual(5.5f, entity["Value"].AsFloat());
			Assert::IsTrue(state.GetAction() == &action3);

			UNINIT();
		}

		TEST_METHOD(TestActionListIf)
		{
			INIT(ActionListIf);
			ActionIncrement& trueAction = *new ActionIncrement;
			ActionIncrement& falseAction = *new ActionIncrement;
			trueAction["Target"] = "Health";
			trueAction["Step"] = 7.1f;
			falseAction["Target"] = "Health";
			falseAction["Step"] = -5.f;
			root.Adopt(trueAction, ActionList::ACTION_LIST_TABLE_KEY);
			root.Adopt(falseAction, ActionList::ACTION_LIST_TABLE_KEY);

			Entity entity;
			entity["Health"] = 10;
			entity.Adopt(root, entity.ACTION_TABLE_KEY);
			GameTime time;
			WorldState state(time);
			entity.Update(state);
			Assert::AreEqual(0, root["Condition"].AsInt());
			Assert::AreEqual(5, entity["Health"].AsInt());
			root["Condition"] = 1;
			entity.Update(state);
			Assert::AreEqual(12, entity["Health"].AsInt());

			UNINIT();
		}

		TEST_METHOD(TestActionCreateAction)
		{
			INIT(ActionCreateAction);

			{
				Entity entity;
				ActionCreateAction& action = *static_cast<ActionCreateAction*>(entity.CreateAction("ActionCreateAction", "Create"));
				action["Class"] = "ActionIncrement";
				action["InstanceName"] = "CreatedAction";
				Assert::AreEqual(1_z, entity.Actions().Size());
				Assert::IsTrue(&action == &entity.Actions().AsTable());

				GameTime time;
				WorldState state(time);
				entity.Update(state);
				Assert::AreEqual(2_z, entity.Actions().Size());
				Action& created = static_cast<Action&>(entity.Actions().AsTable(1));
				Assert::IsTrue(created.Is(ActionIncrement::TypeIdClass()));
				Assert::AreEqual("CreatedAction"s, created.Name());
			}
			
			{
				ActionList list;
				ActionCreateAction& action = *static_cast<ActionCreateAction*>(list.CreateAction("ActionCreateAction", "Create"));
				action["Class"] = "ActionIncrement";
				action["InstanceName"] = "CreatedAction";
				Assert::AreEqual(1_z, list.GetActionList().Size());
				Assert::IsTrue(&action == &list.GetActionList().AsTable());

				GameTime time;
				WorldState state(time);
				list.Update(state);
				Assert::AreEqual(2_z, list.GetActionList().Size());
				Action& created = static_cast<Action&>(list.GetActionList().AsTable(1));
				Assert::IsTrue(created.Is(ActionIncrement::TypeIdClass()));
				Assert::AreEqual("CreatedAction"s, created.Name());
			}

			UNINIT();
		}

		TEST_METHOD(TestActionDestroyAction)
		{
			INIT(ActionDestroyAction);

			GameTime time;
			WorldState state(time);
			World world(time);
			Sector& sector = *world.CreateSector("Sector");
			Entity& entity = *sector.CreateEntity("Entity", "Entity");
			ActionList& action1 = static_cast<ActionList&>(*entity.CreateAction("ActionList", "Action1"));
			ActionList& action2 = static_cast<ActionList&>(*entity.CreateAction("ActionList", "Action2"));
			ActionList& action3 = static_cast<ActionList&>(*entity.CreateAction("ActionList", "Action3"));
			ActionList& action1_1 = static_cast<ActionList&>(*action1.CreateAction("ActionList", "Action1_1"));
			ActionList& action1_2 = static_cast<ActionList&>(*action1.CreateAction("ActionList", "Action1_2"));
			ActionList& action2_1 = static_cast<ActionList&>(*action2.CreateAction("ActionList", "Action2_1"));
			ActionList& action3_1 = static_cast<ActionList&>(*action3.CreateAction("ActionList", "Action3_1"));

			// Destroy self
			{
				ActionDestroyAction& destroy = static_cast<ActionDestroyAction&>(*action1_1.CreateAction("ActionDestroyAction", "Action"));
				destroy["Target"] = "Action";
				Assert::AreEqual(1_z, action1_1.GetActionList().Size());
				world.Update();
				Assert::AreEqual(0_z, action1_1.GetActionList().Size());
			}

			// Destroy parent's sibling
			{
				ActionDestroyAction& destroy = static_cast<ActionDestroyAction&>(*action1_2.CreateAction("ActionDestroyAction", "Action"));
				destroy["Target"] = "Action1_1";
				Assert::AreEqual(2_z, action1.GetActionList().Size());
				world.Update();
				Assert::AreEqual(1_z, action1.GetActionList().Size());
				Assert::IsTrue(&action1.GetActionList().AsTable(0) == &action1_2);
				destroy["Target"] = "Action";
				world.Update();
			}

			// Destroy descedent - can't
			{
				ActionDestroyAction& destroy = static_cast<ActionDestroyAction&>(*action1.CreateAction("ActionDestroyAction", "Action"));
				destroy["Target"] = "Action2_1";
				Assert::AreEqual(2_z, action1.GetActionList().Size());
				world.Update();
				Assert::AreEqual(2_z, action1.GetActionList().Size());
				Assert::IsTrue(&action1.GetActionList().AsTable(0) == &action1_2);
				Assert::IsTrue(&action1.GetActionList().AsTable(1) == &destroy);
				destroy["Target"] = "Action";
				world.Update();
			}

			// Destroy descedent first then parent
			{
				ActionDestroyAction& destroy = static_cast<ActionDestroyAction&>(*action1_2.CreateAction("ActionDestroyAction", "Action"));
				destroy["Target"] = "Action";
				ActionDestroyAction& destroy2 = static_cast<ActionDestroyAction&>(*action1_2.CreateAction("ActionDestroyAction", "Action2"));
				destroy2["Target"] = "Action1_2";
				Assert::AreEqual(1_z, action1.GetActionList().Size());
				world.Update();
				Assert::AreEqual(0_z, action1.GetActionList().Size());
			}

			// Destroy parent first then descedent
			{
				ActionDestroyAction& destroy = static_cast<ActionDestroyAction&>(*action2_1.CreateAction("ActionDestroyAction", "Action"));
				destroy["Target"] = "Action2_1";
				ActionDestroyAction& destroy2 = static_cast<ActionDestroyAction&>(*action2_1.CreateAction("ActionDestroyAction", "Action"));
				destroy2["Target"] = "Action";
				Assert::AreEqual(1_z, action2.GetActionList().Size());
				world.Update();
				Assert::AreEqual(0_z, action2.GetActionList().Size());
			}

			// Destroy same object multiple times
			{
				ActionDestroyAction& destroy = static_cast<ActionDestroyAction&>(*action3_1.CreateAction("ActionDestroyAction", "Action"));
				destroy["Target"] = "Action3_1";
				ActionDestroyAction& destroy2 = static_cast<ActionDestroyAction&>(*action3_1.CreateAction("ActionDestroyAction", "Action"));
				destroy2["Target"] = "Action3_1";
				ActionDestroyAction& destroy3 = static_cast<ActionDestroyAction&>(*action3_1.CreateAction("ActionDestroyAction", "Action"));
				destroy3["Target"] = "Action3_1";
				Assert::AreEqual(1_z, action3.GetActionList().Size());
				world.Update();
				Assert::AreEqual(0_z, action3.GetActionList().Size());
			}

			// Interleved destroy
			{
				ActionDestroyAction& destroy = static_cast<ActionDestroyAction&>(*action1.CreateAction("ActionDestroyAction", "Action"));
				destroy["Target"] = "Action2";
				ActionDestroyAction& destroy2 = static_cast<ActionDestroyAction&>(*action1.CreateAction("ActionDestroyAction", "Action"));
				destroy2["Target"] = "Action3";
				ActionDestroyAction& destroy3 = static_cast<ActionDestroyAction&>(*action1.CreateAction("ActionDestroyAction", "Action"));
				destroy3["Target"] = "Action1";
				ActionDestroyAction& destroy4 = static_cast<ActionDestroyAction&>(*action2.CreateAction("ActionDestroyAction", "Action"));
				destroy4["Target"] = "Action2";
				ActionDestroyAction& destroy5 = static_cast<ActionDestroyAction&>(*action2.CreateAction("ActionDestroyAction", "Action"));
				destroy5["Target"] = "Action1";
				ActionDestroyAction& destroy6 = static_cast<ActionDestroyAction&>(*action2.CreateAction("ActionDestroyAction", "Action"));
				destroy6["Target"] = "Action3";
				ActionDestroyAction& destroy7 = static_cast<ActionDestroyAction&>(*action3.CreateAction("ActionDestroyAction", "Action"));
				destroy7["Target"] = "Action1";
				ActionDestroyAction& destroy8 = static_cast<ActionDestroyAction&>(*action3.CreateAction("ActionDestroyAction", "Action"));
				destroy8["Target"] = "Action2";
				ActionDestroyAction& destroy9 = static_cast<ActionDestroyAction&>(*action3.CreateAction("ActionDestroyAction", "Action"));
				destroy9["Target"] = "Action3";
				ActionDestroyAction& destroy10 = static_cast<ActionDestroyAction&>(*action1.CreateAction("ActionDestroyAction", "Action"));
				destroy10["Target"] = "Action";
				ActionDestroyAction& destroy11 = static_cast<ActionDestroyAction&>(*action2.CreateAction("ActionDestroyAction", "Action"));
				destroy11["Target"] = "Action";
				ActionDestroyAction& destroy12 = static_cast<ActionDestroyAction&>(*action3.CreateAction("ActionDestroyAction", "Action"));
				destroy12["Target"] = "Action";

				Assert::AreEqual(3_z, entity.Actions().Size());
				world.Update();
				Assert::AreEqual(0_z, entity.Actions().Size());
			}

			UNINIT();
		}

		TEST_METHOD(TestIntegration)
		{
			INIT(Sector);
			GameTime time;
			WorldState state(time);
			World world(time);
			world.Adopt(root, World::SECTOR_TABLE_KEY);

			Assert::IsTrue(master.ParseFromFile("content/ActionJsonTest.json"));
			Check(world);
			Entity& entity = static_cast<Entity&>(root.Entities().AsTable(2));
			ActionListIf& actionIf = static_cast<ActionListIf&>(entity.Actions().AsTable(3));

			World copyWorld = world;
			Check(copyWorld);
			World copyWorld2(time);
			copyWorld2 = world;
			Check(copyWorld2);

			World moveWorld = move(copyWorld);
			Check(moveWorld);
			World moveWorld2(time);
			moveWorld2 = move(copyWorld2);
			Check(moveWorld2);

			world.Update();
			Assert::AreEqual(2_z, entity.Actions().Size());
			Assert::IsTrue(&entity.Actions().AsTable(0) == &actionIf);
			ActionIncrement& newIncrement = static_cast<ActionIncrement&>(entity.Actions().AsTable(1));
			Assert::AreEqual(1.f, entity.Actions().AsTable(1)["Step"].AsFloat());
			Assert::AreEqual("CreatedAction"s, entity.Actions().AsTable(1)["Name"].AsString());
			Assert::AreEqual(15, entity["Health"].AsInt());

			actionIf["Condition"] = 1;
			newIncrement["Target"] = "Health";
			world.Update();
			Assert::AreEqual(9, entity["Health"].AsInt());

			world.Orphan(root);
			UNINIT();
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ActionTest::sStartMemState;
}