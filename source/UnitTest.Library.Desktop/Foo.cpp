#include "pch.h"
#include "Foo.h"

using namespace GameEngine;
RTTI_DEFINITIONS(Foo);

Foo::Foo(const int & data)
{
	mDataPointer = new int(data);
}

Foo::Foo(const Foo & other)
{
	mDataPointer = new int(other.Data());
}

Foo::Foo(Foo&& other) :
	mDataPointer(other.mDataPointer)
{
	other.mDataPointer = nullptr;
}

Foo::~Foo()
{
	delete mDataPointer;
}

Foo & Foo::operator=(const Foo & other)
{
	if (this != &other)
	{
		*mDataPointer = other.Data();
	}
	return *this;
}

Foo& Foo::operator=(Foo&& other)
{
	if (this != &other)
	{
		delete mDataPointer;
		mDataPointer = other.mDataPointer;
		other.mDataPointer = nullptr;
	}
	return *this;
}

bool Foo::operator==(const Foo & other) const
{
	return *mDataPointer == other.Data();
}

bool Foo::operator!=(const Foo & other) const
{
	return !operator==(other);
}

int & Foo::Data() const
{
	return *mDataPointer;
}

bool Foo::Equals(const RTTI* rhs) const
{
	Foo* otherFoo = rhs->As<Foo>();
	return (otherFoo != nullptr && *mDataPointer == otherFoo->Data());
}

std::string Foo::ToString() const
{
	return std::to_string(*mDataPointer);
}
