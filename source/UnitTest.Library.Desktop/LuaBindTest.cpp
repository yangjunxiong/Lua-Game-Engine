#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseMaster.h"
#include "JsonTableParseHelper.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "Avatar.h"
#include "WorldState.h"
#include "GameClock.h"
#include "GameTime.h"
#include <json/json.h>
#include "LuaBind.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngine;
using namespace GameEngine::Lua;
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

	template<> inline std::wstring ToString<int64_t>(const int64_t& value)
	{
		RETURN_WIDE_STRING(value);
	}
}

inline std::size_t operator "" _z(unsigned long long int x)
{
	return static_cast<size_t>(x);
}

void f() {};

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(LuaBindTest)
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

		static void Print(const char* str)
		{
			Logger::WriteMessage(str);
		}

		static void Check(const char* expect, const char* actual)
		{
			Assert::AreEqual(string(expect), string(actual));
		}

		static void CheckNumber(double expect, double actual)
		{
			Assert::AreEqual(expect, actual);
		}

		TEST_METHOD(TestTable)
		{
			LuaBind bind;
			Assert::AreEqual(""s, bind.CurrentTableName());
			Assert::AreEqual(0, bind.CurrentTableIndex());

			bind.CreateTable("table1");
			Assert::AreEqual("table1"s, bind.CurrentTableName());
			Assert::AreEqual(1, bind.CurrentTableIndex());

			bind.CreateTable("table2");
			Assert::AreEqual("table2"s, bind.CurrentTableName());
			Assert::AreEqual(2, bind.CurrentTableIndex());

			bind.CreateTable("table3");
			Assert::AreEqual("table3"s, bind.CurrentTableName());
			Assert::AreEqual(3, bind.CurrentTableIndex());

			bind.CloseTable();
			Assert::AreEqual("table2"s, bind.CurrentTableName());
			Assert::AreEqual(2, bind.CurrentTableIndex());

			bind.CloseTable();
			Assert::AreEqual("table1"s, bind.CurrentTableName());
			Assert::AreEqual(1, bind.CurrentTableIndex());

			bind.OpenTable("table2");
			Assert::AreEqual("table2"s, bind.CurrentTableName());
			Assert::AreEqual(2, bind.CurrentTableIndex());

			bind.ClearStack();
			Assert::AreEqual(""s, bind.CurrentTableName());
			Assert::AreEqual(0, bind.CurrentTableIndex());

			bind.OpenTable("table1.table2.table3");
			Assert::AreEqual("table3"s, bind.CurrentTableName());
			Assert::AreEqual(3, bind.CurrentTableIndex());
			bind.CloseTable();
			Assert::AreEqual("table2"s, bind.CurrentTableName());
			Assert::AreEqual(2, bind.CurrentTableIndex());
			bind.CloseTable();
			Assert::AreEqual("table1"s, bind.CurrentTableName());
			Assert::AreEqual(1, bind.CurrentTableIndex());
			bind.CloseTable();
			Assert::AreEqual(""s, bind.CurrentTableName());
			Assert::AreEqual(0, bind.CurrentTableIndex());
		}

		TEST_METHOD(TestSetValue)
		{
			LuaBind bind;
			bind.SetFunction("check", function(Check));
			bind.CreateTable("test");
			bind.SetLuaValue("int32", 10);
			bind.SetLuaValue("int64", 11LL);
			bind.SetLuaValue("float", 12.f);
			bind.SetLuaValue("double", 13.0);
			bind.SetLuaValue("bool", true);
			bind.SetLuaValue("byte", 1);
			bind.SetLuaValue("cstring", static_cast<char*>("ccc"));
			bind.SetLuaValue("string", string("bbb"));
			bind.LoadFile("content/Lua/TestSetValue.lua");
		}

		TEST_METHOD(TestPrimitiveFunctionArgument)
		{
			LuaBind bind;
			auto func0 = []()
			{
				Logger::WriteMessage("Hello LUA 0");
			};
			auto func1 = [](int count)
			{
				for (int i = 0; i < count; ++i)
				{
					Logger::WriteMessage("Hello LUA 1");
				}
			};
			auto func2 = [](long long count)
			{
				for (int i = 0; i < count; ++i)
				{
					Logger::WriteMessage("Hello LUA 2");
				}
			};
			auto func3 = [](float count)
			{
				for (int i = 0; i < (int)count; ++i)
				{
					Logger::WriteMessage("Hello LUA 3");
				}
			};
			auto func4 = [](double count)
			{
				for (int i = 0; i < (int)count; ++i)
				{
					Logger::WriteMessage("Hello LUA 4");
				}
			};
			auto func5 = [](std::string str)
			{
				Logger::WriteMessage(str.c_str());
			};
			auto func6 = [](const char* str)
			{
				Logger::WriteMessage(str);
			};
			auto func7 = [](bool run)
			{
				if (run)
				{
					Logger::WriteMessage("Bool test");
				}
			};
			auto func8 = [](char)
			{
				Logger::WriteMessage("char test");
			};
			bind.SetFunction("test0", function(func0));
			bind.SetFunction("test1", function(func1));
			bind.SetFunction("test2", function(func2));
			bind.SetFunction("test3", function(func3));
			bind.SetFunction("test4", function(func4));
			bind.SetFunction("test5", function(func5));
			bind.SetFunction("test6", function(func6));
			bind.SetFunction("test7", function(func7));
			bind.SetFunction("test8", function(func8));
			
			string lua = R"#(
				test0()
				test1(1)
				test2(2)
				test3(3.0)
				test4(4.0)
				test5("abc")
				test6("def")
				test7(false)
				test8(1)
			)#";
			bind.LoadString(lua);
		}

		TEST_METHOD(TestPrimitiveFunctionReturn)
		{
			LuaBind bind;
			auto func0 = [](int value) -> int {
				return value + 1;
			};
			auto func1 = [](long long value) -> long long
			{
				return value + 2;
			};
			auto func2 = [](float value) -> float
			{
				return value + 3.f;
			};
			auto func3 = [](double value) -> double
			{
				return value + 4.f;
			};
			auto func4 = [](bool value) -> bool
			{
				return !value;
			};
			auto func5 = [](std::string str) -> std::string
			{
				return str + "LUA";
			};
			auto func6 = [](const char* str) -> const char*
			{
				const_cast<char*>(str)[0] = 'L';
				return str;
			};
			auto func7 = [](char c) -> char
			{
				++c;
				return c;
			};

			bind.SetFunction("check", function(Check));
			bind.SetFunction("test0", function(func0));
			bind.SetFunction("test1", function(func1));
			bind.SetFunction("test2", function(func2));
			bind.SetFunction("test3", function(func3));
			bind.SetFunction("test4", function(func4));
			bind.SetFunction("test5", function(func5));
			bind.SetFunction("test6", function(func6));
			bind.SetFunction("test7", function(func7));
			bind.LoadFile("content/Lua/Test.lua");
		}

		TEST_METHOD(TestMultiparamFunction)
		{
			auto func1 = [](int a, int b, int c) -> int
			{
				return a + b + c;
			};
			auto func2 = [](int a, int b, int c, int d) -> int
			{
				return a + b + c + d;
			};
			auto func3 = [](int a, int b, int c, int d, int e) -> int
			{
				return a + b + c + d + e;
			};
			auto func4 = [](int a, int b, int c, int d, int e, int f) -> int
			{
				return a + b + c + d + e + f;
			};

			LuaBind bind;
			bind.SetFunction("check", function(Check));
			bind.SetFunction("test1", function(func1));
			bind.SetFunction("test2", function(func2));
			bind.SetFunction("test3", function(func3));
			bind.SetFunction("test4", function(func4));
			string lua = R"#(
				check("6", tostring(test1(1, 2, 3)));
				check("10", tostring(test2(1, 2, 3, 4)));
				check("15", tostring(test3(1, 2, 3, 4, 5)));
				check("21", tostring(test4(1, 2, 3, 4, 5, 6)));
			)#";
			bind.LoadString(lua);
		}

		TEST_METHOD(TestPointerReference)
		{
			LuaBind bind;
			bind.SetFunction("check", function(Check));
			std::string temp = "123";
			LuaWrapper<std::string>::Create(bind.LuaState(), &temp, "temp");
			string lua = R"#(
				check("123", tostring(temp))
			)#";
			bind.LoadString(lua);

			auto func1 = [](std::string* temp) -> std::string*
			{
				*temp = "234";
				return temp;
			};
			bind.SetFunction("func1", function(func1));
			lua = R"#(check("234", tostring(func1(temp))))#";
			bind.LoadString(lua);

			auto func2 = [](std::string& temp) -> std::string&
			{
				temp = "345";
				return temp;
			};
			bind.SetFunction("func2", function(func2));
			lua = R"#(check("345", tostring(func2(temp))))#";
			bind.LoadString(lua);

			auto func3 = [](const std::string* temp) -> const std::string*
			{
				*const_cast<std::string*>(temp) = "456";
				return temp;
			};
			bind.SetFunction("func3", function(func3));
			lua = R"#(check("456", tostring(func3(temp))))#";
			bind.LoadString(lua);

			auto func4 = [](const std::string& temp) -> const std::string&
			{
				const_cast<std::string&>(temp) = "567";
				return temp;
			};
			bind.SetFunction("func4", function(func4));
			lua = R"#(check("567", tostring(func4(temp))))#";
			bind.LoadString(lua);
		}

		TEST_METHOD(TestClassBinding)
		{
			LuaBind bind;
			bind.SetFunction("check", function(Check));
			bind.SetFunction("checkNumber", function(CheckNumber));
			std::string temp("hello");
			std::string temp2("lua");
			std::string temp3("!");
			std::string temp4("1 + 2 = ");
			LuaWrapper<std::string>::Create(bind.LuaState(), &temp, "Temp");
			LuaWrapper<std::string>::Create(bind.LuaState(), &temp2, "Temp2");
			LuaWrapper<std::string>::Create(bind.LuaState(), &temp3, "Temp3");
			LuaWrapper<std::string>::Create(bind.LuaState(), &temp4, "Temp4");
			bind.LoadFile("content/Lua/TestString.lua");
			Assert::AreEqual("1 + 2 = 3"s, temp4);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState LuaBindTest::sStartMemState;
}