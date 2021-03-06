#include "pch.h"
#include "CppUnitTest.h"
#include "LuaBind.h"
#include "Action.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngine;
using namespace GameEngine::Lua;
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

void f() {};

class Dummy
{
public:
	Dummy() {};
	Dummy(int a) { mDoCount = a; };
	Dummy(int a, int b) { mDoCount = a - b; };
	Dummy(int a, int b, int c) { mDoCount = a - b + c; };
	Dummy(int a, int b, int c, int d) { mDoCount = a - b + c - d; };
	Dummy(int a, int b, int c, int d, int e) { mDoCount = a - b + c - d + e; };
	Dummy(int a, int b, int c, int d, int e, int f) { mDoCount = a - b + c - d + e - f; };

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
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Dummy);
LUA_DEFINE_CUSTOM_COPY_TYPE(Dummy);

class A
{
public:
	int DoA(int a) { return a + 1; }
	virtual int Do(int value) { return DoA(value); }
	int dataA = 0;

	static inline int data = 0;
	static int Foo(int a) { return a + 1; }
};
DECLARE_LUA_WRAPPER(A, "A");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(A);
LUA_DEFINE_CUSTOM_COPY_TYPE(A);

class B : public A
{
public:
	int DoB(int b) { return b + 2; }
	virtual int Do(int value) override { return DoB(value); }
	int dataB = 0;

	static inline int data = 0 ;
	static int Foo(int a) { return a + 2; }
};
DECLARE_LUA_WRAPPER(B, "B");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(B);
LUA_DEFINE_CUSTOM_COPY_TYPE(B);

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

	static inline int data = 0;
	static int Foo(int a) { return a + 3; }
};
DECLARE_LUA_WRAPPER(C, "C");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(C);
LUA_DEFINE_CUSTOM_COPY_TYPE(C);

class ConstClass
{
public:
	const int a = 5;
	const int b = 4;
	int c = 3;

	static inline const int d = 10;
	static inline int e = 11;

	ConstClass()
	{
		mDummy = new Dummy();
	}

	~ConstClass()
	{
		delete mDummy;
	}

	glm::vec4 mVec { 0, 1, 2, 3 };
	const Dummy* mDummy;
	const static inline glm::vec4 sVec { 3, 2, 1, 0 };
	static inline Dummy sDummy;

	void Do() { c += 10; };
	void DoConst() const { int r = c + 10; r; };
};
DECLARE_LUA_WRAPPER(ConstClass, "ConstClass");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(ConstClass);
LUA_DEFINE_CUSTOM_COPY_TYPE(ConstClass);

class Container
{
public:
	std::vector<int> mInts;
	std::vector<float> mFloats;
	std::vector<glm::vec4> mVectors;
	std::vector<Container> mValues;
	std::vector<Container*> mPointers;

	void Set(std::vector<int>& ints, std::vector<float>* floats, std::vector<glm::vec4> vecs)
	{
		mInts = ints;
		floats->clear();
		vecs.clear();
	}

	std::vector<int> GetInts() { return mInts; }
	const std::vector<float>* GetFloats() { return &mFloats; }
	const std::vector<glm::vec4>* GetVectors() { return &mVectors; }
};
DECLARE_LUA_WRAPPER(Container, "Container");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Container);
LUA_DEFINE_CUSTOM_COPY_TYPE(Container);
DECLARE_LUA_VECTOR_WRAPPER_ALL(Container, "Container");

DECLARE_LUA_WRAPPER(Action, "Action");
LUA_DEFINE_CUSTOM_OBJECT_TYPE(Action);

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
			bind.SetConstructor<Dummy>();
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
				local obj2 = Dummy.New()
				CheckNumber(1, obj2:Do())
				CheckNumber(4, obj:Do())

				local obj3 = Dummy.New()
				CheckNumber(2, obj3:Do2(2))
				CheckNumber(1, obj3:Do3(1, 2))
				CheckNumber(3, obj3:Do4(1, 2, 3))
				CheckNumber(1, obj3:Do5(1, 2, 3, 4))
				CheckNumber(4, obj3:Do6(1, 2, 3, 4, 5))
				CheckNumber(1, obj3:Do7(1, 2, 3, 4, 5, 6))
			)#";
			bind.LoadString(lua);
		}

		TEST_METHOD(TestClassMemberVariable)
		{
			LuaBind bind;
			LuaWrapper<Dummy>::Register(bind.LuaState());
			bind.SetConstructor<Dummy>();
			bind.SetFunction("Check", function(Check));
			bind.SetFunction("CheckNumber", std::function(CheckNumber));
			bind.SetProperty("mDoCount", &Dummy::mDoCount);
			bind.SetProperty("mText", &Dummy::mText);
			std::string lua = R"#(
				local dummy = Dummy.New()
				Check("Hello", dummy.mText)
				CheckNumber(0, dummy.mDoCount)
				dummy.mDoCount = dummy.mDoCount + 10
				CheckNumber(10, dummy.mDoCount)
				dummy.mText = dummy.mText .. " World!"
				Check("Hello World!", dummy.mText)
			)#";
			bind.LoadString(lua);
		}

		TEST_METHOD(TestClassInheritance)
		{
			LuaBind bind;
			bind.SetFunction("Print", function(Print));
			bind.SetFunction("Check", function(Check));
			bind.SetFunction("CheckNumber", function(CheckNumber));
			LuaWrapper<A>::Register(bind.LuaState());
			LuaWrapper<B>::Register<A>(bind.LuaState());
			LuaWrapper<C>::Register<B>(bind.LuaState());
			bind.SetConstructor<A>();
			bind.SetConstructor<B>();
			bind.SetConstructor<C>();
			bind.SetFunction("DoA", &A::DoA);
			bind.SetFunction("DoB", &B::DoB);
			bind.SetFunction("DoC", &C::DoC);
			bind.SetFunction("Do", &A::Do);
			bind.SetFunction("Do", &B::Do);
			bind.SetFunction("Do", &C::Do);
			bind.SetFunction<A>("Foo", &A::Foo);
			bind.SetFunction<B>("Foo", &B::Foo);
			bind.SetFunction<C>("Foo", &C::Foo);
			bind.SetProperty("dataA", &A::dataA);
			bind.SetProperty("dataB", &B::dataB);
			bind.SetProperty("dataC", &C::dataC);
			bind.SetProperty<A>("data", &A::data);
			bind.SetProperty<B>("data", &B::data);
			bind.SetProperty<C>("data", &C::data);
			A* a = new C();
			bind.CreateTable("Test");
			bind.SetProperty("a", a, bind.CurrentTableIndex());
			std::string lua = R"#(
				local a = A.New()
				local b = B.New()
				local c = C.New()
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

				CheckNumber(0, A.data)
				CheckNumber(0, B.data)
				CheckNumber(0, C.data)
				A.data = 1
				B.data = 2
				C.data = 3
				CheckNumber(1, A.data)
				CheckNumber(2, B.data)
				CheckNumber(3, C.data)

				CheckNumber(1, A.Foo(0))
				CheckNumber(2, B.Foo(0))
				CheckNumber(3, C.Foo(0))
			)#";
			bind.LoadString(lua);
			delete a;

			bind.SetFunction("Poly", &C::Poly);
			lua = R"#(
				a = A.New()
				a.dataA = 10
				b = B.New()
				b.dataB = 20
				c = C.New()
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

		TEST_METHOD(TestConstClassMember)
		{
			LuaBind bind;
			bind.SetFunction("Check", function(Check));
			bind.SetFunction("CheckNumber", function(CheckNumber));
			LuaWrapper<ConstClass>::Register(bind.LuaState());
			bind.SetConstructor<ConstClass>();
			bind.SetProperty("a", &ConstClass::a);
			bind.SetProperty("b", &ConstClass::b);
			bind.SetProperty("c", &ConstClass::c);
			bind.SetProperty<ConstClass>("d", &ConstClass::d);
			bind.SetProperty<ConstClass>("e", &ConstClass::e);
			bind.SetFunction("Do", &ConstClass::Do);
			bind.SetFunction("DoConst", &ConstClass::DoConst);

			std::string lua = R"#(
				local test = ConstClass.New()
				CheckNumber(5, test.a)
				CheckNumber(4, test.b)
				CheckNumber(3, test.c)
				test.c = 10
				CheckNumber(10, test.c)

				CheckNumber(10, ConstClass.d)
				CheckNumber(11, ConstClass.e)
				ConstClass.e = 12
				CheckNumber(12, ConstClass.e)

				test:Do()
				CheckNumber(20, test.c)
				test:DoConst()
				CheckNumber(20, test.c)
			)#";
			bind.LoadString(lua);
			CheckNumber(12, ConstClass::e);
		}

		TEST_METHOD(TestConstructor)
		{
			LuaBind bind;
			bind.SetFunction("CheckNumber", function(CheckNumber));
			LuaWrapper<Dummy>::Register(bind.LuaState());

			bind.SetConstructor<Dummy>();
			bind.SetProperty("mDoCount", &Dummy::mDoCount);
			string lua = R"#(
				local dummy = Dummy.New()
				CheckNumber(0, dummy.mDoCount)
			)#";
			bind.LoadString(lua);

			bind.SetConstructor<Dummy, int>();
			bind.SetProperty("mDoCount", &Dummy::mDoCount);
			lua = R"#(
				local dummy = Dummy.New(1)
				CheckNumber(1, dummy.mDoCount)
			)#";
			bind.LoadString(lua);

			bind.SetConstructor<Dummy, int, int>();
			bind.SetProperty("mDoCount", &Dummy::mDoCount);
			lua = R"#(
				local dummy = Dummy.New(1, 2)
				CheckNumber(-1, dummy.mDoCount)
			)#";
			bind.LoadString(lua);

			bind.SetConstructor<Dummy, int, int, int>();
			bind.SetProperty("mDoCount", &Dummy::mDoCount);
			lua = R"#(
				local dummy = Dummy.New(1, 2, 3)
				CheckNumber(2, dummy.mDoCount)
			)#";
			bind.LoadString(lua);

			bind.SetConstructor<Dummy, int, int, int, int>();
			bind.SetProperty("mDoCount", &Dummy::mDoCount);
			lua = R"#(
				local dummy = Dummy.New(1, 2, 3, 4)
				CheckNumber(-2, dummy.mDoCount)
			)#";
			bind.LoadString(lua);

			bind.SetConstructor<Dummy, int, int, int, int, int>();
			bind.SetProperty("mDoCount", &Dummy::mDoCount);
			lua = R"#(
				local dummy = Dummy.New(1, 2, 3, 4, 5)
				CheckNumber(3, dummy.mDoCount)
			)#";
			bind.LoadString(lua);

			bind.SetConstructor<Dummy, int, int, int, int, int, int>();
			bind.SetProperty("mDoCount", &Dummy::mDoCount);
			lua = R"#(
				local dummy = Dummy.New(1, 2, 3, 4, 5, 6)
				CheckNumber(-3, dummy.mDoCount)
			)#";
			bind.LoadString(lua);
		}

		TEST_METHOD(TestNestedClass)
		{
			LuaBind bind;
			bind.SetFunction("CheckNumber", function(CheckNumber));
			LuaWrapper<Dummy>::Register(bind.LuaState());
			LuaWrapper<ConstClass>::Register(bind.LuaState());
			bind.SetConstructor<Dummy>();
			bind.SetProperty("mVec", &ConstClass::mVec);
			bind.SetProperty("mDummy", &ConstClass::mDummy);
			bind.SetProperty<ConstClass>("sVec", &ConstClass::sVec);
			bind.SetProperty<ConstClass>("sDummy", &ConstClass::sDummy);
			bind.SetProperty("mDoCount", &Dummy::mDoCount);
			bind.SetFunction("Do", &ConstClass::Do);
			ConstClass test;
			bind.SetProperty("test", &test);
			string lua = R"#(
				CheckNumber(0, test.mVec.x)
				CheckNumber(1, test.mVec.y)
				CheckNumber(2, test.mVec.z)
				CheckNumber(3, test.mVec.w)
				test.mVec.x = 10
				test.mVec.y = 11
				test.mVec.z = 12
				test.mVec.w = 13
				CheckNumber(10, test.mVec.x)
				CheckNumber(11, test.mVec.y)
				CheckNumber(12, test.mVec.z)
				CheckNumber(13, test.mVec.w)

				test.mVec = vec4.New(1, 2, 3, 4)

				CheckNumber(0, test.mDummy.mDoCount);
				test.mDummy.mDoCount = 10
				CheckNumber(10, test.mDummy.mDoCount);

				CheckNumber(3, ConstClass.sVec.x)
				CheckNumber(2, ConstClass.sVec.y)
				CheckNumber(1, ConstClass.sVec.z)
				CheckNumber(0, ConstClass.sVec.w)

				CheckNumber(0, ConstClass.sDummy.mDoCount)
				ConstClass.sDummy.mDoCount = 10
				CheckNumber(10, ConstClass.sDummy.mDoCount)
				ConstClass.sDummy = Dummy.New()
			)#";
			bind.LoadString(lua);
			Assert::AreEqual(glm::vec4(1, 2, 3, 4), test.mVec);
			Assert::AreEqual(0, ConstClass::sDummy.mDoCount);
		}

		TEST_METHOD(TestExtendC)
		{
			LuaBind bind;
			bind.SetFunction("Check", function(Check));
			bind.SetFunction("CheckNumber", function(CheckNumber));
			LuaWrapper<Dummy>::Register(bind.LuaState());
			LuaWrapper<ConstClass>::Register(bind.LuaState());
			
			bind.SetConstructor<ConstClass>();
			bind.SetProperty("mVec", &ConstClass::mVec);
			bind.SetProperty("mDummy", &ConstClass::mDummy);
			bind.SetProperty<ConstClass>("sVec", &ConstClass::sVec);
			bind.SetProperty<ConstClass>("sDummy", &ConstClass::sDummy);
			bind.SetFunction("DoConst", &ConstClass::DoConst);

			bind.SetConstructor<Dummy>();
			bind.SetProperty("mDoCount", &Dummy::mDoCount);
			bind.SetFunction("Do", &Dummy::Do);
			bind.SetFunction("Do2", &Dummy::Do2);
			bind.SetFunction("Do3", &Dummy::Do3);
			bind.SetFunction("Do4", &Dummy::Do4);
			bind.SetFunction("Do5", &Dummy::Do5);
			bind.SetFunction("Do6", &Dummy::Do6);
			bind.SetFunction("Do7", &Dummy::Do7);
			bind.SetFunction("Do", &ConstClass::Do);
			
			bind.LoadFile("content/Lua/TestExtend.lua");
		}

		TEST_METHOD(TestCallLua)
		{
			LuaBind bind;
			bind.SetFunction("Check", function(Check));
			bind.SetFunction("CheckNumber", function(CheckNumber));
			LuaWrapper<Dummy>::Register(bind.LuaState());
			bind.SetProperty("mDoCount", &Dummy::mDoCount);
			bind.LoadFile("content/Lua/TestCallLua.lua");
			Dummy dummy;
			dummy.mDoCount = 10;
			Dummy dummy2;
			dummy2.mDoCount = 20;
			int ret = bind.CallFunction<int>("Main", 1, 2, "Hello World", dummy, &dummy2);
			Assert::AreEqual(3, ret);
			bind.CallFunctionNoReturn("Main", 1, 2, "Hello World", dummy, &dummy2);
		}

		TEST_METHOD(TestVectorBinding)
		{
			LuaBind bind;
			bind.SetFunction("Check", function(Check));
			bind.SetFunction("CheckNumber", function(CheckNumber));
			bind.RegisterType<Container>();
			bind.SetConstructor<Container>();
			bind.SetProperty("mInts", &Container::mInts);
			bind.SetProperty("mFloats", &Container::mFloats);
			bind.SetProperty("mVectors", &Container::mVectors);
			bind.SetProperty("mValues", &Container::mValues);
			bind.SetProperty("mPointers", &Container::mPointers);
			bind.SetFunction("Set", &Container::Set);
			bind.SetFunction("GetInts", &Container::GetInts);
			bind.SetFunction("GetFloats", &Container::GetFloats);
			bind.SetFunction("GetVectors", &Container::GetVectors);
			bind.RegisterType<Action>();
			std::string lua = R"#(
				local test = Container.New()
				CheckNumber(0, test.mInts:Size())
				CheckNumber(0, test.mFloats:Size())
				CheckNumber(0, test.mVectors:Size())

				test.mInts:Append(1)
				CheckNumber(1, test.mInts:Size())
				CheckNumber(1, test.mInts:Get(0))
				test.mInts:Append(2)
				CheckNumber(2, test.mInts:Size())
				CheckNumber(2, test.mInts:Get(1))
				
				test.mInts:Set(0, 10)
				CheckNumber(10, test.mInts:Get(0))
				CheckNumber(2, test.mInts:Get(1))

				test.mInts:Insert(1, 20)
				CheckNumber(3, test.mInts:Size())
				CheckNumber(10, test.mInts:Get(0))
				CheckNumber(20, test.mInts:Get(1))
				CheckNumber(2, test.mInts:Get(2))

				test.mInts:RemoveAt(0)
				CheckNumber(2, test.mInts:Size())
				CheckNumber(20, test.mInts:Get(0))
				CheckNumber(2, test.mInts:Get(1))

				test.mInts:Clear()
				CheckNumber(0, test.mInts:Size())

				test.mInts:Append(1)
				test.mInts:Append(2)
				local ints = test:GetInts()
				local floats = test:GetFloats()
				local vectors = test:GetVectors()

				Check("false", tostring(ints == test.mInts))
				CheckNumber(2, ints:Size())
				Check("true", tostring(floats == test.mFloats))
				Check("true", tostring(vectors == test.mVectors))

				floats = Array_float.New()
				floats:Append(1.25)
				CheckNumber(1.25, floats:Get(0))

				vectors = Array_vec4.New()
				vectors:Append(vec4.New(1, 2, 3, 4))
				CheckNumber(1, vectors:Size())
				
				ints:Append(3)
				test:Set(ints, floats, vectors)
				ints:Clear()
				CheckNumber(0, ints:Size())
				CheckNumber(3, test.mInts:Get(2))
				CheckNumber(0, floats:Size())
				CheckNumber(1, vectors:Size())
			)#";
			bind.LoadString(lua);

			lua = R"#(
				local test = Container.New()
				local values = test.mValues
				local ptrs = test.mPointers
				CheckNumber(0, values:Size())
				CheckNumber(0, ptrs:Size())

				values:Append(Container.New())
				CheckNumber(1, values:Size())
				local child = values:Get(0)
				child.mInts:Append(100)
				CheckNumber(100, test.mValues:Get(0).mInts:Get(0))

				ptrs:Append(child)
				CheckNumber(100, test.mPointers:Get(0).mInts:Get(0))
				ptrs:Get(0).mFloats:Append(1.125)
				CheckNumber(1.125, child.mFloats:Get(0))
				ptrs:Set(0, nil)
				Check("true", tostring(ptrs:Get(0) == nil))
			)#";
			bind.LoadString(lua);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState LuaBindTest::sStartMemState;
}
