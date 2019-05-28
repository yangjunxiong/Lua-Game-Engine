#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "vector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator & it)
	{
		try
		{
			RETURN_WIDE_STRING((*it).Data());
		}
		catch (std::exception e)
		{
			return L"Iterator points to nothing";
		}
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator & it)
	{
		try
		{
			RETURN_WIDE_STRING((*it).Data());
		}
		catch (std::exception e)
		{
			return L"Iterator points to nothing";
		}
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(VectorTest)
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

		TEST_METHOD(TestCustomizedReserve)
		{
			// Default reserve
			Vector<Foo> vector = { Foo(1), Foo(2) };
			Assert::AreEqual(size_t(2), vector.Capacity());
			vector.PushBack(Foo(3));
			Assert::AreEqual(size_t(4), vector.Capacity());
			vector.PushBack(Foo(4));
			Assert::AreEqual(size_t(4), vector.Capacity());
			vector.PushBack(Foo(5));
			Assert::AreEqual(size_t(8), vector.Capacity());

			// Customized reserve
			struct Test
			{
				size_t operator()(size_t, size_t capacity)
				{
					return capacity + 1;
				}
			};
			auto func = [](size_t, size_t capacity)
			{
				return capacity + 1;
			};
			Vector<Foo> vector2(0, Test());
			Assert::AreEqual(size_t(0), vector2.Capacity());
			vector2.PushBack(Foo(0));
			Assert::AreEqual(size_t(1), vector2.Capacity());
			vector2.PushBack(Foo(1));
			Assert::AreEqual(size_t(2), vector2.Capacity());
			vector2.PushBack(Foo(2));
			Assert::AreEqual(size_t(3), vector2.Capacity());
		}

		TEST_METHOD(TestDefaultConstructor)
		{
			{
				Vector<Foo> vector;
				Assert::AreEqual(size_t(0), vector.Size());
				Assert::AreEqual(size_t(0), vector.Capacity());
				auto function = [&vector]
				{
					vector.Front();
				};
				Assert::ExpectException<std::exception>(function);
				vector.PushBack(Foo(0));
				Assert::AreEqual(size_t(8), vector.Capacity());
			}

			{
				auto func = [](size_t, size_t capacity)
				{
					return capacity + 100;
				};
				Vector<Foo> vector(1, func);
				Assert::AreEqual(size_t(0), vector.Size());
				Assert::AreEqual(size_t(1), vector.Capacity());
				vector.PushBack(Foo(0));
				vector.PushBack(Foo(1));
				Assert::AreEqual(size_t(2), vector.Size());
				Assert::AreEqual(size_t(101), vector.Capacity());
			}
		}

		TEST_METHOD(TestInitializerConstructor)
		{
			auto func = [](size_t, size_t capacity)
			{
				return capacity + 100;
			};
			Vector<Foo> vector({Foo(0), Foo(1), Foo(2), Foo(3), Foo(4)}, func);
			Assert::AreEqual(size_t(5), vector.Size());
			Assert::AreEqual(size_t(5), vector.Capacity());
			Assert::AreEqual(Foo(0), vector.Front());
			Assert::AreEqual(Foo(4), vector.Back());
			for (int i = 0; i < 5; ++i)
			{
				Assert::AreEqual(Foo(i), vector[i]);
			}
			vector.PushBack(Foo(100));
			Assert::AreEqual(Foo(100), vector.Back());
			Assert::AreEqual(size_t(6), vector.Size());
			Assert::AreEqual(size_t(105), vector.Capacity());
		}

		TEST_METHOD(TestCopyConstructor)
		{
			auto func = [](size_t, size_t capacity)
			{
				return capacity + 100;
			};
			Vector<Foo> vector({ Foo(0), Foo(1), Foo(2), Foo(3), Foo(4) }, func);
			Vector<Foo> vector2(vector);
			vector.Destroy();

			Assert::AreEqual(size_t(5), vector2.Size());
			Assert::AreEqual(size_t(5), vector2.Capacity());
			Assert::AreEqual(Foo(0), vector2.Front());
			Assert::AreEqual(Foo(4), vector2.Back());
			for (size_t i = 0; i < vector2.Size(); ++i)
			{
				Assert::AreEqual(Foo(static_cast<int>(i)), vector2[i]);
			}
			vector2.PushBack(Foo(5));
			Assert::AreEqual(Foo(5), vector2[5]);
			Assert::AreEqual(size_t(6), vector2.Size());
			Assert::AreEqual(size_t(105), vector2.Capacity());

			Vector<Foo> vector3(vector);
			Assert::AreEqual(size_t(0), vector3.Size());
			Assert::AreEqual(size_t(0), vector3.Capacity());
		}

		TEST_METHOD(TestMoveContstructor)
		{
			auto func = [](size_t, size_t capacity)
			{
				return capacity + 100;
			};
			Vector<Foo> vector({ Foo(0), Foo(1), Foo(2), Foo(3), Foo(4) }, func);
			Vector<Foo> vector2 = std::move(vector);
			Assert::IsTrue(vector.IsEmpty());
			vector.Destroy();

			Assert::AreEqual(size_t(5), vector2.Size());
			Assert::AreEqual(size_t(5), vector2.Capacity());
			Assert::AreEqual(Foo(0), vector2.Front());
			Assert::AreEqual(Foo(4), vector2.Back());
			for (size_t i = 0; i < vector2.Size(); ++i)
			{
				Assert::AreEqual(Foo(static_cast<int>(i)), vector2[i]);
			}
			vector2.PushBack(Foo(5));
			Assert::AreEqual(Foo(5), vector2[5]);
			Assert::AreEqual(size_t(6), vector2.Size());
			Assert::AreEqual(size_t(105), vector2.Capacity());

			Vector<Foo> vector3 = std::move(vector);
			Assert::AreEqual(size_t(0), vector3.Size());
			Assert::AreEqual(size_t(0), vector3.Capacity());
		}

		TEST_METHOD(TestCopyAssignment)
		{
			// Self copy
			auto func = [](size_t, size_t capacity)
			{
				return capacity + 1;
			};
			Vector<Foo> vector({ Foo(0), Foo(1), Foo(2), Foo(3), Foo(4), Foo(5) }, func);
			Assert::AreEqual(size_t(6), vector.Size());
			Assert::AreEqual(size_t(6), vector.Capacity());
			Assert::AreEqual(Foo(0), vector.Front());
			vector = vector;
			Assert::AreEqual(size_t(6), vector.Size());
			Assert::AreEqual(size_t(6), vector.Capacity());
			Assert::AreEqual(Foo(0), vector.Front());

			// Real copy
			Vector<Foo> vector2 = { Foo(100) };
			Assert::AreEqual(size_t(1), vector2.Size());
			Assert::AreEqual(Foo(100), vector2.Front());
			Assert::AreEqual(Foo(100), vector2.Back());
			vector2 = vector;
			vector.Destroy();
			Assert::AreEqual(size_t(6), vector2.Size());
			Assert::AreEqual(size_t(6), vector2.Capacity());
			for (size_t i = 0; i < vector2.Size(); ++i)
			{
				Assert::AreEqual(Foo(static_cast<int>(i)), vector2[i]);
			}
			vector2.PushBack(Foo(101));
			Assert::AreEqual(size_t(7), vector2.Capacity());
		}

		TEST_METHOD(TestMoveAssignment)
		{
			// Self move
			auto func = [](size_t, size_t capacity)
			{
				return capacity + 1;
			};
			Vector<Foo> vector({ Foo(0), Foo(1), Foo(2), Foo(3), Foo(4), Foo(5) }, func);
			Assert::AreEqual(size_t(6), vector.Size());
			Assert::AreEqual(size_t(6), vector.Capacity());
			Assert::AreEqual(Foo(0), vector.Front());
			vector = vector;
			Assert::AreEqual(size_t(6), vector.Size());
			Assert::AreEqual(size_t(6), vector.Capacity());
			Assert::AreEqual(Foo(0), vector.Front());

			// Real move
			Vector<Foo> vector2 = { Foo(100) };
			Assert::AreEqual(size_t(1), vector2.Size());
			Assert::AreEqual(Foo(100), vector2.Front());
			Assert::AreEqual(Foo(100), vector2.Back());
			vector2 = std::move(vector);
			Assert::IsTrue(vector.IsEmpty());
			vector.Destroy();
			Assert::AreEqual(size_t(6), vector2.Size());
			Assert::AreEqual(size_t(6), vector2.Capacity());
			for (size_t i = 0; i < vector2.Size(); ++i)
			{
				Assert::AreEqual(Foo(static_cast<int>(i)), vector2[i]);
			}
			vector2.PushBack(Foo(101));
			Assert::AreEqual(size_t(7), vector2.Capacity());
		}

		TEST_METHOD(TestClear)
		{
			Vector<Foo> vector = { Foo(1), Foo(2), Foo(3) };

			Assert::AreEqual(size_t(3), vector.Size());
			Assert::AreEqual(size_t(3), vector.Capacity());
			vector.Clear();
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(3), vector.Capacity());

			// Clear empty vector
			vector.Clear();
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(3), vector.Capacity());
		}

		TEST_METHOD(TestDestroy)
		{
			Vector<Foo> vector = { Foo(1), Foo(2), Foo(3) };

			Assert::AreEqual(size_t(3), vector.Size());
			Assert::AreEqual(size_t(3), vector.Capacity());
			vector.Destroy();
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(0), vector.Capacity());

			// Destroy empty vector
			vector.Clear();
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(0), vector.Capacity());
		}

		TEST_METHOD(TestDestructor)
		{
			// Test no memory leak
			{
				Vector<Foo> vector = { Foo(0), Foo(1), Foo(2) };
				Assert::AreEqual(size_t(3), vector.Size());
				Assert::AreEqual(Foo(0), vector.Front());
				Assert::AreEqual(Foo(2), vector.Back());
			}
		}

		TEST_METHOD(TestSizeCapacityEmpty)
		{
			Vector<Foo> vector;
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(0), vector.Capacity());
			Assert::IsTrue(vector.IsEmpty());

			vector.PushBack(Foo(1));
			vector.PushBack(Foo(2));
			Assert::AreEqual(size_t(2), vector.Size());
			Assert::IsTrue(vector.Capacity() >= 2);
			Assert::IsFalse(vector.IsEmpty());
		}

		TEST_METHOD(TestGetOperator)
		{
			// Get from empty vector
			Vector<Foo> vector;
			auto lamda = [&vector]
			{
				vector[1000];
			};
			Assert::ExpectException<std::exception>(lamda);

			// Non-const get
			for (int i = 0; i < 7; ++i)
			{
				vector.PushBack(Foo(i));
				Assert::AreEqual(Foo(i), vector[i]);
				vector[i].Data() += 1;                    // Add 1 for ref test later
			}
			Assert::AreEqual(size_t(7), vector.Size());
			Assert::ExpectException<std::exception>(lamda);

			// Const get from empty vector
			const Vector<Foo> vector2;
			auto lamda_const = [&vector2]
			{
				vector2[1000];
			};
			Assert::ExpectException<std::exception>(lamda_const);

			// Const get
			const Vector<Foo> vector3 = vector;
			for (int i = 0; i < 7; ++i)
			{
				Assert::AreEqual(Foo(i + 1), vector3[i]);
			}
			Assert::ExpectException<std::exception>(lamda_const);
		}

		TEST_METHOD(TestAt)
		{
			// At from empty vector
			Vector<Foo> vector;
			auto lamda = [&vector]
			{
				vector.At(1000);
			};
			Assert::ExpectException<std::exception>(lamda);

			// Non-const at
			for (int i = 0; i < 7; ++i)
			{
				vector.PushBack(Foo(i));
				Assert::AreEqual(Foo(i), vector.At(i));
				vector[i].Data() += 1;                    // Add 1 for ref test later
			}
			Assert::AreEqual(size_t(7), vector.Size());
			Assert::ExpectException<std::exception>(lamda);

			// Const at from empty vector
			const Vector<Foo> vector2;
			auto lamda_const = [&vector2]
			{
				vector2.At(1000);
			};
			Assert::ExpectException<std::exception>(lamda_const);

			// Const at
			const Vector<Foo> vector3 = vector;
			for (int i = 0; i < 7; ++i)
			{
				Assert::AreEqual(Foo(i + 1), vector3.At(i));
			}
			Assert::ExpectException<std::exception>(lamda_const);
		}

		TEST_METHOD(TestFront)
		{
			// Front of empty
			Vector<Foo> vector;
			auto lamda = [&vector]
			{
				vector.Front();
			};
			Assert::ExpectException<std::exception>(lamda);

			// Non-const front
			vector.PushBack(Foo(0));
			vector.PushBack(Foo(1));
			vector.PushBack(Foo(2));
			Assert::AreEqual(Foo(0), vector.Front());
			vector.Front() = Foo(10);
			Assert::AreEqual(Foo(10), vector.Front());
			Assert::AreEqual(Foo(1), vector[1]);
			Assert::AreEqual(Foo(2), vector[2]);

			// Const front of empty
			const Vector<Foo> vector2;
			auto lamda_const = [&vector2]
			{
				vector2.Front();
			};
			Assert::ExpectException<std::exception>(lamda_const);

			// Non-const front
			const Vector<Foo> vector3 = vector;
			Assert::AreEqual(Foo(10), vector3.Front());
		}

		TEST_METHOD(TestBack)
		{
			// Back of empty
			Vector<Foo> vector;
			auto lamda = [&vector]
			{
				vector.Back();
			};
			Assert::ExpectException<std::exception>(lamda);

			// Non-const back
			vector.PushBack(Foo(0));
			Assert::AreEqual(Foo(0), vector.Back());
			vector.PushBack(Foo(1));
			Assert::AreEqual(Foo(1), vector.Back());
			vector.PushBack(Foo(2));
			Assert::AreEqual(Foo(2), vector.Back());
			vector.Back() = Foo(10);
			Assert::AreEqual(Foo(10), vector.Back());
			Assert::AreEqual(Foo(0), vector[0]);
			Assert::AreEqual(Foo(1), vector[1]);
			Assert::AreEqual(Foo(10), vector[2]);

			// Const back of empty
			const Vector<Foo> vector2;
			auto lamda_const = [&vector2]
			{
				vector2.Back();
			};
			Assert::ExpectException<std::exception>(lamda_const);

			// Non-const back
			const Vector<Foo> vector3 = vector;
			Assert::AreEqual(Foo(10), vector3.Back());
		}

		TEST_METHOD(TestPushBack)
		{
			// Push 1 to back
			Vector<Foo> vector;
			Assert::IsTrue(vector.IsEmpty());
			Assert::AreEqual(size_t(0), vector.Capacity());
			vector.Reserve(16);
			vector.PushBack(Foo(1));
			Assert::AreEqual(size_t(1), vector.Size());
			Assert::AreEqual(size_t(16), vector.Capacity());
			Assert::AreEqual(Foo(1), vector.Back());

			// Push 2 to back
			vector.PushBack(Foo(2));
			Assert::AreEqual(size_t(2), vector.Size());
			Assert::AreEqual(size_t(16), vector.Capacity());
			Assert::AreEqual(Foo(1), vector[0]);
			Assert::AreEqual(Foo(2), vector.Back());

			// Push many numbers to back without growing capacity
			for (int i = 0; i < 14; ++i)
			{
				vector.PushBack(Foo(i + 3));
				Assert::AreEqual(size_t(i + 3), vector.Size());
				Assert::AreEqual(size_t(16), vector.Capacity());
				Assert::AreEqual(Foo(i + 3), vector.Back());
				Assert::AreEqual(Foo(i + 3), vector[i + 2]);
			}

			// Push 17 to back which will grow the capacity
			vector.PushBack(Foo(17));
			Assert::AreEqual(size_t(17), vector.Size());
			Assert::IsTrue(vector.Capacity() > 16);
			Assert::AreEqual(Foo(17), vector.Back());
		}

		TEST_METHOD(TestPopBack)
		{
			// Pop empty vector
			Vector<Foo> vector;
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(0), vector.Capacity());
			vector.PopBack();
			vector.PopBack();
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(0), vector.Capacity());

			// Pop many until empty
			vector.Reserve(10);
			for (int i = 0; i < 10; ++i)
			{
				vector.PushBack(Foo(i));
				Assert::AreEqual(Foo(i), vector.Back());
			}
			for (int i = 0; i < 10; ++i)
			{
				Assert::AreEqual(Foo(9 - i), vector.Back());
				Assert::AreEqual(Foo(0), vector.Front());
				vector.PopBack();
				Assert::AreEqual(size_t(9 - i), vector.Size());
				Assert::AreEqual(size_t(10), vector.Capacity());
			}
			Assert::IsTrue(vector.IsEmpty());
		}

		TEST_METHOD(TestReserve)
		{
			// Reserve 8
			Vector<Foo> vector;
			Assert::AreEqual(size_t(0), vector.Capacity());
			Assert::AreEqual(size_t(0), vector.Size());
			vector.Reserve(8);
			Assert::AreEqual(size_t(8), vector.Capacity());
			Assert::AreEqual(size_t(0), vector.Size());

			// The reserved capacity actually works
			vector.PushBack(Foo(0));
			vector.PushBack(Foo(1));
			vector.PushBack(Foo(2));
			vector.PushBack(Foo(3));
			Assert::AreEqual(size_t(4), vector.Size());
			Assert::AreEqual(size_t(8), vector.Capacity());
			Assert::AreEqual(Foo(0), vector.Front());
			Assert::AreEqual(Foo(3), vector.Back());

			// Shrink does nothing
			vector.Reserve(3);
			Assert::AreEqual(size_t(4), vector.Size());
			Assert::AreEqual(size_t(8), vector.Capacity());
			Assert::AreEqual(Foo(0), vector.Front());
			Assert::AreEqual(Foo(3), vector.Back());

			// Reserve new capacity will not break existing elements
			vector.Reserve(1000);
			Assert::AreEqual(size_t(4), vector.Size());
			Assert::AreEqual(size_t(1000), vector.Capacity());
			for (size_t i = 0; i < vector.Size(); ++i)
			{
				Assert::AreEqual(Foo(static_cast<int>(i)), vector[i]);
			}
		}

		TEST_METHOD(TestShrinkToFit)
		{
			// Shrink empty vector
			Vector<Foo> vector;
			vector.ShrinkToFit();
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(0), vector.Capacity());

			vector.Reserve(2);
			vector.PushBack(Foo(1));
			vector.PushBack(Foo(2));
			Assert::AreEqual(size_t(2), vector.Size());
			Assert::AreEqual(size_t(2), vector.Capacity());
			vector.ShrinkToFit();
			Assert::AreEqual(size_t(2), vector.Size());
			Assert::AreEqual(size_t(2), vector.Capacity());
			Assert::AreEqual(Foo(1), vector.Front());
			Assert::AreEqual(Foo(2), vector.Back());

			vector.Reserve(10);
			Assert::AreEqual(size_t(2), vector.Size());
			Assert::AreEqual(size_t(10), vector.Capacity());
			vector.ShrinkToFit();
			Assert::AreEqual(size_t(2), vector.Size());
			Assert::AreEqual(size_t(2), vector.Capacity());
			Assert::AreEqual(Foo(1), vector.Front());
			Assert::AreEqual(Foo(2), vector.Back());

			vector.PopBack();
			vector.PopBack();
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(2), vector.Capacity());
			vector.ShrinkToFit();
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(0), vector.Capacity());
		}

		TEST_METHOD(TestResize)
		{
			// Resize same size
			{
				Vector<Foo> vector = {Foo(1), Foo(2), Foo(3), Foo(4), Foo(5)};
				Assert::AreEqual(size_t(5), vector.Size());
				Assert::AreEqual(size_t(5), vector.Capacity());
				vector.Resize(5);
				Assert::AreEqual(size_t(5), vector.Size());
				Assert::AreEqual(size_t(5), vector.Capacity());
			}

			// Resize don't increase capacity
			{
				Vector<Foo> vector(10);
				vector.PushBack(Foo(1));
				vector.PushBack(Foo(2));
				Assert::AreEqual(size_t(2), vector.Size());
				Assert::AreEqual(size_t(10), vector.Capacity());
				vector.Resize(7);
				Assert::AreEqual(size_t(7), vector.Size());
				Assert::AreEqual(size_t(10), vector.Capacity());
				Assert::AreEqual(Foo(1), vector.Front());
				Assert::AreEqual(Foo(), vector.Back());
			}

			// Resize increase capacity
			{
				Vector<Foo> vector = { Foo(1), Foo(2) };
				Assert::AreEqual(size_t(2), vector.Capacity());
				vector.Resize(10);
				Assert::AreEqual(size_t(10), vector.Size());
				Assert::AreEqual(size_t(10), vector.Capacity());
				Assert::AreEqual(Foo(1), vector.Front());
				Assert::AreEqual(Foo(), vector.Back());
			}

			// Resize empty vector
			{
				Vector<Foo> vector;
				vector.Resize(10, Foo(10086));
				Assert::AreEqual(size_t(10), vector.Size());
				Assert::AreEqual(size_t(10), vector.Capacity());
				Assert::AreEqual(Foo(10086), vector.Back());
			}

			// Resize destruct objects
			{
				Vector<Foo> vector = { Foo(1), Foo(2), Foo(3), Foo(4), Foo(5) };
				Assert::AreEqual(size_t(5), vector.Size());
				Assert::AreEqual(size_t(5), vector.Capacity());
				vector.Resize(2);
				Assert::AreEqual(size_t(2), vector.Size());
				Assert::AreEqual(size_t(5), vector.Capacity());
				Assert::AreEqual(Foo(1), vector.Front());
				Assert::AreEqual(Foo(2), vector.Back());
			}

			// Resize to empty
			{
				Vector<Foo> vector = { Foo(1), Foo(2), Foo(3), Foo(4), Foo(5) };
				Assert::AreEqual(size_t(5), vector.Size());
				Assert::AreEqual(size_t(5), vector.Capacity());
				vector.Resize(0);
				Assert::AreEqual(size_t(0), vector.Size());
				Assert::AreEqual(size_t(5), vector.Capacity());
			}
		}

		TEST_METHOD(TestIteratorConstructor)
		{
			// Default constructor
			{
				Vector<Foo>::Iterator it;
				Vector<Foo>::Iterator it2;
				Assert::IsTrue(it == it2);
				auto lamda = [&it]
				{
					*it;
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			// Constructed fron vector
			Vector<Foo> vector = { Foo(0), Foo(1), Foo(2) };
			{
				Vector<Foo>::Iterator it = vector.begin();
				Assert::AreEqual(Foo(0), *it++);
				Assert::AreEqual(Foo(1), *it++);
				Assert::AreEqual(Foo(2), *it++);
				Assert::AreEqual(vector.end(), it);
			}

			// Copy constructor
			{
				Vector<Foo>::Iterator it = vector.begin();
				Assert::AreEqual(Foo(0), *it);
				Vector<Foo>::Iterator it2 = it;
				Assert::AreEqual(Foo(0), *it2);
			}

			// Move constructor
			{
				Vector<Foo>::Iterator it = vector.begin();
				Assert::AreEqual(Foo(0), *it);
				Vector<Foo>::Iterator it2 = std::move(it);
				Assert::AreEqual(Foo(0), *it2);
			}
		}

		TEST_METHOD(TestConstIteratorConstructor)
		{
			// Default constructor
			{
				Vector<Foo>::ConstIterator it;
				Vector<Foo>::ConstIterator it2;
				Assert::IsTrue(it == it2);
				auto lamda = [&it]
				{
					*it;
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			// Constructed fron vector
			Vector<Foo> vector = { Foo(0), Foo(1), Foo(2) };
			{
				Vector<Foo>::ConstIterator it = vector.cbegin();
				Assert::AreEqual(Foo(0), *it++);
				Assert::AreEqual(Foo(1), *it++);
				Assert::AreEqual(Foo(2), *it++);
				Assert::AreEqual(vector.cend(), it);
			}

			// Copy constructor
			{
				Vector<Foo>::ConstIterator it = vector.cbegin();
				Assert::AreEqual(Foo(0), *it);
				Vector<Foo>::ConstIterator it2 = it;
				Assert::AreEqual(Foo(0), *it2);
			}

			// Move constructor
			{
				Vector<Foo>::ConstIterator it = vector.cbegin();
				Assert::AreEqual(Foo(0), *it);
				Vector<Foo>::ConstIterator it2 = std::move(it);
				Assert::AreEqual(Foo(0), *it2);
			}

			// Constructed from non-const iterator
			{
				Vector<Foo>::Iterator it = vector.begin();
				Assert::AreEqual(Foo(0), *it);
				Vector<Foo>::ConstIterator it2 = it;
				Assert::AreEqual(Foo(0), *it2);
				Vector<Foo>::ConstIterator it3;
				it3 = it;
				Assert::AreEqual(Foo(0), *it3);
			}
		}

		TEST_METHOD(TestIteratorCompare)
		{
			// Compare default
			{
				Vector<Foo>::Iterator it;
				Vector<Foo>::Iterator it2;
				Assert::AreEqual(it, it2);
				Vector<Foo>::ConstIterator cit;
				Vector<Foo>::ConstIterator cit2;
				Assert::AreEqual(cit, cit2);
			}

			// Compare same owner
			{
				Vector<Foo> vector = { Foo(0), Foo(1) };
				Vector<Foo>::Iterator it = vector.begin();
				Vector<Foo>::Iterator it2 = vector.begin();
				Assert::AreEqual(it, it2);
				++it2;
				Assert::IsTrue(it != it2);
				Assert::AreEqual(vector.end(), ++it2);

				Vector<Foo>::ConstIterator cit = vector.cbegin();
				Vector<Foo>::ConstIterator cit2 = vector.cbegin();
				Assert::AreEqual(cit, cit2);
				++cit2;
				Assert::IsTrue(cit != cit2);
				Assert::AreEqual(vector.cend(), ++cit2);
			}

			// Compare different owner
			{
				Vector<Foo> vector = { Foo(0), Foo(1) };
				Vector<Foo> vector2 = { Foo(0), Foo(1) };
				Assert::AreNotEqual(vector.begin(), vector2.begin());
				Assert::AreNotEqual(vector.end(), vector2.end());
				Assert::AreNotEqual(vector.cbegin(), vector2.cbegin());
				Assert::AreNotEqual(vector.cend(), vector2.cend());
			}
		}

		TEST_METHOD(TestIteratorIncrement)
		{
			{
				Vector<Foo>::Iterator it;
				auto lamda = [&it]()
				{
					++it;
				};
				Assert::ExpectException<std::exception>(lamda);
				auto lamda2 = [&it]()
				{
					it++;
				};
				Assert::ExpectException<std::exception>(lamda2);
			}

			{
				Vector<Foo>::ConstIterator it;
				auto lamda = [&it]()
				{
					++it;
				};
				Assert::ExpectException<std::exception>(lamda);
				auto lamda2 = [&it]()
				{
					it++;
				};
				Assert::ExpectException<std::exception>(lamda2);
			}

			Vector<Foo> vector = { Foo(0), Foo(1), Foo(2) };
			Vector<Foo>::Iterator it = vector.begin();
			Assert::AreEqual(Foo(0), *it);
			Assert::AreEqual(Foo(1), *++it);
			Assert::AreEqual(Foo(2), *++it);
			Assert::AreEqual(vector.end(), ++it);
			it = vector.begin();
			Assert::AreEqual(Foo(0), *it);
			Assert::AreEqual(Foo(0), *it++);
			Assert::AreEqual(Foo(1), *it++);
			Assert::AreEqual(Foo(2), *it++);
			Assert::AreEqual(vector.end(), it);

			Vector<Foo>::ConstIterator cit = vector.cbegin();
			Assert::AreEqual(Foo(0), *cit);
			Assert::AreEqual(Foo(1), *++cit);
			Assert::AreEqual(Foo(2), *++cit);
			Assert::AreEqual(vector.cend(), ++cit);
			cit = vector.cbegin();
			Assert::AreEqual(Foo(0), *cit);
			Assert::AreEqual(Foo(0), *cit++);
			Assert::AreEqual(Foo(1), *cit++);
			Assert::AreEqual(Foo(2), *cit++);
			Assert::AreEqual(vector.cend(), cit);
		}

		TEST_METHOD(TestIteratorDereference)
		{
			Vector<Foo> vector = { Foo(0), Foo(1), Foo(2) };
			Vector<Foo>::Iterator it;
			auto lamda = [&it]()
			{
				*it;
			};
			Assert::ExpectException<std::exception>(lamda);
			it = vector.end();
			Assert::ExpectException<std::exception>(lamda);

			it = vector.begin();
			Assert::AreEqual(Foo(0), *it);

			Vector<Foo>::ConstIterator cit;
			auto clamda = [&cit]()
			{
				*cit;
			};
			Assert::ExpectException<std::exception>(clamda);
			cit = vector.cend();
			Assert::ExpectException<std::exception>(clamda);

			cit = vector.cbegin();
			Assert::AreEqual(Foo(0), *cit);
		}

		TEST_METHOD(TestIteratorAdd)
		{
			{
				Vector<Foo>::Iterator it;
				auto lamda = [&it]()
				{
					it + 1;
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Vector<Foo>::ConstIterator it;
				auto lamda = [&it]()
				{
					it + 1;
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			Vector<Foo> vector = { Foo(0), Foo(1), Foo(2) };
			Vector<Foo>::Iterator it = vector.begin();
			Assert::AreEqual(Foo(0), *it);
			Assert::AreEqual(Foo(1), *(it + 1));
			Assert::AreEqual(Foo(2), *(it + 2));
			Assert::AreEqual(vector.end(), it + 3);
			it = it + 2;
			Assert::AreEqual(Foo(2), *it);
			Assert::AreEqual(Foo(2), *(it + 0));

			Vector<Foo>::ConstIterator cit = vector.cbegin();
			Assert::AreEqual(Foo(0), *cit);
			Assert::AreEqual(Foo(1), *(cit + 1));
			Assert::AreEqual(Foo(2), *(cit + 2));
			Assert::AreEqual(vector.cend(), cit + 3);
			cit = cit + 2;
			Assert::AreEqual(Foo(2), *cit);
			Assert::AreEqual(Foo(2), *(cit + 0));
		}

		TEST_METHOD(TestBeginEnd)
		{
			{
				Vector<Foo> vector;
				Assert::AreEqual(vector.end(), vector.begin());
				Assert::AreEqual(vector.cend(), vector.cbegin());
				auto lamdaBegin = [&vector]()
				{
					*vector.begin();
				};
				auto lamdaEnd = [&vector]()
				{
					*vector.end();
				};
				auto constLamdaBegin = [&vector]()
				{
					*vector.cbegin();
				};
				auto constLamdaEnd = [&vector]()
				{
					*vector.cend();
				};
				Assert::ExpectException<std::exception>(lamdaBegin);
				Assert::ExpectException<std::exception>(lamdaEnd);
				Assert::ExpectException<std::exception>(constLamdaBegin);
				Assert::ExpectException<std::exception>(constLamdaEnd);
			}

			{
				Vector<Foo> vector = { Foo(0), Foo(1), Foo(2) };
				Assert::AreEqual(Foo(0), *vector.begin());
				Assert::AreEqual(Foo(0), *vector.cbegin());
				Assert::AreEqual(vector.end(), vector.begin() + 3);
				Assert::AreEqual(vector.cend(), vector.cbegin() + 3);
			}

			{
				const Vector<Foo> vector = { Foo(0), Foo(1), Foo(2) };
				Assert::AreEqual(Foo(0), *vector.begin());
				Assert::AreEqual(vector.end(), vector.begin() + 3);
			}
		}

		TEST_METHOD(TestFind)
		{
			// Find in empty vector
			{
				Vector<Foo> vector;
				const Vector<Foo> cvector;
				Vector<Foo>::Iterator it = vector.Find(Foo(100));
				Assert::AreEqual(vector.end(), it);
				Vector<Foo>::ConstIterator cit = cvector.Find(Foo(100));
				Assert::AreEqual(cvector.cend(), cit);
			}

			// Don't find item
			{
				Vector<Foo> vector = { Foo(0), Foo(1), Foo(2) };
				const Vector<Foo> cvector = vector;
				Vector<Foo>::Iterator it = vector.Find(Foo(100));
				Assert::AreEqual(vector.end(), it);
				Vector<Foo>::ConstIterator cit = cvector.Find(Foo(100));
				Assert::AreEqual(cvector.cend(), cit);
			}

			// Find item
			{
				Vector<Foo> vector = { Foo(0), Foo(1), Foo(2), Foo(1), Foo(5) };
				const Vector<Foo> cvector = vector;
				Vector<Foo>::Iterator it = vector.Find(Foo(1));
				Assert::AreEqual(Foo(1), *it);
				Assert::AreEqual(vector.begin() + 1, it);
				Assert::AreNotEqual(vector.begin() + 3, it);
				Vector<Foo>::ConstIterator cit = cvector.Find(Foo(1));
				Assert::AreEqual(Foo(1), *cit);
				Assert::AreEqual(cvector.cbegin() + 1, cit);
				Assert::AreNotEqual(cvector.cbegin() + 3, cit);
			}
		}

		TEST_METHOD(TestRemoveSingle)
		{
			// Remove empty vector
			Vector<Foo> vector;
			Assert::AreEqual(size_t(0), vector.Size());
			vector.Remove(Foo(10));
			vector.Remove(vector.begin());
			vector.Remove(vector.end());
			vector.Remove(vector.begin(), vector.end());
			Assert::AreEqual(size_t(0), vector.Size());

			// Remove by value
			vector.Reserve(10);
			for (int i = 0; i < 10; ++i)
			{
				vector.PushBack(Foo(i));
			}
			Assert::AreEqual(Foo(0), vector.Front());
			Assert::AreEqual(Foo(9), vector.Back());
			vector.Remove(Foo(3));
			Assert::AreEqual(size_t(9), vector.Size());
			Assert::AreEqual(size_t(10), vector.Capacity());
			for (int i = 0; i < 3; ++i)
			{
				Assert::AreEqual(Foo(i), vector[i]);
			}
			for (int i = 4; i < 10; ++i)
			{
				Assert::AreEqual(Foo(i), vector[i - 1]);
			}

			// Remove by value - don't find
			vector.Remove(Foo(100));
			Assert::AreEqual(size_t(9), vector.Size());
			Assert::AreEqual(Foo(0), vector[0]);
			Assert::AreEqual(Foo(9), vector[8]);

			// Remove by iterator
			Vector<Foo>::Iterator it = vector.Find(Foo(5));
			vector.Remove(it);
			Assert::AreEqual(size_t(8), vector.Size());
			Assert::AreEqual(Foo(0), vector[0]);
			Assert::AreEqual(Foo(9), vector[7]);
			Assert::AreEqual(vector.end(), vector.Find(Foo(5)));
		}

		TEST_METHOD(TestRemoveRange)
		{
			// Remove empty vector
			Vector<Foo> vector;
			Assert::AreEqual(size_t(0), vector.Size());
			vector.Remove(vector.begin(), vector.end());
			Assert::AreEqual(size_t(0), vector.Size());

			// Remove invalid iterator
			auto lamda = [&vector]()
			{
				vector.Remove(Vector<Foo>::Iterator(), vector.end());
			};
			Assert::ExpectException<std::exception>(lamda);

			// Remove range
			vector = { Foo(0), Foo(1), Foo(2), Foo(3), Foo(4), Foo(5), Foo(6), Foo(7), Foo(8), Foo(9) };
			vector.Remove(vector.Find(Foo(1)), vector.Find(Foo(7)));
			Assert::AreEqual(size_t(4), vector.Size());
			Assert::AreEqual(size_t(10), vector.Capacity());
			Assert::AreEqual(Foo(0), vector[0]);
			Assert::AreEqual(Foo(7), vector[1]);
			Assert::AreEqual(Foo(8), vector[2]);
			Assert::AreEqual(Foo(9), vector[3]);

			// Remove range backward
			vector = { Foo(0), Foo(1), Foo(2), Foo(3), Foo(4), Foo(5), Foo(6), Foo(7), Foo(8), Foo(9) };
			vector.Remove(vector.Find(Foo(8)), vector.Find(Foo(1)));
			Assert::AreEqual(size_t(3), vector.Size());
			Assert::AreEqual(size_t(10), vector.Capacity());
			Assert::AreEqual(Foo(0), vector[0]);
			Assert::AreEqual(Foo(8), vector[1]);
			Assert::AreEqual(Foo(9), vector[2]);

			// Remove range single
			vector = { Foo(0), Foo(1), Foo(2), Foo(3), Foo(4), Foo(5), Foo(6), Foo(7), Foo(8), Foo(9) };
			vector.Remove(vector.Find(Foo(7)), vector.Find(Foo(8)));
			Assert::AreEqual(size_t(9), vector.Size());
			Assert::AreEqual(size_t(10), vector.Capacity());
			Assert::AreEqual(Foo(8), vector[7]);
			Assert::AreEqual(Foo(9), vector[8]);

			// Remove range same iterator
			vector = { Foo(0), Foo(1), Foo(2), Foo(3), Foo(4), Foo(5), Foo(6), Foo(7), Foo(8), Foo(9) };
			vector.Remove(vector.Find(Foo(7)), vector.Find(Foo(7)));
			Assert::AreEqual(size_t(10), vector.Size());
			Assert::AreEqual(size_t(10), vector.Capacity());
			Assert::AreEqual(Foo(7), vector[7]);

			// Remove range out of index at end
			vector = { Foo(0), Foo(1), Foo(2), Foo(3), Foo(4), Foo(5), Foo(6), Foo(7), Foo(8), Foo(9) };
			vector.Remove(vector.Find(Foo(3)), vector.begin() + 100);
			Assert::AreEqual(size_t(3), vector.Size());
			Assert::AreEqual(size_t(10), vector.Capacity());
			Assert::AreEqual(Foo(0), vector[0]);
			Assert::AreEqual(Foo(1), vector[1]);
			Assert::AreEqual(Foo(2), vector[2]);

			// Remove all
			vector = { Foo(0), Foo(1), Foo(2), Foo(3), Foo(4), Foo(5), Foo(6), Foo(7), Foo(8), Foo(9) };
			vector.Remove(vector.begin(), vector.end());
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(10), vector.Capacity());
			Assert::AreEqual(vector.end(), vector.begin());

			// Remove range out of index both
			vector = { Foo(0), Foo(1), Foo(2), Foo(3), Foo(4), Foo(5), Foo(6), Foo(7), Foo(8), Foo(9) };
			vector.Remove(vector.Find(Foo(100)), vector.begin() + 100);
			Assert::AreEqual(size_t(10), vector.Size());
			Assert::AreEqual(size_t(10), vector.Capacity());
			for (int i = 0; i < 10; ++i)
			{
				Assert::AreEqual(Foo(i), vector[i]);
			}
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState VectorTest::sStartMemState;
}