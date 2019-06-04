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

class Dummy
{
public:
	int mDoCount = 0;
	std::string mText = "Hello";
	int Do()
	{
		return ++mDoCount;
	}

	int Do2(int count)
	{
		mDoCount += count;
		return mDoCount;
	}

	int Do3(int count1, int count2)
	{
		mDoCount = mDoCount + count1 - count2;
		return mDoCount;
	}

	int Do4(int c1, int c2, int c3)
	{
		mDoCount = mDoCount + c1 - c2 + c3;
		return mDoCount;
	}

	int Do5(int c1, int c2, int c3, int c4)
	{
		mDoCount = mDoCount + c1 - c2 + c3 - c4;
		return mDoCount;
	}

	int Do6(int c1, int c2, int c3, int c4, int c5)
	{
		mDoCount = mDoCount + c1 - c2 + c3 - c4 + c5;
		return mDoCount;
	}

	int Do7(int c1, int c2, int c3, int c4, int c5, int c6)
	{
		mDoCount = mDoCount + c1 - c2 + c3 - c4 + c5 - c6;
		return mDoCount;
	}

	std::string& AppendText(Dummy* other)
	{
		mText += other->mText;
		return mText;
	}
};
DECLARE_LUA_WRAPPER(Dummy, "Dummy");

class A
{
public:
	int DoA(int a) { return a + 1; }
	virtual int Do(int value) { return DoA(value); }
	int dataA = 0;
};
DECLARE_LUA_WRAPPER(A, "A");

class B : public A
{
public:
	int DoB(int b) { return b + 2; }
	virtual int Do(int value) override { return DoB(value); }
	int dataB = 0;
};
DECLARE_LUA_WRAPPER(B, "B");

class C : public B
{
public:
	int DoC(int c) { return c + 3; }
	virtual int Do(int value) override { return DoC(value); }
	C* Poly(A& a1, const A& a2, const B* b, C* c)
	{
		dataC = a1.dataA + a2.dataA + b->dataB + c->dataC;
		return this;
	}
	int dataC = 0;
};
DECLARE_LUA_WRAPPER(C, "C");

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
				return a + b - c;
			};
			auto func2 = [](int a, int b, int c, int d) -> int
			{
				return a + b - c + d;
			};
			auto func3 = [](int a, int b, int c, int d, int e) -> int
			{
				return a + b - c + d - e;
			};
			auto func4 = [](int a, int b, int c, int d, int e, int f) -> int
			{
				return a + b - c + d - e + f;
			};

			LuaBind bind;
			bind.SetFunction("CheckNumber", function(CheckNumber));
			bind.SetFunction("test1", function(func1));
			bind.SetFunction("test2", function(func2));
			bind.SetFunction("test3", function(func3));
			bind.SetFunction("test4", function(func4));
			string lua = R"#(
				CheckNumber(0, test1(1, 2, 3));
				CheckNumber(4, test2(1, 2, 3, 4));
				CheckNumber(-1, test3(1, 2, 3, 4, 5));
				CheckNumber(5, test4(1, 2, 3, 4, 5, 6));
			)#";
			bind.LoadString(lua);
		}

		TEST_METHOD(TestPointerReference)
		{
			LuaBind bind;
			bind.SetFunction("check", function(Check));
			std::string temp = "123";
			bind.SetProperty("temp", &temp);
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
			bind.SetProperty("Temp", &temp);
			bind.SetProperty("Temp2", &temp2);
			bind.SetProperty("Temp3", &temp3);
			bind.SetProperty("Temp4", &temp4);
			bind.LoadFile("content/Lua/TestString.lua");
			Assert::AreEqual("1 + 2 = 3"s, temp4);
		}

		TEST_METHOD(TestClassMemberFunction)
		{
			LuaBind bind;
			LuaWrapper<Dummy>::Register(bind.LuaState());
			bind.SetFunction("CheckNumber", std::function(CheckNumber));
			bind.SetFunction("Do", &Dummy::Do);
			bind.SetFunction("Do2", &Dummy::Do2);
			bind.SetFunction("Do3", &Dummy::Do3);
			bind.SetFunction("Do4", &Dummy::Do4);
			bind.SetFunction("Do5", &Dummy::Do5);
			bind.SetFunction("Do6", &Dummy::Do6);
			bind.SetFunction("Do7", &Dummy::Do7);
			Dummy dummy;
			bind.SetProperty("obj", &dummy);
			std::string lua = R"#(
				CheckNumber(1, obj:Do())
				CheckNumber(2, obj:Do())
				CheckNumber(3, obj:Do())
				local obj2 = Dummy()
				CheckNumber(1, obj2:Do())
				CheckNumber(4, obj:Do())

				local obj3 = Dummy()
				CheckNumber(2, obj3:Do2(2))
				CheckNumber(1, obj3:Do3(1, 2))
				CheckNumber(3, obj3:Do4(1, 2, 3))
				CheckNumber(1, obj3:Do5(1, 2, 3, 4))
				CheckNumber(4, obj3:Do6(1, 2, 3, 4, 5))
				CheckNumber(1, obj3:Do7(1, 2, 3, 4, 5, 6))
			)#";
			bind.LoadString(lua);
		}

		TEST_METHOD(TestClassInheritance)
		{
			LuaBind bind;
			bind.SetFunction("Check", function(Check));
			bind.SetFunction("CheckNumber", function(CheckNumber));
			LuaWrapper<A>::Register(bind.LuaState());
			LuaWrapper<B>::Register<A>(bind.LuaState());
			LuaWrapper<C>::Register<B>(bind.LuaState());
			bind.SetFunction("DoA", &A::DoA);
			bind.SetFunction("DoB", &B::DoB);
			bind.SetFunction("DoC", &C::DoC);
			bind.SetFunction("Do", &A::Do);
			bind.SetFunction("Do", &B::Do);
			bind.SetFunction("Do", &C::Do);
			bind.SetProperty("dataA", &A::dataA);
			bind.SetProperty("dataB", &B::dataB);
			bind.SetProperty("dataC", &C::dataC);
			A* a = new C();
			bind.CreateTable("Test");
			bind.SetProperty("a", a, bind.CurrentTableIndex());
			std::string lua = R"#(
				local a = A()
				local b = B()
				local c = C()
				a.dataA = 1
				b.dataA = 1
				b.dataB = 2
				c.dataA = 1
				c.dataB = 2
				c.dataC = 3
				CheckNumber(1, a.dataA)
				CheckNumber(1, b.dataA)
				CheckNumber(2, b.dataB)
				CheckNumber(1, c.dataA)
				CheckNumber(2, c.dataB)
				CheckNumber(3, c.dataC)

				CheckNumber(1, a:DoA(0))
				CheckNumber(1, b:DoA(0))
				CheckNumber(2, b:DoB(0))
				CheckNumber(1, c:DoA(0))
				CheckNumber(2, c:DoB(0))
				CheckNumber(3, c:DoC(0))

				CheckNumber(1, a:Do(0))
				CheckNumber(2, b:Do(0))
				CheckNumber(3, c:Do(0))

				CheckNumber(3, Test.a:Do(0))
			)#";
			bind.LoadString(lua);
			delete a;

			bind.SetFunction("Poly", &C::Poly);
			lua = R"#(
				a = A()
				a.dataA = 10
				b = B()
				b.dataB = 20
				c = C()
				c.dataC = 30
				local c2 = c:Poly(a, c, b, c)
				CheckNumber(60, c2.dataC)
				local c3 = c2
				Check("true", tostring(c2 == c))
				Check("true", tostring(c2 == c3))
				Check("false", tostring(a == c))
				Check("false", tostring(a == b))
				Check("false", tostring(b == c))
				Check("false", tostring(c == 30))
			)#";
			bind.LoadString(lua);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState LuaBindTest::sStartMemState;
}
