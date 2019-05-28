#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "HashMap.h"
#include <set>

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
	{}

	Bar(const Bar& other) = default;

	bool operator==(const Bar& other) const
	{
		return a == other.a && b == other.b && c == other.c;
	}
};

struct FooHashFunction
{
	size_t operator()(const Foo& foo) const
	{
		return foo.Data();
	}
};

struct FooCompareFunction
{
	virtual bool operator()(const Foo& foo1, const Foo& foo2) const
	{
		return foo1.Data() == foo2.Data();
	}
};

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<HashMap<Foo, int, FooHashFunction, FooCompareFunction>::Iterator>(const HashMap<Foo, int, FooHashFunction, FooCompareFunction>::Iterator& it)
	{
		try
		{
			RETURN_WIDE_STRING((*it).first.Data());
		}
		catch (std::exception e)
		{
			return L"Iterator points to nothing";
		}
	}

	template<>
	inline std::wstring ToString<HashMap<Foo, int, FooHashFunction, FooCompareFunction>::ConstIterator>(const HashMap<Foo, int, FooHashFunction, FooCompareFunction>::ConstIterator& it)
	{
		try
		{
			RETURN_WIDE_STRING((*it).first.Data());
		}
		catch (std::exception e)
		{
			return L"Iterator points to nothing";
		}
	}

	template<>
	inline std::wstring ToString<Bar>(const Bar& bar)
	{
		RETURN_WIDE_STRING(bar.a + bar.b + bar.c);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashMapTest)
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
			HashMap<Foo, int> map;
			Assert::AreEqual(size_t(0), map.Size());
			Assert::IsTrue(map.IsEmpty());
		}

		TEST_METHOD(TestInitializeListConstructor)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map =
			{
				std::make_pair(Foo(1), 1),
				std::make_pair(Foo(2), 2),
				std::make_pair(Foo(3), 3),
				std::make_pair(Foo(4), 4)
			};
			Assert::AreEqual(size_t(4), map.Size());
			Assert::IsTrue(map.ContainsKey(Foo(1)));
			Assert::IsTrue(map.ContainsKey(Foo(2)));
			Assert::IsTrue(map.ContainsKey(Foo(3)));
			Assert::IsTrue(map.ContainsKey(Foo(4)));
		}

		TEST_METHOD(TestCopyConstructor)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map;
			map.Insert(std::make_pair(Foo(1), 1));
			map.Insert(std::make_pair(Foo(2), 1));
			map.Insert(std::make_pair(Foo(3), 1));
			Assert::AreEqual(size_t(3), map.Size());

			HashMap map2 = map;
			Assert::AreEqual(size_t(3), map.Size());
			Assert::AreEqual(size_t(3), map2.Size());
			Assert::IsTrue(map2.ContainsKey(Foo(1)));
			Assert::IsTrue(map2.ContainsKey(Foo(2)));
			Assert::IsTrue(map2.ContainsKey(Foo(3)));
			Assert::IsFalse(map2.ContainsKey(Foo(0)));
			Assert::IsTrue(map.ContainsKey(Foo(1)));
		}

		TEST_METHOD(TestMoveConstructor)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map;
			map.Insert(std::make_pair(Foo(1), 1));
			map.Insert(std::make_pair(Foo(2), 1));
			map.Insert(std::make_pair(Foo(3), 1));
			Assert::AreEqual(size_t(3), map.Size());

			HashMap map2 = std::move(map);
			Assert::AreEqual(size_t(0), map.Size());
			Assert::AreEqual(size_t(3), map2.Size());
			Assert::IsTrue(map2.ContainsKey(Foo(1)));
			Assert::IsTrue(map2.ContainsKey(Foo(2)));
			Assert::IsTrue(map2.ContainsKey(Foo(3)));
			Assert::IsFalse(map2.ContainsKey(Foo(0)));
		}

		TEST_METHOD(TestCopyOperator)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map;
			map.Insert(std::make_pair(Foo(1), 1));
			map.Insert(std::make_pair(Foo(2), 1));
			map.Insert(std::make_pair(Foo(3), 1));

			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map2;
			map2.Insert(std::make_pair(Foo(10), 1));
			Assert::AreEqual(size_t(1), map2.Size());
			map2 = map;
			Assert::AreEqual(size_t(3), map.Size());
			Assert::AreEqual(size_t(3), map2.Size());
			Assert::IsTrue(map2.ContainsKey(Foo(1)));
			Assert::IsTrue(map2.ContainsKey(Foo(2)));
			Assert::IsTrue(map2.ContainsKey(Foo(3)));
			Assert::IsFalse(map2.ContainsKey(Foo(10)));
		}

		TEST_METHOD(TestMoveOperator)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map;
			map.Insert(std::make_pair(Foo(1), 1));
			map.Insert(std::make_pair(Foo(2), 1));
			map.Insert(std::make_pair(Foo(3), 1));

			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map2;
			map2.Insert(std::make_pair(Foo(10), 1));
			Assert::AreEqual(size_t(1), map2.Size());
			map2 = std::move(map);
			Assert::AreEqual(size_t(0), map.Size());
			Assert::AreEqual(size_t(3), map2.Size());
			Assert::IsTrue(map2.ContainsKey(Foo(1)));
			Assert::IsTrue(map2.ContainsKey(Foo(2)));
			Assert::IsTrue(map2.ContainsKey(Foo(3)));
			Assert::IsFalse(map2.ContainsKey(Foo(10)));
		}

		TEST_METHOD(TestClear)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map;
			map.Insert(std::make_pair(Foo(1), 2));
			map.Insert(std::make_pair(Foo(2), 3));
			map.Insert(std::make_pair(Foo(3), 4));
			Assert::AreEqual(size_t(3), map.Size());
			map.Clear();
			Assert::AreEqual(size_t(0), map.Size());
		}

		TEST_METHOD(TestInsert)
		{
			{
				HashMap<Bar, int> map;
				Bar key(1, 2.0f, 'c');
				Assert::AreEqual(size_t(0), map.Size());
				map.Insert(std::make_pair(Bar(key), 1));
				Assert::AreEqual(size_t(1), map.Size());
				Assert::IsTrue(map.ContainsKey(Bar(key)));

				auto it = map.Insert(std::make_pair(Bar(key), 2)).first;
				Assert::AreEqual(size_t(1), map.Size());
				Assert::AreEqual(1, (*map.Find(Bar(key))).second);
				it->second = 20;
				Assert::AreEqual(20, (*map.Find(Bar(key))).second);

				Bar key2(1, 2.0f, 'd');
				map.Insert(std::make_pair(key2, 30));
				Assert::AreEqual(size_t(2), map.Size());
				Assert::AreEqual(20, map.Find(Bar(key))->second);
				Assert::AreEqual(30, map.Find(Bar(key2))->second);

				for (int i = 0; i < 100; ++i)
				{
					Bar keyi(i, 10.0f, 'c');
					auto it2 = map.Insert(std::make_pair(keyi, i)).first;
					Assert::AreEqual(size_t(i + 3), map.Size());
					Assert::AreEqual(Bar(keyi), it2->first);
					Assert::AreEqual(i, it2->second);
					Assert::AreEqual(i, map.Find(Bar(keyi))->second);
				}
			}
			
			{
				HashMap<char*, int> map;
				char* key = "Hello";
				char key2[6];
				char key3[6];
				char key4[6];
				strcpy_s(key2, key);
				strcpy_s(key3, key);
				strcpy_s(key4, key);
				Assert::AreEqual(size_t(0), map.Size());
				map.Insert(std::make_pair(key, 2));
				Assert::AreEqual(size_t(1), map.Size());
				Assert::AreEqual(2, map.Find(key3)->second);

				auto it = map.Insert(std::make_pair(key, 5)).first;
				Assert::AreEqual(size_t(1), map.Size());
				Assert::AreEqual(2, it->second);
				Assert::AreEqual(2, map.Find(key3)->second);

				char* newKey = "GFF";
				char newKey2[4];
				strcpy_s(newKey2, newKey);
				map.Insert(std::make_pair(newKey, 10));
				Assert::AreEqual(size_t(2), map.Size());
				Assert::AreEqual(10, map.Find(newKey2)->second);
			}
		}

		TEST_METHOD(TestRemove)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map;
			Assert::IsTrue(map.IsEmpty());
			map.Remove(Foo(1));
			Assert::IsTrue(map.IsEmpty());

			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction>::Iterator it;
				auto lamda = [&it, &map]()
				{
					map.Remove(it);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			map.Insert(std::make_pair(Foo(1), 1));
			map.Insert(std::make_pair(Foo(2), 2));
			map.Insert(std::make_pair(Foo(3), 3));
			Assert::AreEqual(size_t(3), map.Size());
			map.Remove(Foo(4));
			Assert::AreEqual(size_t(3), map.Size());
			map.Remove(Foo(1));
			Assert::AreEqual(size_t(2), map.Size());
			Assert::IsFalse(map.ContainsKey(Foo(1)));
			map.Remove(map.Find(Foo(2)));
			Assert::IsFalse(map.ContainsKey(Foo(2)));
			map.Remove(Foo(3));
			Assert::AreEqual(size_t(0), map.Size());
		}

		TEST_METHOD(TestFind)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map;
			Assert::AreEqual(map.end(), map.Find(Foo()));

			map.Insert(std::make_pair(Foo(1), 1));
			map.Insert(std::make_pair(Foo(2), 2));
			map.Insert(std::make_pair(Foo(3), 3));
			Assert::AreEqual(1, (*map.Find(Foo(1))).second);
			Assert::AreEqual(2, (*map.Find(Foo(2))).second);
			Assert::AreEqual(3, (*map.Find(Foo(3))).second);
			Assert::AreEqual(map.end(), map.Find(Foo(4)));
		}

		TEST_METHOD(TestContainsKey)
		{
			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction> map;
				Assert::IsFalse(map.ContainsKey(Foo()));

				map.Insert(std::make_pair(Foo(1), 1));
				map.Insert(std::make_pair(Foo(2), 2));
				map.Insert(std::make_pair(Foo(3), 3));
				Assert::IsTrue(map.ContainsKey(Foo(1)));
				Assert::IsTrue(map.ContainsKey(Foo(2)));
				Assert::IsTrue(map.ContainsKey(Foo(3)));
				Assert::IsFalse(map.ContainsKey(Foo(4)));
			}
			
			{
				HashMap<Bar, int> map;
				Assert::IsFalse(map.ContainsKey(Bar(1, 2.0f, 'c')));

				map.Insert(std::make_pair(Bar(1, 2.0f, 'c'), 2));
				map.Insert(std::make_pair(Bar(1, 2.0f, 'd'), 3));
				Assert::IsTrue(map.ContainsKey(Bar(1, 2.0f, 'c')));
				Assert::IsTrue(map.ContainsKey(Bar(1, 2.0f, 'd')));
				Assert::IsFalse(map.ContainsKey(Bar(1, 2.0f, 'b')));
			}

			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction> map;
				Assert::IsFalse(map.ContainsKey(Foo()));
				HashMap<Foo, int, FooHashFunction, FooCompareFunction>::Iterator it;
				map.Insert(std::make_pair(Foo(1), 1));
				map.Insert(std::make_pair(Foo(2), 2));
				Assert::IsTrue(map.ContainsKey(Foo(1), it));
				Assert::AreEqual(1, (*it).second);
				Assert::IsTrue(map.ContainsKey(Foo(2), it));
				Assert::AreEqual(2, (*it).second);
				Assert::IsFalse(map.ContainsKey(Foo(3), it));
				Assert::AreEqual(map.end(), it);
			}
			
			{
				const HashMap<Foo, int, FooHashFunction, FooCompareFunction> map =
				{
					std::make_pair(Foo(1), 1),
					std::make_pair(Foo(2), 2)
				};
				Assert::IsFalse(map.ContainsKey(Foo()));
				HashMap<Foo, int, FooHashFunction, FooCompareFunction>::ConstIterator it;
				Assert::IsTrue(map.ContainsKey(Foo(1), it));
				Assert::AreEqual(1, (*it).second);
				Assert::IsTrue(map.ContainsKey(Foo(2), it));
				Assert::AreEqual(2, (*it).second);
				Assert::IsFalse(map.ContainsKey(Foo(3), it));
				Assert::AreEqual(map.end(), it);
			}
		}

		TEST_METHOD(TestSizeEmpty)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map;
			Assert::AreEqual(size_t(0), map.Size());
			Assert::IsTrue(map.IsEmpty());

			for (int i = 0; i < 10000; ++i)
			{
				map.Insert(std::make_pair(Foo(i), i));
				Assert::AreEqual(size_t(i + 1), map.Size());
				Assert::IsFalse(map.IsEmpty());
				Assert::AreEqual(i, (*map.Find(Foo(i))).second);
			}

			for (int i = 0; i < 10000; ++i)
			{
				map.Remove(Foo(i));
				Assert::AreEqual(size_t(10000 - i - 1), map.Size());
				Assert::IsFalse(map.ContainsKey(Foo(i)));
			}
			Assert::IsTrue(map.IsEmpty());

			map =
			{
				std::make_pair(Foo(1), 1),
				std::make_pair(Foo(2), 2),
				std::make_pair(Foo(3), 3),
				std::make_pair(Foo(4), 4)
			};
			Assert::AreEqual(size_t(4), map.Size());
			Assert::IsFalse(map.IsEmpty());
			map.Clear();
			Assert::AreEqual(size_t(0), map.Size());
			Assert::IsTrue(map.IsEmpty());
		}

		TEST_METHOD(TestIndexOperator)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map =
			{
				std::make_pair(Foo(1), 1),
				std::make_pair(Foo(2), 2),
				std::make_pair(Foo(3), 3),
				std::make_pair(Foo(4), 4)
			};
			Assert::AreEqual(size_t(4), map.Size());

			// Just access entry
			Assert::AreEqual(1, map[Foo(1)]);
			Assert::AreEqual(2, map[Foo(2)]);
			Assert::AreEqual(3, map[Foo(3)]);
			Assert::AreEqual(4, map[Foo(4)]);
			Assert::AreEqual(size_t(4), map.Size());

			// Modify existing entry
			map[Foo(2)] = 20;
			Assert::AreEqual(20, map[Foo(2)]);
			Assert::AreEqual(size_t(4), map.Size());

			// Create new entry implicitly
			map[Foo(5)] = 5;
			Assert::AreEqual(5, map[Foo(5)]);
			Assert::AreEqual(size_t(5), map.Size());
		}

		TEST_METHOD(TestConstIndexOperator)
		{
			const HashMap<Foo, int, FooHashFunction, FooCompareFunction> map =
			{
				std::make_pair(Foo(1), 1),
				std::make_pair(Foo(2), 2),
				std::make_pair(Foo(3), 3),
				std::make_pair(Foo(4), 4)
			};
			Assert::AreEqual(1, map[Foo(1)]);
			Assert::AreEqual(2, map[Foo(2)]);
			Assert::AreEqual(3, map[Foo(3)]);
			Assert::AreEqual(4, map[Foo(4)]);
			
			auto lamda = [&map]()
			{
				map[Foo(5)];
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestAt)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map =
			{
				std::make_pair(Foo(1), 1),
				std::make_pair(Foo(2), 2),
				std::make_pair(Foo(3), 3),
				std::make_pair(Foo(4), 4)
			};
			Assert::AreEqual(1, map.At(Foo(1)));
			Assert::AreEqual(2, map.At(Foo(2)));
			Assert::AreEqual(3, map.At(Foo(3)));
			Assert::AreEqual(4, map.At(Foo(4)));
			map.At(Foo(4)) = 40;
			Assert::AreEqual(40, map.At(Foo(4)));

			auto lamda = [&map]()
			{
				map.At(Foo(5));
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestConstAt)
		{
			const HashMap<Foo, int, FooHashFunction, FooCompareFunction> map =
			{
				std::make_pair(Foo(1), 1),
				std::make_pair(Foo(2), 2),
				std::make_pair(Foo(3), 3),
				std::make_pair(Foo(4), 4)
			};
			Assert::AreEqual(1, map.At(Foo(1)));
			Assert::AreEqual(2, map.At(Foo(2)));
			Assert::AreEqual(3, map.At(Foo(3)));
			Assert::AreEqual(4, map.At(Foo(4)));

			auto lamda = [&map]()
			{
				map.At(Foo(5));
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestBegin)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map =
			{
				std::make_pair(Foo(1), 1),
				std::make_pair(Foo(2), 2),
				std::make_pair(Foo(3), 3),
				std::make_pair(Foo(4), 4)
			};
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map2 = map;
			auto it = map.begin();
			auto it2 = map2.begin();
			Assert::AreNotEqual(it, it2);
			Assert::AreEqual((*it).second, (*it).second);

			std::set<int> set = { 1, 2, 3, 4 };
			set.erase((*it).second);
			set.erase((*++it).second);
			set.erase((*++it).second);
			set.erase((*++it).second);
			Assert::AreEqual(map.end(), ++it);
			Assert::AreEqual(size_t(0), set.size());
		}

		TEST_METHOD(TestConstBegin)
		{
			const HashMap<Foo, int, FooHashFunction, FooCompareFunction> map =
			{
				std::make_pair(Foo(1), 1),
				std::make_pair(Foo(2), 2),
				std::make_pair(Foo(3), 3),
				std::make_pair(Foo(4), 4)
			};
			const HashMap<Foo, int, FooHashFunction, FooCompareFunction> map2 = map;

			{
				auto it = map.begin();
				auto it2 = map2.begin();
				Assert::AreNotEqual(it, it2);
				Assert::AreEqual((*it).second, (*it).second);

				std::set<int> set = { 1, 2, 3, 4 };
				set.erase((*it).second);
				set.erase((*++it).second);
				set.erase((*++it).second);
				set.erase((*++it).second);
				Assert::AreEqual(map.end(), ++it);
				Assert::AreEqual(size_t(0), set.size());
			}
			
			{
				auto it = map.cbegin();
				auto it2 = map2.cbegin();
				Assert::AreNotEqual(it, it2);
				Assert::AreEqual((*it).second, (*it).second);

				std::set<int> set = { 1, 2, 3, 4 };
				set.erase((*it).second);
				set.erase((*++it).second);
				set.erase((*++it).second);
				set.erase((*++it).second);
				Assert::AreEqual(map.cend(), ++it);
				Assert::AreEqual(size_t(0), set.size());
			}
		}

		TEST_METHOD(TestEnd)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map;
			Assert::AreEqual(map.end(), map.begin());
			map.Insert(std::make_pair(Foo(1), 1));
			auto it = map.begin();
			Assert::AreEqual(map.end(), ++it);

			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map2 = map;
			Assert::AreNotEqual(map.end(), map2.end());
		}

		TEST_METHOD(TestConstEnd)
		{
			{
				const HashMap<Foo, int, FooHashFunction, FooCompareFunction> map;
				Assert::AreEqual(map.end(), map.begin());
			}

			{
				const HashMap<Foo, int, FooHashFunction, FooCompareFunction> map = { std::make_pair(Foo(1), 1) };
				auto it = map.begin();
				Assert::AreEqual(map.end(), ++it);
				const HashMap<Foo, int, FooHashFunction, FooCompareFunction> map2 = map;
				Assert::AreNotEqual(map.end(), map2.end());
			}

			{
				const HashMap<Foo, int, FooHashFunction, FooCompareFunction> map;
				Assert::AreEqual(map.cend(), map.cbegin());
			}
			
			{
				const HashMap<Foo, int, FooHashFunction, FooCompareFunction> map = { std::make_pair(Foo(1), 1) };
				auto it = map.cbegin();
				Assert::AreEqual(map.cend(), ++it);
				const HashMap<Foo, int, FooHashFunction, FooCompareFunction> map2 = map;
				Assert::AreNotEqual(map.cend(), map2.cend());
			}
		}

		TEST_METHOD(TestIteratorDefaultConstructor)
		{
			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction>::Iterator it;
				HashMap<Foo, int, FooHashFunction, FooCompareFunction>::Iterator it2;
				Assert::AreEqual(it, it2);

				auto incrementLamda = [&it]()
				{
					++it;
				};
				auto dereferenceLamda = [&it]()
				{
					*it;
				};
				Assert::ExpectException<std::exception>(incrementLamda);
				Assert::ExpectException<std::exception>(dereferenceLamda);
			}
			
			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction>::ConstIterator it;
				HashMap<Foo, int, FooHashFunction, FooCompareFunction>::ConstIterator it2;
				Assert::AreEqual(it, it2);

				auto incrementLamda = [&it]()
				{
					++it;
				};
				auto dereferenceLamda = [&it]()
				{
					*it;
				};
				Assert::ExpectException<std::exception>(incrementLamda);
				Assert::ExpectException<std::exception>(dereferenceLamda);
			}
		}

		TEST_METHOD(TestIteratorCopyConstructor)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map = { std::make_pair(Foo(1), 1) };
			auto it = map.begin();
			auto it2 = it;
			Assert::AreEqual(it, it2);
			Assert::AreEqual(1, (*it2).second);

			auto cit = map.cbegin();
			auto cit2 = cit;
			Assert::AreEqual(cit, cit2);
			Assert::AreEqual(1, (*cit2).second);
		}

		TEST_METHOD(TestIteratorMoveConstructor)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map = { std::make_pair(Foo(1), 1) };
			auto it = map.begin();
			auto it2 = std::move(it);
			Assert::AreEqual(it, it2);
			Assert::AreEqual(1, (*it2).second);

			auto cit = map.cbegin();
			auto cit2 = std::move(cit);
			Assert::AreEqual(cit, cit2);
			Assert::AreEqual(1, (*cit2).second);
		}

		TEST_METHOD(TestIteratorCopyOperator)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map = { std::make_pair(Foo(1), 1) };
			auto it = map.end();
			auto it2 = map.begin();
			Assert::AreNotEqual(it, it2);
			it = it2;
			Assert::AreEqual(it, it2);
			Assert::AreEqual(1, (*it).second);

			auto cit = map.cend();
			auto cit2 = map.cbegin();
			Assert::AreNotEqual(cit, cit2);
			cit = cit2;
			Assert::AreEqual(cit, cit2);
			Assert::AreEqual(1, (*cit).second);
		}

		TEST_METHOD(TestIteratorMoveOperator)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map = { std::make_pair(Foo(1), 1) };
			auto it = map.end();
			auto it2 = map.begin();
			Assert::AreNotEqual(it, it2);
			it = std::move(it2);
			Assert::AreEqual(it, it2);
			Assert::AreEqual(1, (*it).second);

			auto cit = map.cend();
			auto cit2 = map.cbegin();
			Assert::AreNotEqual(cit, cit2);
			cit = std::move(cit2);
			Assert::AreEqual(cit, cit2);
			Assert::AreEqual(1, (*cit).second);
		}

		TEST_METHOD(TestIteratorCompare)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction>::Iterator it;
			HashMap<Foo, int, FooHashFunction, FooCompareFunction>::Iterator it2;
			Assert::AreEqual(it, it2);

			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map =
			{
				std::make_pair(Foo(1), 1),
				std::make_pair(Foo(2), 2),
				std::make_pair(Foo(3), 3),
				std::make_pair(Foo(4), 4)
			};
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map2 = map;
			it = map.begin();
			it2 = map.end();
			Assert::AreNotEqual(it, it2);
			++it;
			++it;
			++it;
			++it;
			Assert::AreEqual(it, it2);
			it2 = map2.end();
			Assert::AreNotEqual(it, it2);

			HashMap<Foo, int, FooHashFunction, FooCompareFunction>::ConstIterator cit;
			HashMap<Foo, int, FooHashFunction, FooCompareFunction>::ConstIterator cit2;
			Assert::AreEqual(cit, cit2);
			cit = map.cbegin();
			cit2 = map.cend();
			Assert::AreNotEqual(cit, cit2);
			++cit;
			++cit;
			++cit;
			++cit;
			Assert::AreEqual(cit, cit2);
			cit2 = map2.end();
			Assert::AreNotEqual(cit, cit2);
		}

		TEST_METHOD(TestIteratorDereference)
		{
			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction>::Iterator it;
				auto lamda = [&it]()
				{
					*it;
				};
				Assert::ExpectException<std::exception>(lamda);

				HashMap<Foo, int, FooHashFunction, FooCompareFunction>::ConstIterator cit;
				auto c_lamda = [&cit]()
				{
					*cit;
				};
				Assert::ExpectException<std::exception>(c_lamda);
			}

			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction> map =
				{
					std::make_pair(Foo(1), 1),
					std::make_pair(Foo(2), 2),
					std::make_pair(Foo(3), 3),
					std::make_pair(Foo(4), 4)
				};
				auto it = map.begin();
				auto const cit = it;
				std::set<int> set = { 1, 2, 3, 4 };
				set.erase((*it).second);
				Assert::IsTrue(set.find((*cit).second) == set.end());
				set.erase((*++it).second);
				set.erase((*++it).second);
				set.erase((*++it).second);
				Assert::AreEqual(map.end(), ++it);
				Assert::IsTrue(set.empty());
				auto lamda = [&it]()
				{
					*it;
				};
				Assert::ExpectException<std::exception>(lamda);
			}
			
			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction> map =
				{
					std::make_pair(Foo(1), 1),
					std::make_pair(Foo(2), 2),
					std::make_pair(Foo(3), 3),
					std::make_pair(Foo(4), 4)
				};
				std::set<int> set = { 1, 2, 3, 4 };
				auto cit = map.cbegin();
				set.erase((*cit).second);
				set.erase((*++cit).second);
				set.erase((*++cit).second);
				set.erase((*++cit).second);
				Assert::AreEqual(map.cend(), ++cit);
				Assert::IsTrue(set.empty());
				auto lamda = [&cit]()
				{
					*cit;
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestIteratorArrow)
		{
			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction>::Iterator it;
				auto lamda = [&it]()
				{
					it->first;
				};
				Assert::ExpectException<std::exception>(lamda);

				HashMap<Foo, int, FooHashFunction, FooCompareFunction>::ConstIterator cit;
				auto c_lamda = [&cit]()
				{
					cit->first;
				};
				Assert::ExpectException<std::exception>(c_lamda);
			}

			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction> map =
				{
					std::make_pair(Foo(1), 1),
					std::make_pair(Foo(2), 2),
					std::make_pair(Foo(3), 3),
					std::make_pair(Foo(4), 4)
				};

				auto it = map.begin();
				auto const cit = it;
				std::set<int> set = { 1, 2, 3, 4 };
				set.erase(it->second);
				Assert::IsTrue(set.find(cit->second) == set.end());
				set.erase((++it)->second);
				set.erase((++it)->second);
				set.erase((++it)->second);
				Assert::AreEqual(map.end(), ++it);
				Assert::IsTrue(set.empty());
				auto lamda = [&it]()
				{
					it->first;
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction> map =
				{
					std::make_pair(Foo(1), 1),
					std::make_pair(Foo(2), 2),
					std::make_pair(Foo(3), 3),
					std::make_pair(Foo(4), 4)
				};
				std::set<int> set = { 1, 2, 3, 4 };
				auto cit = map.cbegin();
				set.erase(cit->second);
				set.erase((++cit)->second);
				set.erase((++cit)->second);
				set.erase((++cit)->second);
				Assert::AreEqual(map.cend(), ++cit);
				Assert::IsTrue(set.empty());
				auto lamda = [&cit]()
				{
					cit->first;
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestIteratorPreIncrement)
		{
			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction>::Iterator it;
				auto lamda = [&it]()
				{
					++it;
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction>::ConstIterator it;
				auto lamda = [&it]()
				{
					++it;
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction> map(2);
				map.Insert(std::make_pair(Foo(1), 1));
				map.Insert(std::make_pair(Foo(2), 2));
				map.Insert(std::make_pair(Foo(3), 3));
				map.Insert(std::make_pair(Foo(4), 4));
				auto it = map.begin();
				std::set<int> set = { 1, 2, 3, 4 };
				set.erase((*it).second);
				set.erase((*++it).second);
				set.erase((*++it).second);
				set.erase((*++it).second);
				Assert::IsTrue(set.empty());
				Assert::AreEqual(map.end(), ++it);
				Assert::AreEqual(map.end(), ++it);
			}
			
			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction> map(2);
				map.Insert(std::make_pair(Foo(1), 1));
				map.Insert(std::make_pair(Foo(2), 2));
				map.Insert(std::make_pair(Foo(3), 3));
				map.Insert(std::make_pair(Foo(4), 4));
				auto it = map.cbegin();
				std::set<int> set = { 1, 2, 3, 4 };
				set.erase((*it).second);
				set.erase((*++it).second);
				set.erase((*++it).second);
				set.erase((*++it).second);
				Assert::IsTrue(set.empty());
				Assert::AreEqual(map.cend(), ++it);
				Assert::AreEqual(map.cend(), ++it);
			}
		}

		TEST_METHOD(TestIteratorPostIncrement)
		{
			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction>::Iterator it;
				auto lamda = [&it]()
				{
					it++;
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction>::ConstIterator it;
				auto lamda = [&it]()
				{
					it++;
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction> map(2);
				map.Insert(std::make_pair(Foo(1), 1));
				map.Insert(std::make_pair(Foo(2), 2));
				map.Insert(std::make_pair(Foo(3), 3));
				map.Insert(std::make_pair(Foo(4), 4));
				auto it = map.begin();
				std::set<int> set = { 1, 2, 3, 4 };
				set.erase((*it++).second);
				set.erase((*it++).second);
				set.erase((*it++).second);
				set.erase((*it++).second);
				Assert::IsTrue(set.empty());
				Assert::AreEqual(map.end(), it++);
				Assert::AreEqual(map.end(), it);
			}

			{
				HashMap<Foo, int, FooHashFunction, FooCompareFunction> map(2);
				map.Insert(std::make_pair(Foo(1), 1));
				map.Insert(std::make_pair(Foo(2), 2));
				map.Insert(std::make_pair(Foo(3), 3));
				map.Insert(std::make_pair(Foo(4), 4));
				auto it = map.cbegin();
				std::set<int> set = { 1, 2, 3, 4 };
				set.erase((*it++).second);
				set.erase((*it++).second);
				set.erase((*it++).second);
				set.erase((*it++).second);
				Assert::IsTrue(set.empty());
				Assert::AreEqual(map.cend(), it++);
				Assert::AreEqual(map.cend(), it);
			}
		}

		TEST_METHOD(TestResize)
		{
			HashMap<Foo, int, FooHashFunction, FooCompareFunction> map(10);
			map.Insert(std::make_pair(Foo(1), 1));
			map.Insert(std::make_pair(Foo(2), 2));
			map.Insert(std::make_pair(Foo(3), 3));
			map.Insert(std::make_pair(Foo(4), 4));
			Assert::AreEqual(size_t(4), map.Size());

			map.Resize(100);
			Assert::AreEqual(size_t(4), map.Size());
			auto it = map.begin();
			std::set<int> set = { 1, 2, 3, 4 };
			for (int i = 0; i < 4; ++i)
			{
				set.erase(it++->second);
			}
			Assert::AreEqual(map.end(), it);
			Assert::IsTrue(set.empty());

			map.Resize(0);
			Assert::AreEqual(size_t(4), map.Size());
			it = map.begin();
			set = { 1, 2, 3, 4 };
			for (int i = 0; i < 4; ++i)
			{
				set.erase(it++->second);
			}
			Assert::AreEqual(map.end(), it);
			Assert::IsTrue(set.empty());

			map.Resize(10);
			Assert::AreEqual(size_t(4), map.Size());
			it = map.begin();
			set = { 1, 2, 3, 4 };
			for (int i = 0; i < 4; ++i)
			{
				set.erase(it++->second);
			}
			Assert::AreEqual(map.end(), it);
			Assert::IsTrue(set.empty());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState HashMapTest::sStartMemState;
}