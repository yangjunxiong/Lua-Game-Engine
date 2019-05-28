#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseHelperInteger.h"
#include "JsonParseHelperTest.h"
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
	TEST_CLASS(JsonParserHelperTest)
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

		TEST_METHOD(TestIntegerHelper)
		{
			// Default state
			JsonParseHelperInteger helper;
			JsonParseHelperInteger::SharedData data;
			Assert::IsFalse(helper.mIsParsing);
			Assert::AreEqual(0_z, data.Depth());
			Assert::AreEqual(0, data.mData);

			// StartHandler
			string key = "integer";
			Json::Value value(666);
			JsonParseHelperTest::SharedData dummyData;
			Json::Value dummyValue;
			Assert::IsFalse(helper.StartHandler(dummyData, key, value, false));
			Assert::IsFalse(helper.StartHandler(data, key, dummyValue, false));
			Assert::IsTrue(helper.StartHandler(data, key, value, false));
			Assert::IsTrue(helper.mIsParsing);

			// EndHandler
			Assert::IsFalse(helper.StartHandler(data, key, value, false));
			Assert::IsFalse(helper.EndHandler(dummyData, key, value, false));
			Assert::IsFalse(helper.EndHandler(data, key, dummyValue, false));
			Assert::IsTrue(helper.EndHandler(data, key, value, false));
			Assert::IsFalse(helper.mIsParsing);
			Assert::IsFalse(helper.EndHandler(data, key, value, false));
			Assert::AreEqual(666, data.mData);
		}

		TEST_METHOD(TestTestHelper)
		{
			// Default state
			JsonParseHelperTest helper;
			JsonParseHelperTest::SharedData data;
			Assert::IsFalse(helper.mIsParsing);
			Assert::AreEqual(0_z, data.Depth());
			Assert::AreEqual(0_z, data.mMaxDepth);
			Assert::AreEqual(0_z, helper.mStartHandlerCount);
			Assert::AreEqual(0_z, helper.mEndHandlerCount);
			Assert::IsFalse(helper.mIsParsing);
			helper.mStartHandlerCount = 100;
			helper.mIsParsing = true;
			helper.Initialize();
			Assert::AreEqual(0_z, helper.mStartHandlerCount);
			Assert::IsFalse(helper.mIsParsing);

			// One level
			string key = "integer";
			Json::Value value(666);
			JsonParseHelperInteger::SharedData dummyData;
			Assert::IsFalse(helper.StartHandler(dummyData, key, value, false));
			Assert::IsTrue(helper.StartHandler(data, key, value, false));
			Assert::IsTrue(helper.mIsParsing);
			Assert::AreEqual(1_z, helper.mStartHandlerCount);
			Assert::AreEqual(0_z, helper.mEndHandlerCount);
			Assert::AreEqual(0_z, data.mMaxDepth);
			Assert::IsFalse(helper.EndHandler(dummyData, key, value, false));
			Assert::IsTrue(helper.EndHandler(data, key, value, false));
			Assert::IsFalse(helper.mIsParsing);
			Assert::IsFalse(helper.EndHandler(data, key, value, false));
			Assert::AreEqual(1_z, helper.mStartHandlerCount);
			Assert::AreEqual(1_z, helper.mEndHandlerCount);
			Assert::AreEqual(0_z, data.mMaxDepth);

			// Multi level
			helper.Initialize();
			data.mMaxDepth = 0;
			Json::Value object(Json::ValueType::objectValue);
			Assert::IsTrue(helper.StartHandler(data, key, value, false));
			Assert::IsTrue(helper.StartHandler(data, key, value, false));
			Assert::IsTrue(helper.StartHandler(data, key, value, false));
			data.IncrementDepth();
			Assert::IsTrue(helper.StartHandler(data, key, object, false));
			data.IncrementDepth();
			Assert::IsTrue(helper.StartHandler(data, key, object, false));
			Assert::AreEqual(5_z, helper.mStartHandlerCount);
			Assert::AreEqual(2_z, data.Depth());
			Assert::AreEqual(2_z, data.mMaxDepth);
			Assert::IsTrue(helper.mIsParsing);

			Assert::IsTrue(helper.EndHandler(data, key, object, false));
			data.DecrementDepth();
			Assert::IsTrue(helper.EndHandler(data, key, object, false));
			data.DecrementDepth();
			Assert::IsTrue(helper.EndHandler(data, key, value, false));
			Assert::IsTrue(helper.EndHandler(data, key, value, false));
			Assert::AreEqual(5_z, helper.mStartHandlerCount);
			Assert::AreEqual(4_z, helper.mEndHandlerCount);
			Assert::AreEqual(0_z, data.Depth());
			Assert::AreEqual(2_z, data.mMaxDepth);
			Assert::IsTrue(helper.mIsParsing);

			data.IncrementDepth();
			Assert::IsTrue(helper.StartHandler(data, key, object, false));
			data.IncrementDepth();
			Assert::IsTrue(helper.StartHandler(data, key, object, false));
			Assert::AreEqual(7_z, helper.mStartHandlerCount);
			Assert::AreEqual(2_z, data.Depth());
			Assert::AreEqual(2_z, data.mMaxDepth);
			Assert::IsTrue(helper.mIsParsing);
			Assert::IsTrue(helper.EndHandler(data, key, object, false));
			data.DecrementDepth();
			Assert::IsTrue(helper.EndHandler(data, key, object, false));
			data.DecrementDepth();
			Assert::IsTrue(helper.EndHandler(data, key, value, false));
			Assert::AreEqual(7_z, helper.mEndHandlerCount);
			Assert::AreEqual(0_z, data.Depth());
			Assert::AreEqual(2_z, data.mMaxDepth);
			Assert::IsFalse(helper.mIsParsing);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState JsonParserHelperTest::sStartMemState;
}