#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Stack.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngine;

inline std::size_t operator "" _z(unsigned long long int x)
{
	return static_cast<size_t>(x);
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(StackTest)
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

		TEST_METHOD(TestConstructor)
		{
			Stack<Foo> stack;
			Assert::AreEqual(0_z, stack.Size());
			Assert::IsTrue(stack.IsEmpty());

			stack.Push(Foo(1));
			Assert::AreEqual(1_z, stack.Size());
			Assert::IsFalse(stack.IsEmpty());
			Assert::AreEqual(Foo(1), stack.Peak());

			{
				Stack<Foo> copy = stack;
				Assert::AreEqual(1_z, copy.Size());
				Assert::IsFalse(copy.IsEmpty());
				Assert::AreEqual(Foo(1), copy.Peak());

				Stack<Foo> moved = std::move(copy);
				Assert::AreEqual(1_z, moved.Size());
				Assert::IsFalse(moved.IsEmpty());
				Assert::AreEqual(Foo(1), moved.Peak());
			}

			{
				Stack<Foo> copy;
				copy.Push(Foo(2));
				copy = stack;
				Assert::AreEqual(1_z, copy.Size());
				Assert::IsFalse(copy.IsEmpty());
				Assert::AreEqual(Foo(1), copy.Peak());

				Stack<Foo> moved;
				moved.Push(Foo(3));
				moved = move(copy);
				Assert::AreEqual(1_z, moved.Size());
				Assert::IsFalse(moved.IsEmpty());
				Assert::AreEqual(Foo(1), moved.Peak());
			}
		}

		TEST_METHOD(TestPushPop)
		{
			Stack<Foo> stack;
			stack.Push(Foo(1));
			Assert::AreEqual(1_z, stack.Size());
			Assert::AreEqual(Foo(1), stack.Peak());
			stack.Push(Foo(2));
			Assert::AreEqual(2_z, stack.Size());
			Assert::AreEqual(Foo(2), stack.Peak());
			stack.Push(Foo(3));
			Assert::AreEqual(3_z, stack.Size());
			Assert::AreEqual(Foo(3), stack.Peak());
			Assert::AreEqual(Foo(3), const_cast<const Stack<Foo>&>(stack).Peak());

			Stack<Foo> copy = stack;

			stack.Pop();
			Assert::AreEqual(2_z, stack.Size());
			Assert::AreEqual(Foo(2), stack.Peak());
			stack.Pop();
			Assert::AreEqual(1_z, stack.Size());
			Assert::AreEqual(Foo(1), stack.Peak());
			stack.Pop();
			Assert::IsTrue(stack.IsEmpty());

			copy.Clear();
			Assert::IsTrue(copy.IsEmpty());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState StackTest::sStartMemState;
}