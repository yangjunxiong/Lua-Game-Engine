#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedFoo.h"
#include "Attributed.h"

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
	template<> inline std::wstring ToString(RTTI*)
	{
		return L"RTTI"s;
	}
	template<> inline std::wstring ToString(const AttributedFoo&)
	{
		return L"AttributedFoo"s;
	}
}

inline std::size_t operator "" _z(unsigned long long int x)
{
	return static_cast<size_t>(x);
}

class Bar : public Attributed
{
public:
	Bar() : Attributed(0) {}
	int mInt = 10;
};

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(AttributedTest)
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

		inline size_t DefaultSize(RTTI::IdType type)
		{
			return AttributedTypeManager::GetSignature(type)->Size() + 1;
		}

		void Check(const AttributedFoo& foo)
		{
			Assert::IsTrue(foo.Is("AttributedFoo"));
			Assert::IsTrue(foo.Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(const_cast<AttributedFoo*>(&foo)->QueryInterface(AttributedFoo::TypeIdClass()) != nullptr);

			Assert::IsTrue(foo[0].AsPointer() == reinterpret_cast<RTTI*>(const_cast<AttributedFoo*>(&foo)));
			Assert::IsTrue(foo.Find("this")->AsPointer() == reinterpret_cast<RTTI*>(const_cast<AttributedFoo*>(&foo)));
			Assert::IsTrue(foo.IsAttribute("this"));
			Assert::IsTrue(foo.IsAttribute("Int"));
			Assert::IsTrue(foo.IsAttribute("Float"));
			Assert::IsTrue(foo.IsAttribute("Vector"));
			Assert::IsTrue(foo.IsAttribute("Matrix"));
			Assert::IsTrue(foo.IsAttribute("String"));
			Assert::IsTrue(foo.IsAttribute("Pointer"));
			Assert::IsTrue(foo.IsAttribute("IntArray"));
			Assert::IsTrue(foo.IsAttribute("FloatArray"));
			Assert::IsTrue(foo.IsAttribute("VectorArray"));
			Assert::IsTrue(foo.IsAttribute("MatrixArray"));
			Assert::IsTrue(foo.IsAttribute("StringArray"));
			Assert::IsTrue(foo.IsAttribute("PointerArray"));
			Assert::IsTrue(foo.IsAttribute("InternalInt"));
			Assert::IsTrue(foo.IsAttribute("InternalFloat"));
			Assert::IsTrue(foo.IsAttribute("InternalVector"));
			Assert::IsTrue(foo.IsAttribute("InternalMatrix"));
			Assert::IsTrue(foo.IsAttribute("InternalString"));
			Assert::IsTrue(foo.IsAttribute("InternalPointer"));
			Assert::IsTrue(foo.IsAttribute("InternalIntArray"));
			Assert::IsTrue(foo.IsAttribute("InternalFloatArray"));
			Assert::IsTrue(foo.IsAttribute("InternalVectorArray"));
			Assert::IsTrue(foo.IsAttribute("InternalMatrixArray"));
			Assert::IsTrue(foo.IsAttribute("InternalStringArray"));
			Assert::IsTrue(foo.IsAttribute("InternalPointerArray"));
			Assert::IsTrue(foo.IsAttribute("InternalScopeArray"));
			Assert::IsTrue(foo.IsAttribute("InternalScopeEmpty"));

			Assert::IsTrue(foo.IsPrescribedAttribute("this"));
			Assert::IsTrue(foo.IsPrescribedAttribute("Int"));
			Assert::IsTrue(foo.IsPrescribedAttribute("Float"));
			Assert::IsTrue(foo.IsPrescribedAttribute("Vector"));
			Assert::IsTrue(foo.IsPrescribedAttribute("Matrix"));
			Assert::IsTrue(foo.IsPrescribedAttribute("String"));
			Assert::IsTrue(foo.IsPrescribedAttribute("Pointer"));
			Assert::IsTrue(foo.IsPrescribedAttribute("IntArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("FloatArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("VectorArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("MatrixArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("StringArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("PointerArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("InternalInt"));
			Assert::IsTrue(foo.IsPrescribedAttribute("InternalFloat"));
			Assert::IsTrue(foo.IsPrescribedAttribute("InternalVector"));
			Assert::IsTrue(foo.IsPrescribedAttribute("InternalMatrix"));
			Assert::IsTrue(foo.IsPrescribedAttribute("InternalString"));
			Assert::IsTrue(foo.IsPrescribedAttribute("InternalPointer"));
			Assert::IsTrue(foo.IsPrescribedAttribute("InternalIntArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("InternalFloatArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("InternalVectorArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("InternalMatrixArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("InternalStringArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("InternalPointerArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("InternalScopeArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("InternalScopeEmpty"));

			// External storage address check
			Assert::AreSame(foo.mInt, foo.Find("Int")->AsInt());
			Assert::AreSame(foo.mInt, foo[1].AsInt());
			Assert::AreSame(foo.mFloat, foo.Find("Float")->AsFloat());
			Assert::AreSame(foo.mFloat, foo[2].AsFloat());
			Assert::AreSame(foo.mVector, foo.Find("Vector")->AsVector());
			Assert::AreSame(foo.mVector, foo[3].AsVector());
			Assert::AreSame(foo.mMatrix, foo.Find("Matrix")->AsMatrix());
			Assert::AreSame(foo.mMatrix, foo[4].AsMatrix());
			Assert::AreSame(foo.mString, foo.Find("String")->AsString());
			Assert::AreSame(foo.mString, foo[5].AsString());
			Assert::AreSame(foo.mPointer, foo.Find("Pointer")->AsPointer());
			Assert::AreSame(foo.mPointer, foo[6].AsPointer());
			for (size_t i = 0; i < AttributedFoo::ArraySize; ++i)
			{
				Assert::AreSame(foo.mIntArray[i], foo.Find("IntArray")->AsInt(i));
				Assert::AreSame(foo.mFloatArray[i], foo.Find("FloatArray")->AsFloat(i));
				Assert::AreSame(foo.mVectorArray[i], foo.Find("VectorArray")->AsVector(i));
				Assert::AreSame(foo.mMatrixArray[i], foo.Find("MatrixArray")->AsMatrix(i));
				Assert::AreSame(foo.mStringArray[i], foo.Find("StringArray")->AsString(i));
				Assert::AreSame(foo.mPointerArray[i], foo.Find("PointerArray")->AsPointer(i));
				Assert::AreSame(foo.mIntArray[i], foo[7].AsInt(i));
				Assert::AreSame(foo.mFloatArray[i], foo[8].AsFloat(i));
				Assert::AreSame(foo.mVectorArray[i], foo[9].AsVector(i));
				Assert::AreSame(foo.mMatrixArray[i], foo[10].AsMatrix(i));
				Assert::AreSame(foo.mStringArray[i], foo[11].AsString(i));
				Assert::AreSame(foo.mPointerArray[i], foo[12].AsPointer(i));
			}

			// Internal storage check
			Assert::AreEqual(1_z, foo.Find("InternalInt")->Size());
			Assert::AreEqual(Datum::DatumType::Integer, foo.Find("InternalInt")->Type());
			Assert::AreSame(foo["InternalInt"], foo[13]);
			Assert::AreEqual(1_z, foo.Find("InternalFloat")->Size());
			Assert::AreEqual(Datum::DatumType::Float, foo.Find("InternalFloat")->Type());
			Assert::AreSame(foo["InternalFloat"], foo[14]);
			Assert::AreEqual(1_z, foo.Find("InternalVector")->Size());
			Assert::AreEqual(Datum::DatumType::Vector, foo.Find("InternalVector")->Type());
			Assert::AreSame(foo["InternalVector"], foo[15]);
			Assert::AreEqual(1_z, foo.Find("InternalMatrix")->Size());
			Assert::AreEqual(Datum::DatumType::Matrix, foo.Find("InternalMatrix")->Type());
			Assert::AreSame(foo["InternalMatrix"], foo[16]);
			Assert::AreEqual(1_z, foo.Find("InternalString")->Size());
			Assert::AreEqual(Datum::DatumType::String, foo.Find("InternalString")->Type());
			Assert::AreSame(foo["InternalString"], foo[17]);
			Assert::AreEqual(1_z, foo.Find("InternalPointer")->Size());
			Assert::AreEqual(Datum::DatumType::Pointer, foo.Find("InternalPointer")->Type());
			Assert::AreSame(foo["InternalPointer"], foo[18]);

			Assert::AreEqual(AttributedFoo::ArraySize, foo.Find("InternalIntArray")->Size());
			Assert::AreEqual(Datum::DatumType::Integer, foo.Find("InternalIntArray")->Type());
			Assert::AreSame(foo["InternalIntArray"], foo[19]);
			Assert::AreEqual(AttributedFoo::ArraySize, foo.Find("InternalFloatArray")->Size());
			Assert::AreEqual(Datum::DatumType::Float, foo.Find("InternalFloatArray")->Type());
			Assert::AreSame(foo["InternalFloatArray"], foo[20]);
			Assert::AreEqual(AttributedFoo::ArraySize, foo.Find("InternalVectorArray")->Size());
			Assert::AreEqual(Datum::DatumType::Vector, foo.Find("InternalVectorArray")->Type());
			Assert::AreSame(foo["InternalVectorArray"], foo[21]);
			Assert::AreEqual(AttributedFoo::ArraySize, foo.Find("InternalMatrixArray")->Size());
			Assert::AreEqual(Datum::DatumType::Matrix, foo.Find("InternalMatrixArray")->Type());
			Assert::AreSame(foo["InternalMatrixArray"], foo[22]);
			Assert::AreEqual(AttributedFoo::ArraySize, foo.Find("InternalStringArray")->Size());
			Assert::AreEqual(Datum::DatumType::String, foo.Find("InternalStringArray")->Type());
			Assert::AreSame(foo["InternalStringArray"], foo[23]);
			Assert::AreEqual(AttributedFoo::ArraySize, foo.Find("InternalPointerArray")->Size());
			Assert::AreEqual(Datum::DatumType::Pointer, foo.Find("InternalPointerArray")->Type());
			Assert::AreSame(foo["InternalPointerArray"], foo[24]);
			Assert::AreEqual(AttributedFoo::ArraySize, foo.Find("InternalScopeArray")->Size());
			Assert::AreEqual(Datum::DatumType::Table, foo.Find("InternalScopeArray")->Type());
			Assert::AreSame(foo["InternalScopeArray"], foo[25]);
			Assert::AreEqual(0_z, foo.Find("InternalScopeEmpty")->Size());
			Assert::AreEqual(Datum::DatumType::Table, foo.Find("InternalScopeEmpty")->Type());
			Assert::AreSame(foo["InternalScopeEmpty"], foo[26]);
		}

		TEST_METHOD(TestDefaultConstructor)
		{
			// Normal Foo
			AttributedFoo foo(1);
			Assert::AreEqual(DefaultSize(AttributedFoo::TypeIdClass()), foo.Size());
			Check(foo);
			AttributedFoo foo2(1);
			Assert::AreEqual(foo, foo);
			Assert::AreEqual(foo, foo2);
			foo2["Int"] = 10;
			Assert::IsTrue(foo != foo2);

			// Type without signature map
			Bar bar;
			Assert::AreEqual(1_z, bar.Size());
			Assert::IsTrue(bar.IsAttribute("this"));
			Assert::IsTrue(bar.IsPrescribedAttribute("this"));
			Assert::IsTrue(bar.Find("this")->AsPointer() == reinterpret_cast<RTTI*>(&bar));
		}

		TEST_METHOD(TestPrescribedAttribute)
		{
			AttributedFoo foo(1);
			Check(foo);

			// External storage
			foo.Find("Int")->Set(10);
			foo.Find("IntArray")->Set(300, 2);
			foo.Find("Pointer")->Set(reinterpret_cast<RTTI*>(&foo));
			Assert::AreEqual(10, foo.Find("Int")->AsInt());
			Assert::AreEqual(300, foo.Find("IntArray")->AsInt(2));
			Assert::IsTrue(foo.Find("Pointer")->AsPointer() == reinterpret_cast<RTTI*>(&foo));
			Assert::AreEqual(10, foo.mInt);
			Assert::AreEqual(300, foo.mIntArray[2]);
			Assert::IsTrue(foo.mPointer == reinterpret_cast<RTTI*>(&foo));

			// Internal storage
			Assert::AreEqual(3_z, foo.Find("InternalScopeArray")->Size());
			foo.Find("InternalInt")->Set(100);
			foo.Find("InternalString")->Set("abc"s);
			foo.AppendScope("InternalScopeArray").Append("Int").first = 666;
			Assert::AreEqual(100, foo.Find("InternalInt")->AsInt());
			Assert::AreEqual("abc"s, foo.Find("InternalString")->AsString());
			Assert::AreEqual(4_z, foo.Find("InternalScopeArray")->Size());
			Assert::AreEqual(666, foo.Find("InternalScopeArray")->AsTable(3).Find("Int")->AsInt());

			// Clear prescribed attributes
			foo.Clear();
			Check(foo);
			Assert::AreEqual(10, foo.Find("Int")->AsInt());
			Assert::AreEqual(300, foo.Find("IntArray")->AsInt(2));
			Assert::IsTrue(foo.Find("Pointer")->AsPointer() == reinterpret_cast<RTTI*>(&foo));
			Assert::AreEqual(10, foo.mInt);
			Assert::AreEqual(300, foo.mIntArray[2]);
			Assert::IsTrue(foo.mPointer == reinterpret_cast<RTTI*>(&foo));
			Assert::AreEqual(0, foo.Find("InternalInt")->AsInt());
			Assert::AreEqual(string(), foo.Find("InternalString")->AsString());
			Assert::AreEqual(3_z, foo.Find("InternalScopeArray")->Size());
		}

		TEST_METHOD(TestAuxiliaryAttribute)
		{
			AttributedFoo foo(1);
			foo.AppendAuxiliaryAttribute("int") = 10;
			foo.AppendAuxiliaryAttribute("float") = 10.f;
			foo.AppendAuxiliaryAttribute("vector") = vec4(10.f);
			foo.AppendAuxiliaryAttribute("matrix") = mat4(10.f);
			foo.AppendAuxiliaryAttribute("string") = "10.f"s;
			foo.AppendAuxiliaryAttribute("pointer") = reinterpret_cast<RTTI*>(&foo);
			Assert::AreEqual(10, foo.Find("int")->AsInt());
			Assert::AreEqual(10.f, foo.Find("float")->AsFloat());
			Assert::AreEqual(vec4(10.f), foo.Find("vector")->AsVector());
			Assert::AreEqual(mat4(10.f), foo.Find("matrix")->AsMatrix());
			Assert::AreEqual("10.f"s, foo.Find("string")->AsString());
			Assert::IsTrue(foo.Find("pointer")->AsPointer() == reinterpret_cast<RTTI*>(&foo));

			foo.Clear();
			Check(foo);
			Assert::AreEqual(DefaultSize(AttributedFoo::TypeIdClass()), foo.Size());
			Assert::IsFalse(foo.IsAttribute("int"));
			Assert::IsFalse(foo.IsAttribute("float"));
			Assert::IsFalse(foo.IsAttribute("vector"));
			Assert::IsFalse(foo.IsAttribute("matrix"));
			Assert::IsFalse(foo.IsAttribute("string"));
			Assert::IsFalse(foo.IsAttribute("pointer"));
		}

		TEST_METHOD(TestCopyConstructor)
		{
			AttributedFoo foo(1);
			AttributedFoo* childFoo = new AttributedFoo(2);
			foo["InternalScopeArray"][0].Adopt(*childFoo, "Foo");
			Assert::AreEqual(2, childFoo->Data());
			Assert::IsTrue(childFoo->GetParent() == &foo["InternalScopeArray"][0]);
			Assert::AreEqual(3_z, foo["InternalScopeArray"].Size());
			Assert::AreEqual(1_z, foo["InternalScopeArray"][0].Size());
			Assert::IsTrue(&foo["InternalScopeArray"][0]["Foo"][0] == childFoo);
			foo["FloatArray"].Set(100.f, 2);
			foo.AppendAuxiliaryAttribute("a"s) = 10086;

			AttributedFoo clone = foo;
			AttributedFoo* cloneChildFoo = reinterpret_cast<AttributedFoo*>(&clone["InternalScopeArray"][0]["Foo"][0]);
			Check(foo);
			Check(clone);
			Check(*childFoo);
			Check(*cloneChildFoo);
			Assert::AreEqual(foo, clone);
			Assert::AreEqual(*childFoo, *cloneChildFoo);
			Assert::IsFalse(childFoo == cloneChildFoo);
			Assert::IsTrue(clone != *childFoo);
			Assert::AreEqual(1, clone.Data());
			Assert::AreEqual(3_z, clone["InternalScopeArray"].Size());
			Assert::AreEqual(2, clone["InternalScopeArray"][0]["Foo"][0].As<AttributedFoo>()->Data());
			delete childFoo;
			Assert::AreEqual(0_z, foo["InternalScopeArray"][0]["Foo"].Size());
			Assert::AreEqual(1_z, clone["InternalScopeArray"][0]["Foo"].Size());
			Assert::AreEqual(100.f, clone["FloatArray"].AsFloat(2));
			Assert::AreEqual(10086, clone["a"].AsInt());

			Bar bar;
			bar.AppendAuxiliaryAttribute("a"s) = 10;
			Bar bar2 = bar;
			Assert::AreEqual(2_z, bar.Size());
			Assert::IsTrue(bar["this"].AsPointer() == reinterpret_cast<RTTI*>(&bar));
			Assert::AreEqual(10, bar["a"].AsInt());
			Assert::AreSame(bar["this"], bar[0]);
			Assert::AreSame(bar["a"], bar[1]);
			Assert::AreNotSame(bar["this"], bar2["this"]);
			Assert::AreNotSame(bar["a"], bar2["a"]);
			Assert::AreEqual(2_z, bar2.Size());
			Assert::IsTrue(bar2["this"].AsPointer() == reinterpret_cast<RTTI*>(&bar2));
			Assert::AreEqual(10, bar["a"].AsInt());
			Assert::AreSame(bar2["this"], bar2[0]);
			Assert::AreSame(bar2["a"], bar2[1]);
		}

		TEST_METHOD(TestMoveConstructor)
		{
			AttributedFoo foo(1);
			AttributedFoo* childFoo = new AttributedFoo(2);
			foo["InternalScopeArray"][0].Adopt(*childFoo, "Foo");
			Assert::AreEqual(2, childFoo->Data());
			Assert::IsTrue(childFoo->GetParent() == &foo["InternalScopeArray"][0]);
			Assert::AreEqual(3_z, foo["InternalScopeArray"].Size());
			Assert::AreEqual(1_z, foo["InternalScopeArray"][0].Size());
			Assert::IsTrue(&foo["InternalScopeArray"][0]["Foo"][0] == childFoo);
			foo["FloatArray"].Set(100.f, 2);
			foo.AppendAuxiliaryAttribute("a"s) = 10086;

			AttributedFoo clone = move(foo);
			AttributedFoo* cloneChildFoo = reinterpret_cast<AttributedFoo*>(&clone["InternalScopeArray"][0]["Foo"][0]);
			Assert::AreEqual(0_z, foo.Size());
			Assert::AreEqual(1, clone.Data());
			Check(clone);
			Check(*cloneChildFoo);
			Assert::IsTrue(childFoo == cloneChildFoo);
			Assert::AreEqual(3_z, clone["InternalScopeArray"].Size());
			Assert::AreEqual(2, clone["InternalScopeArray"][0]["Foo"][0].As<AttributedFoo>()->Data());
			Assert::AreEqual(1_z, clone["InternalScopeArray"][0]["Foo"].Size());
			delete childFoo;
			Assert::AreEqual(0_z, clone["InternalScopeArray"][0]["Foo"].Size());
			Assert::AreEqual(100.f, clone["FloatArray"].AsFloat(2));
			Assert::AreEqual(10086, clone["a"].AsInt());
		}

		TEST_METHOD(TestCopyOperator)
		{
			AttributedFoo foo(1);
			foo.AppendAuxiliaryAttribute("a"s) = 10;
			foo.AppendAuxiliaryAttribute("b"s);
			Scope& child = foo.AppendScope("b"s);
			child.Append("a"s).first = 10.f;
			AttributedFoo foo2(2);
			foo2["Int"].Set(9);
			foo2["InternalInt"].Set(8);
			foo2.AppendScope("abc"s);

			foo2 = foo;
			Assert::AreEqual(foo2, foo);
			Check(foo);
			Assert::AreEqual(10, foo["a"].AsInt());
			Assert::AreEqual(10.f, foo["b"][0]["a"].AsFloat());
			Check(foo2);
			Assert::AreEqual(1, foo2.Data());
			Assert::AreNotEqual(9, foo2["Int"].AsInt());
			Assert::AreNotEqual(8, foo2["InternalInt"].AsInt());
			Assert::IsFalse(foo2.IsAttribute("abc"));
			Assert::AreEqual(10, foo2["a"].AsInt());
			Assert::AreEqual(10.f, foo2["b"][0]["a"].AsFloat());
			Assert::AreEqual(child, foo2["b"][0]);
			Assert::AreNotSame(child, foo2["b"][0]);
		}

		TEST_METHOD(TestMoveOperator)
		{
			AttributedFoo foo(1);
			foo.AppendAuxiliaryAttribute("a"s) = 10;
			foo.AppendAuxiliaryAttribute("b"s);
			Scope& child = foo.AppendScope("b"s);
			child.Append("a"s).first = 10.f;
			AttributedFoo foo2(2);
			foo2["Int"].Set(9);
			foo2["InternalInt"].Set(8);
			foo2.AppendScope("abc"s);

			foo2 = move(foo);
			Assert::AreEqual(0_z, foo.Size());
			Check(foo2);
			Assert::AreEqual(1, foo2.Data());
			Assert::AreNotEqual(9, foo2["Int"].AsInt());
			Assert::AreNotEqual(8, foo2["InternalInt"].AsInt());
			Assert::IsFalse(foo2.IsAttribute("abc"));
			Assert::AreEqual(10, foo2["a"].AsInt());
			Assert::AreEqual(10.f, foo2["b"][0]["a"].AsFloat());
			Assert::AreEqual(child, foo2["b"][0]);
			Assert::AreSame(child, foo2["b"][0]);
		}

		TEST_METHOD(TestAppendAuxiliaryAttribute)
		{
			AttributedFoo foo(1);
			Assert::IsFalse(foo.IsAuxiliaryAttribute("a"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("b"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("c"));
			foo.AppendAuxiliaryAttribute("a") = { 10, 11, 12 };
			foo.AppendAuxiliaryAttribute("b") = "abc"s;
			foo.AppendAuxiliaryAttribute("c") = reinterpret_cast<RTTI*>(&foo);
			Assert::IsTrue(foo.IsAuxiliaryAttribute("a"));
			Assert::IsTrue(foo.IsAuxiliaryAttribute("b"));
			Assert::IsTrue(foo.IsAuxiliaryAttribute("c"));
			Assert::AreEqual(11, foo.Find("a")->AsInt(1));
			Assert::AreEqual("abc"s, foo.Find("b")->AsString());
			Assert::IsTrue(foo.Find("c")->AsPointer() == reinterpret_cast<RTTI*>(&foo));

			Assert::AreEqual(3_z, foo.AppendAuxiliaryAttribute("a").Size());
			Assert::AreEqual(1_z, foo.AppendAuxiliaryAttribute("b").Size());
			Assert::AreEqual(1_z, foo.AppendAuxiliaryAttribute("c").Size());
			Assert::AreEqual(0_z, foo.AppendAuxiliaryAttribute("d").Size());

			Assert::ExpectException<std::exception>([&foo] { foo.AppendAuxiliaryAttribute(string()); });
			Assert::ExpectException<std::exception>([&foo] { foo.AppendAuxiliaryAttribute("this"); });
		}

		TEST_METHOD(TestGetAttributes)
		{
			// Default
			AttributedFoo foo(1);
			Check(foo);
			foo.AppendAuxiliaryAttribute("a");
			foo.AppendAuxiliaryAttribute("b");
			foo.AppendAuxiliaryAttribute("c");
			auto vector = foo.Attributes();
			auto signatures = AttributedTypeManager::GetSignature(AttributedFoo::TypeIdClass());
			Assert::AreEqual(signatures->Size() + 4, vector.Size());
			Assert::AreEqual("this"s, vector[0]->first);
			for (size_t i = 0; i < signatures->Size(); ++i)
			{
				Assert::AreEqual((*signatures)[i].mName, vector[i + 1]->first);
			}
			Assert::AreEqual("a"s, vector[signatures->Size() + 1]->first);
			Assert::AreEqual("b"s, vector[signatures->Size() + 2]->first);
			Assert::AreEqual("c"s, vector[signatures->Size() + 3]->first);

			// Const
			AttributedFoo const& cFoo = foo;
			auto cVector = cFoo.Attributes();
			Assert::AreEqual(signatures->Size() + 4, cVector.Size());
			Assert::AreEqual("this"s, cVector[0]->first);
			for (size_t i = 0; i < signatures->Size(); ++i)
			{
				Assert::AreEqual((*signatures)[i].mName, cVector[i + 1]->first);
			}
			Assert::AreEqual("a"s, cVector[signatures->Size() + 1]->first);
			Assert::AreEqual("b"s, cVector[signatures->Size() + 2]->first);
			Assert::AreEqual("c"s, cVector[signatures->Size() + 3]->first);

			// Empty
			Bar bar;
			bar.AppendAuxiliaryAttribute("a"s);
			bar.AppendAuxiliaryAttribute("b"s);
			auto barVector = bar.Attributes();
			Assert::AreEqual(3_z, barVector.Size());
			Assert::IsTrue(barVector[0]->second.AsPointer() == reinterpret_cast<RTTI*>(&bar));
			Assert::AreSame(barVector[0]->second, bar["this"]);
			Assert::AreEqual("a"s, barVector[1]->first);
			Assert::AreEqual("b"s, barVector[2]->first);
		}

		TEST_METHOD(TestGetPrescribedAttribute)
		{
			// Default
			AttributedFoo foo(1);
			Check(foo);
			foo.AppendAuxiliaryAttribute("a");
			foo.AppendAuxiliaryAttribute("b");
			foo.AppendAuxiliaryAttribute("c");
			auto vector = foo.PrescribedAttributes();
			auto signatures = AttributedTypeManager::GetSignature(AttributedFoo::TypeIdClass());
			Assert::AreEqual(signatures->Size() + 1, vector.Size());
			Assert::AreEqual("this"s, vector[0]->first);
			for (size_t i = 0; i < signatures->Size(); ++i)
			{
				Assert::AreEqual((*signatures)[i].mName, vector[i + 1]->first);
			}

			// Const
			AttributedFoo const& cFoo = foo;
			auto cVector = cFoo.PrescribedAttributes();
			Assert::AreEqual(signatures->Size() + 1, cVector.Size());
			Assert::AreEqual("this"s, cVector[0]->first);
			for (size_t i = 0; i < signatures->Size(); ++i)
			{
				Assert::AreEqual((*signatures)[i].mName, cVector[i + 1]->first);
			}

			// Empty
			Bar bar;
			bar.AppendAuxiliaryAttribute("a"s);
			bar.AppendAuxiliaryAttribute("b"s);
			auto barVector = bar.PrescribedAttributes();
			Assert::AreEqual(1_z, barVector.Size());
			Assert::IsTrue(barVector[0]->second.AsPointer() == reinterpret_cast<RTTI*>(&bar));
			Assert::AreSame(barVector[0]->second, bar["this"]);
		}

		TEST_METHOD(TestGetAuxiliaryAttribute)
		{
			// Default
			AttributedFoo foo(1);
			Check(foo);
			foo.AppendAuxiliaryAttribute("a");
			foo.AppendAuxiliaryAttribute("b");
			foo.AppendAuxiliaryAttribute("c");
			auto vector = foo.AuxiliaryAttributes();
			Assert::AreEqual(3_z, vector.Size());
			Assert::AreEqual("a"s, vector[0]->first);
			Assert::AreEqual("b"s, vector[1]->first);
			Assert::AreEqual("c"s, vector[2]->first);

			// Const
			AttributedFoo const& cFoo = foo;
			auto cVector = cFoo.AuxiliaryAttributes();
			Assert::AreEqual(3_z, vector.Size());
			Assert::AreEqual("a"s, vector[0]->first);
			Assert::AreEqual("b"s, vector[1]->first);
			Assert::AreEqual("c"s, vector[2]->first);

			// Empty
			Bar bar;
			bar.AppendAuxiliaryAttribute("a"s);
			bar.AppendAuxiliaryAttribute("b"s);
			auto barVector = bar.AuxiliaryAttributes();
			Assert::AreEqual(2_z, barVector.Size());
			Assert::AreEqual("a"s, barVector[0]->first);
			Assert::AreEqual("b"s, barVector[1]->first);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState AttributedTest::sStartMemState;
}