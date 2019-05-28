#include "pch.h"
#include "Datum.h"
#include "RTTI.h"
#pragma warning(push)
#pragma warning(disable : 4201)
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#pragma warning(pop)
#include <algorithm>

using namespace std;
using namespace glm;

#pragma region HandyMacro
/// <summary>
/// Function body of initilizer list contructor
/// </summary>
#define INIT_LIST_CONSTRUCTOR_BODY(_list)  \
AllocateMemory(_list.size());			   \
for (const auto& item : _list)			   \
{										   \
	PushBack(item);						   \
}

/// <summary>
/// Function body of operator=() for all supported data types
/// </summary>
#define ASSIGNMENT_BODY(_value)     \
if (mSize == 0)                     \
{                                   \
	PushBack(_value);               \
}                                   \
else                                \
{                                   \
	Set(_value);                    \
}                                   \
return *this;

/// <summary>
/// Function body of Set(index, value) for all supported data types
/// </summary>
#define SET_BODY(_value, _index, _type)                                 \
if (mType == DatumType::_type)                                          \
{                                                                       \
	if (_index >= mSize)                                                \
	{                                                                   \
		throw std::exception("Index out of range");                     \
	}                                                                   \
	mData._type[_index] = _value;                                       \
}                                                                       \
else                                                                    \
{                                                                       \
	throw std::exception("Try to assign datum with incompatible type"); \
}

/// <summary>
/// Function body of Asxxx(index) for all supported data types
/// </summary>
#define GET_BODY(_index, _type)                              \
if (mType != DatumType::_type)								 \
{															 \
	throw std::exception("Incompatible type");				 \
}															 \
if (mSize <= _index)                                         \
{                                                            \
	throw std::exception("Get datum: index out of range");   \
}                                                            \
return mData._type[_index];

/// <summary>
/// Function body of PushBack(value) for all supported data types
/// </summary>
#define PUSH_BACK_BODY(_value, _enumType, _constructType)   \
if (mType == DatumType::Unknown)                            \
{                                                           \
	mType = DatumType::_enumType;                           \
}															\
else if (mType != DatumType::_enumType)						\
{															\
	throw std::exception("Datum has incompatible type");	\
}															\
															\
CheckIsInternal();											\
EnsureCapacity();											\
new(mData._enumType + mSize) _constructType(_value);        \
++mSize;

/// <summary>
/// Function body of Front()
/// </summary>
#define FRONT_BODY(_type)									\
if (mType == DatumType::_type && mSize > 0)					\
{															\
	return mData._type[0];									\
}															\
throw std::exception("Invalid query");

/// <summary>
/// Function body of Back()
/// </summary>
#define BACK_BODY(_type)									\
if (mType == DatumType::_type && mSize > 0)					\
{															\
	return mData._type[mSize - 1];							\
}															\
throw std::exception("Invalid query");

/// <summary>
/// Function body of Iterator::Asxxx()
/// </summary>
#define ITERATOR_GET_BODY(_type)                            \
if (mOwner == nullptr)										\
{															\
	throw std::exception("Iterator doesn't have owner");	\
}															\
if (mOwner->mType != DatumType::_type)						\
{															\
	throw std::exception("Incompatible iterator type");		\
}															\
if (mIndex >= mOwner->mSize)								\
{															\
	throw std::exception("Iterator points to nothing");		\
}															\
return mOwner->mData._type[mIndex];

/// <summary>
/// Function body of Find(value)
/// </summary>
#define FIND_BODY(_value, _type)							\
if (mType != DatumType::_type)								\
{															\
	throw std::exception("Datum has incompatible type");	\
}															\
for (size_t i = 0; i < mSize; ++i)							\
{															\
	if (mData._type[i] == _value)							\
	{														\
		return Iterator(*this, i);							\
	}														\
}															\
return end();

#define SET_STORAGE_BODY(_value, _type, _size)                         \
if (_value == nullptr)												   \
{																	   \
	throw std::exception("Set storage null pointer");				   \
}																	   \
if (mType == DatumType::Unknown)									   \
{																	   \
	mType = DatumType::_type;										   \
}																	   \
else if (mType != DatumType::_type)									   \
{																	   \
	throw std::exception("Set storage on incompatible type");		   \
}																	   \
else if (_size == 0)												   \
{																	   \
	throw std::exception("External storage can't be empty");		   \
}																	   \
																	   \
ResetSelf();														   \
mIsInternal = false;												   \
mData._type = _value;												   \
mSize = _size;														   \
mCapacity = _size;
#pragma endregion

namespace GameEngine
{
	Vector<size_t> Datum::sTypeSizeTable =
	{
		0,                         // Unknown
		sizeof(int32_t),           // Integer
		sizeof(float),             // Float
		sizeof(glm::vec4),         // Vector
		sizeof(glm::mat4),         // Matrix
		sizeof(Scope*),            // Table
		sizeof(std::string),       // String
		sizeof(RTTI*)              // Pointer
	};

	Vector<Datum::CompareFunction> Datum::sCompareFunctions =
	{
		nullptr,                   // Unknown
		&Datum::ComparePrimitive,  // Integer
		&Datum::ComparePrimitive,  // Float
		&Datum::ComparePrimitive,  // Vector
		&Datum::ComparePrimitive,  // Matrix
		&Datum::ComparePointer,    // Table
		&Datum::CompareString,     // String,
		&Datum::ComparePointer     // Pointer
	};

	Vector<Datum::CreateDefaultFunction> Datum::sCreateDefaultFunctions =
	{
		nullptr,
		&Datum::CreateDefaultPrimitive,    // Integer
		&Datum::CreateDefaultPrimitive,    // Float
		&Datum::CreateDefaultPrimitive,    // Vector
		&Datum::CreateDefaultPrimitive,    // Matrix
		&Datum::CreateDefaultPrimitive,    // Table
		&Datum::CreateDefaultString,       // String
		&Datum::CreateDefaultPrimitive     // Pointer
	};

	Vector<Datum::CopyFunction> Datum::sCopyFunctions =
	{
		&Datum::CopyPrimitive,             // Unknown
		&Datum::CopyPrimitive,             // Integer
		&Datum::CopyPrimitive,			   // Float
		&Datum::CopyPrimitive,			   // Vector
		&Datum::CopyPrimitive,			   // Matrix
		&Datum::CopyPrimitive,			   // Table
		&Datum::CopyString,				   // String
		&Datum::CopyPrimitive,			   // Pointer
	};

	Datum::Datum(Datum::DatumType type) :
		mType(type)
	{}

	Datum::Datum(const Datum& other)
	{
		DeepCopy(other);
	}

	Datum::Datum(Datum&& other) :
		mType(other.mType),
		mSize(other.mSize),
		mCapacity(other.mCapacity),
		mData(other.mData),
		mIsInternal(other.mIsInternal)
	{
		other.mSize = 0;
		other.mData.Integer = nullptr;
	}

	Datum::Datum(const int32_t& value) :
		mType(DatumType::Integer)
	{
		PushBack(value);
	}

	Datum::Datum(const float& value) :
		mType(DatumType::Float)
	{
		PushBack(value);
	}

	Datum::Datum(const glm::vec4& value) :
		mType(DatumType::Vector)
	{
		PushBack(value);
	}

	Datum::Datum(const glm::mat4& value) :
		mType(DatumType::Matrix)
	{
		PushBack(value);
	}

	Datum::Datum(const std::string& value) :
		mType(DatumType::String)
	{
		PushBack(value);
	}

	Datum::Datum(RTTI* const & value) :
		mType(DatumType::Pointer)
	{
		PushBack(value);
	}

	Datum::Datum(const Scope& value) :
		mType(DatumType::Table)
	{
		PushBack(value);
	}

	Datum::Datum(const std::initializer_list<int32_t>& list) :
		mType(DatumType::Integer)
	{
		INIT_LIST_CONSTRUCTOR_BODY(list);
	}

	Datum::Datum(const std::initializer_list<float>& list) :
		mType(DatumType::Float)
	{
		INIT_LIST_CONSTRUCTOR_BODY(list);
	}

	Datum::Datum(const std::initializer_list<glm::vec4>& list) :
		mType(DatumType::Vector)
	{
		INIT_LIST_CONSTRUCTOR_BODY(list);
	}

	Datum::Datum(const std::initializer_list<glm::mat4>& list) :
		mType(DatumType::Matrix)
	{
		INIT_LIST_CONSTRUCTOR_BODY(list);
	}

	Datum::Datum(const std::initializer_list<std::string>& list) :
		mType(DatumType::String)
	{
		INIT_LIST_CONSTRUCTOR_BODY(list);
	}

	Datum::Datum(const std::initializer_list<RTTI*>& list) :
		mType(DatumType::Pointer)
	{
		INIT_LIST_CONSTRUCTOR_BODY(list);
	}

	Datum& Datum::operator=(const Datum& other)
	{
		if (this != &other)
		{
			ResetSelf();
			DeepCopy(other);
		}
		return *this;
	}

	Datum& Datum::operator=(Datum&& other)
	{
		if (this != &other)
		{
			ResetSelf();
			mType = other.mType;
			mSize = other.mSize;
			mCapacity = other.mCapacity;
			mData = other.mData;
			mIsInternal = other.mIsInternal;
			other.mSize = 0;
			other.mData.Integer = nullptr;
		}
		return *this;
	}

	Datum& Datum::operator=(const int32_t& value)
	{
		ASSIGNMENT_BODY(value);
	}
	
	Datum& Datum::operator=(const float& value)
	{
		ASSIGNMENT_BODY(value);
	}

	Datum& Datum::operator=(const glm::vec4& value)
	{
		ASSIGNMENT_BODY(value);
	}

	Datum& Datum::operator=(const glm::mat4& value)
	{
		ASSIGNMENT_BODY(value);
	}

	Datum& Datum::operator=(const std::string& value)
	{
		ASSIGNMENT_BODY(value);
	}

	Datum& Datum::operator=(RTTI* const& value)
	{
		ASSIGNMENT_BODY(value);
	}

	Datum& Datum::operator=(const Scope& value)
	{
		ASSIGNMENT_BODY(value);
	}

	bool Datum::operator==(const Datum& other) const
	{
		if (mType != other.mType || mSize != other.mSize)
		{
			return false;
		}
		return this == &other || mType == DatumType::Unknown || (this->*sCompareFunctions[static_cast<size_t>(mType)])(other.mData.Universe);
	}

	bool Datum::operator!=(const Datum& other) const
	{
		return !operator==(other);
	}

	bool Datum::operator==(const int32_t& value) const
	{
		return mType == DatumType::Integer && mSize > 0 && mData.Integer[0] == value;
	}

	bool Datum::operator==(const float& value) const
	{
		return mType == DatumType::Float && mSize > 0 && mData.Float[0] == value;
	}

	bool Datum::operator==(const glm::vec4& value) const
	{
		return mType == DatumType::Vector && mSize > 0 && mData.Vector[0] == value;
	}

	bool Datum::operator==(const glm::mat4& value) const
	{
		return mType == DatumType::Matrix && mSize > 0 && mData.Matrix[0] == value;
	}

	bool Datum::operator==(const std::string& value) const
	{
		return mType == DatumType::String && mSize > 0 && mData.String[0] == value;
	}

	bool Datum::operator==(const RTTI* const& value) const
	{
		return (mType == DatumType::Pointer || mType == DatumType::Table) && mSize > 0 &&
			(mData.Pointer[0] == value || (mData.Pointer[0] != nullptr && value != nullptr && mData.Pointer[0]->Equals(value)));
	}

	bool Datum::operator==(const Scope& value) const
	{
		return operator==(reinterpret_cast<const RTTI*>(&value));
	}

	bool Datum::operator!=(const int32_t& value) const
	{
		return !operator==(value);
	}

	bool Datum::operator!=(const float& value) const
	{
		return !operator==(value);
	}

	bool Datum::operator!=(const glm::vec4& value) const
	{
		return !operator==(value);
	}

	bool Datum::operator!=(const glm::mat4& value) const
	{
		return !operator==(value);
	}

	bool Datum::operator!=(const std::string& value) const
	{
		return !operator==(value);
	}

	bool Datum::operator!=(const RTTI* const& value) const
	{
		return !operator==(value);
	}

	bool Datum::operator!=(const Scope& value) const
	{
		return !operator==(value);
	}

	Scope& Datum::operator[](std::uint32_t index)
	{
		return AsTable(index);
	}

	const Scope& Datum::operator[](std::uint32_t index) const
	{
		return AsTable(index);
	}

	Datum Datum::operator+(const Datum& other) const
	{
		Datum result;
		if (mType == DatumType::Integer)
		{
			if (other.Type() == DatumType::Integer)
			{
				result = AsInt() + other.AsInt();
			}
			else if (other.Type() == DatumType::Float)
			{
				result = AsInt() + other.AsFloat();
			}
		}
		else if (mType == DatumType::Float)
		{
			if (other.Type() == DatumType::Integer)
			{
				result = AsFloat() + other.AsInt();
			}
			else if (other.Type() == DatumType::Float)
			{
				result = AsFloat() + other.AsFloat();
			}
		}
		else if (mType == DatumType::Vector)
		{
			result = AsVector() + other.AsVector();
		}
		else if (mType == DatumType::Matrix)
		{
			result = AsMatrix() + other.AsMatrix();
		}
		else if (mType == DatumType::String)
		{
			result = AsString() + other.AsString();
		}

		return result;
	}

	Datum Datum::operator-(const Datum& other) const
	{
		Datum result;
		if (mType == DatumType::Integer)
		{
			if (other.Type() == DatumType::Integer)
			{
				result = AsInt() - other.AsInt();
			}
			else if (other.Type() == DatumType::Float)
			{
				result = AsInt() - other.AsFloat();
			}
		}
		else if (mType == DatumType::Float)
		{
			if (other.Type() == DatumType::Integer)
			{
				result = AsFloat() - other.AsInt();
			}
			else if (other.Type() == DatumType::Float)
			{
				result = AsFloat() - other.AsFloat();
			}
		}
		else if (mType == DatumType::Vector)
		{
			result = AsVector() - other.AsVector();
		}
		else if (mType == DatumType::Matrix)
		{
			result = AsMatrix() - other.AsMatrix();
		}

		return result;
	}

	Datum Datum::operator*(const Datum& other) const
	{
		Datum result;
		if (mType == DatumType::Integer)
		{
			if (other.Type() == DatumType::Integer)
			{
				result = AsInt() * other.AsInt();
			}
			else if (other.Type() == DatumType::Float)
			{
				result = AsInt() * other.AsFloat();
			}
		}
		else if (mType == DatumType::Float)
		{
			if (other.Type() == DatumType::Integer)
			{
				result = AsFloat() * other.AsInt();
			}
			else if (other.Type() == DatumType::Float)
			{
				result = AsFloat() * other.AsFloat();
			}
		}
		else if (mType == DatumType::Vector)
		{
			result = AsVector() * other.AsVector();
		}
		else if (mType == DatumType::Matrix)
		{
			result = AsMatrix() * other.AsMatrix();
		}

		return result;
	}

	Datum Datum::operator/(const Datum& other) const
	{
		Datum result;
		if (mType == DatumType::Integer)
		{
			if (other.Type() == DatumType::Integer)
			{
				result = AsInt() / other.AsInt();
			}
			else if (other.Type() == DatumType::Float)
			{
				result = AsInt() / other.AsFloat();
			}
		}
		else if (mType == DatumType::Float)
		{
			if (other.Type() == DatumType::Integer)
			{
				result = AsFloat() / other.AsInt();
			}
			else if (other.Type() == DatumType::Float)
			{
				result = AsFloat() / other.AsFloat();
			}
		}
		else if (mType == DatumType::Vector)
		{
			result = AsVector() / other.AsVector();
		}
		else if (mType == DatumType::Matrix)
		{
			result = AsMatrix() / other.AsMatrix();
		}

		return result;
	}

	Datum Datum::operator%(const Datum& other) const
	{
		Datum result;
		if (mType == DatumType::Integer)
		{
			if (other.mType == DatumType::Integer)
			{
				result = AsInt() % other.AsInt();
			}
			else if (other.mType == DatumType::Float)
			{
				result = AsInt() % static_cast<int>(other.AsFloat());
			}
		}
		else if (mType == DatumType::Float)
		{
			if (other.mType == DatumType::Integer)
			{
				result = static_cast<int>(AsFloat()) % other.AsInt();
			}
			else if (other.mType == DatumType::Float)
			{
				result = static_cast<int>(AsFloat()) % static_cast<int>(other.AsFloat());
			}
		}

		return result;
	}

	Datum Datum::operator-() const
	{
		Datum result;
		if (mType == DatumType::Integer)
		{
			result = -AsInt();
		}
		else if (mType == DatumType::Float)
		{
			result = -AsFloat();
		}

		return result;
	}

	Datum::operator bool() const
	{
		if (mType == DatumType::Integer)
		{
			return AsInt() == 0 ? false : true;
		}
		else if (mType == DatumType::Float)
		{
			return AsFloat() == 0.f ? false : true;
		}

		return false;
	}

	Datum Datum::operator!() const
	{
		Datum result;
		if (mType == DatumType::Integer)
		{
			result = AsInt() == 0 ? 1 : 0;
		}
		else if (mType == DatumType::Float)
		{
			result = AsFloat() == 0.f ? 0 : 1;
		}
		
		return result;
	}

	bool Datum::operator>(const Datum& other) const
	{
		if (mType == DatumType::Integer)
		{
			if (other.mType == DatumType::Integer)
			{
				return AsInt() > other.AsInt();
			}
			else if (other.mType == DatumType::Float)
			{
				return AsInt() > static_cast<int>(other.AsFloat());
			}
		}
		else if (mType == DatumType::Float)
		{
			if (other.mType == DatumType::Integer)
			{
				return static_cast<int>(AsFloat()) > other.AsInt();
			}
			else if (other.mType == DatumType::Float)
			{
				return static_cast<int>(AsFloat()) > static_cast<int>(other.AsFloat());
			}
		}

		return false;
	}

	bool Datum::operator>=(const Datum& other) const
	{
		if (mType == DatumType::Integer)
		{
			if (other.mType == DatumType::Integer)
			{
				return AsInt() >= other.AsInt();
			}
			else if (other.mType == DatumType::Float)
			{
				return AsInt() >= static_cast<int>(other.AsFloat());
			}
		}
		else if (mType == DatumType::Float)
		{
			if (other.mType == DatumType::Integer)
			{
				return static_cast<int>(AsFloat()) >= other.AsInt();
			}
			else if (other.mType == DatumType::Float)
			{
				return static_cast<int>(AsFloat()) >= static_cast<int>(other.AsFloat());
			}
		}

		return false;
	}

	bool Datum::operator<(const Datum& other) const
	{
		if (mType == DatumType::Integer)
		{
			if (other.mType == DatumType::Integer)
			{
				return AsInt() < other.AsInt();
			}
			else if (other.mType == DatumType::Float)
			{
				return AsInt() < static_cast<int>(other.AsFloat());
			}
		}
		else if (mType == DatumType::Float)
		{
			if (other.mType == DatumType::Integer)
			{
				return static_cast<int>(AsFloat()) < other.AsInt();
			}
			else if (other.mType == DatumType::Float)
			{
				return static_cast<int>(AsFloat()) < static_cast<int>(other.AsFloat());
			}
		}

		return false;
	}

	bool Datum::operator<=(const Datum& other) const
	{
		if (mType == DatumType::Integer)
		{
			if (other.mType == DatumType::Integer)
			{
				return AsInt() <= other.AsInt();
			}
			else if (other.mType == DatumType::Float)
			{
				return AsInt() <= static_cast<int>(other.AsFloat());
			}
		}
		else if (mType == DatumType::Float)
		{
			if (other.mType == DatumType::Integer)
			{
				return static_cast<int>(AsFloat()) <= other.AsInt();
			}
			else if (other.mType == DatumType::Float)
			{
				return static_cast<int>(AsFloat()) <= static_cast<int>(other.AsFloat());
			}
		}

		return false;
	}

	bool Datum::operator&&(const Datum& other) const
	{
		return this->operator bool() && other.operator bool();
	}

	bool Datum::operator||(const Datum& other) const
	{
		return this->operator bool() || other.operator bool();
	}

	Datum::~Datum()
	{
		ResetSelf();
	}

	void Datum::CopyValue(const Datum& other)
	{
		if (mType == other.Type() && mType != DatumType::Unknown)
		{
			SetSize(other.mSize);
			if (mType == DatumType::String)
			{
				for (size_t i = 0; i < mSize; ++i)
				{
					mData.String[i] = other.AsString(i);
				}
			}
			else
			{
				CopyPrimitive(other.mData.Universe);
			}
		}
	}

	void Datum::Set(const int32_t& value, size_t index)
	{
		SET_BODY(value, index, Integer);
	}

	void Datum::Set(const float& value, size_t index)
	{
		SET_BODY(value, index, Float);
	}

	void Datum::Set(const glm::vec4& value, size_t index)
	{
		SET_BODY(value, index, Vector);
	}

	void Datum::Set(const glm::mat4& value, size_t index)
	{
		SET_BODY(value, index, Matrix);
	}

	void Datum::Set(const std::string& value, size_t index)
	{
		SET_BODY(value, index, String);
	}

	void Datum::Set(RTTI* const& value, size_t index)
	{
		SET_BODY(value, index, Pointer);
	}

	void Datum::Set(const Scope& value, size_t index)
	{
		Scope* pointer = const_cast<Scope*>(&value);
		SET_BODY(pointer, index, Table);
	}

	int32_t& Datum::AsInt(size_t index)
	{
		GET_BODY(index, Integer);
	}

	float& Datum::AsFloat(size_t index)
	{
		GET_BODY(index, Float);
	}

	glm::vec4& Datum::AsVector(size_t index)
	{
		GET_BODY(index, Vector);
	}

	glm::mat4& Datum::AsMatrix(size_t index)
	{
		GET_BODY(index, Matrix);
	}

	std::string& Datum::AsString(size_t index)
	{
		GET_BODY(index, String);
	}

	RTTI*& Datum::AsPointer(size_t index)
	{
		GET_BODY(index, Pointer);
	}

	Scope& Datum::AsTable(size_t index)
	{
		if (mType != DatumType::Table)
		{
			throw std::exception("Incompatible type");
		}
		if (mSize <= index)
		{
			throw std::exception("Get datum: index out of range");
		}
		return *mData.Table[index];
	}

	const int32_t& Datum::AsInt(size_t index) const
	{
		GET_BODY(index, Integer);
	}

	const float& Datum::AsFloat(size_t index) const
	{
		GET_BODY(index, Float);
	}

	const glm::vec4& Datum::AsVector(size_t index) const
	{
		GET_BODY(index, Vector);
	}

	const glm::mat4& Datum::AsMatrix(size_t index) const
	{
		GET_BODY(index, Matrix);
	}

	const std::string& Datum::AsString(size_t index) const
	{
		GET_BODY(index, String);
	}

	RTTI* const& Datum::AsPointer(size_t index) const
	{
		GET_BODY(index, Pointer);
	}

	const Scope& Datum::AsTable(size_t index) const
	{
		return const_cast<Datum*>(this)->AsTable(index);
	}

	bool Datum::Remove(const int32_t& value)
	{
		return RemoveAt(Find(value));
	}

	bool Datum::Remove(const float& value)
	{
		return RemoveAt(Find(value));
	}

	bool Datum::Remove(const glm::vec4& value)
	{
		return RemoveAt(Find(value));
	}

	bool Datum::Remove(const glm::mat4& value)
	{
		return RemoveAt(Find(value));
	}

	bool Datum::Remove(const std::string& value)
	{
		return RemoveAt(Find(value));
	}

	bool Datum::Remove(const RTTI* const& value)
	{
		return RemoveAt(Find(value));
	}

	bool Datum::RemoveAt(size_t index)
	{
		CheckIsInternal();
		if (index < mSize)
		{
			if (mType == DatumType::String)
			{
				mData.String[index].~string();
			}
			size_t size = sTypeSizeTable[static_cast<size_t>(mType)];
			char* destination = reinterpret_cast<char*>(mData.Universe) + size * index;
			char* source = destination + size;
			memmove(destination, source, size * (mSize - index - 1));
			--mSize;
			return true;
		}
		return false;
	}

	bool Datum::RemoveAt(const Iterator& it)
	{
		if (it.mOwner != this)
		{
			throw std::exception("Remove invalid iterator");
		}
		return RemoveAt(it.mIndex);
	}

	Datum::Iterator Datum::Find(const int32_t& value)
	{
		FIND_BODY(value, Integer);
	}

	Datum::Iterator Datum::Find(const float& value)
	{
		FIND_BODY(value, Float);
	}

	Datum::Iterator Datum::Find(const glm::vec4& value)
	{
		FIND_BODY(value, Vector);
	}

	Datum::Iterator Datum::Find(const glm::mat4& value)
	{
		FIND_BODY(value, Matrix);
	}

	Datum::Iterator Datum::Find(const std::string& value)
	{
		FIND_BODY(value, String);
	}

	Datum::Iterator Datum::Find(const Scope& value)
	{
		Scope* pointer = const_cast<Scope*>(&value);
		return Find(reinterpret_cast<RTTI*>(pointer));
	}

	Datum::Iterator Datum::Find(const RTTI* const& value)
	{
		if (mType != DatumType::Pointer && mType != DatumType::Table)							
		{														
			throw std::exception("Datum has incompatible type");
		}														
		for (size_t i = 0; i < mSize; ++i)	
		{									
			if (mData.Pointer[i] == value || (mData.Pointer[i] != nullptr && value != nullptr && mData.Pointer[i]->Equals(value)))	
			{								
				return Iterator(*this, i);	
			}								
		}									
		return end();
	}

	Datum::ConstIterator Datum::Find(const int32_t& value) const
	{
		FIND_BODY(value, Integer);
	}

	Datum::ConstIterator Datum::Find(const float& value) const
	{
		FIND_BODY(value, Float);
	}

	Datum::ConstIterator Datum::Find(const glm::vec4& value) const
	{
		FIND_BODY(value, Vector);
	}

	Datum::ConstIterator Datum::Find(const glm::mat4& value) const
	{
		FIND_BODY(value, Matrix);
	}

	Datum::ConstIterator Datum::Find(const std::string& value) const
	{
		FIND_BODY(value, String);
	}

	Datum::ConstIterator Datum::Find(const RTTI* const& value) const
	{
		return ConstIterator(const_cast<Datum*>(this)->Find(value));
	}

	Datum::ConstIterator Datum::Find(const Scope& value) const
	{
		return ConstIterator(const_cast<Datum*>(this)->Find(value));
	}

	Datum::DatumType Datum::Type() const
	{
		return mType;
	}

	void Datum::SetType(DatumType type)
	{
		if (mType == DatumType::Unknown || mType == type)
		{
			mType = type;
		}
		else
		{
			throw std::exception("Try to assign type to datum which has a type already");
		}
	}

	size_t Datum::Size() const
	{
		return mSize;
	}

	size_t Datum::Capacity() const
	{
		return mCapacity;
	}

	bool Datum::IsInternal() const
	{
		return mIsInternal;
	}

	void Datum::SetSize(size_t size)
	{
		CheckHasType();

		if (size < mSize)
		{
			if (mIsInternal)
			{
				if (mType == DatumType::String)
				{
					for (size_t i = size; i < mSize; ++i)
					{
						mData.String[i].~string();
					}
				}
			}
		}
		else if (size > mSize)
		{
			CheckIsInternal();
			if (size > mCapacity)
			{
				AllocateMemory(size);
			}
			auto func = sCreateDefaultFunctions[static_cast<size_t>(mType)];
			assert(func != nullptr);
			(this->*func)(size);
		}
		mSize = size;
	}

	void Datum::Clear()
	{
		CheckIsInternal();
		if (mType == DatumType::String)
		{
			for (size_t i = 0; i < mSize; ++i)
			{
				mData.String[i].~string();
			}
		}
		mSize = 0;
	}

	void Datum::PopBack()
	{
		CheckIsInternal();
		if (mSize > 0)
		{
			if (mType == DatumType::String)
			{
				mData.String[mSize - 1].~string();
			}
			--mSize;
		}
	}

	void Datum::Reserve(size_t capacity)
	{
		CheckIsInternal();
		CheckHasType();

		if (capacity > mCapacity)
		{
			AllocateMemory(capacity);
		}
	}

	void Datum::ShrinkToFit()
	{
		CheckIsInternal();
		CheckHasType();

		if (mSize < mCapacity)
		{
			AllocateMemory(mSize);
		}
	}

	void Datum::PushBack(const int32_t& value)
	{
		PUSH_BACK_BODY(value, Integer, int32_t);
	}

	void Datum::PushBack(const float& value)
	{
		PUSH_BACK_BODY(value, Float, float);
	}

	void Datum::PushBack(const glm::vec4& value)
	{
		PUSH_BACK_BODY(value, Vector, glm::vec4);
	}

	void Datum::PushBack(const glm::mat4& value)
	{
		PUSH_BACK_BODY(value, Matrix, glm::mat4);
	}

	void Datum::PushBack(const std::string& value)
	{
		PUSH_BACK_BODY(value, String, std::string);
	}

	void Datum::PushBack(RTTI* const& value)
	{
		PUSH_BACK_BODY(value, Pointer, RTTI*);
	}

	void Datum::PushBack(const Scope& value)
	{
		Scope* pointer = const_cast<Scope*>(&value);
		PUSH_BACK_BODY(pointer, Table, Scope*);
	}

	template<> int32_t& Datum::Front<int32_t>()
	{
		FRONT_BODY(Integer);
	}

	template<> float& Datum::Front<float>()
	{
		FRONT_BODY(Float);
	}

	template<> glm::vec4& Datum::Front<glm::vec4>()
	{
		FRONT_BODY(Vector);
	}

	template<> glm::mat4& Datum::Front<glm::mat4>()
	{
		FRONT_BODY(Matrix);
	}

	template<> std::string& Datum::Front<std::string>()
	{
		FRONT_BODY(String);
	}

	template<> RTTI*& Datum::Front<RTTI*>()
	{
		FRONT_BODY(Pointer);
	}

	template<> const int32_t& Datum::Front<int32_t>() const
	{
		FRONT_BODY(Integer);
	}

	template<> const float& Datum::Front<float>() const
	{
		FRONT_BODY(Float);
	}

	template<> const glm::vec4& Datum::Front<glm::vec4>() const
	{
		FRONT_BODY(Vector);
	}

	template<> const glm::mat4& Datum::Front<glm::mat4>() const
	{
		FRONT_BODY(Matrix);
	}

	template<> const std::string& Datum::Front<std::string>() const
	{
		FRONT_BODY(String);
	}

	template<> RTTI* const& Datum::Front<RTTI*>() const
	{
		FRONT_BODY(Pointer);
	}

	template<> int32_t& Datum::Back()
	{
		BACK_BODY(Integer);
	}

	template<> float& Datum::Back()
	{
		BACK_BODY(Float);
	}

	template<> glm::vec4& Datum::Back()
	{
		BACK_BODY(Vector);
	}

	template<> glm::mat4& Datum::Back()
	{
		BACK_BODY(Matrix);
	}

	template<> std::string& Datum::Back()
	{
		BACK_BODY(String);
	}

	template<> RTTI*& Datum::Back()
	{
		BACK_BODY(Pointer);
	}

	template<> const int32_t& Datum::Back() const
	{
		BACK_BODY(Integer);
	}

	template<> const float& Datum::Back() const
	{
		BACK_BODY(Float);
	}

	template<> const glm::vec4& Datum::Back() const
	{
		BACK_BODY(Vector);
	}

	template<> const glm::mat4& Datum::Back() const
	{
		BACK_BODY(Matrix);
	}

	template<> const std::string& Datum::Back() const
	{
		BACK_BODY(String);
	}

	template<> RTTI* const& Datum::Back() const
	{
		BACK_BODY(Pointer);
	}

	bool Datum::SetFromString(const std::string& str, size_t index)
	{
		switch (mType)
		{
		case DatumType::Integer:
			return SetFromStringInt(str, index);
		case DatumType::Float:
			return SetFromStringFloat(str, index);
		case DatumType::Vector:
			return SetFromStringVector(str, index);
		case DatumType::Matrix:
			return SetFromStringMatrix(str, index);
		case DatumType::String:
			return SetFromStringString(str, index);
		default:
			throw std::exception("Set from string invalid type");
		}
	}

	std::string Datum::ToString(size_t index) const
	{
		CheckHasType();
		std::string result;
		switch (mType)
		{
		case DatumType::Integer:
			result = std::to_string(AsInt(index));
			break;
		case DatumType::Float:
			result = std::to_string(AsFloat(index));
			break;
		case DatumType::Vector:
			result = glm::to_string(AsVector(index));
			break;
		case DatumType::Matrix:
			result = glm::to_string(AsMatrix(index));
			break;
		case DatumType::String:
			result.append("\"");
			result.append(AsString(index));
			result.append("\"");
			break;
		case DatumType::Pointer:
			result = AsPointer(index)->ToString();
			break;
		case DatumType::Table:
			result = reinterpret_cast<RTTI*>(const_cast<Scope*>(&AsTable(index)))->ToString();
			break;
		default:
			break;
		}
		return result;
	}

	void Datum::SetStorage(void* address, size_t size)
	{
		if (address == nullptr)
		{
			throw std::exception("Set storage null pointer");
		}
		else if (mType == DatumType::Unknown)
		{
			throw std::exception("Set storage on incompatible type");
		}
		else if (size == 0)
		{
			throw std::exception("External storage can be empty");
		}
		ResetSelf();
		mIsInternal = false;
		mData.Universe = address;
		mSize = size;
		mCapacity = size;
	}

	void Datum::SetStorage(int32_t* value, size_t size)
	{
		SET_STORAGE_BODY(value, Integer, size);
	}

	void Datum::SetStorage(float* value, size_t size)
	{
		SET_STORAGE_BODY(value, Float, size);
	}

	void Datum::SetStorage(glm::vec4* value, size_t size)
	{
		SET_STORAGE_BODY(value, Vector, size);
	}

	void Datum::SetStorage(glm::mat4* value, size_t size)
	{
		SET_STORAGE_BODY(value, Matrix, size);
	}

	void Datum::SetStorage(std::string* value, size_t size)
	{
		SET_STORAGE_BODY(value, String, size);
	}

	void Datum::SetStorage(RTTI** value, size_t size)
	{
		SET_STORAGE_BODY(value, Pointer, size);
	}

	Datum::Iterator Datum::begin()
	{
		return Iterator(*this, 0);
	}

	Datum::Iterator Datum::end()
	{
		return Iterator(*this, mSize);
	}

	Datum::ConstIterator Datum::begin() const
	{
		return ConstIterator(*this, 0);
	}

	Datum::ConstIterator Datum::end() const
	{
		return ConstIterator(*this, mSize);
	}

	Datum::ConstIterator Datum::cbegin() const
	{
		return begin();
	}

	Datum::ConstIterator Datum::cend() const
	{
		return end();
	}
	
#pragma region Private
	bool Datum::SetFromStringInt(const std::string& str, size_t index)
	{
		int value;
		if (sscanf_s(str.c_str(), "%d", &value) == 1)
		{
			Set(value, index);
			return true;
		}
		return false;
	}

	bool Datum::SetFromStringFloat(const std::string& str, size_t index)
	{
		float value;
		if (sscanf_s(str.c_str(), "%f", &value) == 1)
		{
			Set(value, index);
			return true;
		}
		return false;
	}

	bool Datum::SetFromStringVector(const std::string& str, size_t index)
	{
		float v[4];
		if (sscanf_s(str.c_str(), "vec4(%f, %f, %f, %f)", &v[0], &v[1], &v[2], &v[3]) == 4)
		{
			Set(glm::vec4(v[0], v[1], v[2], v[3]), index);
			return true;
		}
		return false;
	}

	bool Datum::SetFromStringMatrix(const std::string& str, size_t index)
	{
		float v[16];
		if (sscanf_s(str.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
					 &v[0], &v[1], &v[2], &v[3],
					 &v[4], &v[5], &v[6], &v[7],
					 &v[8], &v[9], &v[10], &v[11],
					 &v[12], &v[13], &v[14], &v[15]) == 16)
		{
			Set(glm::mat4(v[0], v[1], v[2], v[3],
						  v[4], v[5], v[6], v[7],
						  v[8], v[9], v[10], v[11],
						  v[12], v[13], v[14], v[15]), index);
			return true;
		}
		return false;
	}

	bool Datum::SetFromStringString(const std::string& str, size_t index)
	{
		Set(str, index);
		return true;
	}

	void Datum::DeepCopy(const Datum& other)
	{
		mSize = other.mSize;
		mCapacity = other.mCapacity;
		mType = other.mType;
		mIsInternal = other.mIsInternal;

		if (other.mIsInternal)
		{
			AllocateMemory(mCapacity);
			auto func = sCopyFunctions[static_cast<size_t>(mType)];
			assert(func != nullptr);
			(this->*func)(other.mData.Universe);
		}
		else
		{
			mData = other.mData;
		}
	}

	inline void Datum::EnsureCapacity()
	{
		if (mSize == mCapacity)
		{
			AllocateMemory(std::max(size_t(1), mCapacity * 2));
		}
	}

	inline void Datum::CheckIsInternal() const
	{
		if (!mIsInternal)
		{
			throw std::exception("Can't modify external storage");
		}
	}

	inline void Datum::CheckHasType() const
	{
		if (mType == DatumType::Unknown)
		{
			throw std::exception("Can't perform action on datum without type");
		}
	}

	void Datum::AllocateMemory(size_t capacity)
	{
		if (capacity > 0)
		{
			mData.Universe = realloc(mData.Universe, capacity * sTypeSizeTable[static_cast<size_t>(mType)]);
		}
		else
		{
			free(mData.Integer);
			mData.Integer = nullptr;
		}
		mCapacity = capacity;
	}

	void Datum::ResetSelf()
	{
		if (mIsInternal)
		{
			Clear();
			free(mData.Integer);
		}
		mData.Integer = nullptr;
	}

	void Datum::DummyFunction() const
	{
		Front<int32_t>();
		Front<float>();
		Front<glm::vec4>();
		Front<glm::mat4>();
		Front<std::string>();
		Front<RTTI*>();
		Back<int32_t>();
		Back<float>();
		Back<glm::vec4>();
		Back<glm::mat4>();
		Back<std::string>();
		Back<RTTI*>();

		Datum* p = const_cast<Datum*>(this);
		p->Front<int32_t>();
		p->Front<float>();
		p->Front<glm::vec4>();
		p->Front<glm::mat4>();
		p->Front<std::string>();
		p->Front<RTTI*>();
		p->Back<int32_t>();
		p->Back<float>();
		p->Back<glm::vec4>();
		p->Back<glm::mat4>();
		p->Back<std::string>();
		p->Back<RTTI*>();
	}

	bool Datum::ComparePrimitive(void* data) const
	{
		return memcmp(mData.Universe, data, sTypeSizeTable[static_cast<size_t>(mType)] * mSize) == 0;
	}

	bool Datum::CompareString(void* data) const
	{
		bool equal = true;
		std::string* other = reinterpret_cast<std::string*>(data);
		for (size_t i = 0; i < mSize; ++i)
		{
			if (mData.String[i] != other[i])
			{
				equal = false;
				break;
			}
		}
		return equal;
	}

	bool Datum::ComparePointer(void* data) const
	{
		bool equal = true;
		RTTI** other = reinterpret_cast<RTTI**>(data);
		for (size_t i = 0; i < mSize; ++i)
		{
			RTTI* me = mData.Pointer[i];
			if (me != other[i] && (me == nullptr || !me->Equals(other[i])))
			{
				equal = false;
				break;
			}
		}
		return equal;
	}

	void Datum::CreateDefaultPrimitive(size_t size)
	{
		size_t eachSize = sTypeSizeTable[static_cast<size_t>(mType)];
		char* dest = reinterpret_cast<char*>(mData.Universe) + mSize * eachSize;
		memset(dest, 0, (size - mSize) * eachSize);
	}

	void Datum::CreateDefaultString(size_t size)
	{
		for (size_t i = mSize; i < size; ++i)
		{
			new(mData.String + i) string();
		}
	}

	void Datum::CopyPrimitive(void* data)
	{
		memcpy(mData.Universe, data, sTypeSizeTable[static_cast<size_t>(mType)] * mSize);
	}

	void Datum::CopyString(void* data)
	{
		string* other = reinterpret_cast<string*>(data);
		for (size_t i = 0; i < mSize; ++i)
		{
			new(mData.String + i) string(other[i]);
		}
	}
#pragma endregion

#pragma region Iterator
	Datum::Iterator::Iterator(const Datum& owner, const size_t& index) :
		mOwner(&owner),
		mIndex(index)
	{}

	Datum::Iterator& Datum::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Iterator doesn't have owner");
		}
		if (mIndex < mOwner->mSize)
		{
			++mIndex;
		}
		return *this;
	}

	Datum::Iterator Datum::Iterator::operator++(int)
	{
		Iterator copy = *this;
		operator++();
		return copy;
	}

	bool Datum::Iterator::operator==(const Iterator& other) const
	{
		return mOwner == other.mOwner && mIndex == other.mIndex;
	}

	bool Datum::Iterator::operator!=(const Iterator& other) const
	{
		return !operator==(other);
	}

	void* Datum::Iterator::operator*()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Iterator doesn't have owner");
		}
		if (mIndex < mOwner->mSize)
		{
			switch (mOwner->mType)
			{
			case DatumType::Integer:
				return &mOwner->mData.Integer[mIndex];
			case DatumType::Float:
				return &mOwner->mData.Float[mIndex];
			case DatumType::Vector:
				return &mOwner->mData.Vector[mIndex];
			case DatumType::Matrix:
				return &mOwner->mData.Matrix[mIndex];
			case DatumType::String:
				return &mOwner->mData.String[mIndex];
			case DatumType::Pointer:
				return &mOwner->mData.Pointer[mIndex];
			default:
				return nullptr;
			}
		}
		else
		{
			return nullptr;
		}
	}

	int32_t& Datum::Iterator::AsInt()
	{
		ITERATOR_GET_BODY(Integer);
	}

	float& Datum::Iterator::AsFloat()
	{
		ITERATOR_GET_BODY(Float);
	}

	glm::vec4& Datum::Iterator::AsVector()
	{
		ITERATOR_GET_BODY(Vector);
	}

	glm::mat4& Datum::Iterator::AsMatrix()
	{
		ITERATOR_GET_BODY(Matrix);
	}

	std::string& Datum::Iterator::AsString()
	{
		ITERATOR_GET_BODY(String);
	}

	RTTI*& Datum::Iterator::AsPointer()
	{
		ITERATOR_GET_BODY(Pointer);
	}

	Scope& Datum::Iterator::AsTable()
	{
		if (mOwner == nullptr)										
		{															
			throw std::exception("Iterator doesn't have owner");	
		}															
		if (mOwner->mType != DatumType::Table)						
		{															
			throw std::exception("Incompatible iterator type");		
		}															
		if (mIndex >= mOwner->mSize)								
		{															
			throw std::exception("Iterator points to nothing");		
		}															
		return *mOwner->mData.Table[mIndex];
	}

	const int32_t& Datum::Iterator::AsInt() const
	{
		ITERATOR_GET_BODY(Integer);
	}

	const float& Datum::Iterator::AsFloat() const
	{
		ITERATOR_GET_BODY(Float);
	}

	const glm::vec4& Datum::Iterator::AsVector() const
	{
		ITERATOR_GET_BODY(Vector);
	}

	const glm::mat4& Datum::Iterator::AsMatrix() const
	{
		ITERATOR_GET_BODY(Matrix);
	}

	const std::string& Datum::Iterator::AsString() const
	{
		ITERATOR_GET_BODY(String);
	}

	RTTI* const& Datum::Iterator::AsPointer() const
	{
		ITERATOR_GET_BODY(Pointer);
	}

	const Scope& Datum::Iterator::AsTable() const
	{
		return const_cast<Iterator*>(this)->AsTable();
	}

	Datum::DatumType Datum::Iterator::Type() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Iterator doesn't have owner");
		}
		return mOwner->mType;
	}
#pragma endregion

#pragma region ConstIterator
	Datum::ConstIterator::ConstIterator(const Datum& owner, const size_t& index) :
		mOwner(&owner),
		mIndex(index)
	{}

	Datum::ConstIterator::ConstIterator(const Iterator& other) :
		mOwner(other.mOwner),
		mIndex(other.mIndex)
	{}

	Datum::ConstIterator& Datum::ConstIterator::operator=(const Iterator& other)
	{
		mOwner = other.mOwner;
		mIndex = other.mIndex;
		return *this;
	}

	bool Datum::ConstIterator::operator==(const ConstIterator& other) const
	{
		return mOwner == other.mOwner && mIndex == other.mIndex;
	}

	bool Datum::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return !operator==(other);
	}

	Datum::ConstIterator& Datum::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Iterator doesn't have owner");
		}
		if (mIndex < mOwner->mSize)
		{
			++mIndex;
		}
		return *this;
	}

	Datum::ConstIterator Datum::ConstIterator::operator++(int)
	{
		ConstIterator copy = *this;
		operator++();
		return copy;
	}

	const void* Datum::ConstIterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Iterator doesn't have owner");
		}
		if (mIndex < mOwner->mSize)
		{
			switch (mOwner->mType)
			{
			case DatumType::Integer:
				return &mOwner->mData.Integer[mIndex];
			case DatumType::Float:
				return &mOwner->mData.Float[mIndex];
			case DatumType::Vector:
				return &mOwner->mData.Vector[mIndex];
			case DatumType::Matrix:
				return &mOwner->mData.Matrix[mIndex];
			case DatumType::String:
				return &mOwner->mData.String[mIndex];
			case DatumType::Pointer:
				return &mOwner->mData.Pointer[mIndex];
			default:
				return nullptr;
			}
		}
		else
		{
			return nullptr;
		}
	}

	const int32_t& Datum::ConstIterator::AsInt() const
	{
		ITERATOR_GET_BODY(Integer);
	}

	const float& Datum::ConstIterator::AsFloat() const
	{
		ITERATOR_GET_BODY(Float);
	}

	const glm::vec4& Datum::ConstIterator::AsVector() const
	{
		ITERATOR_GET_BODY(Vector);
	}

	const glm::mat4& Datum::ConstIterator::AsMatrix() const
	{
		ITERATOR_GET_BODY(Matrix);
	}

	const std::string& Datum::ConstIterator::AsString() const
	{
		ITERATOR_GET_BODY(String);
	}

	RTTI* const& Datum::ConstIterator::AsPointer() const
	{
		ITERATOR_GET_BODY(Pointer);
	}

	const Scope& Datum::ConstIterator::AsTable() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Iterator doesn't have owner");
		}
		if (mOwner->mType != DatumType::Table)
		{
			throw std::exception("Incompatible iterator type");
		}
		if (mIndex >= mOwner->mSize)
		{
			throw std::exception("Iterator points to nothing");
		}
		return *mOwner->mData.Table[mIndex];
	}

	Datum::DatumType Datum::ConstIterator::Type() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Iterator doesn't have owner");
		}
		return mOwner->mType;
	}

#pragma endregion


}
