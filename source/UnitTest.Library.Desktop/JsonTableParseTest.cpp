#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseMaster.h"
#include "JsonTableParseHelper.h"
#include "JsonParseHelperVector.h"
#include "JsonParseHelperInteger.h"
#include "Scope.h"
#include "AttributedFoo.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "Avatar.h"
#include "GameTime.h"
#include "ActionIncrement.h"
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
	TEST_CLASS(JsonTableParseTest)
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

		TEST_METHOD(TestSharedDataCreate)
		{
			{
				shared_ptr<Scope> scopePtr(new Scope());
				Scope& scope = *scopePtr;
				JsonTableParseHelper::SharedData data(scopePtr);
				Assert::IsTrue(&scope == &data.GetTable());
				Assert::IsTrue(&scope == &const_cast<const JsonTableParseHelper::SharedData&>(data).GetTable());
				auto* cloneData = reinterpret_cast<JsonTableParseHelper::SharedData*>(data.Create());
				Assert::IsTrue(&scope == &data.GetTable());
				Assert::IsFalse(&scope == &cloneData->GetTable());
				delete cloneData;
			}
			
			{
				shared_ptr<AttributedFoo> fooPtr(new AttributedFoo(0));
				AttributedFoo& foo = *fooPtr;
				JsonTableParseHelper::SharedData data(fooPtr);
				Assert::IsTrue(&foo == &data.GetTable());
				auto* cloneData = reinterpret_cast<JsonTableParseHelper::SharedData*>(data.Create());
				Assert::IsTrue(cloneData->GetTable().Is(AttributedFoo::TypeIdClass()));
				Assert::IsTrue(&foo == &data.GetTable());
				Assert::IsFalse(&foo == &cloneData->GetTable());
				delete cloneData;
			}
		}

		TEST_METHOD(TestParsePrimitive)
		{
			// Init
			shared_ptr<Scope> scopePtr(new Scope());
			Scope& scope = *scopePtr;
			JsonTableParseHelper::SharedData data(scopePtr);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			Assert::IsTrue(helper.Is(IJsonParseHelper::TypeIdClass()));
			master.AddHelper(helper);

			// Parse string
			string jsonString = R"#({
				"Health": { "type": "integer", "value": [1,2,3] },
				"Speed": { "type": "float", "value": 10.5 },
				"Velocity": { "type": "vector", "value": "vec4(1, 2, 3, 4)" },
				"Transform": { "type": "matrix", "value": "mat4x4((1, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16))" },
				"Names": { "type": "string", "value": ["Jack", "Rose", "Leo"] }
			})#";
			Assert::IsTrue(master.Parse(jsonString));

			Assert::IsTrue(&scope == &data.GetTable());
			Assert::AreEqual(5_z, scope.Size());
			Assert::AreEqual(3_z, scope["Health"].Size());
			Assert::AreEqual(1, scope["Health"].AsInt(0));
			Assert::AreEqual(2, scope["Health"].AsInt(1));
			Assert::AreEqual(3, scope["Health"].AsInt(2));
			Assert::AreEqual(10.5f, scope["Speed"].AsFloat());
			Assert::AreEqual(vec4(1, 2, 3, 4), scope["Velocity"].AsVector());
			Assert::AreEqual(mat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16), scope["Transform"].AsMatrix());
			Assert::AreEqual(3_z, scope["Names"].Size());
			Assert::AreEqual("Jack"s, scope["Names"].AsString(0));
			Assert::AreEqual("Rose"s, scope["Names"].AsString(1));
			Assert::AreEqual("Leo"s, scope["Names"].AsString(2));
		}

		TEST_METHOD(TestParseNestedScope)
		{
			// Init
			shared_ptr<Scope> scopePtr(new Scope());
			Scope& scope = *scopePtr;;
			JsonTableParseHelper::SharedData data(scopePtr);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			string jsonString = R"#({
				"Table": { "type": "table", "value": {} },
				"value": { "type": "table", "value": [{}, {}, {}, {}] },
				"Content": { "type": "table", "value": [{}, {}, { 
					"Obj": { "type": "table", "value": {} },
					"type": { "type": "table", "value": { "Deep": { "type": "integer", "value": [1,3,5] } } }
				 }] }
			})#";
			Assert::IsTrue(master.Parse(jsonString));
			Assert::IsTrue(&scope == &data.GetTable());
			Assert::AreEqual(3_z, scope.Size());
			Assert::AreEqual(Datum::DatumType::Table, scope["Table"].Type());
			Assert::AreEqual(0_z, scope["Table"][0].Size());
			Assert::AreEqual(Datum::DatumType::Table, scope["value"].Type());
			Assert::AreEqual(4_z, scope["value"].Size());
			Assert::AreEqual(0_z, scope["value"][0].Size());
			Assert::AreEqual(0_z, scope["value"][1].Size());
			Assert::AreEqual(0_z, scope["value"][2].Size());
			Assert::AreEqual(0_z, scope["value"][3].Size());
			Assert::AreEqual(3_z, scope["Content"].Size());
			Assert::AreEqual(0_z, scope["Content"][0].Size());
			Assert::AreEqual(0_z, scope["Content"][1].Size());
			Assert::AreEqual(2_z, scope["Content"][2].Size());
			Assert::AreEqual(0_z, scope["Content"][2]["Obj"][0].Size());
			Assert::AreEqual(1_z, scope["Content"][2]["type"][0].Size());
			Assert::AreEqual(3_z, scope["Content"][2]["type"][0]["Deep"].Size());
			Assert::AreEqual(1, scope["Content"][2]["type"][0]["Deep"].AsInt(0));
			Assert::AreEqual(3, scope["Content"][2]["type"][0]["Deep"].AsInt(1));
			Assert::AreEqual(5, scope["Content"][2]["type"][0]["Deep"].AsInt(2));
		}

		TEST_METHOD(TestParseNormalScope)
		{
			// Init
			shared_ptr<Scope> scopePtr(new Scope());
			Scope& scope = *scopePtr;;
			JsonTableParseHelper::SharedData data(scopePtr);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			Assert::IsTrue(master.ParseFromFile("content/scopeJsonTest.json"));
			Assert::AreEqual(8_z, scope.Size());
			Assert::AreEqual(10.5f, scope["Health"].AsFloat());
			Assert::AreEqual(100, scope["Money"].AsInt());
			Assert::AreEqual(vec4(100, 200, 300.123, 0), scope["Position"].AsVector());
			Assert::AreEqual(mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16), scope["Transform"].AsMatrix());
			Assert::AreEqual("Jack"s, scope["Name"].AsString());
			Assert::AreEqual(4_z, scope["Traits"].Size());
			Assert::AreEqual(300, scope["Traits"].AsInt(0));
			Assert::AreEqual(301, scope["Traits"].AsInt(1));
			Assert::AreEqual(452, scope["Traits"].AsInt(2));
			Assert::AreEqual(342, scope["Traits"].AsInt(3));
			Assert::AreEqual(3_z, scope["Tags"].Size());
			Assert::AreEqual("Hero"s, scope["Tags"].AsString(0));
			Assert::AreEqual("Human"s, scope["Tags"].AsString(1));
			Assert::AreEqual("Stressed"s, scope["Tags"].AsString(2));

			Assert::AreEqual(Datum::DatumType::Table, scope["Inventory"].Type());
			Assert::AreEqual(2_z, scope["Inventory"].Size());

			Scope& scope1 = scope["Inventory"][0];
			Assert::AreEqual(4_z, scope1.Size());
			Assert::AreEqual(0, scope1["Id"].AsInt());
			Assert::AreEqual("Mother's Soup"s, scope1["Name"].AsString());
			Assert::AreEqual(3, scope1["Count"].AsInt());
			Assert::AreEqual(Datum::DatumType::Table, scope1["Effect"].Type());
			Assert::AreEqual(1_z, scope1["Effect"].Size());
			Scope& scope1_1 = scope1["Effect"][0];
			Assert::AreEqual(3_z, scope1_1.Size());
			Assert::AreEqual(30, scope1_1["Id"].AsInt());
			Assert::AreEqual(5.35f, scope1_1["Duration"].AsFloat());
			Assert::AreEqual(Datum::DatumType::Table, scope1_1["Requirement"].Type());
			Assert::AreEqual(3_z, scope1_1["Requirement"].Size());
			Assert::AreEqual(1_z, scope1_1["Requirement"][0].Size());
			Assert::AreEqual(1_z, scope1_1["Requirement"][1].Size());
			Assert::AreEqual(1_z, scope1_1["Requirement"][2].Size());
			Assert::AreEqual(0, scope1_1["Requirement"][0]["Id"].AsInt());
			Assert::AreEqual(1, scope1_1["Requirement"][1]["Id"].AsInt());
			Assert::AreEqual(2, scope1_1["Requirement"][2]["Id"].AsInt());

			Scope& scope2 = scope["Inventory"][1];
			Assert::AreEqual(3_z, scope2.Size());
			Assert::AreEqual(10, scope2["Id"].AsInt());
			Assert::AreEqual("Sword"s, scope2["Name"].AsString());
			Assert::AreEqual(10.f, scope2["Damage"].AsFloat());
		}

		TEST_METHOD(TestParseAttributed)
		{
			shared_ptr<AttributedFoo> fooPtr(new AttributedFoo(0));
			AttributedFoo& foo = *fooPtr;
			JsonTableParseHelper::SharedData data(fooPtr);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);
			JsonParseMaster* cloneMaster = master.Clone();
			AttributedFoo& cloneFoo = reinterpret_cast<AttributedFoo&>(reinterpret_cast<JsonTableParseHelper::SharedData*>(cloneMaster->GetSharedData())->GetTable());

			EntityFactory entityFactory;
			Assert::IsTrue(master.ParseFromFile("content/AttributedFooJsonTest.json"));

			// Verify clone operation
			Assert::IsTrue(cloneMaster->Parse(R"({ "Int": { "type": "integer", "value": 999 } })"));
			Assert::AreEqual(999, cloneFoo.mInt);
			delete cloneMaster;

			// External attributes
			Assert::AreEqual(100, foo.mInt);
			Assert::AreEqual(11.123f, foo.mFloat);
			Assert::AreEqual(vec4(1, 2, 3, 4), foo.mVector);
			Assert::AreEqual(mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16), foo.mMatrix);
			Assert::AreEqual("I'm a string"s, foo.mString);
			Assert::AreEqual(1, foo.mIntArray[0]);
			Assert::AreEqual(2, foo.mIntArray[1]);
			Assert::AreEqual(3, foo.mIntArray[2]);
			Assert::AreEqual(1.1f, foo.mFloatArray[0]);
			Assert::AreEqual(2.2f, foo.mFloatArray[1]);
			Assert::AreEqual(3.3f, foo.mFloatArray[2]);
			Assert::AreEqual(vec4(), foo.mVectorArray[0]);
			Assert::AreEqual(vec4(1, 2, 3, 4), foo.mVectorArray[1]);
			Assert::AreEqual(vec4(5, 6, 7, 8), foo.mVectorArray[2]);
			Assert::AreEqual(mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16), foo.mMatrixArray[0]);
			Assert::AreEqual(mat4(-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16), foo.mMatrixArray[1]);
			Assert::AreEqual(mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1), foo.mMatrixArray[2]);
			Assert::AreEqual("A"s, foo.mStringArray[0]);
			Assert::AreEqual("B"s, foo.mStringArray[1]);
			Assert::AreEqual("C"s, foo.mStringArray[2]);

			// Internal attributes
			Assert::AreEqual(999, foo["InternalInt"].AsInt());
			Assert::AreEqual(99.99f, foo["InternalFloat"].AsFloat());
			Assert::AreEqual(vec4(9, 9, 9, 9), foo["InternalVector"].AsVector());
			Assert::AreEqual(mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16), foo["InternalMatrix"].AsMatrix());
			Assert::AreEqual("InternalString"s, foo["InternalString"].AsString());
			Assert::AreEqual(4_z, foo["InternalIntArray"].Size());
			Assert::AreEqual(3, foo["InternalIntArray"].AsInt(0));
			Assert::AreEqual(2, foo["InternalIntArray"].AsInt(1));
			Assert::AreEqual(1, foo["InternalIntArray"].AsInt(2));
			Assert::AreEqual(0, foo["InternalIntArray"].AsInt(3));
			Assert::AreEqual(4_z, foo["InternalFloatArray"].Size());
			Assert::AreEqual(3.5f, foo["InternalFloatArray"].AsFloat(0));
			Assert::AreEqual(2.5f, foo["InternalFloatArray"].AsFloat(1));
			Assert::AreEqual(1.5f, foo["InternalFloatArray"].AsFloat(2));
			Assert::AreEqual(0.5f, foo["InternalFloatArray"].AsFloat(3));
			Assert::AreEqual(4_z, foo["InternalVectorArray"].Size());
			Assert::AreEqual(vec4(), foo["InternalVectorArray"].AsVector(0));
			Assert::AreEqual(vec4(4, 3, 2, 1), foo["InternalVectorArray"].AsVector(1));
			Assert::AreEqual(vec4(9, 8, 7, 6), foo["InternalVectorArray"].AsVector(2));
			Assert::AreEqual(vec4(10, 10, 10, 10), foo["InternalVectorArray"].AsVector(3));
			Assert::AreEqual(3_z, foo["InternalMatrixArray"].Size());
			Assert::AreEqual(mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1), foo["InternalMatrixArray"].AsMatrix(0));
			Assert::AreEqual(mat4(-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16), foo["InternalMatrixArray"].AsMatrix(1));
			Assert::AreEqual(mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16), foo["InternalMatrixArray"].AsMatrix(2));
			Assert::AreEqual(3_z, foo["InternalStringArray"].Size());
			Assert::AreEqual("C"s, foo["InternalStringArray"].AsString(0));
			Assert::AreEqual("B"s, foo["InternalStringArray"].AsString(1));
			Assert::AreEqual("A"s, foo["InternalStringArray"].AsString(2));

			// Nested scopes
			Assert::AreEqual(3_z, foo["InternalScopeArray"].Size());
			Assert::AreEqual(2_z, foo["InternalScopeArray"][0].Size());
			Assert::AreEqual(1_z, foo["InternalScopeArray"][1].Size());
			Assert::AreEqual(0_z, foo["InternalScopeArray"][2].Size());
			Assert::AreEqual("Crusader Kings II"s, foo["InternalScopeArray"][0]["CK2"].AsString());
			Assert::AreEqual(233, foo["InternalScopeArray"][0]["MoreTable"][0]["value"].AsInt());
			Assert::AreEqual(vec4(1, 2, 3, 4), foo["InternalScopeArray"][1]["Haha"].AsVector());

			// Factory pattern valid
			Assert::AreEqual(3_z, foo["InternalScopeEmpty"].Size());
			Assert::IsTrue(foo["InternalScopeEmpty"][0].Is("Entity"));
			Assert::AreEqual("Bob"s, foo["InternalScopeEmpty"][0]["Name"].AsString());
			Assert::AreEqual(4_z, foo["InternalScopeEmpty"][0]["FIEA"].Size());
			Assert::AreEqual(1.1f, foo["InternalScopeEmpty"][0]["FIEA"].AsFloat(0));
			Assert::AreEqual(2.2f, foo["InternalScopeEmpty"][0]["FIEA"].AsFloat(1));
			Assert::AreEqual(3.3f, foo["InternalScopeEmpty"][0]["FIEA"].AsFloat(2));
			Assert::AreEqual(4.4f, foo["InternalScopeEmpty"][0]["FIEA"].AsFloat(3));
			Entity& entity = reinterpret_cast<Entity&>(foo["InternalScopeEmpty"][0]);
			Assert::AreEqual("Bob"s, entity.Name());
			Assert::AreEqual(123, foo["InternalScopeEmpty"][1]["value"].AsInt());
			Assert::IsFalse(foo["InternalScopeEmpty"][1].Is("Entity"));
			Assert::IsTrue(foo["InternalScopeEmpty"][2].Is("Entity"));
			Assert::AreEqual(string(), foo["InternalScopeEmpty"][2]["Name"].AsString());

			// Auxiliary attributes
			Assert::AreEqual("Auxiliary"s, foo["Auxiliary"].AsString());
		}

		TEST_METHOD(TestParseAttributedInvalid)
		{
			shared_ptr<AttributedFoo> fooPtr(new AttributedFoo(0));
			JsonTableParseHelper::SharedData data(fooPtr);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			Assert::IsFalse(master.Parse(R"({ "Int": { "type": "i", "value": 100 } })"));
			Assert::IsFalse(master.Parse(R"({ "Int": { "type": "integer", "value": "abc" } })"));
			Assert::IsFalse(master.Parse(R"({ "Float": { "type": "float", "value": "abc" } })"));
			Assert::IsFalse(master.Parse(R"({ "Vector": { "type": "vector", "value": 100.5 } })"));
			Assert::IsFalse(master.Parse(R"({ "Matrix": { "type": "matrix", "value": 100 } })"));
			Assert::IsFalse(master.Parse(R"({ "InternalScopeEmpty": { "type": "table", "value": 1} })"));
			Assert::IsFalse(master.Parse(R"({ "Int": { "type": "integer", "value": 100, "dummy": "1" } })"));
			Assert::IsFalse(master.Parse(R"({ "A": 1 })"));
			Assert::ExpectException<exception>([&master] { master.Parse(R"({ "Int": { "type": "float", "value": 10.6 } })"); });
			Assert::ExpectException<exception>([&master] { master.Parse(R"({ "IntArray": { "type": "integer", "value": [1,2,3,4,5] } })"); });

			JsonParseHelperInteger::SharedData badData;
			master.SetSharedData(badData);
			Assert::IsFalse(master.Parse(R"({ "Int": { "type": "integer", "value": 100 } })"));
		}

		TEST_METHOD(TestMixtureHelper)
		{
			shared_ptr<AttributedFoo> fooPtr(new AttributedFoo(0));
			JsonTableParseHelper::SharedData data(fooPtr);
			JsonParseMaster master(data);
			JsonTableParseHelper helper1;
			JsonParseHelperVector helper2;
			Assert::IsTrue(helper2.TypeIdInstance() == JsonParseHelperVector::TypeIdClass());
			master.AddHelper(helper1);

			string json = R"##({
				"VectorArray": {
					"type": "vector",
					"value": [
						"vec4(1, 2, 3, 4)",
						"[5,6,7,8]",
						"vec4(9, 10, 11, 12)"
					]
				}
			})##";
			Assert::IsFalse(master.Parse(json));
			master.AddHelper(helper2);
			Assert::IsTrue(master.Parse(json));
			Assert::AreEqual(vec4(1, 2, 3, 4), fooPtr->Find("VectorArray")->AsVector(0));
			Assert::AreEqual(vec4(5, 6, 7, 8), fooPtr->Find("VectorArray")->AsVector(1));
			Assert::AreEqual(vec4(9, 10, 11, 12), fooPtr->Find("VectorArray")->AsVector(2));
		}

		TEST_METHOD(TestParseEntity)
		{
			GameTime time;
			shared_ptr<World> worldPtr = make_shared<World>(time);
			JsonTableParseHelper::SharedData data(worldPtr);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);
			SectorFactory sf;
			EntityFactory ef;
			AvatarFactory af;
			AttributedTypeManager::RegisterType(Avatar::TypeIdClass(), Avatar::Signatures(), Entity::TypeIdClass());
			AttributedTypeManager::FinalizeSignature();

			// Check sector
			Assert::IsTrue(master.ParseFromFile("content/EntityJsonTest.json"));
			World& world = *worldPtr;
			Assert::AreEqual("The World"s, world.Name());
			Assert::AreEqual(3_z, world["Sectors"].Size());
			Assert::IsTrue(world["Sectors"][0].Is("Sector"));
			Assert::IsTrue(world["Sectors"][1].Is("Sector"));
			Assert::IsTrue(world["Sectors"][2].Is("Sector"));
			Assert::AreEqual(4_z, world["Sectors"][0].Size());
			Assert::AreEqual(4_z, world["Sectors"][1].Size());
			Assert::AreEqual(4_z, world["Sectors"][2].Size());
			Assert::AreEqual("Sector1"s, static_cast<Sector&>(world["Sectors"][0]).Name());
			Assert::AreEqual("Sector2"s, static_cast<Sector&>(world["Sectors"][1]).Name());
			Assert::AreEqual("Sector3"s, static_cast<Sector&>(world["Sectors"][2]).Name());
			Assert::AreEqual(0_z, world["Sectors"][0]["Entities"].Size());
			Assert::AreEqual(3_z, world["Sectors"][1]["Entities"].Size());
			Assert::AreEqual(0_z, world["Sectors"][2]["Entities"].Size());

			// Check entity
			Datum& entities = world["Sectors"][1]["Entities"];
			Assert::IsTrue(entities[0].Is("Entity"));
			Assert::AreEqual("Entity1"s, entities[0]["Name"].AsString());
			Assert::AreEqual("Entity1"s, static_cast<Entity&>(entities[0]).Name());

			Assert::IsTrue(entities[1].Is("Entity"));
			Assert::AreEqual("Entity2"s, static_cast<Entity&>(entities[1]).Name());
			Assert::AreEqual(0_z, entities[1]["Actions"].Size());

			Assert::IsTrue(entities[2].Is("Avatar"));
			Assert::AreEqual("AvatarTM"s, static_cast<Avatar&>(entities[2]).Name());
			Assert::AreEqual(123, static_cast<Avatar&>(entities[2]).mValue);
			Assert::AreEqual("Other"s, entities[2]["Other"].AsString());

			AttributedTypeManager::UnregisterType(Avatar::TypeIdClass());
		}

		TEST_METHOD(TestParseFromMultipleFiles)
		{
			shared_ptr<AttributedFoo> fooPtr = make_shared<AttributedFoo>(0);
			AttributedFoo& foo = *fooPtr;
			JsonTableParseHelper::SharedData data(fooPtr);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);
			EntityFactory ef;
			AttributedFooFactory aff;
			ActionIncrementFactory aif;

			Assert::IsTrue(master.ParseFromFile("content/MultiJson/MultiJsonTestRoot.json"));
			Assert::AreEqual(100, foo.mInt);
			Assert::AreEqual(3_z, foo["InternalScopeEmpty"].Size());
			Assert::AreEqual(1_z, foo["InternalScopeEmpty"].AsTable(0)["Actions"].Size());
			Assert::AreEqual(4_z, foo["InternalScopeEmpty"].AsTable(0)["Actions"].AsTable(0).Size());

			Entity& scope1 = static_cast<Entity&>(foo["InternalScopeEmpty"].AsTable(0));
			Assert::AreEqual("Entity1"s, scope1.Name());
			Assert::AreEqual(1_z, scope1.Actions().Size());
			ActionIncrement& increment = static_cast<ActionIncrement&>(scope1.Actions().AsTable(0));
			Assert::AreEqual("ActionIncrement"s, increment.Name());
			Assert::AreEqual("A"s, increment[ActionIncrement::TARGET_KEY].AsString());
			Assert::AreEqual(5.f, increment[ActionIncrement::STEP_KEY].AsFloat());
			Assert::AreEqual(3_z, scope1["Foo"].Size());
			Entity& entity1 = static_cast<Entity&>(scope1["Foo"].AsTable(0));
			Entity& entity2 = static_cast<Entity&>(scope1["Foo"].AsTable(1));
			Entity& entity3 = static_cast<Entity&>(scope1["Foo"].AsTable(2));
			auto entityCheck = [](Entity& entity) {
				Assert::AreEqual(1, entity["Int"].AsInt());
				Assert::AreEqual(vec4(1, 2, 3, 4), entity["Vector"].AsVector());
				Assert::AreEqual(1_z, entity["InternalScopeEmpty"].Size());
				Assert::AreEqual(1_z, entity["InternalScopeEmpty"].AsTable(0).Size());
				Assert::AreEqual("Balabala"s, entity["InternalScopeEmpty"].AsTable(0)["Bar"].AsString());
			};
			entityCheck(entity1);
			entityCheck(entity2);
			entityCheck(entity3);

			Scope& scope2 = foo["InternalScopeEmpty"].AsTable(1);
			Assert::AreEqual(0_z, scope2.Size());
			
			Entity& scope3 = static_cast<Entity&>(foo["InternalScopeEmpty"].AsTable(2));
			Assert::AreEqual("Hand Written Entity"s, scope3.Name());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState JsonTableParseTest::sStartMemState;
}