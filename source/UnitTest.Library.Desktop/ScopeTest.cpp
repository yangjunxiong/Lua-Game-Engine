#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Scope.h"
#include <string>

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
	template<> inline std::wstring ToString<glm::vec4>(const glm::vec4& vec)
	{
		RETURN_WIDE_STRING(vec.x);
	}
	template<> inline std::wstring ToString<glm::mat4>(const glm::mat4& mat)
	{
		RETURN_WIDE_STRING(mat[0][0]);
	}
	template<> inline std::wstring ToString<Datum::Iterator>(const Datum::Iterator& it)
	{
		RETURN_WIDE_STRING(static_cast<int>(it.Type()));
	}
	template<> inline std::wstring ToString<Scope>(const Scope&)
	{
		return L"scope"s;
	}
	template<> inline std::wstring ToString<Scope>(Scope*)
	{
		return L"scope"s;
	}
	template<> inline std::wstring ToString<Datum>(const Datum&)
	{
		return L"datum"s;
	}
}

inline std::size_t operator "" _z(unsigned long long int x)
{
	return static_cast<size_t>(x);
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ScopeTest)
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

		const string key1 = "a";
		const string key2 = "b";
		const string key3 = "c";

		// Sample test scope
		//                Root
		//     /            |             \
		//  0"a":10,11   1"b":"abc"     2"c":2 scopes
		//                          /                   \
		//                         /                     \
		//                        /                       \
		//                     child1 ("Dummy")          child2 ("Genius")
		//                  /    |    \                     |
		//                 /     |     \                "c":vec4(1.f)
		//                /      |      \
		//               /       |       \
		//              /        |        \
		//       "a":10.f   "b":"cba"  "c":scope
		//                                |
		//                             "a":mat4(2.f)
		void FillScope(Scope& scope)
		{
			scope.Append(key1).first = 10;
			scope.Append(key1).first.PushBack(11);
			scope.Append(key2).first = "abc"s;
			Scope& child1 = scope.AppendScope(key3);
			child1.Append(key1).first = 10.f;
			child1.Append(key2).first = "cba"s;
			child1.AppendScope(key3).Append(key1).first = mat4(2.f);
			Scope& child2 = scope.AppendScope(key3);
			child2.Append(key3).first = vec4(1.f);
		}

		void Check(Scope& scope)
		{
			Assert::AreEqual(3_z, scope.Size());
			Assert::AreEqual(10, scope[key1].AsInt());
			Assert::AreEqual(11, scope[key1].AsInt(1));
			Assert::AreEqual("abc"s, scope[key2].AsString());
			Assert::AreEqual(2_z, scope[key3].Size());
			Assert::AreEqual(3_z, scope[key3][0].Size());
			Assert::AreEqual(10.f, scope[key3][0][key1].AsFloat());
			Assert::AreEqual("cba"s, scope[key3][0][key2].AsString());
			Assert::AreEqual(1_z, scope[key3][0][key3].AsTable().Size());
			Assert::AreEqual(mat4(2.f), scope[key3][0][key3][0][key1].AsMatrix());
			Assert::AreEqual(1_z, scope[key3][1].Size());
			Assert::AreEqual(vec4(1.f), scope[key3][1][0].AsVector());
		}

		TEST_METHOD(TestDefaultConstructor)
		{
			{
				Scope scope;
				Assert::AreEqual(0_z, scope.Size());
				Assert::IsTrue(scope.GetParent() == nullptr);
				Assert::IsTrue(scope.Find("a"s) == nullptr);
			}
			
			{
				Scope scope(1000);
				Assert::AreEqual(0_z, scope.Size());
				Assert::IsTrue(scope.GetParent() == nullptr);
				Assert::IsTrue(scope.Find("a"s) == nullptr);
				FillScope(scope);
				Check(scope);
			}

			{
				Scope scope(0);
				FillScope(scope);
				Check(scope);
			}
		}

		TEST_METHOD(TestCopyConstructor)
		{
			Scope s1;
			FillScope(s1);
			Scope s2 = s1;
			Check(s1);
			Check(s2);
		}

		TEST_METHOD(TestMoveConstructor)
		{
			{
				Scope s1;
				FillScope(s1);
				Scope s2 = std::move(s1);
				Assert::AreEqual(0_z, s1.Size());
				Check(s2);
			}
			
			{
				Scope s1;
				FillScope(s1);
				Scope& child = s1["c"][0];
				Scope s2 = move(child);
				Assert::AreEqual(3_z, s2.Size());
				Assert::AreEqual(10.f, s2[0].AsFloat());
				Assert::AreEqual(mat4(2.f), s2[2][0][0].AsMatrix());
				Assert::AreEqual(0_z, child.Size());
				Assert::AreSame(s1, *child.GetParent());
				child.Append("abc"s).first = vec4(2.f);
				Assert::AreEqual(1_z, child.Size());
				Assert::AreEqual(vec4(2.f), child["abc"s].AsVector());
				Assert::AreSame(child["abc"], child[0]);
			}
		}

		TEST_METHOD(TestCopyAssignment)
		{
			// Construct original scope
			Scope s1;
			FillScope(s1);

			// Construct target scope
			Scope s2(100);
			s2.AppendScope(key1).AppendScope(key2);
			Assert::AreEqual(1_z, s2.Size());
			Assert::AreEqual(1_z, s2[key1].Size());
			Assert::AreEqual(1_z, s2[key1].AsTable().Size());
			Assert::AreEqual(0_z, s2[key1].AsTable()[key2].AsTable().Size());
			Assert::AreEqual(Datum::DatumType::Table, s2[0].Type());
			Assert::AreEqual(Datum::DatumType::Table, s2[0].AsTable()[0].Type());

			// Copy
			s2 = s1;
			Check(s1);
			Check(s2);
		}

		TEST_METHOD(TestMoveAssignment)
		{
			// Construct original scope
			Scope s1;
			FillScope(s1);

			// Construct target scope
			Scope s2(100);
			s2.AppendScope(key1).AppendScope(key2);
			Assert::AreEqual(1_z, s2.Size());
			Assert::AreEqual(1_z, s2[key1].Size());
			Assert::AreEqual(1_z, s2[key1].AsTable().Size());
			Assert::AreEqual(0_z, s2[key1].AsTable()[key2].AsTable().Size());
			Assert::AreEqual(Datum::DatumType::Table, s2[0].Type());
			Assert::AreEqual(Datum::DatumType::Table, s2[0].AsTable()[0].Type());

			// Move
			s2 = std::move(s1);
			Assert::AreEqual(0_z, s1.Size());
			Check(s2);
		}

		TEST_METHOD(TestManualDelete)
		{
			Scope scope;
			FillScope(scope);
			Check(scope);
			Scope& child1 = scope[key3].AsTable();
			Assert::AreEqual(2_z, scope[key3].Size());
			delete &child1;
			Assert::AreEqual(1_z, scope[key3].Size());
			Assert::AreEqual(vec4(1.f), scope[key3].AsTable()[0].AsVector());
		}

		TEST_METHOD(TestEuqalityOperator)
		{
			{
				Scope scope1;
				Scope scope2;
				Assert::AreEqual(scope1, scope2);
			}

			{
				Scope scope;
				FillScope(scope);
				Assert::AreEqual(scope, scope);
			}

			{
				Scope scope;
				Scope scope2;
				scope2.Append(key1);
				Assert::IsTrue(scope != scope2);
			}

			{
				Scope scope1;
				Scope scope2;
				FillScope(scope1);
				FillScope(scope2);
				Check(scope1);
				Check(scope2);
				Assert::AreEqual(scope1, scope2);
				scope1[key1] = 100;
				Assert::IsFalse(scope1 == scope2);
				scope2[key1] = 100;
				Assert::AreEqual(scope1, scope2);

				scope1[key3].AsTable(1)[key3].PushBack(vec4(10.f));
				Assert::AreNotEqual(scope1, scope2);
				Assert::AreEqual(scope1[key3].AsTable(), scope2[key3].AsTable());
				Assert::AreNotEqual(scope1[key3].AsTable(1), scope2[key3].AsTable(1));
			}
		}

		TEST_METHOD(TestKeyOperator)
		{
			{
				Scope scope;
				Assert::ExpectException<std::exception>([&scope] { scope[std::string()]; });
				Datum& datum = scope[key1];
				Assert::AreEqual(1_z, scope.Size());
				datum = 10;
				Assert::IsTrue(scope[key1] == 10);
				Assert::AreEqual(10, scope[0].AsInt());

				scope[key2] = "abc"s;
				Assert::AreEqual(2_z, scope.Size());
				Assert::IsTrue(scope[key2] == "abc"s);
				Assert::AreEqual("abc"s, scope[1].AsString());

				Datum& datum2 = scope[key2];
				Assert::AreEqual(2_z, scope.Size());
				Assert::AreEqual(datum2, scope[1]);
				Assert::AreEqual("abc"s, scope[1].AsString());
				Assert::AreEqual("abc"s, scope[key2].AsString());
			}

			{
				const Scope scope;
				FillScope(*const_cast<Scope*>(&scope));
				Assert::AreEqual(10, scope[key1].AsInt());
				Assert::AreEqual(vec4(1.f), scope[key3].AsTable(1)[0].AsVector());
				Assert::ExpectException<std::exception>([&scope] { scope["abc"s]; });
				Assert::ExpectException<std::exception>([&scope] { scope[std::string()]; });
			}
		}

		TEST_METHOD(TestIndexOperator)
		{
			const Scope scope;
			FillScope(*const_cast<Scope*>(&scope));
			Assert::ExpectException<std::exception>([&scope] { scope[4]; });
			Assert::AreEqual(10, scope[0].AsInt());
			Assert::AreEqual("abc"s, scope[1].AsString());
			Assert::AreEqual("cba"s, scope[2].AsTable()[1].AsString());
			Assert::AreEqual(vec4(1.f), scope[2].AsTable(1)[0].AsVector());
		}

		TEST_METHOD(TestAppend)
		{
			Scope scope;
			Assert::ExpectException<std::exception>([&scope] { scope.Append(std::string()); });
			auto [datum, inserted] = scope.Append(key1);
			Assert::IsTrue(inserted);
			Assert::AreEqual(1_z, scope.Size());
			datum = 10;
			Assert::IsTrue(scope[key1] == 10);
			Assert::AreEqual(10, scope[0].AsInt());

			auto[datum2, inserted2] = scope.Append(key2);
			Assert::IsTrue(inserted2);
			Assert::AreEqual(2_z, scope.Size());
			datum2 = "abc"s;
			Assert::IsTrue(scope[key2] == "abc"s);
			Assert::AreEqual("abc"s, scope[1].AsString());

			auto[_, inserted3] = scope.Append(key2);
			Assert::IsFalse(inserted3);
			Assert::AreEqual(2_z, scope.Size());
			Assert::AreEqual("abc"s, scope[1].AsString());
			Assert::AreEqual("abc"s, scope[key2].AsString());
		}

		TEST_METHOD(TestAppendScope)
		{
			Scope scope;
			Assert::ExpectException<std::exception>([&scope] { scope.AppendScope(std::string()); });
			scope.Append(key1).first = 10;
			scope.Append(key2);
			Assert::ExpectException<std::exception>([&scope, this] { scope.AppendScope(key1); });
			Scope& child1 = scope.AppendScope(key2);
			Scope& child2 = scope.AppendScope(key2);
			Assert::AreEqual(&scope, child1.GetParent());
			Assert::AreEqual(&scope, child2.GetParent());
			child1.Append(key1).first = 10.f;
			child2.Append(key1).first = vec4(1.f);
			Assert::AreEqual(10.f, scope[key2].AsTable()[key1].AsFloat());
			Assert::AreEqual(vec4(1.f), scope[key2].AsTable(1)[key1].AsVector());

			Scope& child3 = child1.AppendScope(key2);
			Assert::AreEqual(&child1, child3.GetParent());
			Assert::AreSame(child3, scope[key2].AsTable()[key2].AsTable());

			Scope& child4 = scope.AppendScope(key3);
			Assert::AreEqual(&scope, child4.GetParent());
			Assert::AreSame(child4, scope[key3].AsTable());
		}

		TEST_METHOD(TestAdopt)
		{
			Scope scope;
			Scope* child1 = new Scope();
			Scope* child2 = new Scope();
			Scope* child3 = new Scope();

			// Adopt standalone child
			Assert::AreEqual(0_z, scope.Size());
			scope.Adopt(*child1, key1);
			Assert::AreEqual(1_z, scope.Size());
			Assert::AreEqual(1_z, scope[key1].Size());
			Assert::AreEqual(child1, &scope[key1].AsTable());
			Assert::AreEqual(&scope, child1->GetParent());

			// Child adopts another standalond child
			Assert::AreEqual(0_z, child1->Size());
			child1->Adopt(*child2, key1);
			Assert::AreEqual(1_z, child1->Size());
			Assert::AreEqual(1_z, (*child1)[key1].Size());
			Assert::AreEqual(child2, &(*child1)[key1].AsTable());
			Assert::AreEqual(child1, child2->GetParent());
			Assert::AreEqual(child2, &scope[key1].AsTable()[key1].AsTable());

			// Exception cases
			scope.Append(key3).first = 10;
			Assert::ExpectException<std::exception>([&scope, &child3, this] { scope.Adopt(*child3, key3); });
			Assert::ExpectException<std::exception>([&scope, &child3] { scope.Adopt(*child3, string()); });
			Assert::ExpectException<std::exception>([&scope, this] { scope.Adopt(scope, key1); });
			Assert::ExpectException<std::exception>([&scope, &child2, this] { child2->Adopt(scope, key1); });
			Assert::ExpectException<std::exception>([&child1, &child2, this] { child2->Adopt(*child1, key1); });
			delete child3;

			// Adopt child from another scope
			scope.Adopt(*child2, key1);
			Assert::AreEqual(2_z, scope.Size());
			Assert::AreEqual(2_z, scope[key1].Size());
			Assert::AreEqual(child1, &scope[key1].AsTable());
			Assert::AreEqual(child2, &scope[key1].AsTable(1));
			Assert::AreEqual(&scope, child2->GetParent());
			Assert::AreEqual(0_z, (*child1)[key1].Size());

			// Adopt child within same scope
			scope.Adopt(*child2, key2);
			Assert::AreEqual(3_z, scope.Size());
			Assert::AreEqual(1_z, scope[key1].Size());
			Assert::AreEqual(1_z, scope[key2].Size());
			Assert::AreEqual(child1, &scope[key1].AsTable());
			Assert::AreEqual(child2, &scope[key2].AsTable());
			Assert::AreEqual(&scope, child2->GetParent());

			// Adopt child same place
			scope.Adopt(*child2, key2);
			Assert::AreEqual(3_z, scope.Size());
			Assert::AreEqual(1_z, scope[key1].Size());
			Assert::AreEqual(1_z, scope[key2].Size());
			Assert::AreEqual(child1, &scope[key1].AsTable());
			Assert::AreEqual(child2, &scope[key2].AsTable());
			Assert::AreEqual(&scope, child2->GetParent());
		}

		TEST_METHOD(TestOrphan)
		{
			Scope scope;
			FillScope(scope);
			Scope& child1 = scope[key3].AsTable();
			Scope& child2 = scope[key3].AsTable(1);
			Scope& child3 = child1[key3].AsTable();

			Assert::AreEqual(&scope, child1.GetParent());
			Assert::AreEqual(&scope, child2.GetParent());
			Assert::AreEqual(&child1, child3.GetParent());
			Assert::AreEqual(3_z, child1.Size());
			Assert::AreEqual(2_z, scope[key3].Size());
			scope.Orphan(child1);
			Assert::IsTrue(child1.GetParent() == nullptr);
			Assert::AreEqual(3_z, child1.Size());
			Assert::AreEqual(1_z, scope[key3].Size());

			child1.Orphan(child3);
			Assert::AreEqual(3_z, child1.Size());
			Assert::AreEqual(0_z, child1[key3].Size());
			Assert::IsTrue(child3.GetParent() == nullptr);

			delete &child3;
			delete &child1;
		}

		TEST_METHOD(TestFind)
		{
			Scope scope;
			FillScope(scope);
			Assert::IsTrue(scope.Find(string()) == nullptr);
			Assert::IsTrue(scope.Find("d"s) == nullptr);
			Assert::AreEqual(10, scope.Find(key1)->AsInt());
			Assert::AreEqual(2_z, scope.Find(key3)->Size());

			const Scope scope2 = scope;
			Assert::IsTrue(scope2.Find(string()) == nullptr);
			Assert::AreEqual(10.f, scope2.Find(key3)->AsTable().Find(key1)->AsFloat());
			Assert::AreEqual(mat4(2.f), scope2.Find(key3)->AsTable().Find(key3)->AsTable().Find(key1)->AsMatrix());
		}

		TEST_METHOD(TestSearch)
		{
			Scope root;
			FillScope(root);
			Scope& child1 = root[key3].AsTable();
			Scope& child2 = root[key3].AsTable(1);
			Scope& child3 = child1[key3].AsTable();
			child1.Append("Name").first = "Dummy";
			child2.Append("Name").first = "Genius";

			{
				auto[datum, scope] = child3.Search(key1);
				Assert::AreEqual(mat4(2.f), datum->AsMatrix());
				Assert::AreEqual(&child3, scope);
			}
			{
				auto[datum, scope] = child3.Search(key2);
				Assert::AreEqual("cba"s, datum->AsString());
				Assert::AreEqual(&child1, scope);
			}
			{
				auto[datum, scope] = child3.Search(key3);
				Assert::AreSame(child3, datum->AsTable());
				Assert::AreEqual(&child1, scope);
			}
			{
				auto[datum, scope] = child1.Search(key1);
				Assert::AreEqual(10.f, datum->AsFloat());
				Assert::AreEqual(&child1, scope);
			}
			{
				auto[datum, scope] = child1.Search(key2);
				Assert::AreEqual("cba"s, datum->AsString());
				Assert::AreEqual(&child1, scope);
			}
			{
				auto[datum, scope] = child1.Search(key3);
				Assert::AreSame(child3, datum->AsTable());
				Assert::AreEqual(&child1, scope);
			}
			{
				auto[datum, scope] = child2.Search(key1);
				Assert::AreEqual(11, datum->AsInt(1));
				Assert::AreEqual(&root, scope);
			}
			{
				auto[datum, scope] = child2.Search(key2);
				Assert::AreEqual("abc"s, datum->AsString());
				Assert::AreEqual(&root, scope);
			}
			{
				auto[datum, scope] = child2.Search(key3);
				Assert::AreEqual(vec4(1.f), datum->AsVector());
				Assert::AreEqual(&child2, scope);
			}
			{
				auto[datum, scope] = child3.Search("bb"s);
				Assert::IsTrue(datum == nullptr);
				Assert::IsTrue(scope == nullptr);
			}
			{
				auto[datum, scope] = root.Search("bb"s);
				Assert::IsTrue(datum == nullptr);
				Assert::IsTrue(scope == nullptr);
			}
			{
				const Scope& c = child2;
				auto[datum, scope] = c.Search(key3);
				Assert::AreEqual(vec4(1.f), datum->AsVector());
				Assert::IsTrue(&c == scope);
			}
			{
				auto[datum, scope] = root.Search("/a"s);
				Assert::AreEqual(10, datum->AsInt(0));
				Assert::IsTrue(scope == &root);
			}
			{
				auto[datum, scope] = root.Search("/././././././b"s);
				Assert::AreEqual("abc"s, datum->AsString(0));
				Assert::IsTrue(scope == &root);
			}
			{
				auto[datum, scope] = child3.Search("../a"s);
				Assert::AreEqual(10.f, datum->AsFloat(0));
				Assert::IsTrue(scope == &child1);
			}
			{
				auto[datum, scope] = child2.Search("../c[0]/c[0]/a"s);
				Assert::AreEqual(mat4(2.f), datum->AsMatrix());
				Assert::IsTrue(scope == &child3);
			}
			{
				auto[datum, scope] = child3.Search("/c[Dummy]/c[0]/a"s);
				Assert::AreEqual(mat4(2.f), datum->AsMatrix());
				Assert::IsTrue(scope == &child3);
			}
			{
				auto[datum, scope] = child3.Search("/c[Dummy]/c[0]/../../c[Genius]/c"s);
				Assert::AreEqual(vec4(1.f), datum->AsVector());
				Assert::IsTrue(scope == &child2);
			}
		}

		TEST_METHOD(TestFindScope)
		{
			Scope scope;
			FillScope(scope);
			Scope& child1 = scope[key3].AsTable();
			Scope& child2 = scope[key3].AsTable(1);
			Scope& child3 = child1[key3].AsTable();
			{
				auto[key, datum] = scope.FindScope(&scope);
				Assert::AreEqual(string(), key);
				Assert::IsTrue(datum == nullptr);
			}
			{
				auto[key, datum] = scope.FindScope(&child1);
				Assert::AreEqual(key3, key);
				Assert::AreSame(*datum, scope[key3]);
			}
			{
				auto[key, datum] = scope.FindScope(&child2);
				Assert::AreEqual(key3, key);
				Assert::AreSame(*datum, scope[key3]);
			}
			{
				auto[key, datum] = scope.FindScope(&child3);
				Assert::AreEqual(string(), key);
				Assert::IsTrue(datum == nullptr);
			}
			{
				auto[key, datum] = child1.FindScope(&child3);
				Assert::AreEqual(key3, key);
				Assert::AreSame(*datum, child1[key3]);
			}
			{
				const Scope& c = scope;
				auto[key, datum] = c.FindScope(&child1);
				Assert::AreEqual(key3, key);
				Assert::AreSame(*datum, c[key3]);
			}
			{
				auto[key, datum] = child1.FindScope(nullptr);
				Assert::AreEqual(string(), key);
				Assert::IsTrue(datum == nullptr);
			}
		}

		TEST_METHOD(TestClear)
		{
			Scope scope;
			FillScope(scope);
			Scope& child1 = scope[key3].AsTable();
			child1.Clear();
			Assert::AreEqual(0_z, child1.Size());
			Assert::AreEqual(2_z, scope[key3].Size());
			scope.Clear();
			Assert::AreEqual(0_z, scope.Size());
			scope.Append(key1).first = 10;
			scope.AppendScope(key2);
			Assert::AreEqual(2_z, scope.Size());
			scope.Clear();
			Assert::AreEqual(0_z, scope.Size());
		}

		TEST_METHOD(TestToString)
		{
			Scope scope;
			scope.Append(key1).first = 10;
			Assert::AreEqual("{\n\"a\": [10]\n}"s, scope.ToString());
			scope[0].PushBack(11);
			Assert::AreEqual("{\n\"a\": [10, 11]\n}"s, scope.ToString());
			scope.AppendScope(key2).Append(key1).first = "abc"s;
			Assert::AreEqual("{\n\"a\": [10, 11],\n\"b\": [{\n\"a\": [\"abc\"]\n}]\n}"s, scope.ToString());
		}

		TEST_METHOD(TestRTTI)
		{
			Scope scope;
			RTTI* rtti = &scope;
			Assert::IsTrue(rtti->Is("Scope"));
			Assert::IsTrue(rtti->Is(Scope::TypeIdClass()));
			Assert::IsFalse(rtti->Is("Foo"));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->QueryInterface(Scope::TypeIdClass()) != nullptr);
			Assert::IsTrue(rtti->QueryInterface(Foo::TypeIdClass()) == nullptr);

			class Bar : public RTTI
			{
				std::uint64_t TypeIdInstance() const
				{
					return 0;
				}
			};
			Bar bar;
			Bar bar2;
			Assert::AreEqual("RTTI"s, bar.ToString());
			Assert::IsTrue(bar.Equals(&bar));
			Assert::IsFalse(bar.Equals(&bar2));
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ScopeTest::sStartMemState;
}