#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseHelperInteger.h"
#include "JsonParseHelperTest.h"
#include "JsonParseHelperObject.h"
#include "JsonParseMaster.h"
#include "Scope.h"
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
}

inline std::size_t operator "" _z(unsigned long long int x)
{
	return static_cast<size_t>(x);
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonParserMasterTest)
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

		TEST_METHOD(TestDefalutConstructor)
		{
			JsonParseHelperInteger::SharedData data;
			JsonParseMaster master(data);
			Assert::IsTrue(master.GetFileName().empty());
			Assert::IsFalse(master.IsClone());
			Assert::IsTrue(reinterpret_cast<BaseSharedData*>(&data) == master.GetSharedData());
			Assert::IsTrue(data.GetJsonParseMaster() == &master);
		}

		TEST_METHOD(TestClone)
		{
			JsonParseHelperObject::SharedData data;
			(*data.mScope)["integer"] = 100;
			JsonParseMaster master(data);
			JsonParseHelperInteger intHelper;
			JsonParseHelperObject objHelper;
			master.AddHelper(intHelper);
			master.AddHelper(objHelper);

			JsonParseMaster* clone = master.Clone();
			Assert::IsTrue(clone->IsClone());
			Assert::IsFalse(clone->GetSharedData() == &data);
			Assert::IsTrue(clone->GetSharedData()->GetJsonParseMaster() == clone);
			Assert::AreEqual(0_z, reinterpret_cast<JsonParseHelperObject::SharedData*>(clone->GetSharedData())->mScope->Size());
			Assert::IsTrue(clone->Parse(R"({ "integer": 10, "obj": [ {}, { "regetni": 01 }, {} ] })"));
			Assert::AreEqual(10, reinterpret_cast<JsonParseHelperObject::SharedData*>(clone->GetSharedData())->mScope->Find("integer")->AsInt());
			Assert::ExpectException<exception>([&clone, &intHelper] { clone->AddHelper(intHelper); });
			Assert::ExpectException<exception>([&clone, &intHelper] { clone->RemoveHelper(intHelper); });
			delete clone;
		}

		TEST_METHOD(TestMoveConstructor)
		{
			JsonParseHelperInteger::SharedData data;
			JsonParseMaster master(data);
			JsonParseHelperInteger intHelper;
			master.AddHelper(intHelper);

			JsonParseMaster copy = move(master);
			Assert::IsFalse(copy.IsClone());
			Assert::IsTrue(copy.GetSharedData() == &data);
			Assert::IsTrue(copy.Parse(R"({ "integer": 10 })"));
			Assert::IsTrue(copy.GetSharedData()->GetJsonParseMaster() == &copy);
			Assert::IsFalse(master.IsClone());
			Assert::IsTrue(master.GetSharedData() == nullptr);

			JsonParseMaster* clone = copy.Clone();
			BaseSharedData* cloneData = clone->GetSharedData();
			JsonParseMaster copyFromClone = move(*clone);
			Assert::IsTrue(copyFromClone.IsClone());
			Assert::IsTrue(cloneData = copyFromClone.GetSharedData());
			Assert::IsTrue(copyFromClone.GetSharedData()->GetJsonParseMaster() == &copyFromClone);
			Assert::IsTrue(copyFromClone.Parse(R"({ "integer": 10 })"));
			Assert::IsFalse(clone->IsClone());
			Assert::IsTrue(clone->GetSharedData() == nullptr);
			delete clone;
		}

		TEST_METHOD(TestMoveOperator)
		{
			// Create original
			JsonParseHelperInteger::SharedData data;
			JsonParseMaster master(data);
			JsonParseHelperInteger intHelper;
			master.AddHelper(intHelper);

			// Fill in copy
			JsonParseHelperObject::SharedData data2;
			JsonParseMaster copy(data2);
			JsonParseHelperObject objectHelper;
			copy.AddHelper(objectHelper);
			Assert::IsTrue(copy.Parse(R"({ "obj": {} })"));
			Assert::IsFalse(copy.Parse(R"({ "integer": 10 })"));

			// Clear copy and move from master to copy
			copy = move(master);
			Assert::IsFalse(copy.IsClone());
			Assert::IsTrue(copy.GetSharedData() == &data);
			Assert::IsTrue(copy.Parse(R"({ "integer": 10 })"));
			Assert::IsFalse(copy.Parse(R"({ "obj": {} })"));
			Assert::IsFalse(master.IsClone());
			Assert::IsTrue(master.GetSharedData() == nullptr);

			// Create a clone from copy and move from copy to clone
			JsonParseMaster* clone = copy.Clone();
			Assert::IsTrue(clone->IsClone());
			Assert::IsTrue(clone->GetSharedData() != &data);
			Assert::IsFalse(clone->GetSharedData()->GetJsonParseMaster() == &copy);
			*clone = move(copy);
			Assert::IsTrue(clone->GetSharedData()->GetJsonParseMaster() == clone);
			Assert::IsTrue(clone->GetSharedData() == &data);
			clone->SetSharedData(data2);
			Assert::IsTrue(data2.GetJsonParseMaster() == clone);
			delete clone;
		}

		TEST_METHOD(TestAddRemoveHelper)
		{
			JsonParseHelperInteger::SharedData data;
			JsonParseMaster master(data);
			JsonParseHelperInteger helper[2];
			JsonParseHelperTest helper2[2];

			// Original
			Assert::IsTrue(master.AddHelper(helper[0]));
			Assert::IsTrue(master.AddHelper(helper2[0]));
			Assert::IsFalse(master.AddHelper(helper[1]));
			Assert::IsFalse(master.AddHelper(helper2[1]));
			Assert::IsTrue(master.RemoveHelper(helper[1]));
			Assert::IsFalse(master.RemoveHelper(helper[0]));
			Assert::IsTrue(master.RemoveHelper(helper2[1]));
			Assert::IsFalse(master.RemoveHelper(helper2[0]));

			// Clone
			master.AddHelper(helper[0]);
			master.AddHelper(helper2[0]);
			auto clone = master.Clone();
			Assert::ExpectException<exception>([&clone, &helper] { clone->AddHelper(helper[0]); });
			Assert::ExpectException<exception>([&clone, &helper] { clone->RemoveHelper(helper[0]); });
			delete clone;
		}

		TEST_METHOD(TestParseRecursion)
		{
			JsonParseHelperTest helper;
			JsonParseHelperTest::SharedData data;
			JsonParseMaster master(data);
			master.AddHelper(helper);

			string text = R"({
				"obj": [
					1,
					{ "obj": { "obj": { "value": 10 } } },
					{ "obj": [1, 2, 3] }
				]
			})";
			Assert::IsTrue(master.Parse(text));
			Assert::AreEqual(9_z, helper.mStartHandlerCount);
			Assert::AreEqual(9_z, helper.mEndHandlerCount);
			Assert::AreEqual(6_z, helper.mArrayCount);
			Assert::IsFalse(helper.mIsParsing);
			Assert::AreEqual(3_z, data.mMaxDepth);
		}

		TEST_METHOD(TestParsePrimitive)
		{
			JsonParseHelperInteger::SharedData data;
			JsonParseMaster master(data);
			JsonParseHelperInteger helper;
			master.AddHelper(helper);

			Assert::IsTrue(master.Parse(R"({ "integer": 666, "integer2": 10086 })"));
			Assert::AreEqual(10086, data.mData);
			Assert::AreEqual(0_z, data.Depth());
			Assert::IsFalse(helper.mIsParsing);

			data.mData = 0;
			Assert::IsFalse(master.Parse(R"({ "notInteger": "a" })"));
			Assert::AreEqual(0, data.mData);
			Assert::AreEqual(0_z, data.Depth());
			Assert::IsFalse(helper.mIsParsing);
		}

		TEST_METHOD(TestParseArray)
		{
			JsonParseHelperInteger::SharedData data;
			JsonParseMaster master(data);
			JsonParseHelperInteger helper;
			master.AddHelper(helper);

			Assert::IsTrue(master.Parse(R"({ "data": [1] })"));
			Assert::AreEqual(1, data.mData);
			Assert::AreEqual(0_z, data.Depth());
			Assert::IsFalse(helper.mIsParsing);

			Assert::IsTrue(master.Parse(R"({ "data": [1, 2, 3] })"));
			Assert::AreEqual(3, data.mData);
			Assert::AreEqual(0_z, data.Depth());
			Assert::IsFalse(helper.mIsParsing);

			Assert::IsFalse(master.Parse(R"({ "data": [1, 2, "a", "b", 3] })"));
			Assert::AreEqual(2, data.mData);
			Assert::AreEqual(0_z, data.Depth());
			Assert::IsFalse(helper.mIsParsing);
		}

		TEST_METHOD(TestParseObject)
		{
			JsonParseHelperObject::SharedData data;
			JsonParseMaster master(data);
			JsonParseHelperInteger integerHelper;
			JsonParseHelperObject objectHelper;
			master.AddHelper(integerHelper);
			master.AddHelper(objectHelper);

			Assert::IsTrue(master.Parse(R"({ "obj": { "integer": 10 } })"));
			Assert::AreEqual(1_z, data.mScope->Size());
			Assert::AreEqual(Datum::DatumType::Table, data.mScope->Find("obj"s)->Type());
			Assert::AreEqual(Datum::DatumType::Integer, (*data.mScope)["obj"][0]["integer"].Type());
			Assert::AreEqual(10, (*data.mScope)["obj"][0]["integer"].AsInt());
			Assert::AreEqual(0_z, data.Depth());
			Assert::IsFalse(integerHelper.mIsParsing);
			Assert::AreEqual(0_z, objectHelper.mStack.Size());

			data.mScope->Clear();
			Assert::IsTrue(master.Parse(R"({ "objs": [ {}, { "value": 233, "value2": 666, "value3": {} }, {}, { "values": [1, 2, 3] } ] })"));
			Assert::AreEqual(1_z, data.mScope->Size());
			Assert::AreEqual(Datum::DatumType::Table, (*data.mScope)[0].Type());
			Assert::AreEqual(4_z, (*data.mScope)["objs"].Size());
			Assert::AreEqual(0_z, (*data.mScope)["objs"][0].Size());
			Assert::AreEqual(3_z, (*data.mScope)["objs"][1].Size());
			Assert::AreEqual(0_z, (*data.mScope)["objs"][2].Size());
			Assert::AreEqual(1_z, (*data.mScope)["objs"][3].Size());
			Assert::AreEqual(666, (*data.mScope)["objs"][1]["value2"].AsInt());
			Assert::AreEqual(0_z, (*data.mScope)["objs"][1]["value3"].AsTable().Size());
			Assert::AreEqual(Datum::DatumType::Integer, (*data.mScope)["objs"][3]["values"].Type());
			Assert::AreEqual(3_z, (*data.mScope)["objs"][3]["values"].Size());
			Assert::AreEqual(1, (*data.mScope)["objs"][3]["values"].AsInt());
			Assert::AreEqual(2, (*data.mScope)["objs"][3]["values"].AsInt(1));
			Assert::AreEqual(3, (*data.mScope)["objs"][3]["values"].AsInt(2));
			Assert::AreEqual(0_z, data.Depth());
			Assert::IsFalse(integerHelper.mIsParsing);
			Assert::AreEqual(0_z, objectHelper.mStack.Size());

			data.mScope->Clear();
			Assert::IsFalse(master.Parse(R"({ "obj": { "integer": 123, "value": "123" } })"));
			Assert::AreEqual(0_z, data.Depth());
			Assert::IsFalse(integerHelper.mIsParsing);
			Assert::AreEqual(0_z, objectHelper.mStack.Size());

			Assert::IsFalse(master.Parse(R"([1, 2, { "integer": 10 }])"));
		}

		TEST_METHOD(TestSetGetSharedData)
		{
			JsonParseHelperObject::SharedData data[3];
			JsonParseMaster master(data[0]);
			Assert::IsTrue(master.GetSharedData() == &data[0]);
			Assert::IsTrue(data[0].GetJsonParseMaster() == &master);
			master.SetSharedData(data[1]);
			Assert::IsTrue(master.GetSharedData() == &data[1]);
			Assert::IsTrue(const_cast<const JsonParseMaster*>(&master)->GetSharedData() == &data[1]);
			Assert::IsTrue(data[1].GetJsonParseMaster() == &master);
			Assert::IsTrue(data[0].GetJsonParseMaster() == nullptr);

			JsonParseMaster* clone = master.Clone();
			Assert::IsTrue(data[1].GetJsonParseMaster() == &master);
			Assert::IsFalse(clone->GetSharedData() == &data[1]);
			clone->SetSharedData(data[2]);
			Assert::IsTrue(clone->GetSharedData() == &data[2]);
			Assert::IsTrue(data[2].GetJsonParseMaster() == clone);
			delete clone;

			JsonParseHelperInteger::SharedData data2;
			master.SetSharedData(data2);
			Assert::IsTrue(master.GetSharedData() == &data2);
			clone = master.Clone();
			Assert::IsTrue(clone->GetSharedData()->Is(JsonParseHelperInteger::SharedData::TypeIdClass()));
			Assert::IsFalse(clone->GetSharedData() == &data2);
			delete clone;
		}

		TEST_METHOD(TestParseFile)
		{
			JsonParseHelperObject::SharedData data;
			JsonParseMaster master(data);
			JsonParseHelperInteger intHelper;
			JsonParseHelperObject objectHelper;
			master.AddHelper(intHelper);
			master.AddHelper(objectHelper);
			Assert::IsTrue(master.ParseFromFile("content/jsonTest.json"));

			Assert::AreEqual("content/jsonTest.json"s, master.GetFileName());
			Scope& scope = *data.mScope;
			Assert::AreEqual(3_z, scope["integer"].Size());
			Assert::AreEqual(1, scope["integer"].AsInt());
			Assert::AreEqual(2, scope["integer"].AsInt(1));
			Assert::AreEqual(3, scope["integer"].AsInt(2));
			Assert::AreEqual(Datum::DatumType::Table, scope["object"].Type());
			Assert::AreEqual(10, scope["object"][0]["integer"].AsInt());
			Assert::AreEqual(11, scope["object"][0]["integer2"].AsInt());
			Assert::AreEqual(3_z, scope["object"][0]["object"][0]["integer"].Size());
			Assert::AreEqual(2, scope["object"][0]["object"][0]["integer"].AsInt());
			Assert::AreEqual(3, scope["object"][0]["object"][0]["integer"].AsInt(1));
			Assert::AreEqual(4, scope["object"][0]["object"][0]["integer"].AsInt(2));
		}

		TEST_METHOD(TestParseInvalidJson)
		{
			JsonParseHelperObject::SharedData data;
			JsonParseMaster master(data);
			JsonParseHelperInteger intHelper;
			JsonParseHelperObject objectHelper;
			master.AddHelper(intHelper);
			master.AddHelper(objectHelper);

			Assert::ExpectException<exception>([&master] { master.Parse("hello world"); });
			Assert::ExpectException<exception>([&master] { stringstream stream("yahaha"); master.Parse(stream); });
			Assert::ExpectException<exception>([&master] { master.ParseFromFile("ImaginaryFile.json"); });
			Assert::ExpectException<exception>([&master] { master.ParseFromFile("content/invalidJson.json"); });
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState JsonParserMasterTest::sStartMemState;
}