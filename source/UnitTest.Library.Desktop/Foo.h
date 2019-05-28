#pragma once
#include "RTTI.h"
#include "Factory.h"

/// <summary>
/// A pure test class, should only be used in unit test
/// Contains an int pointer member variable to test user defined data
/// </summary>
class Foo : public GameEngine::RTTI
{
	RTTI_DECLARATIONS(Foo, RTTI);

public:
	/// <summary>
	/// Explicit constructor
	/// </summary>
	/// <param name="data">An int data for test</param>
	explicit Foo(const int & data = 0);

	/// <summary>
	/// Copy constructor
	/// </summary>
	/// <param name="other">The other Foo object to copy from</param>
	Foo(const Foo & other);

	/// <summary>
	/// Move constructor
	/// </summary>
	/// <param name="other">Other Foo to move from</param>
	Foo(Foo&& other);

	/// <summary>
	/// Destructor, will release memory
	/// </summary>
	virtual ~Foo();

	/// <summary>
	/// Copy assignment
	/// </summary>
	/// <param name="other">The other Foo object to copy from</param>
	/// <returns>this Foo object</returns>
	Foo & operator=(const Foo & other);

	/// <summary>
	/// Move assignemnt
	/// </summary>
	/// <param name="other">Other foo to move from</param>
	/// <returns>This Foo after moving</returns>
	Foo& operator=(Foo&& other);

	/// <summary>
	/// Check if the int value in member variables are equal
	/// </summary>
	/// <param name="other">The other Foo object to check with</param>
	/// <returns>True if both int values are equal, False otherwise</returns>
	bool operator==(const Foo & other) const;

	/// <summary>
	/// Check if the int value in member variables are equal
	/// </summary>
	/// <param name="other">The other Foo object to check with</param>
	/// <returns>True if both int values are not equal, False otherwise</returns>
	bool operator!=(const Foo & other) const;

	/// <summary>
	/// Get the int value stored in memeber variable
	/// </summary>
	/// <returns>Int value in member variable</returns>
	int & Data() const;

	/// <summary>
	/// Check if this RTTI instance equals to other
	/// </summary>
	/// <param name="rhs">Other RTTI instance to check with</param>
	/// <returns>True if both RTTIs equals</returns>
	virtual bool Equals(const RTTI* rhs) const override;

	/// <summary>
	/// Convert data to string
	/// </summary>
	/// <returns>Int data as string</returns>
	virtual std::string ToString() const override;

private:
	/// <summary>
	/// An int pointer that stores the data for testing
	/// </summary>
	int* mDataPointer = nullptr;
};

// Must define tostring method for Foo, as required by MS unit test
namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> inline std::wstring ToString<Foo>(const Foo & foo)
			{
				return std::to_wstring(foo.Data());
			}
			template<> inline std::wstring ToString<Foo>(const Foo * foo)
			{
				return std::to_wstring(foo->Data());
			}
			template<> inline std::wstring ToString<Foo>(Foo * foo)
			{
				return std::to_wstring(foo->Data());
			}
		}
	}
}

DECLARE_FACTORY(Foo, Foo);
