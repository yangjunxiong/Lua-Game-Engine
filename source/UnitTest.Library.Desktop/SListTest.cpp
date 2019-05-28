#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "SList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SListTest)
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

		TEST_METHOD(TestDefaultConstructor)
		{
			SList<int> list;
			Assert::IsTrue(list.IsEmpty());
			Assert::IsTrue(list.Size() == 0);
			
			// Check that the list is empty by default
			auto getFrontFunction = [&list]()
			{
				list.Front();
			};
			Assert::ExpectException<std::exception>(getFrontFunction);
			auto getBackFunction = [&list]()
			{
				list.Back();
			};
			Assert::ExpectException<std::exception>(getBackFunction);
		}

		TEST_METHOD(TestInitListConstructor)
		{
			SList<int> list = {0, 1, 2, 3, 4, 5};
			Assert::AreEqual(size_t(6), list.Size());
			Assert::AreEqual(0, list.Front());
			Assert::AreEqual(5, list.Back());
		}

		TEST_METHOD(TestCopyConstructor)
		{
			// Create a float list
			SList<float> list;
			list.PushFront(1.0f);
			list.PushFront(3.0f);
			list.PushBack(5.0f);

			// Test copy constructor
			SList<float> anotherList(list);
			list.Clear();
			Assert::IsTrue(list.IsEmpty());
			Assert::IsFalse(anotherList.IsEmpty());
			Assert::IsTrue(anotherList.Size() == 3U);
			Assert::AreEqual(anotherList.Front(), 3.0f);
			Assert::AreEqual(anotherList.Back(), 5.0f);
			anotherList.PopFront();
			Assert::AreEqual(anotherList.Front(), 1.0f);

			SList<float> thirdList(list);
			Assert::IsTrue(thirdList.IsEmpty());
		}

		TEST_METHOD(TestMoveConstructor)
		{
			SList<int> list = { 1, 2, 3 };
			SList<int> list2 = std::move(list);
			Assert::AreEqual(size_t(0), list.Size());
			Assert::ExpectException<std::exception>([&list] { list.Front(); });
			Assert::ExpectException<std::exception>([&list] { list.Back(); });
			Assert::AreEqual(size_t(3), list2.Size());
			Assert::AreEqual(1, list2.Front());
			Assert::AreEqual(2, *++list2.begin());
			Assert::AreEqual(3, list2.Back());
		}

		TEST_METHOD(TestCopyAssignment)
		{
			// Create a list
			SList<char> list;
			list.PushFront('a');
			list.PushFront('b');
			list.PushFront('c');

			// Create second list, test self copy
			SList<char> anotherList;
			anotherList.PushFront('z');
			anotherList = anotherList;  // Should do nothing
			Assert::IsTrue(anotherList.Size() == 1U);
			Assert::AreEqual(anotherList.Front(), 'z');

			// Test copy assignment
			anotherList = list;
			list.Clear();             // Clear original make sure we have deep copy
			Assert::IsTrue(anotherList.Size() == 3U);
			Assert::AreEqual(anotherList.Front(), 'c');
			Assert::AreEqual(anotherList.Back(), 'a');
			anotherList.PopBack();
			Assert::AreEqual(anotherList.Back(), 'b');
			anotherList = list;
			Assert::IsTrue(anotherList.IsEmpty());
		}

		TEST_METHOD(TestMoveAssignment)
		{
			SList<int> list = { 1, 2, 3 };
			SList<int> list2 = { 4, 5, 6 };
			list2 = std::move(list);
			Assert::AreEqual(size_t(0), list.Size());
			Assert::ExpectException<std::exception>([&list] { list.Front(); });
			Assert::ExpectException<std::exception>([&list] { list.Back(); });
			Assert::AreEqual(size_t(3), list2.Size());
			Assert::AreEqual(1, list2.Front());
			Assert::AreEqual(2, *++list2.begin());
			Assert::AreEqual(3, list2.Back());
		}

		TEST_METHOD(TestDestructor)
		{
			// Create a list of char*
			SList<char*>* list = new SList<char*>;
			const int size = 100;
			char* testArray[size];
			for (int i = 0; i < size; i++)
			{
				testArray[i] = nullptr;
				list->PushFront(testArray[i]);
			}

			// Delete it, there should be no memory leak
			delete list;
		}

		TEST_METHOD(TestClear)
		{
			// Create a list of char*
			SList<char*> list;
			const int size = 100;
			char* testArray[size];
			for (int i = 0; i < size; i++)
			{
				testArray[i] = nullptr;
				list.PushFront(testArray[i]);
			}

			// Verify the list content
			Assert::IsTrue(list.Size() == size);
			Assert::AreEqual(list.Front(), nullptr);
			Assert::AreEqual(list.Back(), nullptr);

			// Clear list, there should be no memory leak
			list.Clear();
			Assert::IsTrue(list.Size() == 0);
		}

		TEST_METHOD(TestGetFront)
		{
			// Define a new type
			typedef struct
			{
				int a = 1;
				double b = 2.0;
				char c = 'a';
				float* d = nullptr;
			} TestStruct;

			// Test get functions return correct value
			SList<TestStruct> list;
			auto getFrontFunction = [&list]()
			{
				list.Front();
			};
			Assert::ExpectException<std::exception>(getFrontFunction);
			TestStruct data;
			data.a = 1024;
			list.PushFront(data);
			Assert::AreEqual(list.Front().a, 1024);
			data.a = 100;
			list.PushFront(data);
			Assert::AreEqual(list.Front().a, 100);
			Assert::AreEqual(list.Back().a, 1024);

			// Test get functions return reference
			list.Clear();
			list.PushFront(data);
			Assert::AreEqual(list.Front().a, 100);
			TestStruct & dataFront = list.Front();
			dataFront.a = 666;
			Assert::AreEqual(list.Front().a, 666);
		}

		TEST_METHOD(TestGetBack)
		{
			// Check that it throws exception when list is empty
			SList<unsigned int> list;
			auto getBackFunction = [&list]()
			{
				list.Back();
			};
			Assert::ExpectException<std::exception>(getBackFunction);

			list.PushBack(1);
			Assert::AreEqual(list.Back(), 1U);
			list.PushBack(2);
			Assert::AreEqual(list.Back(), 2U);
			list.PushBack(3);
			Assert::AreEqual(list.Back(), 3U);
			unsigned int & thirdInt = list.Back();
			thirdInt = 30;
			Assert::AreEqual(list.Back(), 30U);
		}

		TEST_METHOD(TestPushFront)
		{
			SList<double> list;
			list.PushFront(20.0);
			Assert::IsTrue(list.Size() == 1U);
			Assert::AreEqual(list.Front(), 20.0);
			Assert::AreSame(list.Back(), list.Front());

			list.PushFront(100.0);
			Assert::IsTrue(list.Size() == 2U);
			Assert::AreEqual(list.Front(), 100.0);
			Assert::AreEqual(list.Back(), 20.0);

			list.PushFront(-10.0);
			Assert::IsTrue(list.Size() == 3U);
			Assert::AreEqual(list.Front(), -10.0);
			Assert::AreEqual(list.Back(), 20.0);
		}

		TEST_METHOD(TestPushBack)
		{
			SList<double> list;
			list.PushBack(50.0);
			Assert::IsTrue(list.Size() == 1U);
			Assert::AreEqual(list.Back(), 50.0);
			Assert::AreSame(list.Front(), list.Back());

			list.PushBack(1.0);
			Assert::IsTrue(list.Size() == 2U);
			Assert::AreEqual(list.Front(), 50.0);
			Assert::AreEqual(list.Back(), 1.0);
		}

		TEST_METHOD(TestPopFront)
		{
			// Create int* list
			SList<int*> list;
			list.PopFront();           // This should do nothing
			int* pointer1 = new int;
			int* pointer2 = new int;
			int* pointer3 = new int;
			list.PushFront(pointer1);
			list.PushBack(pointer2);
			list.PushBack(pointer3);

			// Check original status
			Assert::IsTrue(list.Size() == 3U);
			Assert::AreEqual(list.Front(), pointer1);
			Assert::AreEqual(list.Back(), pointer3);

			// Pop first one
			list.PopFront();
			Assert::IsTrue(list.Size() == 2U);
			Assert::AreEqual(list.Front(), pointer2);
			Assert::AreEqual(list.Back(), pointer3);

			// Pop second one
			list.PopFront();
			Assert::IsTrue(list.Size() == 1U);
			Assert::AreEqual(list.Front(), pointer3);
			Assert::AreEqual(list.Back(), pointer3);

			// Pop last one
			list.PopFront();
			Assert::IsTrue(list.IsEmpty());
			auto getFrontFunction = [&list]()
			{
				list.Front();
			};
			Assert::ExpectException<std::exception>(getFrontFunction);
			auto getBackFunction = [&list]()
			{
				list.Back();
			};
			Assert::ExpectException<std::exception>(getBackFunction);

			// Release test pointers
			delete pointer1;
			delete pointer2;
			delete pointer3;
		}

		TEST_METHOD(TestPopBack)
		{
			// Construct list of Foo, from 0 - 9
			SList<Foo> list;
			for (int i = 0; i < 10; i++)
			{
				list.PushBack(Foo(i));
				Assert::AreEqual(list.Back().Data(), i);
			}
			Assert::IsTrue(list.Size() == 10U);
			Assert::AreEqual(list.Back().Data(), 9);

			// Check that PopBack deletes the last one
			for (int i = 0; i < 10; i++)
			{
				Assert::AreEqual(list.Front().Data(), 0);
				Assert::AreEqual(list.Back().Data(), 9 - i);
				list.PopBack();
				Assert::IsTrue(list.Size() == (unsigned int)(9 - i));
			}
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(TestUserDefinedData)
		{
			SList<Foo*> list;
			Assert::IsTrue(list.IsEmpty());
			Foo foo1(1);
			Foo foo2(2);
			list.PushFront(&foo1);
			list.PushBack(&foo2);

			Foo* fooPointer1 = list.Front();
			Foo* fooPointer2 = list.Back();
			Assert::AreNotEqual(fooPointer1, fooPointer2);
			Assert::AreEqual(fooPointer1->Data(), 1);
			Assert::AreEqual(fooPointer2->Data(), 2);

			*fooPointer1 = *fooPointer2;
			Assert::AreEqual(fooPointer1->Data(), fooPointer2->Data());
			Assert::AreEqual(*fooPointer1, *fooPointer2);
			Assert::AreNotEqual(fooPointer1, fooPointer2);

			list.PopFront();
			Assert::IsTrue(list.Size() == 1U);
			Assert::AreEqual(list.Front()->Data(), 2);

			list.Clear();
			list.PushBack(new Foo(3));
			list.PushBack(new Foo(4));
			delete list.Front();
			delete list.Back();
			Assert::IsTrue(list.Size() == 2U);

			// Nested list
			SList<SList<Foo*>*> upperList;
			upperList.PushFront(&list);
			Assert::IsTrue(upperList.Size() == 1U);
			Assert::IsTrue(upperList.Back()->Size() == 2U);
			upperList.Front()->Clear();
			Assert::IsFalse(upperList.IsEmpty());
			Assert::IsTrue(list.IsEmpty());
			upperList.PopFront();
			Assert::IsTrue(upperList.IsEmpty());
		}

		TEST_METHOD(TestIteratorConstructor)
		{
			// Test by default iterator points to nothing
			SList<int>::Iterator it;
			auto function = [&it]
			{
				*it;
			};
			Assert::ExpectException<std::exception>(function);

			// Test copy constructor
			SList<int> list = { 1, 2, 3, 4, 5 };
			it = list.begin();
			++it;
			Assert::IsTrue(2 == *it);
			SList<int>::Iterator it2(it);
			Assert::IsTrue(it2 == it);
		}

		TEST_METHOD(TestIteratorDereference)
		{
			SList<int> list1 = {1, 2, 3};
			auto it = list1.begin();
			Assert::IsTrue(1 == *it);
			Assert::IsTrue(2 == *++it);
			Assert::IsTrue(3 == *++it);

			float a = 2.0f;
			float b = 3.0f;
			SList<float*> list2 = {&a, &b};
			auto it2 = list2.begin();
			Assert::IsTrue(2.0f == *(*it2));
			Assert::IsTrue(3.0f == *(*++it2));

			// Test trying to deference null iterator throws exception
			it2 = list2.end();
			auto function = [&it2]
			{
				*it2;
			};
			Assert::ExpectException<std::exception>(function);
		}

		TEST_METHOD(TestIteratorPreincrement)
		{
			Foo a(1);
			Foo b(2);
			Foo c(3);
			SList<Foo> list = {a, b, c};

			auto it = list.begin();
			Assert::IsTrue(a == *it);
			Assert::IsTrue(b == *++it);

			auto & itRef = ++it;
			Assert::IsTrue(c == *itRef);
			++itRef;
			Assert::IsTrue(list.end() == it);

			++it;
			++it;
			++it;
			Assert::IsTrue(list.end() == it);
		}

		TEST_METHOD(TestIteratorPostincrement)
		{
			Foo a(1);
			Foo b(2);
			Foo c(3);
			SList<Foo> list = { a, b, c };

			auto it = list.begin();
			Assert::IsTrue(a == *it);
			Assert::IsTrue(a == *it++);

			auto itCopy = it++;
			Assert::IsTrue(b == *itCopy);
			Assert::IsTrue(c == *it);
			itCopy++;
			Assert::IsTrue(c == *itCopy);
			Assert::IsTrue(c == *it);

			it++;
			it++;
			it++;
			Assert::IsTrue(list.end() == it);
		}

		TEST_METHOD(TestIteratorComparision)
		{
			SList<char> list = {'a', 'b', 'c'};
			auto it = list.begin();
			auto it2 = list.begin();
			Assert::IsTrue(it == it2);
			it2++;
			Assert::IsFalse(it == it2);
			it2++;
			it2++;
			Assert::IsTrue(it2 == list.end());

			SList<char> list2 = list;
			auto it3 = list2.begin();
			Assert::AreEqual('a', *it);
			Assert::AreEqual('a', *it3);
			Assert::IsFalse(it == it3);
			Assert::IsFalse(it2 == list2.end());
		}

		TEST_METHOD(TestBeginEnd)
		{
			SList<int> list;
			Assert::IsTrue(list.begin() == list.end());

			list.PushFront(1);
			auto it = list.begin();
			Assert::AreEqual(1, *it);
			auto it2 = list.end();
			Assert::IsFalse(it2 == SList<int>::Iterator());
			it++;
			Assert::IsTrue(it == it2);
			it++;
			it++;
			Assert::IsTrue(it == it2);
		}

		TEST_METHOD(TestConstBeginEnd)
		{
			const SList<int> list;
			Assert::IsTrue(list.begin() == list.end());

			const SList<int> list2 = {0, 1, 2, 3};
			auto it = list2.begin();
			Assert::AreEqual(0, *it);
			auto it2 = list2.end();
			Assert::IsFalse(it2 == SList<int>::Iterator());
			Assert::AreEqual(1, *++it);
			Assert::AreEqual(2, *++it);
			Assert::AreEqual(3, *++it);
			Assert::IsTrue(it2 == ++it);
		}

		TEST_METHOD(TestFind)
		{
			// Find empty list
			SList<Foo> list;
			auto it = list.Find(Foo(1));
			Assert::IsTrue(it == list.end());

			// Find the only element
			list.PushFront(Foo(1));
			it = list.Find(Foo(1));
			Assert::IsTrue((*it).Data() == 1);
			Assert::IsTrue(list.Find(Foo(0)) == list.end());

			// Find in list with multiple items
			list.PushBack(Foo(2));
			list.PushBack(Foo(3));
			list.PushBack(Foo(4));
			it = list.Find(Foo(3));
			Assert::IsTrue((*it).Data() == 3);
			Assert::AreEqual(Foo(4), *++it);
			Assert::IsTrue(list.end() == ++it);

			// Find returns the right iterator
			it = list.Find(Foo(2));
			list.Remove(it);
			Assert::IsTrue(list.end() == list.Find(Foo(2)));
			it = list.begin();
			Assert::IsTrue(Foo(1) == *it);
			Assert::IsTrue(Foo(3) == *++it);
		}

		TEST_METHOD(TestConstFind)
		{
			// Find empty list
			const SList<int> list;
			auto it = list.Find(0);
			Assert::IsTrue(it == list.end());

			// Find the only element
			const SList<int> list2 = {0};
			it = list2.Find(0);
			Assert::IsTrue(*it == 0);
			Assert::IsTrue(list2.Find(1) == list2.end());

			// Find in list with multiple items
			const SList<int> list3 = {0, 1, 2, 3, 4};
			it = list3.Find(2);
			Assert::AreEqual(2, *it);
			Assert::AreEqual(3, *++it);
			Assert::AreEqual(4, *++it);
			Assert::IsTrue(list3.end() == ++it);
		}

		TEST_METHOD(TestInsertAfter)
		{
			// Insert to empty list
			SList<int> emptyList;
			emptyList.InsertAfter(emptyList.begin(), 10);
			Assert::AreEqual(size_t(1), emptyList.Size());
			Assert::AreEqual(10, emptyList.Front());

			// Insert after end
			SList<int> list = { 0, 1, 2, 3, 4, 5 };
			auto result = list.InsertAfter(list.end(), 55);
			Assert::AreEqual(size_t(7), list.Size());
			Assert::AreEqual(55, *result);

			// Insert after the front
			auto it = list.begin();
			list.InsertAfter(it, 10);
			Assert::AreEqual(size_t(8), list.Size());
			it = list.begin();
			Assert::IsTrue(*it == 0);
			Assert::IsTrue(*++it == 10);
			Assert::IsTrue(*++it == 1);
			Assert::AreEqual(55, list.Back());

			// Insert in the middle
			list.InsertAfter(it, 20);
			Assert::AreEqual(size_t(9), list.Size());
			it = list.begin();
			Assert::IsTrue(*it == 0);
			Assert::IsTrue(*++it == 10);
			Assert::IsTrue(*++it == 1);
			Assert::IsTrue(*++it == 20);
			Assert::IsTrue(*++it == 2);
			Assert::IsTrue(list.Back() == 55);

			// Insert after the last
			list = { 0, 1, 2 };
			it = list.begin();
			++it;
			++it;
			Assert::AreEqual(2, *it);
			list.InsertAfter(it, 3);
			Assert::AreEqual(size_t(4), list.Size());
			Assert::AreEqual(3, list.Back());

			// Insert to invalid interator
			SList<int> v;
			SList<int>::Iterator it2;
			auto lamda = [&it2, &v]()
			{
				v.InsertAfter(it2, 10);
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestRemoveIterator)
		{
			SList<float> list = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f};

			// Remove invalid
			Assert::AreEqual(size_t(5), list.Size());
			list.Remove(list.end());
			Assert::AreEqual(size_t(5), list.Size());
			SList<float> anotherList(list);
			auto anotherIt = anotherList.begin();
			list.Remove(anotherIt);
			Assert::AreEqual(size_t(5), list.Size());
			Assert::AreEqual(0.0f, list.Front());

			// Remove front
			// list {0.0, 1.0, 2.0, 3.0, 4.0}
			auto it = list.begin();
			list.Remove(it);
			Assert::AreEqual(size_t(4), list.Size());
			Assert::AreEqual(1.0f, list.Front());
			it = list.begin();
			for (int i = 1; i < 5; ++i)
			{
				Assert::AreEqual((float)i, *it);
				++it;
			}
			Assert::IsTrue(list.end() == it);

			// Remove middle
			// list {1.0, 2.0, 3.0, 4.0}
			it = list.begin();
			++it;
			Assert::AreEqual(2.0f, *it);
			list.Remove(it);
			Assert::AreEqual(1.0f, list.Front());
			Assert::AreEqual(4.0f, list.Back());
			Assert::AreEqual(size_t(3), list.Size());
			it = list.begin();
			++it;
			Assert::AreEqual(3.0f, *it);

			// Remove back
			// list {1.0, 3.0, 4.0}
			it = list.begin();
			++it;
			++it;
			list.Remove(it);
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(1.0f, list.Front());
			Assert::AreEqual(3.0f, list.Back());

			// Remove last
			it = list.begin();
			while (it != list.end())
			{
				list.Remove(it);
				it = list.begin();
			}
			Assert::AreEqual(size_t(0), list.Size());
			Assert::IsTrue(list.end() == list.begin());
		}

		TEST_METHOD(TestRemoveValue)
		{
			// Remove empty
			SList<int> emptyList;
			emptyList.Remove(0);
			Assert::AreEqual(size_t(0), emptyList.Size());

			// Remove not found
			SList<double> list = { 0.0, 1.0, 2.0 };
			list.Remove(3.0);
			Assert::AreEqual(size_t(3), list.Size());
			Assert::AreEqual(0.0, list.Front());
			Assert::AreEqual(2.0, list.Back());
			Assert::AreEqual(1.0, *++list.begin());

			// Remove middle
			list.Remove(1.0);
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(0.0, list.Front());
			Assert::AreEqual(2.0, list.Back());
			Assert::AreEqual(2.0, *++list.begin());

			// Remove front
			list.Remove(0.0);
			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(2.0, list.Front());
			Assert::AreEqual(2.0, list.Back());

			// Remove back
			list = SList<double> {0.0, 1.0, 2.0};
			Assert::AreEqual(size_t(3), list.Size());
			list.Remove(2.0);
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(0.0, list.Front());
			Assert::AreEqual(1.0, list.Back());

			// Remove last
			list.Remove(0.0);
			list.Remove(1.0);
			Assert::AreEqual(size_t(0), list.Size());
			Assert::IsTrue(list.begin() == list.end());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState SListTest::sStartMemState;
}