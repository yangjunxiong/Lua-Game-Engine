#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Datum.h"
#include "Scope.h"

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
		return L"Scope"s;
	}
}

inline std::size_t operator "" _z(unsigned long long int x)
{
	return static_cast<size_t>(x);
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTest)
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
			Datum d;
			Assert::AreEqual(0_z, d.Size());
			Assert::AreEqual(0_z, d.Capacity());
			Assert::AreEqual(Datum::DatumType::Unknown, d.Type());
			Assert::IsTrue(d.IsInternal());
		}

		TEST_METHOD(TestCopyConstructor)
		{
			Datum d = 10;
			d.PushBack(11);
			d.PushBack(12);
			Datum d2 = d;
			Assert::AreEqual(3_z, d.Size());
			Assert::AreEqual(Datum::DatumType::Integer, d2.Type());
			Assert::AreEqual(size_t(3), d2.Size());
			Assert::AreEqual(d.Capacity(), d2.Capacity());
			Assert::IsTrue(d2.IsInternal());
			Assert::AreEqual(10, d2.AsInt());
			Assert::AreEqual(12, d2.AsInt(2));
			d.Clear();
			Assert::AreEqual(size_t(0), d.Size());
			Assert::AreEqual(size_t(3), d2.Size());

			int a[3] = { 1, 2, 3 };
			d.SetStorage(a, 3);
			Datum d3 = d;
			Assert::AreEqual(Datum::DatumType::Integer, d3.Type());
			Assert::AreEqual(size_t(3), d3.Size());
			Assert::AreEqual(3_z, d3.Capacity());
			Assert::IsFalse(d3.IsInternal());
			Assert::AreEqual(1, d3.AsInt());
			Assert::AreEqual(3, d3.AsInt(2));
		}

		TEST_METHOD(TestCopyConstructorDatumFloat)
		{
			Datum d = { 1.f, 2.f, 3.f };
			Datum d2 = d;
			Assert::AreEqual(Datum::DatumType::Float, d2.Type());
			Assert::AreEqual(3_z, d2.Size());
			Assert::AreEqual(3_z, d2.Capacity());
			Assert::AreEqual(1.f, d2.AsFloat());
			Assert::AreEqual(2.f, d2.AsFloat(1));
			Assert::AreEqual(3.f, d2.AsFloat(2));
		}

		TEST_METHOD(TestCopyConstructorDatumVector)
		{
			Datum d = { vec4(1.f), vec4(2.f), vec4(3.f) };
			Datum d2 = d;
			Assert::AreEqual(Datum::DatumType::Vector, d2.Type());
			Assert::AreEqual(3_z, d2.Size());
			Assert::AreEqual(3_z, d2.Capacity());
			Assert::AreEqual(vec4(1.f), d2.AsVector());
			Assert::AreEqual(vec4(2.f), d2.AsVector(1));
			Assert::AreEqual(vec4(3.f), d2.AsVector(2));
		}

		TEST_METHOD(TestCopyConstructorDatumMatrix)
		{
			Datum d = { mat4(1.f), mat4(2.f), mat4(3.f) };
			Datum d2 = d;
			Assert::AreEqual(Datum::DatumType::Matrix, d2.Type());
			Assert::AreEqual(3_z, d2.Size());
			Assert::AreEqual(3_z, d2.Capacity());
			Assert::AreEqual(mat4(1.f), d2.AsMatrix());
			Assert::AreEqual(mat4(2.f), d2.AsMatrix(1));
			Assert::AreEqual(mat4(3.f), d2.AsMatrix(2));
		}

		TEST_METHOD(TestCopyConstructorDatumString)
		{
			Datum d = { "1"s, "2"s, "3"s };
			Datum d2 = d;
			Assert::AreEqual(Datum::DatumType::String, d2.Type());
			Assert::AreEqual(3_z, d2.Size());
			Assert::AreEqual(3_z, d2.Capacity());
			Assert::AreEqual("1"s, d2.AsString());
			Assert::AreEqual("2"s, d2.AsString(1));
			Assert::AreEqual("3"s, d2.AsString(2));
		}

		TEST_METHOD(TestCopyConstructorDatumPointer)
		{
			Foo f1(1), f2(2), f3(3);
			Datum d = { &f1, &f2, &f3 };
			Datum d2 = d;
			Assert::AreEqual(Datum::DatumType::Pointer, d2.Type());
			Assert::AreEqual(3_z, d2.Size());
			Assert::AreEqual(3_z, d2.Capacity());
			Assert::IsTrue(d2.AsPointer() == &f1);
			Assert::IsTrue(d2.AsPointer(1) == &f2);
			Assert::IsTrue(d2.AsPointer(2) == &f3);
		}

		TEST_METHOD(TestDatumScope)
		{
			Scope s;
			Datum d = s;
			Assert::AreSame(s, d.AsTable());
			Datum d2 = d;
			Assert::AreSame(s, d2.AsTable());
			Datum d3;
			d3 = d2;
			Assert::AreSame(s, d3.AsTable());
			Assert::IsTrue(d3 == s);
			Assert::IsFalse(d3 != s);
			Assert::AreSame(s, d3[0]);

			Datum d4 = 10;
			Assert::ExpectException<std::exception>([&d4] { d4.AsTable(); });
			Assert::ExpectException<std::exception>([&d3] { d3.AsTable(1); });
			Assert::AreSame(s, d3.Find(s).AsTable());

			Datum const& cd3 = d3;
			Assert::AreSame(s, cd3.Find(s).AsTable());
		}

		TEST_METHOD(TestMoveConstructor)
		{
			Datum d = 10;
			d.PushBack(11);
			d.PushBack(12);
			Datum d2 = std::move(d);
			Assert::AreEqual(size_t(0), d.Size());
			Assert::AreEqual(Datum::DatumType::Integer, d2.Type());
			Assert::AreEqual(size_t(3), d2.Size());
			Assert::IsTrue(d2.IsInternal());
			Assert::AreEqual(10, d2.AsInt());
			Assert::AreEqual(12, d2.AsInt(2));

			int a[3] = { 1, 2, 3 };
			d.SetStorage(a, 3);
			Datum d3 = std::move(d);
			Assert::AreEqual(Datum::DatumType::Integer, d3.Type());
			Assert::AreEqual(size_t(3), d3.Size());
			Assert::AreEqual(3_z, d3.Capacity());
			Assert::IsFalse(d3.IsInternal());
			Assert::AreEqual(1, d3.AsInt());
			Assert::AreEqual(3, d3.AsInt(2));
		}

		TEST_METHOD(TestCopyContructorInt)
		{
			Datum d = 10;
			Assert::AreEqual(Datum::DatumType::Integer, d.Type());
			Assert::AreEqual(size_t(1), d.Size());
			Assert::AreEqual(10, d.AsInt());
		}

		TEST_METHOD(TestCopyConstructorFloat)
		{
			Datum d = 10.0f;
			Assert::AreEqual(Datum::DatumType::Float, d.Type());
			Assert::AreEqual(size_t(1), d.Size());
			Assert::AreEqual(10.0f, d.AsFloat());
		}

		TEST_METHOD(TestCopyConstructorVector)
		{
			Datum d = glm::vec4(1, 2, 3, 4);
			Assert::AreEqual(Datum::DatumType::Vector, d.Type());
			Assert::AreEqual(size_t(1), d.Size());
			Assert::AreEqual(glm::vec4(1, 2, 3, 4), d.AsVector());
		}

		TEST_METHOD(TestCopyConstructorMatrix)
		{
			Datum d = glm::mat4(5.0f);
			Assert::AreEqual(Datum::DatumType::Matrix, d.Type());
			Assert::AreEqual(size_t(1), d.Size());
			Assert::AreEqual(glm::mat4(5.0f), d.AsMatrix());
		}

		TEST_METHOD(TestCopyConstructorString)
		{
			Datum d = "abc";
			Assert::AreEqual(Datum::DatumType::String, d.Type());
			Assert::AreEqual(size_t(1), d.Size());
			Assert::AreEqual(std::string("abc"), d.AsString());
		}

		TEST_METHOD(TestCopyConstructorPointer)
		{
			Foo f;
			Datum d = &f;
			Assert::AreEqual(Datum::DatumType::Pointer, d.Type());
			Assert::AreEqual(size_t(1), d.Size());
			Assert::IsTrue(reinterpret_cast<RTTI*>(&f) == d.AsPointer());
		}

		TEST_METHOD(TestCopyAssignment)
		{
			// Internal to internal
			Datum d = 10;
			d.PushBack(11);
			Datum d2 = "abc";
			d2 = d;
			Assert::AreEqual(size_t(2), d.Size());
			Assert::AreEqual(Datum::DatumType::Integer, d2.Type());
			Assert::AreEqual(size_t(2), d2.Size());
			Assert::AreEqual(10, d2.AsInt());
			Assert::AreEqual(11, d2.AsInt(1));

			// External to internal
			std::string s = "abc";
			Datum d3;
			d3.SetStorage(&s, 1);
			d3 = d2;
			Assert::AreEqual(Datum::DatumType::Integer, d3.Type());
			Assert::IsTrue(d3.IsInternal());
			Assert::AreEqual(size_t(2), d3.Size());
			Assert::AreEqual(10, d3.AsInt());
			Assert::AreEqual(11, d3.AsInt(1));

			// Internal to external
			Datum d4;
			d4.SetStorage(&s, 1);
			Datum d5 = 12;
			d5 = d4;
			Assert::IsFalse(d5.IsInternal());
			Assert::AreEqual(Datum::DatumType::String, d5.Type());
			Assert::AreEqual(1_z, d5.Size());
			Assert::AreEqual("abc"s, d5.AsString());

			// External to external
			int a = 1;
			std::string ss = "aaa"s;
			Datum d6, d7;
			d6.SetStorage(&a, 1);
			d7.SetStorage(&ss, 1);
			d6 = d7;
			Assert::AreEqual(Datum::DatumType::String, d6.Type());
			Assert::IsFalse(d6.IsInternal());
			Assert::AreEqual(1_z, d6.Size());
			Assert::AreEqual("aaa"s, d6.AsString());
		}

		TEST_METHOD(TestMoveAssignment)
		{
			// Internal to internal
			Datum d = 10;
			d.PushBack(11);
			Datum d2 = "abc";
			d2 = std::move(d);
			Assert::AreEqual(0_z, d.Size());
			Assert::AreEqual(Datum::DatumType::Integer, d2.Type());
			Assert::AreEqual(size_t(2), d2.Size());
			Assert::AreEqual(10, d2.AsInt());
			Assert::AreEqual(11, d2.AsInt(1));

			// External to internal
			std::string s = "abc";
			Datum d3;
			d3.SetStorage(&s, 1);
			d3 = std::move(d2);
			Assert::AreEqual(Datum::DatumType::Integer, d3.Type());
			Assert::IsTrue(d3.IsInternal());
			Assert::AreEqual(size_t(2), d3.Size());
			Assert::AreEqual(10, d3.AsInt());
			Assert::AreEqual(11, d3.AsInt(1));

			// Internal to external
			Datum d4;
			d4.SetStorage(&s, 1);
			Datum d5 = 12;
			d5 = std::move(d4);
			Assert::IsFalse(d5.IsInternal());
			Assert::AreEqual(Datum::DatumType::String, d5.Type());
			Assert::AreEqual(1_z, d5.Size());
			Assert::AreEqual("abc"s, d5.AsString());

			// External to external
			int a = 1;
			std::string ss = "aaa"s;
			Datum d6, d7;
			d6.SetStorage(&a, 1);
			d7.SetStorage(&ss, 1);
			d6 = std::move(d7);
			Assert::AreEqual(Datum::DatumType::String, d6.Type());
			Assert::IsFalse(d6.IsInternal());
			Assert::AreEqual(1_z, d6.Size());
			Assert::AreEqual("aaa"s, d6.AsString());
		}

		TEST_METHOD(TestCopyAssignmentInt)
		{
			{
				Datum d;
				d = 10;
				Assert::AreEqual(Datum::DatumType::Integer, d.Type());
				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(10, d.AsInt());
				d = 11;
				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(11, d.AsInt());
			}

			{
				Datum d = "abc";
				auto lamda = [&d]()
				{
					d = 10;
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestCopyAssigmentFloat)
		{
			{
				Datum d;
				d = 10.0f;
				Assert::AreEqual(Datum::DatumType::Float, d.Type());
				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(10.0f, d.AsFloat());
				d = 11.0f;
				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(11.0f, d.AsFloat());
			}

			{
				Datum d = "abc";
				auto lamda = [&d]()
				{
					d = 10.0f;
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestCopyAssignmentVector)
		{
			{
				Datum d;
				d = glm::vec4(1.0f);
				Assert::AreEqual(Datum::DatumType::Vector, d.Type());
				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(glm::vec4(1.0f), d.AsVector());
				d = glm::vec4(2.0f);
				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(glm::vec4(2.0f), d.AsVector());
			}

			{
				Datum d = "abc";
				auto lamda = [&d]()
				{
					d = glm::vec4(1.0f);
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestCopyAssignmentMatrix)
		{
			{
				Datum d;
				d = glm::mat4(1.0f);
				Assert::AreEqual(Datum::DatumType::Matrix, d.Type());
				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(glm::mat4(1.0f), d.AsMatrix());
				d = glm::mat4(2.0f);
				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(glm::mat4(2.0f), d.AsMatrix());
			}

			{
				Datum d = "abc";
				auto lamda = [&d]()
				{
					d = glm::mat4();
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestCopyAssignmentString)
		{
			{
				Datum d;
				d = std::string("ggg");
				Assert::AreEqual(Datum::DatumType::String, d.Type());
				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(std::string("ggg"), d.AsString());
				d = std::string("hhh");
				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(std::string("hhh"), d.AsString());
			}

			{
				Datum d = 10;
				auto lamda = [&d]()
				{
					d = std::string("abc");
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestCopyAssignmentPointer)
		{
			{
				Datum d;
				Foo f;
				Foo f2;
				d = &f;
				Assert::AreEqual(Datum::DatumType::Pointer, d.Type());
				Assert::AreEqual(size_t(1), d.Size());
				Assert::IsTrue(d.AsPointer() == &f);
				d = &f2;
				Assert::AreEqual(size_t(1), d.Size());
				Assert::IsTrue(d.AsPointer() == &f2);
			}

			{
				Datum d = "abc";
				auto lamda = [&d]()
				{
					Foo f;
					d = &f;
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestEqualityOperator)
		{
			{
				Datum d;
				Datum d2;
				Assert::IsTrue(d == d);
				Assert::IsFalse(d != d);
				Assert::IsTrue(d == d2);
				Assert::IsFalse(d != d2);
				d2.SetType(Datum::DatumType::Integer);
				Assert::IsFalse(d == d2);
				d.SetType(Datum::DatumType::Integer);
				Assert::IsTrue(d == d2);
				d2.Reserve(100);
				Assert::IsTrue(d == d2);
			}
			
			{
				Datum d = {10, 11, 12, 13};
				Datum d2 = {10, 11, 12};
				Assert::IsTrue(d != d2);
				d2.PushBack(130);
				Assert::IsTrue(d != d2);
				d2.Set(13, 3);
				Assert::IsTrue(d == d2);
			}

			{
				Datum d = { 10.f, 11.f, 12.f, 13.f };
				Datum d2 = { 10.f, 11.f, 12.f };
				Assert::IsTrue(d != d2);
				d2.PushBack(130.f);
				Assert::IsTrue(d != d2);
				d2.Set(13.f, 3);
				Assert::IsTrue(d == d2);
			}

			{
				Datum d = { glm::vec4(1), glm::vec4(2), glm::vec4(3), glm::vec4(4) };
				Datum d2 = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				Assert::IsTrue(d != d2);
				d2.PushBack(glm::vec4(40));
				Assert::IsTrue(d != d2);
				d2.Set(vec4(4), 3);
				Assert::IsTrue(d == d2);
			}

			{
				Datum d = { glm::mat4(1), glm::mat4(2), glm::mat4(3), glm::mat4(4) };
				Datum d2 = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				Assert::IsTrue(d != d2);
				d2.PushBack(glm::mat4(40));
				Assert::IsTrue(d != d2);
				d2.Set(mat4(4), 3);
				Assert::IsTrue(d == d2);
			}

			{
				Datum d = { "a", "b", "c", "d" };
				Datum d2 = { "a", "b", "c" };
				Assert::IsTrue(d != d2);
				d2.PushBack("dd");
				Assert::IsTrue(d != d2);
				d2.Set("d"s, 3);
				Assert::IsTrue(d == d2);
			}

			{
				Foo f1(1), f2(2), f3(3), f4(4);
				Foo g1(1), g2(2), g3(3), g4(4);
				Datum d = { &f1, &f2, &f3, &f4 };
				Datum d2 = { &g1, &g2, &g3 };
				Assert::IsTrue(d != d2);
				d2.PushBack(&g4);
				Assert::IsTrue(d == d2);
				d2.Set(&f2, 2);
				Assert::IsFalse(d == d2);
				d.Set(&g2, 2);
				Assert::IsTrue(d == d2);
				d2.Set(&g3, 2);
				Assert::IsFalse(d == d2);
				d.Set(&f1, 2);
				Assert::IsFalse(d == d2);
			}
		}

		TEST_METHOD(TestEqualityOperatorInt)
		{
			Datum d;
			Assert::IsFalse(d == 10);
			d = 20;
			Assert::IsTrue(d != 10);
			d = 10;
			Assert::IsTrue(d == 10);
		}

		TEST_METHOD(TestEqualityOperatorFloat)
		{
			Datum d;
			Assert::IsFalse(d == 10.f);
			d = 20.f;
			Assert::IsTrue(d != 10.f);
			d = 10.f;
			Assert::IsTrue(d == 10.f);
		}

		TEST_METHOD(TestEqualityOperatorVector)
		{
			Datum d;
			Assert::IsFalse(d == glm::vec4(5.f));
			d = glm::vec4(10.f);
			Assert::IsTrue(d != glm::vec4(5.f));
			d = glm::vec4(5.f);
			Assert::IsTrue(d == glm::vec4(5.f));
		}

		TEST_METHOD(TestEqualityOperatorMatrix)
		{
			Datum d;
			Assert::IsFalse(d == glm::mat4(5.f));
			d = glm::mat4(10.f);
			Assert::IsTrue(d != glm::mat4(5.f));
			d = glm::mat4(5.f);
			Assert::IsTrue(d == glm::mat4(5.f));
		}

		TEST_METHOD(TestEqualityOperatorString)
		{
			Datum d;
			Assert::IsFalse(d == "abc");
			d = "def";
			Assert::IsTrue(d != "abc");
			d = "abc";
			Assert::IsTrue(d == "abc");
		}

		TEST_METHOD(TestEqualityOperatorPointer)
		{
			Foo f1(1), f2(2), f3(1);
			Datum d;
			Assert::IsFalse(d == &f1);
			d = &f2;
			Assert::IsTrue(d != &f1);
			Assert::IsTrue(d == &f2);
			Assert::IsTrue(d != nullptr);
			d = &f1;
			Assert::IsTrue(d == &f1);
			Assert::IsTrue(d == &f3);
			/*d = nullptr;
			Assert::IsTrue(d == nullptr);*/
		}

		TEST_METHOD(TestPushBackInt)
		{
			{
				Datum d;
				d.PushBack(0);
				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(Datum::DatumType::Integer, d.Type());
				Assert::AreEqual(0, d.AsInt());
				Assert::IsTrue(d == 0);
				d.PushBack(1);
				Assert::AreEqual(size_t(2), d.Size());
				Assert::AreEqual(Datum::DatumType::Integer, d.Type());
				Assert::AreEqual(1, d.AsInt(1));
				Assert::IsTrue(d == 0);
			}

			{
				Datum d;
				d = "abc";
				auto lamda = [&d]()
				{
					d.PushBack(1);
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestPushBackFloat)
		{
			{
				Datum d;
				d.PushBack(1.0f);
				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(Datum::DatumType::Float, d.Type());
				Assert::AreEqual(1.0f, d.AsFloat());
				Assert::IsTrue(d == 1.0f);
				d.PushBack(2.0f);
				Assert::AreEqual(size_t(2), d.Size());
				Assert::AreEqual(Datum::DatumType::Float, d.Type());
				Assert::AreEqual(2.0f, d.AsFloat(1));
				Assert::IsTrue(d == 1.0f);
			}
			
			{
				Datum d;
				d = "abc";
				auto lamda = [&d]()
				{
					d.PushBack(1.0f);
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestPushBackVector)
		{
			{
				glm::vec4 v;
				v.x = 1;
				v.y = 2;
				v.z = 3;
				v.w = 4;
				Datum d;
				d.PushBack(glm::vec4(v));
				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(Datum::DatumType::Vector, d.Type());
				Assert::AreEqual(glm::vec4(v), d.AsVector());
				Assert::IsTrue(d == glm::vec4(v));
				v.x = 10.0f;
				d.PushBack(glm::vec4(v));
				Assert::AreEqual(size_t(2), d.Size());
				Assert::AreEqual(Datum::DatumType::Vector, d.Type());
				Assert::AreEqual(10.0f, d.AsVector(1).x);
				Assert::AreEqual(1.0f, d.AsVector().x);
			}
			
			{
				Datum d;
				d = "abc";
				auto lamda = [&d]()
				{
					d.PushBack(glm::vec4());
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestPushBackMatrix)
		{
			{
				glm::mat4 m(1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8);
				glm::mat4 m2(4,3,2,1, 8,7,6,5, 4,3,2,1, 8,7,6,5);
				Datum d;
				d.PushBack(glm::mat4(m));
				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(Datum::DatumType::Matrix, d.Type());
				Assert::AreEqual(glm::mat4(m), d.AsMatrix());
				Assert::IsTrue(d == glm::mat4(m));
				d.PushBack(glm::mat4(m2));
				Assert::AreEqual(size_t(2), d.Size());
				Assert::AreEqual(Datum::DatumType::Matrix, d.Type());
				Assert::AreEqual(glm::mat4(m2), d.AsMatrix(1));
				Assert::IsTrue(d == glm::mat4(m));
			}
			
			{
				Datum d;
				d = "abc";
				auto lamda = [&d]()
				{
					d.PushBack(glm::mat4());
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestPushBackString)
		{
			{
				Datum d;
				d.PushBack(std::string("abc"));
				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(Datum::DatumType::String, d.Type());
				Assert::AreEqual(std::string("abc"), d.AsString());
				Assert::IsTrue(d == std::string("abc"));
				d.PushBack(std::string("cbd"));
				Assert::AreEqual(size_t(2), d.Size());
				Assert::AreEqual(Datum::DatumType::String, d.Type());
				Assert::AreEqual(std::string("cbd"), d.AsString(1));
				Assert::IsTrue(d == std::string("abc"));
				const Datum d2 = d;
				d2.AsString();
			}
			
			{
				Datum d;
				d = 1;
				auto lamda = [&d]()
				{
					d.PushBack(std::string("abc"));
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestPushBackPointer)
		{
			{
				Foo f(1);
				Foo f2(2);
				RTTI* pf = &f;
				RTTI* pf2 = &f2;
				Datum d;
				d.PushBack(pf);
				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(Datum::DatumType::Pointer, d.Type());
				Assert::IsTrue(d.AsPointer()->Is("Foo"));
				Assert::IsTrue(d == pf);
				d.PushBack(pf2);
				Assert::AreEqual(size_t(2), d.Size());
				Assert::AreEqual(Datum::DatumType::Pointer, d.Type());
				Assert::AreEqual(2, d.AsPointer(1)->As<Foo>()->Data());
				Assert::IsTrue(d == pf);
			}
			
			{
				Datum d;
				d = 1;
				auto lamda = [&d]()
				{
					Foo f(1);
					d.PushBack(&f);
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestSetType)
		{
			Datum d;
			Assert::AreEqual(Datum::DatumType::Unknown, d.Type());
			d.SetType(Datum::DatumType::Matrix);
			Assert::AreEqual(Datum::DatumType::Matrix, d.Type());
			auto lamda = [&d]()
			{
				d.SetType(Datum::DatumType::Vector);
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestSetSize)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.SetSize(1);
				};
				Assert::ExpectException<std::exception>(lamda);

				d.SetType(Datum::DatumType::String);
				Assert::AreEqual(size_t(0), d.Size());
				d.SetSize(1);
				Assert::AreEqual(size_t(1), d.Size());
				d = "a"s;
				Assert::AreEqual("a"s, d.AsString());
				d.SetSize(5);
				Assert::AreEqual(size_t(5), d.Size());
				Assert::AreEqual(string(), d.AsString(4));
				Assert::AreEqual(size_t(5), d.Capacity());
				Assert::AreEqual("a"s, d.AsString());
				d.Reserve(10);
				Assert::AreEqual(size_t(10), d.Capacity());
				d.SetSize(8);
				Assert::AreEqual(size_t(8), d.Size());
				Assert::AreEqual(size_t(10), d.Capacity());
				d.SetSize(2);
				Assert::AreEqual(size_t(2), d.Size());
				Assert::AreEqual(size_t(10), d.Capacity());
			}
			
			{
				Datum d;
				d.SetType(Datum::DatumType::Vector);
				Assert::AreEqual(size_t(0), d.Size());
				d.SetSize(1);
				Assert::AreEqual(size_t(1), d.Size());
				d = vec4(1.f);
				Assert::AreEqual(vec4(1.f), d.AsVector());
				d.SetSize(5);
				Assert::AreEqual(size_t(5), d.Size());
				Assert::AreEqual(vec4(), d.AsVector(4));
				Assert::AreEqual(size_t(5), d.Capacity());
				Assert::AreEqual(vec4(1.f), d.AsVector());
				d.Reserve(10);
				Assert::AreEqual(size_t(10), d.Capacity());
				d.SetSize(8);
				Assert::AreEqual(size_t(8), d.Size());
				Assert::AreEqual(size_t(10), d.Capacity());
				d.SetSize(2);
				Assert::AreEqual(size_t(2), d.Size());
				Assert::AreEqual(size_t(10), d.Capacity());
			}
		}

		TEST_METHOD(TestClear)
		{
			Datum d;
			d.Clear();
			Assert::AreEqual(size_t(0), d.Size());
			Assert::AreEqual(Datum::DatumType::Unknown, d.Type());

			d.PushBack("abc"s);
			d.PushBack("def"s);
			d.PushBack("a"s);
			Assert::AreEqual(size_t(3), d.Size());
			Assert::AreEqual(Datum::DatumType::String, d.Type());
			d.Clear();
			Assert::AreEqual(size_t(0), d.Size());
			Assert::AreEqual(size_t(4), d.Capacity());
			Assert::AreEqual(Datum::DatumType::String, d.Type());
			d.PushBack("aaa"s);
			Assert::AreEqual(size_t(1), d.Size());
			Assert::AreEqual(Datum::DatumType::String, d.Type());
			Assert::IsTrue(d == "aaa"s);
		}

		TEST_METHOD(TestReserve)
		{
			Datum d;
			auto lamda = [&d]()
			{
				d.Reserve(10);
			};
			Assert::ExpectException<std::exception>(lamda);

			d.SetType(Datum::DatumType::Matrix);
			Assert::AreEqual(size_t(0), d.Capacity());
			d.Reserve(10);
			Assert::AreEqual(size_t(10), d.Capacity());
			Assert::AreEqual(size_t(0), d.Size());
			d.Reserve(5);
			Assert::AreEqual(size_t(10), d.Capacity());
		}

		TEST_METHOD(TestShrinkToFit)
		{
			Datum d = { 1, 2, 3, 4, 5 };
			d.ShrinkToFit();
			Assert::AreEqual(5_z, d.Size());
			Assert::AreEqual(5_z, d.Capacity());
			d.Reserve(10);
			Assert::AreEqual(5_z, d.Size());
			Assert::AreEqual(10_z, d.Capacity());
			d.ShrinkToFit();
			Assert::AreEqual(5_z, d.Size());
			Assert::AreEqual(5_z, d.Capacity());

			d.Clear();
			Assert::AreEqual(0_z, d.Size());
			Assert::AreEqual(5_z, d.Capacity());
			d.ShrinkToFit();
			Assert::AreEqual(0_z, d.Capacity());
		}

		TEST_METHOD(TestPopBack)
		{
			Datum d;
			d.PopBack();
			Assert::AreEqual(size_t(0), d.Size());
			Assert::AreEqual(Datum::DatumType::Unknown, d.Type());

			d.PushBack("abc"s);
			d.PushBack("aa"s);
			d.PushBack("b"s);
			Assert::AreEqual(size_t(3), d.Size());
			Assert::AreEqual(Datum::DatumType::String, d.Type());
			d.PopBack();
			Assert::AreEqual(size_t(2), d.Size());
			Assert::AreEqual("aa"s, d.AsString(1));
			d.PopBack();
			Assert::AreEqual(size_t(1), d.Size());
			Assert::AreEqual("abc"s, d.AsString());
			d.PopBack();
			Assert::AreEqual(size_t(0), d.Size());
		}

		TEST_METHOD(TestSetInt)
		{
			Datum d;
			auto lamda = [&d]()
			{
				d.Set(10);
			};
			Assert::ExpectException<std::exception>(lamda);

			d.SetType(Datum::DatumType::Integer);
			d.SetSize(10);
			auto lamda2 = [&d]()
			{
				d.Set(1000, 10);
			};
			Assert::ExpectException<std::exception>(lamda2);

			Assert::AreEqual(0, d.AsInt(5));
			d.Set(1000, 5);
			Assert::AreEqual(1000, d.AsInt(5));
		}

		TEST_METHOD(TestSetFloat)
		{
			Datum d;
			auto lamda = [&d]()
			{
				d.Set(10.f);
			};
			Assert::ExpectException<std::exception>(lamda);

			d.SetType(Datum::DatumType::Float);
			d.SetSize(10);
			auto lamda2 = [&d]()
			{
				d.Set(1000.f, 10);
			};
			Assert::ExpectException<std::exception>(lamda2);

			Assert::AreEqual(0.f, d.AsFloat(5));
			d.Set(1000.f, 5);
			Assert::AreEqual(1000.f, d.AsFloat(5));
		}

		TEST_METHOD(TestSetVector)
		{
			Datum d;
			auto lamda = [&d]()
			{
				d.Set(glm::vec4(1));
			};
			Assert::ExpectException<std::exception>(lamda);

			d.SetType(Datum::DatumType::Vector);
			d.SetSize(10);
			auto lamda2 = [&d]()
			{
				d.Set(glm::vec4(1), 10);
			};
			Assert::ExpectException<std::exception>(lamda2);

			Assert::AreEqual(glm::vec4(), d.AsVector(5));
			d.Set(glm::vec4(1000.f), 5);
			Assert::AreEqual(glm::vec4(1000.f), d.AsVector(5));
		}

		TEST_METHOD(TestSetMatrix)
		{
			Datum d;
			auto lamda = [&d]()
			{
				d.Set(glm::mat4());
			};
			Assert::ExpectException<std::exception>(lamda);

			d.SetType(Datum::DatumType::Matrix);
			d.SetSize(10);
			auto lamda2 = [&d]()
			{
				d.Set(glm::mat4(), 10);
			};
			Assert::ExpectException<std::exception>(lamda2);

			Assert::AreEqual(glm::mat4(), d.AsMatrix(5));
			d.Set(glm::mat4(1000.f), 5);
			Assert::AreEqual(glm::mat4(1000.f), d.AsMatrix(5));
		}

		TEST_METHOD(TestSetString)
		{
			Datum d;
			auto lamda = [&d]()
			{
				d.Set("abc"s);
			};
			Assert::ExpectException<std::exception>(lamda);

			d.SetType(Datum::DatumType::String);
			d.SetSize(10);
			auto lamda2 = [&d]()
			{
				d.Set("bcd"s, 10);
			};
			Assert::ExpectException<std::exception>(lamda2);

			Assert::AreEqual(std::string(), d.AsString(5));
			d.Set("abc"s, 5);
			Assert::AreEqual("abc"s, d.AsString(5));
		}

		TEST_METHOD(TestSetPointer)
		{
			Datum d;
			auto lamda = [&d]()
			{
				Foo f;
				d.Set(&f);
			};
			Assert::ExpectException<std::exception>(lamda);

			d.SetType(Datum::DatumType::Pointer);
			d.SetSize(10);
			auto lamda2 = [&d]()
			{
				Foo f;
				d.Set(&f, 10);
			};
			Assert::ExpectException<std::exception>(lamda2);

			Foo f;
			Assert::IsTrue(d.AsPointer(5) == nullptr);
			d.Set(&f, 5);
			Assert::IsTrue(&f == d.AsPointer(5));
		}

		TEST_METHOD(TestAsInt)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.AsInt(1);
				};
				Assert::ExpectException<std::exception>(lamda);
				d = 1;
				Assert::AreEqual(1, d.AsInt());
				Assert::ExpectException<std::exception>(lamda);
				d.PushBack(2);
				Assert::AreEqual(2, d.AsInt(1));
			}

			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.AsInt();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				const Datum d = 1;
				Assert::AreEqual(1, d.AsInt());
				auto lamda = [&d]()
				{
					d.AsInt(1);
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestAsFloat)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.AsFloat(1);
				};
				Assert::ExpectException<std::exception>(lamda);
				d = 1.f;
				Assert::AreEqual(1.f, d.AsFloat());
				Assert::ExpectException<std::exception>(lamda);
				d.PushBack(2.f);
				Assert::AreEqual(2.f, d.AsFloat(1));
			}
			
			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.AsFloat();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				const Datum d = 1.f;
				Assert::AreEqual(1.f, d.AsFloat());
				auto lamda = [&d]()
				{
					d.AsFloat(1);
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestAsVector)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.AsVector(1);
				};
				Assert::ExpectException<std::exception>(lamda);
				d = glm::vec4(1.f);
				Assert::AreEqual(glm::vec4(1.f), d.AsVector());
				Assert::ExpectException<std::exception>(lamda);
				d.PushBack(glm::vec4(2.f));
				Assert::AreEqual(glm::vec4(2.f), d.AsVector(1));
			}
			
			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.AsVector();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				const Datum d = vec4(1.f);
				Assert::AreEqual(vec4(1.f), d.AsVector());
				auto lamda = [&d]()
				{
					d.AsVector(1);
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestAsMatrix)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.AsMatrix(1);
				};
				Assert::ExpectException<std::exception>(lamda);
				d = glm::mat4(1.f);
				Assert::AreEqual(glm::mat4(1.f), d.AsMatrix());
				Assert::ExpectException<std::exception>(lamda);
				d.PushBack(glm::mat4(2.f));
				Assert::AreEqual(glm::mat4(2.f), d.AsMatrix(1));
			}
			
			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.AsMatrix();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				const Datum d = mat4(1.f);
				Assert::AreEqual(mat4(1.f), d.AsMatrix());
				auto lamda = [&d]()
				{
					d.AsMatrix(1);
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestAsString)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.AsString(1);
				};
				Assert::ExpectException<std::exception>(lamda);
				d = "abc"s;
				Assert::AreEqual("abc"s, d.AsString());
				Assert::ExpectException<std::exception>(lamda);
				d.PushBack("def"s);
				Assert::AreEqual("def"s, d.AsString(1));
			}
			
			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.AsString();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				const Datum d = "abc"s;
				Assert::AreEqual("abc"s, d.AsString());
				auto lamda = [&d]()
				{
					d.AsString(1);
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestAsPointer)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.AsPointer(1);
				};
				Assert::ExpectException<std::exception>(lamda);

				Foo f1(1), f2(2);
				d = &f1;
				Assert::AreEqual(Foo(1), *d.AsPointer()->As<Foo>());
				Assert::ExpectException<std::exception>(lamda);
				d.PushBack(&f2);
				Assert::AreEqual(Foo(2), *d.AsPointer(1)->As<Foo>());
			}
			
			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.AsPointer();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Foo f1(1), f2(1);
				const Datum d = &f1;
				Assert::IsTrue(reinterpret_cast<RTTI*>(&f1) == d.AsPointer());
				Assert::IsTrue(d.AsPointer()->Equals(&f2));
				auto lamda = [&d]()
				{
					d.AsPointer(1);
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestFrontInt)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.Front<int32_t>();
				};
				Assert::ExpectException<std::exception>(lamda);
				d.SetType(Datum::DatumType::Integer);
				Assert::ExpectException<std::exception>(lamda);
				d = 1;
				d.PushBack(2);
				d.PushBack(3);
				Assert::AreEqual(1, d.Front<int32_t>());
			}

			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.Front<int32_t>();
				};
				Assert::ExpectException<std::exception>(lamda);
				Datum* dp = const_cast<Datum*>(&d);
				dp->SetType(Datum::DatumType::Integer);
				Assert::ExpectException<std::exception>(lamda);
				dp->PushBack(1);
				dp->PushBack(2);
				dp->PushBack(3);
				Assert::AreEqual(1, d.Front<int32_t>());
			}
		}

		TEST_METHOD(TestFrontFloat)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.Front<float>();
				};
				Assert::ExpectException<std::exception>(lamda);
				d.SetType(Datum::DatumType::Float);
				Assert::ExpectException<std::exception>(lamda);
				d = 1.f;
				d.PushBack(2.f);
				d.PushBack(3.f);
				Assert::AreEqual(1.f, d.Front<float>());
			}

			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.Front<float>();
				};
				Assert::ExpectException<std::exception>(lamda);
				Datum* dp = const_cast<Datum*>(&d);
				dp->SetType(Datum::DatumType::Float);
				Assert::ExpectException<std::exception>(lamda);
				dp->PushBack(1.f);
				dp->PushBack(2.f);
				dp->PushBack(3.f);
				Assert::AreEqual(1.f, d.Front<float>());
			}
		}

		TEST_METHOD(TestFrontVector)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.Front<vec4>();
				};
				Assert::ExpectException<std::exception>(lamda);
				d.SetType(Datum::DatumType::Vector);
				Assert::ExpectException<std::exception>(lamda);
				d = vec4(1.f);
				d.PushBack(vec4(2.f));
				d.PushBack(vec4(3.f));
				Assert::AreEqual(vec4(1.f), d.Front<vec4>());
			}

			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.Front<vec4>();
				};
				Assert::ExpectException<std::exception>(lamda);
				Datum* dp = const_cast<Datum*>(&d);
				dp->SetType(Datum::DatumType::Vector);
				Assert::ExpectException<std::exception>(lamda);
				dp->PushBack(vec4(1.f));
				dp->PushBack(vec4(2.f));
				dp->PushBack(vec4(3.f));
				Assert::AreEqual(vec4(1.f), d.Front<vec4>());
			}
		}

		TEST_METHOD(TestFrontMatrix)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.Front<mat4>();
				};
				Assert::ExpectException<std::exception>(lamda);
				d.SetType(Datum::DatumType::Matrix);
				Assert::ExpectException<std::exception>(lamda);
				d = mat4(1.f);
				d.PushBack(mat4(2.f));
				d.PushBack(mat4(3.f));
				Assert::AreEqual(mat4(1.f), d.Front<mat4>());
			}

			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.Front<mat4>();
				};
				Assert::ExpectException<std::exception>(lamda);
				Datum* dp = const_cast<Datum*>(&d);
				dp->SetType(Datum::DatumType::Matrix);
				Assert::ExpectException<std::exception>(lamda);
				dp->PushBack(mat4(1.f));
				dp->PushBack(mat4(2.f));
				dp->PushBack(mat4(3.f));
				Assert::AreEqual(mat4(1.f), d.Front<mat4>());
			}
		}

		TEST_METHOD(TestFrontString)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.Front<string>();
				};
				Assert::ExpectException<std::exception>(lamda);
				d.SetType(Datum::DatumType::String);
				Assert::ExpectException<std::exception>(lamda);
				d = "a"s;
				d.PushBack("b"s);
				d.PushBack("c"s);
				Assert::AreEqual("a"s, d.Front<string>());
			}

			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.Front<string>();
				};
				Assert::ExpectException<std::exception>(lamda);
				Datum* dp = const_cast<Datum*>(&d);
				dp->SetType(Datum::DatumType::String);
				Assert::ExpectException<std::exception>(lamda);
				dp->PushBack("a"s);
				dp->PushBack("b"s);
				dp->PushBack("c"s);
				Assert::AreEqual("a"s, d.Front<string>());
			}
		}

		TEST_METHOD(TestFrontPointer)
		{
			Foo f1(1), f2(2), f3(3);
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.Front<RTTI*>();
				};
				Assert::ExpectException<std::exception>(lamda);
				d.SetType(Datum::DatumType::Pointer);
				Assert::ExpectException<std::exception>(lamda);
				d = &f1;
				d.PushBack(&f2);
				d.PushBack(&f3);
				Assert::IsTrue(reinterpret_cast<RTTI*>(&f1) == d.Front<RTTI*>());
			}

			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.Front<RTTI*>();
				};
				Assert::ExpectException<std::exception>(lamda);
				Datum* dp = const_cast<Datum*>(&d);
				dp->SetType(Datum::DatumType::Pointer);
				Assert::ExpectException<std::exception>(lamda);
				dp->PushBack(&f1);
				dp->PushBack(&f2);
				dp->PushBack(&f3);
				Assert::IsTrue(reinterpret_cast<RTTI*>(&f1) == d.Front<RTTI*>());
			}
		}

		TEST_METHOD(TestBackInt)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.Back<int32_t>();
				};
				Assert::ExpectException<std::exception>(lamda);
				d.SetType(Datum::DatumType::Integer);
				Assert::ExpectException<std::exception>(lamda);
				d = 1;
				d.PushBack(2);
				d.PushBack(3);
				Assert::AreEqual(3, d.Back<int32_t>());
			}

			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.Back<int32_t>();
				};
				Assert::ExpectException<std::exception>(lamda);
				Datum* dp = const_cast<Datum*>(&d);
				dp->SetType(Datum::DatumType::Integer);
				Assert::ExpectException<std::exception>(lamda);
				dp->PushBack(1);
				dp->PushBack(2);
				dp->PushBack(3);
				Assert::AreEqual(3, d.Back<int32_t>());
			}
		}

		TEST_METHOD(TestBackFloat)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.Back<float>();
				};
				Assert::ExpectException<std::exception>(lamda);
				d.SetType(Datum::DatumType::Float);
				Assert::ExpectException<std::exception>(lamda);
				d = 1.f;
				d.PushBack(2.f);
				d.PushBack(3.f);
				Assert::AreEqual(3.f, d.Back<float>());
			}

			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.Back<float>();
				};
				Assert::ExpectException<std::exception>(lamda);
				Datum* dp = const_cast<Datum*>(&d);
				dp->SetType(Datum::DatumType::Float);
				Assert::ExpectException<std::exception>(lamda);
				dp->PushBack(1.f);
				dp->PushBack(2.f);
				dp->PushBack(3.f);
				Assert::AreEqual(3.f, d.Back<float>());
			}
		}

		TEST_METHOD(TestBackVector)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.Back<vec4>();
				};
				Assert::ExpectException<std::exception>(lamda);
				d.SetType(Datum::DatumType::Vector);
				Assert::ExpectException<std::exception>(lamda);
				d = vec4(1.f);
				d.PushBack(vec4(2.f));
				d.PushBack(vec4(3.f));
				Assert::AreEqual(vec4(3.f), d.Back<vec4>());
			}

			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.Back<vec4>();
				};
				Assert::ExpectException<std::exception>(lamda);
				Datum* dp = const_cast<Datum*>(&d);
				dp->SetType(Datum::DatumType::Vector);
				Assert::ExpectException<std::exception>(lamda);
				dp->PushBack(vec4(1.f));
				dp->PushBack(vec4(2.f));
				dp->PushBack(vec4(3.f));
				Assert::AreEqual(vec4(3.f), d.Back<vec4>());
			}
		}

		TEST_METHOD(TestBackMatrix)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.Back<mat4>();
				};
				Assert::ExpectException<std::exception>(lamda);
				d.SetType(Datum::DatumType::Matrix);
				Assert::ExpectException<std::exception>(lamda);
				d = mat4(1.f);
				d.PushBack(mat4(2.f));
				d.PushBack(mat4(3.f));
				Assert::AreEqual(mat4(3.f), d.Back<mat4>());
			}

			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.Back<mat4>();
				};
				Assert::ExpectException<std::exception>(lamda);
				Datum* dp = const_cast<Datum*>(&d);
				dp->SetType(Datum::DatumType::Matrix);
				Assert::ExpectException<std::exception>(lamda);
				dp->PushBack(mat4(1.f));
				dp->PushBack(mat4(2.f));
				dp->PushBack(mat4(3.f));
				Assert::AreEqual(mat4(3.f), d.Back<mat4>());
			}
		}

		TEST_METHOD(TestBackString)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.Back<string>();
				};
				Assert::ExpectException<std::exception>(lamda);
				d.SetType(Datum::DatumType::String);
				Assert::ExpectException<std::exception>(lamda);
				d = "a"s;
				d.PushBack("b"s);
				d.PushBack("c"s);
				Assert::AreEqual("c"s, d.Back<string>());
			}

			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.Back<string>();
				};
				Assert::ExpectException<std::exception>(lamda);
				Datum* dp = const_cast<Datum*>(&d);
				dp->SetType(Datum::DatumType::String);
				Assert::ExpectException<std::exception>(lamda);
				dp->PushBack("a"s);
				dp->PushBack("b"s);
				dp->PushBack("c"s);
				Assert::AreEqual("c"s, d.Back<string>());
			}
		}

		TEST_METHOD(TestBackPointer)
		{
			Foo f1(1), f2(2), f3(3);
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.Back<RTTI*>();
				};
				Assert::ExpectException<std::exception>(lamda);
				d.SetType(Datum::DatumType::Pointer);
				Assert::ExpectException<std::exception>(lamda);
				d = &f1;
				d.PushBack(&f2);
				d.PushBack(&f3);
				Assert::IsTrue(reinterpret_cast<RTTI*>(&f3) == d.Back<RTTI*>());
			}

			{
				const Datum d;
				auto lamda = [&d]()
				{
					d.Back<RTTI*>();
				};
				Assert::ExpectException<std::exception>(lamda);
				Datum* dp = const_cast<Datum*>(&d);
				dp->SetType(Datum::DatumType::Pointer);
				Assert::ExpectException<std::exception>(lamda);
				dp->PushBack(&f1);
				dp->PushBack(&f2);
				dp->PushBack(&f3);
				Assert::IsTrue(reinterpret_cast<RTTI*>(&f3) == d.Back<RTTI*>());
			}
		}

		TEST_METHOD(TestFindInt)
		{
			Datum d = { 1, 2, 3, 4, 5 };
			auto it = d.begin();
			Assert::AreEqual(d.end(), d.Find(0));
			Assert::AreEqual(it, d.Find(1));
			Assert::AreEqual(++it, d.Find(2));
			auto lamda = []()
			{
				Datum dd;
				dd.Find(1);
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestFindFloat)
		{
			Datum d = { 1.f, 2.f, 3.f, 4.f, 5.f };
			auto it = d.begin();
			Assert::AreEqual(d.end(), d.Find(0.f));
			Assert::AreEqual(it, d.Find(1.f));
			Assert::AreEqual(++it, d.Find(2.f));
			auto lamda = []()
			{
				Datum dd;
				dd.Find(1.f);
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestFindVector)
		{
			Datum d = { vec4(1.f), vec4(2.f), vec4(3.f), vec4(4.f), vec4(5.f) };
			auto it = d.begin();
			Assert::AreEqual(d.end(), d.Find(vec4(0.f)));
			Assert::AreEqual(it, d.Find(vec4(1.f)));
			Assert::AreEqual(++it, d.Find(vec4(2.f)));
			auto lamda = []()
			{
				Datum dd;
				dd.Find(vec4(1.f));
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestFindMatrix)
		{
			Datum d = { mat4(1.f), mat4(2.f), mat4(3.f), mat4(4.f), mat4(5.f) };
			auto it = d.begin();
			Assert::AreEqual(d.end(), d.Find(mat4(0.f)));
			Assert::AreEqual(it, d.Find(mat4(1.f)));
			Assert::AreEqual(++it, d.Find(mat4(2.f)));
			auto lamda = []()
			{
				Datum dd;
				dd.Find(mat4(1.f));
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestFindString)
		{
			Datum d = { "a"s, "b"s, "c"s, "d"s, "e"s };
			auto it = d.begin();
			Assert::AreEqual(d.end(), d.Find("z"s));
			Assert::AreEqual(it, d.Find("a"s));
			Assert::AreEqual(++it, d.Find("b"s));
			auto lamda = []()
			{
				Datum dd;
				dd.Find("a"s);
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestFindPointer)
		{
			Foo f1(1), f2(2), f3(3), f4(4), f11(1);
			Datum d = { &f1, &f2, &f3, &f4 };
			auto it = d.begin();
			Assert::AreEqual(d.end(), d.Find(nullptr));
			Assert::AreEqual(it, d.Find(&f1));
			Assert::AreEqual(++it, d.Find(&f2));
			Assert::AreEqual(d.begin(), d.Find(&f11));
			auto lamda = []()
			{
				Datum dd;
				dd.Find(nullptr);
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestRemoveInt)
		{
			auto lamda = []()
			{
				Datum d = "abd"s;
				d.Remove(1);
			};
			Assert::ExpectException<std::exception>(lamda);

			Datum d = { 1, 2, 3};
			Assert::IsFalse(d.Remove(0));
			Assert::AreEqual(3_z, d.Size());
			Assert::IsTrue(d.Remove(2));
			Assert::AreEqual(2_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
			Assert::AreEqual(1, d.AsInt(0));
			Assert::AreEqual(3, d.AsInt(1));

			d.Remove(1);
			d.Remove(3);
			Assert::AreEqual(0_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
		}

		TEST_METHOD(TestRemoveFloat)
		{
			auto lamda = []()
			{
				Datum d = "abd"s;
				d.Remove(1.f);
			};
			Assert::ExpectException<std::exception>(lamda);

			Datum d = { 1.f, 2.f, 3.f };
			Assert::IsFalse(d.Remove(0.f));
			Assert::AreEqual(3_z, d.Size());
			Assert::IsTrue(d.Remove(2.f));
			Assert::AreEqual(2_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
			Assert::AreEqual(1.f, d.AsFloat(0));
			Assert::AreEqual(3.f, d.AsFloat(1));

			d.Remove(1.f);
			d.Remove(3.f);
			Assert::AreEqual(0_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
		}

		TEST_METHOD(TestRemoveVector)
		{
			auto lamda = []()
			{
				Datum d = "abd"s;
				d.Remove(vec4());
			};
			Assert::ExpectException<std::exception>(lamda);

			Datum d = { vec4(1), vec4(2), vec4(3) };
			Assert::IsFalse(d.Remove(vec4(0)));
			Assert::AreEqual(3_z, d.Size());
			Assert::IsTrue(d.Remove(vec4(2)));
			Assert::AreEqual(2_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
			Assert::AreEqual(vec4(1), d.AsVector(0));
			Assert::AreEqual(vec4(3), d.AsVector(1));

			d.Remove(vec4(1));
			d.Remove(vec4(3));
			Assert::AreEqual(0_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
		}

		TEST_METHOD(TestRemoveMatrix)
		{
			auto lamda = []()
			{
				Datum d = "abd"s;
				d.Remove(mat4());
			};
			Assert::ExpectException<std::exception>(lamda);

			Datum d = { mat4(1), mat4(2), mat4(3) };
			Assert::IsFalse(d.Remove(mat4(0)));
			Assert::AreEqual(3_z, d.Size());
			Assert::IsTrue(d.Remove(mat4(2)));
			Assert::AreEqual(2_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
			Assert::AreEqual(mat4(1), d.AsMatrix(0));
			Assert::AreEqual(mat4(3), d.AsMatrix(1));

			d.Remove(mat4(1));
			d.Remove(mat4(3));
			Assert::AreEqual(0_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
		}

		TEST_METHOD(TestRemoveString)
		{
			auto lamda = []()
			{
				Datum d = 1;
				d.Remove("abd"s);
			};
			Assert::ExpectException<std::exception>(lamda);

			Datum d = { "a"s, "b"s, "c"s };
			Assert::IsFalse(d.Remove("z"s));
			Assert::AreEqual(3_z, d.Size());
			Assert::IsTrue(d.Remove("b"s));
			Assert::AreEqual(2_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
			Assert::AreEqual("a"s, d.AsString(0));
			Assert::AreEqual("c"s, d.AsString(1));

			d.Remove("a"s);
			d.Remove("c"s);
			Assert::AreEqual(0_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
		}

		TEST_METHOD(TestRemovePointer)
		{
			auto lamda = []()
			{
				Datum d = "abd"s;
				d.Remove(nullptr);
			};
			Assert::ExpectException<std::exception>(lamda);

			Foo f1(1), f2(2), f3(3);
			Datum d = { &f1, &f2, &f3 };
			Assert::IsFalse(d.Remove(nullptr));
			Assert::AreEqual(3_z, d.Size());
			Assert::IsTrue(d.Remove(&f2));
			Assert::AreEqual(2_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
			Assert::IsTrue(reinterpret_cast<RTTI*>(&f1) == d.AsPointer(0));
			Assert::IsTrue(reinterpret_cast<RTTI*>(&f3) == d.AsPointer(1));

			d.Remove(&f1);
			d.Remove(&f3);
			Assert::AreEqual(0_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
		}

		TEST_METHOD(TestRemoveAt)
		{
			{
				Datum d = { 1, 2, 3, 4, 5 };
				Assert::IsTrue(d.RemoveAt(2));
				Assert::AreEqual(4_z, d.Size());
				Assert::AreEqual(2, d.AsInt(1));
				Assert::AreEqual(4, d.AsInt(2));
				Assert::IsFalse(d.RemoveAt(5));
				Assert::AreEqual(4_z, d.Size());
				while (d.Size() > 0)
				{
					Assert::IsTrue(d.RemoveAt(0));
				}
				Assert::AreEqual(0_z, d.Size());
				Assert::AreEqual(5_z, d.Capacity());
			}
			
			{
				Datum d = { 1.f, 2.f, 3.f, 4.f, 5.f };
				Assert::IsTrue(d.RemoveAt(2));
				Assert::AreEqual(4_z, d.Size());
				Assert::AreEqual(2.f, d.AsFloat(1));
				Assert::AreEqual(4.f, d.AsFloat(2));
				Assert::IsFalse(d.RemoveAt(5));
				Assert::AreEqual(4_z, d.Size());
				while (d.Size() > 0)
				{
					Assert::IsTrue(d.RemoveAt(0));
				}
				Assert::AreEqual(0_z, d.Size());
				Assert::AreEqual(5_z, d.Capacity());
			}

			{
				Datum d = { vec4(1.f), vec4(2.f), vec4(3.f), vec4(4.f), vec4(5.f) };
				Assert::IsTrue(d.RemoveAt(2));
				Assert::AreEqual(4_z, d.Size());
				Assert::AreEqual(vec4(2.f), d.AsVector(1));
				Assert::AreEqual(vec4(4.f), d.AsVector(2));
				Assert::IsFalse(d.RemoveAt(5));
				Assert::AreEqual(4_z, d.Size());
				while (d.Size() > 0)
				{
					Assert::IsTrue(d.RemoveAt(0));
				}
				Assert::AreEqual(0_z, d.Size());
				Assert::AreEqual(5_z, d.Capacity());
			}

			{
				Datum d = { mat4(1.f), mat4(2.f), mat4(3.f), mat4(4.f), mat4(5.f) };
				Assert::IsTrue(d.RemoveAt(2));
				Assert::AreEqual(4_z, d.Size());
				Assert::AreEqual(mat4(2.f), d.AsMatrix(1));
				Assert::AreEqual(mat4(4.f), d.AsMatrix(2));
				Assert::IsFalse(d.RemoveAt(5));
				Assert::AreEqual(4_z, d.Size());
				while (d.Size() > 0)
				{
					Assert::IsTrue(d.RemoveAt(0));
				}
				Assert::AreEqual(0_z, d.Size());
				Assert::AreEqual(5_z, d.Capacity());
			}

			{
				Datum d = { "1"s, "2"s, "3"s, "4"s, "5"s };
				Assert::IsTrue(d.RemoveAt(2));
				Assert::AreEqual(4_z, d.Size());
				Assert::AreEqual("2"s, d.AsString(1));
				Assert::AreEqual("4"s, d.AsString(2));
				Assert::IsFalse(d.RemoveAt(5));
				Assert::AreEqual(4_z, d.Size());
				while (d.Size() > 0)
				{
					Assert::IsTrue(d.RemoveAt(0));
				}
				Assert::AreEqual(0_z, d.Size());
				Assert::AreEqual(5_z, d.Capacity());
			}

			{
				Foo f1(1), f2(2), f3(3), f4(4), f5(5);
				Datum d = { &f1, &f2, &f3, &f4, &f5 };
				Assert::IsTrue(d.RemoveAt(2));
				Assert::AreEqual(4_z, d.Size());
				Assert::IsTrue(&f2 == d.AsPointer(1));
				Assert::IsTrue(&f4 == d.AsPointer(2));
				Assert::IsFalse(d.RemoveAt(5));
				Assert::AreEqual(4_z, d.Size());
				while (d.Size() > 0)
				{
					Assert::IsTrue(d.RemoveAt(0));
				}
				Assert::AreEqual(0_z, d.Size());
				Assert::AreEqual(5_z, d.Capacity());
			}

			{
				Datum d;
				Assert::IsFalse(d.RemoveAt(0));
			}
		}

		TEST_METHOD(TestRemoveAtIterator)
		{
			Datum d = { 1, 2, 3, 4, 5 };
			auto it = d.begin();
			Assert::IsTrue(d.RemoveAt(it));
			Assert::AreEqual(4_z, d.Size());
			Assert::AreEqual(2, d.AsInt());
			it = d.end();
			Assert::IsFalse(d.RemoveAt(it));
			it = d.begin();
			Assert::IsTrue(d.RemoveAt(++it));
			Assert::AreEqual(3_z, d.Size());
			Assert::AreEqual(2, d.AsInt());
			Assert::AreEqual(4, d.AsInt(1));

			auto lamda = [&d]()
			{
				d.RemoveAt(Datum::Iterator());
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestToStringInt)
		{
			{
				Datum d;
				auto lamda = [&d]()
				{
					d.ToString();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = { 1, 2, 3, 4, 5 };
				Assert::AreEqual("1"s, d.ToString());
				Assert::AreEqual("2"s, d.ToString(1));
				Assert::AreEqual("3"s, d.ToString(2));
				Assert::AreEqual("4"s, d.ToString(3));
				Assert::AreEqual("5"s, d.ToString(4));
				auto lamda = [&d]()
				{
					d.ToString(5);
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestToStringFloat)
		{
			Datum d = { 1.f, 2.f, 3.f, 4.f, 5.f };
			Assert::AreEqual("1.000000"s, d.ToString());
			Assert::AreEqual("2.000000"s, d.ToString(1));
			Assert::AreEqual("3.000000"s, d.ToString(2));
			Assert::AreEqual("4.000000"s, d.ToString(3));
			Assert::AreEqual("5.000000"s, d.ToString(4));
			auto lamda = [&d]()
			{
				d.ToString(5);
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestToStringVector)
		{
			Datum d = { vec4(1), vec4(2), vec4(3) };
			Assert::AreEqual("vec4(1.000000, 1.000000, 1.000000, 1.000000)"s, d.ToString(0));
			Assert::AreEqual("vec4(2.000000, 2.000000, 2.000000, 2.000000)"s, d.ToString(1));
			Assert::AreEqual("vec4(3.000000, 3.000000, 3.000000, 3.000000)"s, d.ToString(2));
			auto lamda = [&d]()
			{
				d.ToString(5);
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestToStringMatrix)
		{
			Datum d = { mat4(1), mat4(2), mat4(3) };
			Assert::AreEqual("mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))"s, d.ToString(0));
			Assert::AreEqual("mat4x4((2.000000, 0.000000, 0.000000, 0.000000), (0.000000, 2.000000, 0.000000, 0.000000), (0.000000, 0.000000, 2.000000, 0.000000), (0.000000, 0.000000, 0.000000, 2.000000))"s, d.ToString(1));
			Assert::AreEqual("mat4x4((3.000000, 0.000000, 0.000000, 0.000000), (0.000000, 3.000000, 0.000000, 0.000000), (0.000000, 0.000000, 3.000000, 0.000000), (0.000000, 0.000000, 0.000000, 3.000000))"s, d.ToString(2));
			auto lamda = [&d]()
			{
				d.ToString(5);
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestToStringString)
		{
			Datum d = { "a"s, "b"s, "c"s };
			Assert::AreEqual("\"a\""s, d.ToString(0));
			Assert::AreEqual("\"b\""s, d.ToString(1));
			Assert::AreEqual("\"c\""s, d.ToString(2));
			auto lamda = [&d]()
			{
				d.ToString(5);
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestToStringPointer)
		{
			Foo f1(1), f2(2), f3(3);
			Datum d = { &f1, &f2, &f3 };
			Assert::AreEqual("1"s, d.ToString(0));
			Assert::AreEqual("2"s, d.ToString(1));
			Assert::AreEqual("3"s, d.ToString(2));
			auto lamda = [&d]()
			{
				d.ToString(5);
			};
			Assert::ExpectException<std::exception>(lamda);
		}

		TEST_METHOD(TestSetFromStringInt)
		{
			{
				auto lamda = []()
				{
					Datum d;
					d.SetFromString("12"s);
				};
				Assert::ExpectException<std::exception>(lamda);
			}
			
			{
				Datum d = 1;
				auto lamda = [&d]()
				{
					d.SetFromString("12"s, 1);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d;
				d.SetType(Datum::DatumType::Integer);
				d.SetSize(3);
				Assert::IsFalse(d.SetFromString("abs"s));
				Assert::AreEqual(0, d.AsInt(0));
				Assert::IsTrue(d.SetFromString("10"s));
				Assert::AreEqual(10, d.AsInt(0));
				Assert::AreEqual(3_z, d.Size());
				Assert::IsTrue(d.SetFromString("20"s, 1));
				Assert::AreEqual(20, d.AsInt(1));
				Assert::IsTrue(d.SetFromString("30"s, 2));
				Assert::AreEqual(30, d.AsInt(2));
				Assert::IsTrue(d.SetFromString("110"s));
				Assert::AreEqual(110, d.AsInt());
			}
		}

		TEST_METHOD(TestSetFromStringFloat)
		{
			{
				auto lamda = []()
				{
					Datum d;
					d.SetFromString("12.0"s);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = 1.f;
				auto lamda = [&d]()
				{
					d.SetFromString("12.0"s, 1);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d;
				d.SetType(Datum::DatumType::Float);
				d.SetSize(3);
				Assert::IsFalse(d.SetFromString("abs"s));
				Assert::AreEqual(0.f, d.AsFloat(0));
				Assert::IsTrue(d.SetFromString("10.123"s));
				Assert::AreEqual(10.123f, d.AsFloat(0));
				Assert::AreEqual(3_z, d.Size());
				Assert::IsTrue(d.SetFromString("20.123"s, 1));
				Assert::AreEqual(20.123f, d.AsFloat(1));
				Assert::IsTrue(d.SetFromString("30.123"s, 2));
				Assert::AreEqual(30.123f, d.AsFloat(2));
				Assert::IsTrue(d.SetFromString("110.11"s));
				Assert::AreEqual(110.11f, d.AsFloat());
			}
		}

		TEST_METHOD(TestSetFromStringVector)
		{
			{
				auto lamda = []()
				{
					Datum d;
					d.SetFromString("vec4(1, 1, 1, 1)"s);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = vec4();
				auto lamda = [&d]()
				{
					d.SetFromString("vec4(1, 1, 1, 1)"s, 1);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d;
				d.SetType(Datum::DatumType::Vector);
				d.SetSize(3);
				Assert::IsFalse(d.SetFromString("abs"s));
				Assert::AreEqual(vec4(), d.AsVector(0));
				Assert::IsTrue(d.SetFromString("vec4(1, 1, 1, 1)"s));
				Assert::AreEqual(vec4(1), d.AsVector(0));
				Assert::AreEqual(3_z, d.Size());
				Assert::IsTrue(d.SetFromString("vec4(2, 2, 2, 2)"s, 1));
				Assert::AreEqual(vec4(2), d.AsVector(1));
				Assert::IsTrue(d.SetFromString("vec4(3, 3, 3, 3)"s, 2));
				Assert::AreEqual(vec4(3), d.AsVector(2));
				Assert::IsTrue(d.SetFromString("vec4(1, 2, 3, 4)"s));
				Assert::AreEqual(vec4(1, 2, 3, 4), d.AsVector());
			}
		}

		TEST_METHOD(TestSetFromStringMatrix)
		{
			{
				auto lamda = []()
				{
					Datum d;
					d.SetFromString("mat4x4((1, 1, 1, 1), (2, 2, 2, 2), (3, 3, 3, 3), (4, 4, 4, 4))"s);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = mat4();
				auto lamda = [&d]()
				{
					d.SetFromString("mat4x4((1, 1, 1, 1), (2, 2, 2, 2), (3, 3, 3, 3), (4, 4, 4, 4))"s, 1);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d;
				d.SetType(Datum::DatumType::Matrix);
				d.SetSize(3);
				Assert::IsFalse(d.SetFromString("abs"s));
				Assert::AreEqual(mat4(), d.AsMatrix());
				Assert::IsTrue(d.SetFromString("mat4x4((1, 1, 1, 1), (2, 2, 2, 2), (3, 3, 3, 3), (4, 4, 4, 4))"s));
				Assert::AreEqual(mat4(1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4), d.AsMatrix(0));
				Assert::AreEqual(3_z, d.Size());
				Assert::IsTrue(d.SetFromString("mat4x4((2, 2, 2, 2), (3, 3, 3, 3), (4, 4, 4, 4), (5, 5, 5, 5))"s, 1));
				Assert::AreEqual(mat4(2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5), d.AsMatrix(1));
				Assert::IsTrue(d.SetFromString("mat4x4((2, 2, 2, 2), (3, 3, 3, 3), (4, 4, 4, 4), (10, 10, 10, 10))"s, 2));
				Assert::AreEqual(mat4(mat4(2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 10, 10, 10, 10)), d.AsMatrix(2));
				Assert::IsTrue(d.SetFromString("mat4x4((1, 0, 0, 0), (0, 1, 0, 0), (0, 0, 1, 0), (0, 0, 0, 1))"s));
				Assert::AreEqual(mat4(1), d.AsMatrix());
			}
		}

		TEST_METHOD(TestSetFromStringString)
		{
			{
				auto lamda = []()
				{
					Datum d;
					d.SetFromString("sb"s);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = string();
				auto lamda = [&d]()
				{
					d.SetFromString("sb"s, 1);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d;
				d.SetType(Datum::DatumType::String);
				d.SetSize(3);
				Assert::AreEqual(string(), d.AsString());
				Assert::IsTrue(d.SetFromString("sb"s));
				Assert::AreEqual("sb"s, d.AsString(0));
				Assert::AreEqual(3_z, d.Size());
				Assert::IsTrue(d.SetFromString("sss"s, 1));
				Assert::AreEqual("sss"s, d.AsString(1));
				Assert::IsTrue(d.SetFromString("bbb"s, 2));
				Assert::AreEqual("bbb"s, d.AsString(2));
				Assert::IsTrue(d.SetFromString("bcs"s));
				Assert::AreEqual("bcs"s, d.AsString());
			}
		}

		TEST_METHOD(TestSetStorageInt)
		{
			Datum d;
			int a[5] = { 1, 2, 3, 4, 5 };
			d.SetStorage(a, 3);
			Assert::IsFalse(d.IsInternal());
			Assert::AreEqual(3_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
			Assert::AreEqual(1, d.AsInt());
			Assert::IsTrue(a == &d.AsInt());

			d = 10;
			Assert::AreEqual(10, d.AsInt());
			d.Set(30, 2);
			Assert::AreEqual(30, d.AsInt(2));
			d.SetFromString("20"s, 1);
			Assert::AreEqual(20, d.AsInt(1));
			Assert::AreEqual(10, a[0]);
			Assert::AreEqual(20, a[1]);
			Assert::AreEqual(30, a[2]);
			Assert::AreEqual(4, a[3]);
			Assert::AreEqual(5, a[4]);

			{
				auto lamda = []()
				{
					Datum d2;
					int* b = nullptr;
					d2.SetStorage(b, 1);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d2;
				d2.SetType(Datum::DatumType::Float);
				auto lamda = [&d2, &a]()
				{
					d2.SetStorage(a, 5);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.PopBack();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.PushBack(1);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.Remove(10);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.RemoveAt(0);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.RemoveAt(d.begin());
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.Reserve(10);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.SetSize(10);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.ShrinkToFit();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			Assert::ExpectException<std::exception>([&d, &a] { d.SetStorage(a, 0); });
		}

		TEST_METHOD(TestSetStorageFloat)
		{
			Datum d;
			float a[5] = { 1.f, 2.f, 3.f, 4.f, 5.f };
			d.SetStorage(a, 3);
			Assert::IsFalse(d.IsInternal());
			Assert::AreEqual(3_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
			Assert::AreEqual(1.f, d.AsFloat());
			Assert::IsTrue(a == &d.AsFloat());

			d = 10.f;
			Assert::AreEqual(10.f, d.AsFloat());
			d.Set(30.f, 2);
			Assert::AreEqual(30.f, d.AsFloat(2));
			d.SetFromString("20.00"s, 1);
			Assert::AreEqual(20.f, d.AsFloat(1));
			Assert::AreEqual(10.f, a[0]);
			Assert::AreEqual(20.f, a[1]);
			Assert::AreEqual(30.f, a[2]);
			Assert::AreEqual(4.f, a[3]);
			Assert::AreEqual(5.f, a[4]);

			{
				Datum d2;
				d2.SetType(Datum::DatumType::Integer);
				auto lamda = [&d2, &a]()
				{
					d2.SetStorage(a, 5);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.PopBack();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.PushBack(1.f);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.Remove(10.f);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.RemoveAt(0);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.RemoveAt(d.begin());
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.Reserve(10);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.SetSize(10);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.ShrinkToFit();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			Assert::ExpectException<std::exception>([&d, &a] { d.SetStorage(a, 0); });
		}

		TEST_METHOD(TestSetStorageVector)
		{
			Datum d;
			vec4 a[5] = { vec4(1.f), vec4(2.f), vec4(3.f), vec4(4.f), vec4(5.f) };
			d.SetStorage(a, 3);
			Assert::IsFalse(d.IsInternal());
			Assert::AreEqual(3_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
			Assert::AreEqual(vec4(1.f), d.AsVector());
			Assert::IsTrue(a == &d.AsVector());

			d = vec4(10.f);
			Assert::AreEqual(vec4(10.f), d.AsVector());
			d.Set(vec4(30.f), 2);
			Assert::AreEqual(vec4(30.f), d.AsVector(2));
			d.SetFromString("vec4(20, 20, 20, 20)"s, 1);
			Assert::AreEqual(vec4(20.f), d.AsVector(1));
			Assert::AreEqual(vec4(10.f), a[0]);
			Assert::AreEqual(vec4(20.f), a[1]);
			Assert::AreEqual(vec4(30.f), a[2]);
			Assert::AreEqual(vec4(4.f), a[3]);
			Assert::AreEqual(vec4(5.f), a[4]);

			{
				Datum d2;
				d2.SetType(Datum::DatumType::Integer);
				auto lamda = [&d2, &a]()
				{
					d2.SetStorage(a, 5);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.PopBack();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.PushBack(vec4(1.f));
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.Remove(vec4(10.f));
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.RemoveAt(0);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.RemoveAt(d.begin());
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.Reserve(10);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.SetSize(10);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.ShrinkToFit();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			Assert::ExpectException<std::exception>([&d, &a] { d.SetStorage(a, 0); });
		}

		TEST_METHOD(TestSetStorageMatrix)
		{
			Datum d;
			mat4 a[5] = { mat4(1.f), mat4(2.f), mat4(3.f), mat4(4.f), mat4(5.f) };
			d.SetStorage(a, 3);
			Assert::IsFalse(d.IsInternal());
			Assert::AreEqual(3_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
			Assert::AreEqual(mat4(1.f), d.AsMatrix());
			Assert::IsTrue(a == &d.AsMatrix());

			d = mat4(10.f);
			Assert::AreEqual(mat4(10.f), d.AsMatrix());
			d.Set(mat4(30.f), 2);
			Assert::AreEqual(mat4(30.f), d.AsMatrix(2));
			d.SetFromString("mat4x4((20, 0, 0, 0), (0, 20, 0, 0), (0, 0, 20, 0), (0, 0, 0, 20))"s, 1);
			Assert::AreEqual(mat4(20.f), d.AsMatrix(1));
			Assert::AreEqual(mat4(10.f), a[0]);
			Assert::AreEqual(mat4(20.f), a[1]);
			Assert::AreEqual(mat4(30.f), a[2]);
			Assert::AreEqual(mat4(4.f), a[3]);
			Assert::AreEqual(mat4(5.f), a[4]);

			{
				Datum d2;
				d2.SetType(Datum::DatumType::Integer);
				auto lamda = [&d2, &a]()
				{
					d2.SetStorage(a, 5);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.PopBack();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.PushBack(mat4(1.f));
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.Remove(mat4(10.f));
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.RemoveAt(0);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.RemoveAt(d.begin());
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.Reserve(10);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.SetSize(10);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.ShrinkToFit();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			Assert::ExpectException<std::exception>([&d, &a] { d.SetStorage(a, 0); });
		}

		TEST_METHOD(TestSetStoragePointer)
		{
			Datum d;
			Foo foos[5] = { Foo(1), Foo(2), Foo(3), Foo(4), Foo(5) };
			RTTI* a[5] = { &foos[0], &foos[1], &foos[2], &foos[3], &foos[4] };
			d.SetStorage(a, 3);
			Assert::IsFalse(d.IsInternal());
			Assert::AreEqual(3_z, d.Size());
			Assert::AreEqual(3_z, d.Capacity());
			Assert::IsTrue(d.AsPointer()->Equals(a[0]));
			Assert::IsTrue(a == &d.AsPointer());

			d = &foos[1];
			Assert::IsTrue(&foos[1] == d.AsPointer());
			Assert::IsTrue(d.AsPointer()->Equals(&foos[1]));
			d.Set(&foos[0], 2);
			Assert::IsTrue(&foos[0] == d.AsPointer(2));
			Assert::IsTrue(d.AsPointer(2)->Equals(&foos[0]));
			Assert::IsTrue(&foos[1] == a[0]);
			Assert::IsTrue(&foos[1] == a[1]);
			Assert::IsTrue(&foos[0] == a[2]);
			Assert::IsTrue(&foos[3] == a[3]);
			Assert::IsTrue(&foos[4] == a[4]);

			{
				Datum d2;
				d2.SetType(Datum::DatumType::Integer);
				auto lamda = [&d2, &a]()
				{
					d2.SetStorage(a, 5);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.PopBack();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d, &foos]()
				{
					d.PushBack(&foos[0]);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d, &foos]()
				{
					d.Remove(&foos[1]);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.RemoveAt(0);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.RemoveAt(d.begin());
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.Reserve(10);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.SetSize(10);
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = [&d]()
				{
					d.ShrinkToFit();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			Assert::ExpectException<std::exception>([&d, &a] { d.SetStorage(a, 0); });
		}

		TEST_METHOD(TestIteratorIncrement)
		{
			Datum d = { 1.f, 2.f, 3.f };
			{
				auto it = d.begin();
				Assert::AreEqual(2.f, ++it.AsFloat());
				Assert::AreEqual(2.f, it++.AsFloat());
			}
			{
				auto it = d.cbegin();
			}

			{
				auto lamda = []()
				{
					Datum::Iterator it;
					++it;
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = []()
				{
					Datum::Iterator it;
					it++;
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = []()
				{
					Datum::ConstIterator it;
					++it;
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = []()
				{
					Datum::ConstIterator it;
					it++;
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestIteratorAsInt)
		{
			{
				auto lamda = []()
				{
					Datum::Iterator it;
					it.AsInt();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = []()
				{
					Datum d = "a"s;
					auto it = d.begin();
					it.AsInt();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				auto lamda = []()
				{
					Datum d = 1;
					auto it = d.end();
					it.AsInt();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = { 1, 2 };
				auto it = d.begin();
				Assert::AreEqual(1, it.AsInt());
				const auto cit = d.begin();
				Assert::AreEqual(1, cit.AsInt());
				auto ccit = d.cbegin();
				Assert::AreEqual(1, ccit.AsInt());
			}
		}

		TEST_METHOD(TestIteratorAsFloat)
		{
			{
				Datum d = { 1.f, 2.f, 3.f };
				auto it = d.begin();
				Assert::AreEqual(1.f, it.AsFloat());
				const auto cit = d.begin();
				Assert::AreEqual(1.f, cit.AsFloat());
				auto ccit = d.cbegin();
				Assert::AreEqual(1.f, ccit.AsFloat());
			}

			{
				Datum::Iterator it;
				auto lamda = [&it]()
				{
					it.AsFloat();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = { 1 };
				auto it = d.begin();
				auto lamda = [&it]()
				{
					it.AsFloat();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = { 1.f };
				auto it = d.begin();
				auto lamda = [&it]()
				{
					++it;
					it.AsFloat();
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestIteratorAsVector)
		{
			{
				Datum d = { vec4(1.f), vec4(2.f), vec4(3.f) };
				auto it = d.begin();
				Assert::AreEqual(vec4(1.f), it.AsVector());
				const auto cit = d.begin();
				Assert::AreEqual(vec4(1.f), cit.AsVector());
				auto ccit = d.cbegin();
				Assert::AreEqual(vec4(1.f), ccit.AsVector());
			}

			{
				Datum::Iterator it;
				auto lamda = [&it]()
				{
					it.AsVector();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = { 1 };
				auto it = d.begin();
				auto lamda = [&it]()
				{
					it.AsVector();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = { vec4(1) };
				auto it = d.begin();
				auto lamda = [&it]()
				{
					++it;
					it.AsVector();
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestIteratorAsMatrix)
		{
			{
				Datum d = { mat4(1.f), mat4(2.f), mat4(3.f) };
				auto it = d.begin();
				Assert::AreEqual(mat4(1.f), it.AsMatrix());
				const auto cit = d.begin();
				Assert::AreEqual(mat4(1.f), cit.AsMatrix());
				auto ccit = d.cbegin();
				Assert::AreEqual(mat4(1.f), ccit.AsMatrix());
			}

			{
				Datum::Iterator it;
				auto lamda = [&it]()
				{
					it.AsMatrix();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = { 1 };
				auto it = d.begin();
				auto lamda = [&it]()
				{
					it.AsMatrix();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = { mat4(1) };
				auto it = d.begin();
				auto lamda = [&it]()
				{
					++it;
					it.AsMatrix();
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestIteratorAsString)
		{
			{
				Datum d = { "a"s, "b"s, "c"s };
				auto it = d.begin();
				Assert::AreEqual("a"s, it.AsString());
				const auto cit = d.begin();
				Assert::AreEqual("a"s, cit.AsString());
				auto ccit = d.cbegin();
				Assert::AreEqual("a"s, ccit.AsString());
			}

			{
				Datum::Iterator it;
				auto lamda = [&it]()
				{
					it.AsString();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = { 1 };
				auto it = d.begin();
				auto lamda = [&it]()
				{
					it.AsString();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = { "a"s };
				auto it = d.begin();
				auto lamda = [&it]()
				{
					++it;
					it.AsString();
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestIteratorAsPointer)
		{
			Foo f1(1), f2(2), f3(3);
			{
				Datum d = { &f1, &f2, &f3 };
				auto it = d.begin();
				Assert::IsTrue(&f1 == it.AsPointer());
				const auto cit = d.begin();
				Assert::IsTrue(&f1 == cit.AsPointer());
				auto ccit = d.cbegin();
				Assert::IsTrue(&f1 == ccit.AsPointer());
			}

			{
				Datum::Iterator it;
				auto lamda = [&it]()
				{
					it.AsPointer();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = { 1 };
				auto it = d.begin();
				auto lamda = [&it]()
				{
					it.AsPointer();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = { &f1 };
				auto it = d.begin();
				auto lamda = [&it]()
				{
					++it;
					it.AsPointer();
				};
				Assert::ExpectException<std::exception>(lamda);
			}
		}

		TEST_METHOD(TestIteratorType)
		{
			{
				Datum::Iterator it;
				auto lamda = [&it]()
				{
					it.Type();
				};
				Assert::ExpectException<std::exception>(lamda);
			}

			{
				Datum d = 10;
				auto it = d.begin();
				Assert::AreEqual(Datum::DatumType::Integer, it.Type());
			}
			
			{
				Datum d = 10.f;
				auto it = d.begin();
				Assert::AreEqual(Datum::DatumType::Float, it.Type());
			}

			{
				Datum d = vec4(1);
				auto it = d.begin();
				Assert::AreEqual(Datum::DatumType::Vector, it.Type());
			}

			{
				Datum d = mat4(1);
				auto it = d.begin();
				Assert::AreEqual(Datum::DatumType::Matrix, it.Type());
			}
			
			{
				Datum d = "a"s;
				auto it = d.begin();
				Assert::AreEqual(Datum::DatumType::String, it.Type());
			}

			{
				Foo f;
				Datum d = &f;
				auto it = d.begin();
				Assert::AreEqual(Datum::DatumType::Pointer, it.Type());
			}
		}

		TEST_METHOD(TestIteratorDereference)
		{
			{
				const Datum d = { 1, 2 };
				auto cit = d.Find(2);
				Assert::IsTrue(&d.AsInt(1) == *cit);
				auto it = const_cast<Datum*>(&d)->Find(2);
				Assert::IsTrue(&d.AsInt(1) == *it);
			}

			{
				const Datum d = { 1.f, 2.f };
				auto cit = d.Find(2.f);
				Assert::IsTrue(&d.AsFloat(1) == *cit);
				auto it = const_cast<Datum*>(&d)->Find(2.f);
				Assert::IsTrue(&d.AsFloat(1) == *it);
			}

			{
				const Datum d = { vec4(1), vec4(2) };
				auto cit = d.Find(vec4(2));
				Assert::IsTrue(&d.AsVector(1) == *cit);
				auto it = const_cast<Datum*>(&d)->Find(vec4(2));
				Assert::IsTrue(&d.AsVector(1) == *it);
			}

			{
				const Datum d = { mat4(1), mat4(2) };
				auto cit = d.Find(mat4(2));
				Assert::IsTrue(&d.AsMatrix(1) == *cit);
				auto it = const_cast<Datum*>(&d)->Find(mat4(2));
				Assert::IsTrue(&d.AsMatrix(1) == *it);
			}

			{
				const Datum d = { "a"s, "b"s };
				auto cit = d.Find("b"s);
				Assert::IsTrue(&d.AsString(1) == *cit);
				auto it = const_cast<Datum*>(&d)->Find("b"s);
				Assert::IsTrue(&d.AsString(1) == *it);
			}

			{
				Foo f1(1), f2(2);
				const Datum d = { &f1, &f2 };
				auto cit = d.Find(&f2);
				Assert::IsTrue(&d.AsPointer(1) == *cit);
				auto it = const_cast<Datum*>(&d)->Find(&f2);
				Assert::IsTrue(&d.AsPointer(1) == *it);
			}
		}

		TEST_METHOD(TestIteratorItegrate)
		{
			Datum d = { 1, 2, 3, 4, 5 };
			int32_t sum = 0;
			for (const auto& item : d)
			{
				sum += *reinterpret_cast<int32_t*>(item);
			}
			Assert::AreEqual(15, sum);

			sum = 0;
			for (auto it = d.begin(); it != d.end(); ++it)
			{
				sum += it.AsInt();
			}
			Assert::AreEqual(15, sum);

			sum = 0;
			for (auto it = d.cbegin(); it != d.cend(); ++it)
			{
				sum += it.AsInt();
			}
			Assert::AreEqual(15, sum);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState DatumTest::sStartMemState;
}