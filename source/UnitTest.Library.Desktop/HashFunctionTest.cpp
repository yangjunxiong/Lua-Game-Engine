#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "DefaultHashFunction.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngine;

struct Bar
{
	int a;
	float b;
	char c;
	Bar(int a, float b, char c) :
		a(a),
		b(b),
		c(c)
	{
	}

	bool operator==(const Bar& other) const
	{
		return a == other.a && b == other.b && c == other.c;
	}
};

template<>
struct DefaultHash<Foo>
{
	size_t operator()(const Foo& foo)
	{
		return foo.Data();
	}
};

template<>
struct DefaultKeyEquality<Foo>
{
	bool operator()(const Foo& foo1, const Foo& foo2)
	{
		return foo1.Data() == foo2.Data();
	}
};

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashFunctionTest)
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

		TEST_METHOD(TestDefaultHashFunction)
		{
			{
				DefaultHash<int> func;
				int a = 10;
				int b = 20;
				int c = 10;
				Assert::AreEqual(func(a), func(a));
				Assert::AreEqual(func(a), func(c));
				Assert::AreNotEqual(func(a), func(b));
				Assert::AreNotEqual(func(b), func(c));
			}

			{
				DefaultHash<double> func;
				double a = 10.0;
				double b = 20.0;
				double c = 10.0;
				Assert::AreEqual(func(a), func(a));
				Assert::AreEqual(func(a), func(c));
				Assert::AreNotEqual(func(a), func(b));
				Assert::AreNotEqual(func(b), func(c));
			}

			{
				Bar a(10, 20.0f, 'c');
				Bar b(20, 20.0f, 'c');
				Bar c(a);
				DefaultHash<Bar> func;
				Assert::AreEqual(func(a), func(a));
				Assert::AreEqual(func(a), func(c));
				Assert::AreNotEqual(func(a), func(b));
				Assert::AreNotEqual(func(b), func(c));
			}
		}

		TEST_METHOD(TestDefaultHashFunctionChar)
		{
			DefaultHash<char*> func;
			char* a = "Hello";
			char* b = "Bye";
			char c[10];
			strcpy_s(c, 10, a);
			Assert::AreEqual(func(a), func(a));
			Assert::AreEqual(func(a), func(c));
			Assert::AreNotEqual(func(a), func(b));
			Assert::AreNotEqual(func(b), func(c));

			char* gg = nullptr;
			auto lamda = [&func, &gg]()
			{
				func(gg);
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestDefaultHashFunctionString)
		{
			{
				DefaultHash<std::string> func;
				std::string a = "Hello";
				std::string b = "Bye";
				std::string c(a);
				Assert::AreEqual(func(a), func(a));
				Assert::AreEqual(func(a), func(c));
				Assert::AreNotEqual(func(a), func(b));
				Assert::AreNotEqual(func(b), func(c));
			}
			
			{
				DefaultHash<const std::string> func;
				const std::string a = "Hello";
				const std::string b = "Bye";
				const std::string c(a);
				Assert::AreEqual(func(a), func(a));
				Assert::AreEqual(func(a), func(c));
				Assert::AreNotEqual(func(a), func(b));
				Assert::AreNotEqual(func(b), func(c));
			}
		}

		TEST_METHOD(TestDefaultHashFunctionFooSpecialization)
		{
			DefaultHash<Foo> func;
			Foo a(1);
			Foo b(2);
			Foo c(a);

			Assert::AreEqual(func(a), func(a));
			Assert::AreEqual(func(a), func(c));
			Assert::AreEqual(size_t(1), func(a));
			Assert::AreEqual(size_t(2), func(b));
			Assert::AreNotEqual(func(a), func(b));
			Assert::AreNotEqual(func(b), func(c));
		}

		TEST_METHOD(TestDefaultEqualityFunction)
		{
			{
				DefaultKeyEquality<int> func;
				int a = 10;
				int b = 20;
				int c = 10;
				Assert::IsTrue(func(a, a));
				Assert::IsTrue(func(a, c));
				Assert::IsFalse(func(a, b));
				Assert::IsFalse(func(b, c));
			}

			{
				DefaultKeyEquality<double> func;
				double a = 10.0;
				double b = 20.0;
				double c = 10.0;
				Assert::IsTrue(func(a, a));
				Assert::IsTrue(func(a, c));
				Assert::IsFalse(func(a, b));
				Assert::IsFalse(func(b, c));
			}
			
			{
				Bar a(10, 20.0f, 'c');
				Bar b(20, 20.0f, 'c');
				Bar c(a);
				DefaultKeyEquality<Bar> func;
				Assert::IsTrue(func(a, a));
				Assert::IsTrue(func(a, c));
				Assert::IsFalse(func(a, b));
				Assert::IsFalse(func(b, c));
			}
		}

		TEST_METHOD(TestDefaultEqualityFunctionChar)
		{
			DefaultKeyEquality<char*> func;
			char* a = "Hello";
			char* b = "Bye";
			char c[10];
			strcpy_s(c, 10, a);
			Assert::IsTrue(func(a, a));
			Assert::IsTrue(func(a, c));
			Assert::IsFalse(func(a, b));
			Assert::IsFalse(func(b, c));
		}

		TEST_METHOD(TestDefaultEqualityFunctionString)
		{
			DefaultKeyEquality<std::string> func;
			std::string a = "Hello";
			std::string b = "Bye";
			std::string c(a);
			Assert::IsTrue(func(a, a));
			Assert::IsTrue(func(a, c));
			Assert::IsFalse(func(a, b));
			Assert::IsFalse(func(b, c));
		}

		TEST_METHOD(TestDefaultEqualityFunctionFooSpecialization)
		{
			DefaultKeyEquality<Foo> func;
			Foo a(1);
			Foo b(2);
			Foo c(a);
			Assert::IsTrue(func(a, a));
			Assert::IsTrue(func(a, c));
			Assert::IsFalse(func(a, b));
			Assert::IsFalse(func(b, c));
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState HashFunctionTest::sStartMemState;
}