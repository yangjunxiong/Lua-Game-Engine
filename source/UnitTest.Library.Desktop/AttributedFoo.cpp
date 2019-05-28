#include "pch.h"
#include "AttributedFoo.h"

using namespace GameEngine;
RTTI_DEFINITIONS(AttributedFoo);

AttributedFoo::AttributedFoo(int data) :
	Attributed(AttributedFoo::TypeIdClass()),
	Foo(data)
{}

bool AttributedFoo::operator==(const AttributedFoo& other) const
{
	return Attributed::operator==(other) && Foo::operator==(other);
}

bool AttributedFoo::operator!=(const AttributedFoo& other) const
{
	return Attributed::operator!=(other) || Foo::operator!=(other);
}

std::string AttributedFoo::ToString() const
{
	return Foo::ToString();
}

bool AttributedFoo::Equals(const RTTI* other) const
{
	AttributedFoo* foo = other != nullptr ? other->As<AttributedFoo>() : nullptr;
	return foo != nullptr && operator==(*foo);
}

gsl::owner<AttributedFoo*> AttributedFoo::Clone() const
{
	return new AttributedFoo(Data());
}

const Vector<Attributed::Signature> AttributedFoo::Signatures()
{
	return Vector<Attributed::Signature>(
	{
		Signature("Int", Datum::DatumType::Integer, 1, offsetof(AttributedFoo, mInt)),
		Signature("Float", Datum::DatumType::Float, 1, offsetof(AttributedFoo, mFloat)),
		Signature("Vector", Datum::DatumType::Vector, 1, offsetof(AttributedFoo, mVector)),
		Signature("Matrix", Datum::DatumType::Matrix, 1, offsetof(AttributedFoo, mMatrix)),
		Signature("String", Datum::DatumType::String, 1, offsetof(AttributedFoo, mString)),
		Signature("Pointer", Datum::DatumType::Pointer, 1, offsetof(AttributedFoo, mPointer)),
		Signature("IntArray", Datum::DatumType::Integer, AttributedFoo::ArraySize, offsetof(AttributedFoo, mIntArray)),
		Signature("FloatArray", Datum::DatumType::Float, AttributedFoo::ArraySize, offsetof(AttributedFoo, mFloatArray)),
		Signature("VectorArray", Datum::DatumType::Vector, AttributedFoo::ArraySize, offsetof(AttributedFoo, mVectorArray)),
		Signature("MatrixArray", Datum::DatumType::Matrix, AttributedFoo::ArraySize, offsetof(AttributedFoo, mMatrixArray)),
		Signature("StringArray", Datum::DatumType::String, AttributedFoo::ArraySize, offsetof(AttributedFoo, mStringArray)),
		Signature("PointerArray", Datum::DatumType::Pointer, AttributedFoo::ArraySize, offsetof(AttributedFoo, mPointerArray)),

		Signature("InternalInt", Datum::DatumType::Integer, 1),
		Signature("InternalFloat", Datum::DatumType::Float, 1),
		Signature("InternalVector", Datum::DatumType::Vector, 1),
		Signature("InternalMatrix", Datum::DatumType::Matrix, 1),
		Signature("InternalString", Datum::DatumType::String, 1),
		Signature("InternalPointer", Datum::DatumType::Pointer, 1),
		Signature("InternalIntArray", Datum::DatumType::Integer, AttributedFoo::ArraySize),
		Signature("InternalFloatArray", Datum::DatumType::Float, AttributedFoo::ArraySize),
		Signature("InternalVectorArray", Datum::DatumType::Vector, AttributedFoo::ArraySize),
		Signature("InternalMatrixArray", Datum::DatumType::Matrix, AttributedFoo::ArraySize),
		Signature("InternalStringArray", Datum::DatumType::String, AttributedFoo::ArraySize),
		Signature("InternalPointerArray", Datum::DatumType::Pointer, AttributedFoo::ArraySize),
		Signature("InternalScopeArray", Datum::DatumType::Table, AttributedFoo::ArraySize),
		Signature("InternalScopeEmpty", Datum::DatumType::Table, 0)
	});
}