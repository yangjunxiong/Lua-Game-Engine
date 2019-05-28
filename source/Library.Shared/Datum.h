#pragma once
#include <string>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include "glm/fwd.hpp"
#include "vector.h"

namespace GameEngine
{
	class RTTI;
	class Scope;

	/// <summary>
	/// Datum is a data wrapper that can polymorphically represent different data types at runtime.
	/// Datum suppor int32_t, float, OpenGL vec4, OpenGL mat4, and RTTI*
	/// Datum can be regarded as a scalar or a homogeneous vector of supported data type
	/// Once type is set, datum can't change its type and will throw exception if user tries to use it as different data type.
	/// Datum can also serve as a wrapper for external storage
	/// </summary>
	class Datum final
	{
		friend Scope;

	public:
		/// <summary>
		/// Data types supported by Datum
		/// </summary>
		enum class DatumType
		{
			Unknown = 0,
			Integer,
			Float,
			Vector,
			Matrix,
			Table,
			String,
			Pointer,

			Begin = Integer,
			End = Pointer
		};

#pragma region Iterator
		/// <summary>
		/// An Iterator represents one item in datum, if the datum is treate as vector
		/// </summary>
		class Iterator final
		{
			friend Datum;
			friend class ConstIterator;

		public:
			/// <summary>
			/// Default constructor
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Copy constructor
			/// </summary>
			/// <param name="other">The other Iterator to copy from</param>
			Iterator(const Iterator& other) = default;

			/// <summary>
			/// Move constructor
			/// </summary>
			/// <param name="other">The other Iterator to move from</param>
			Iterator(Iterator&& other) = default;

			/// <summary>
			/// Desctructor
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// Copy assignment
			/// </summary>
			/// <param name="other">The other Iterator to copy from</param>
			/// <returns>This Iterator after copy</returns>
			Iterator& operator=(const Iterator& other) = default;

			/// <summary>
			/// Move assignment
			/// </summary>
			/// <param name="other">The other Iterator to move from</param>
			/// <returns>This Iterator after move</returns>
			Iterator& operator=(Iterator&& other) = default;

			/// <summary>
			/// Compare the content with another Iterator
			/// </summary>
			/// <param name="other">The other Iterator to compare with</param>
			/// <returns>True if two Iterators are equals, False otherwise</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// Compare the content with another Iterator
			/// </summary>
			/// <param name="other">The other Iterator to compare with</param>
			/// <returns>False if two Iterators are equals, True otherwise</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Pre-increment this Iterator to next item
			/// </summary>
			/// <returns>This Iterator after increment</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			Iterator& operator++();

			/// <summary>
			/// Post-increment this Iterator to next item
			/// </summary>
			/// <param name="">A signature to mark "post"</param>
			/// <returns>Copy of this Iterator before increment</returns>
			/// /// <exception cref="std::exception">Iterator has no owner</exception>
			Iterator operator++(int);

			/// <summary>
			/// Get the address of item this Iterator points to
			/// </summary>
			/// <returns>The address of item this Iterator points to</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			void* operator*();

			/// <summary>
			/// Return an int value pointed by the Iterator
			/// </summary>
			/// <returns>An int value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			int32_t& AsInt();

			/// <summary>
			/// Return an float value pointed by the Iterator
			/// </summary>
			/// <returns>An float value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			float& AsFloat();

			/// <summary>
			/// Return an vec4 value pointed by the Iterator
			/// </summary>
			/// <returns>An vec4 value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			glm::vec4& AsVector();

			/// <summary>
			/// Return an mat4 value pointed by the Iterator
			/// </summary>
			/// <returns>An mat4 value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			glm::mat4& AsMatrix();

			/// <summary>
			/// Return an string value pointed by the Iterator
			/// </summary>
			/// <returns>An string value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			std::string& AsString();

			/// <summary>
			/// Return an RTTI* value pointed by the Iterator
			/// </summary>
			/// <returns>An RTTI* value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			RTTI*& AsPointer();

			/// <summary>
			/// Return an Scope value pointed by the Iterator
			/// </summary>
			/// <returns>An Scope value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			Scope& AsTable();

			/// <summary>
			/// Get the address of current item
			/// </summary>
			/// <returns>The address of current item, nullptr if it points to nothing</returns>
			/// <exception cref="std::exception">Iterator doesn't have owner</exception>
			const void* operator*() const;

			/// <summary>
			/// Return an int value pointed by the Iterator
			/// </summary>
			/// <returns>An int value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			const int32_t& AsInt() const;

			/// <summary>
			/// Return an float value pointed by the Iterator
			/// </summary>
			/// <returns>An float value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			const float& AsFloat() const;

			/// <summary>
			/// Return an vec4 value pointed by the Iterator
			/// </summary>
			/// <returns>An vec4 value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			const glm::vec4& AsVector() const;

			/// <summary>
			/// Return an mat4 value pointed by the Iterator
			/// </summary>
			/// <returns>An mat4 value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			const glm::mat4& AsMatrix() const;

			/// <summary>
			/// Return an string value pointed by the Iterator
			/// </summary>
			/// <returns>An string value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			const std::string& AsString() const;

			/// <summary>
			/// Return an RTTI* value pointed by the Iterator
			/// </summary>
			/// <returns>An RTTI* value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			RTTI* const& AsPointer() const;

			/// <summary>
			/// Return an Scope value pointed by the Iterator
			/// </summary>
			/// <returns>An Scope value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			const Scope& AsTable() const;

			/// <summary>
			/// Get the type of the item iterator points to
			/// </summary>
			/// <returns>The type of item</returns>
			/// <exception cref="std::exception">Iterator doesn't have owner</exception>
			DatumType Type() const;

		private:
			/// <summary>
			/// Construct Iterator with owner and index
			/// </summary>
			/// <param name="owner">The Datum Iterator belongs to</param>
			/// <param name="index">The index of item in Datum</param>
			Iterator(const Datum& owner, const size_t& index);

			/// <summary>
			/// The Datum Iterator belongs to
			/// </summary>
			const Datum* mOwner = nullptr;

			/// <summary>
			/// The index of item in Datum
			/// </summary>
			size_t mIndex = 0;
		};

		/// <summary>
		/// ConstIterator is same as Iterator, except that it doesn't allow user to change the item it points to
		/// </summary>
		class ConstIterator final
		{
			friend Datum;

		public:
			/// <summary>
			/// Default constructor
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Copy constructor
			/// </summary>
			/// <param name="other">The other ConstIterator to copy from</param>
			ConstIterator(const ConstIterator& other) = default;

			/// <summary>
			/// Move constructor
			/// </summary>
			/// <param name="other">The other ConstIterator to move from</param>
			ConstIterator(ConstIterator&& other) = default;

			/// <summary>
			/// Copy constructor which promotes Iterator to ConstIterator
			/// </summary>
			/// <param name="other">The Iterator to copy from</param>
			ConstIterator(const Iterator& other);

			/// <summary>
			/// Destructor
			/// </summary>
			~ConstIterator() = default;

			/// <summary>
			/// Copy assignment
			/// </summary>
			/// <param name="other">The other ConstIterator to copy from</param>
			/// <returns>The ConstIterator after copy</returns>
			ConstIterator& operator=(const ConstIterator& other) = default;

			/// <summary>
			/// Move assignment
			/// </summary>
			/// <param name="other">The other ConstIterator to move from</param>
			/// <returns>The ConstIterator after move</returns>
			ConstIterator& operator=(ConstIterator&& other) = default;

			/// <summary>
			/// Copy assignment which promotes Iterator to ConstIterator
			/// </summary>
			/// <param name="other">The Iterator to copy from</param>
			/// <returns>This ConstIterator after copy</returns>
			ConstIterator& operator=(const Iterator& other);

			/// <summary>
			/// Compare the content with another Iterator
			/// </summary>
			/// <param name="other">The other Iterator to compare with</param>
			/// <returns>True if two Iterators are equals, False otherwise</returns>
			bool operator==(const ConstIterator& other) const;

			/// <summary>
			/// Compare the content with another Iterator
			/// </summary>
			/// <param name="other">The other Iterator to compare with</param>
			/// <returns>False if two Iterators are equals, True otherwise</returns>
			bool operator!=(const ConstIterator& other) const;

			/// <summary>
			/// Pre-increment this Iterator to next item
			/// </summary>
			/// <returns>This Iterator after increment</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			ConstIterator& operator++();

			/// <summary>
			/// Post-increment this Iterator to next item
			/// </summary>
			/// <param name="">A signature to mark "post"</param>
			/// <returns>Copy of this Iterator before increment</returns>
			/// /// <exception cref="std::exception">Iterator has no owner</exception>
			ConstIterator operator++(int);

			/// <summary>
			/// Get the address of current item
			/// </summary>
			/// <returns>The address of current item, nullptr if it points to nothing</returns>
			/// <exception cref="std::exception">Iterator doesn't have owner</exception>
			const void* operator*() const;

			/// <summary>
			/// Return an int value pointed by the Iterator
			/// </summary>
			/// <returns>An int value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			const int32_t& AsInt() const;

			/// <summary>
			/// Return an float value pointed by the Iterator
			/// </summary>
			/// <returns>An float value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			const float& AsFloat() const;

			/// <summary>
			/// Return an vec4 value pointed by the Iterator
			/// </summary>
			/// <returns>An vec4 value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			const glm::vec4& AsVector() const;

			/// <summary>
			/// Return an mat4 value pointed by the Iterator
			/// </summary>
			/// <returns>An mat4 value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			const glm::mat4& AsMatrix() const;

			/// <summary>
			/// Return an string value pointed by the Iterator
			/// </summary>
			/// <returns>An string value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			const std::string& AsString() const;

			/// <summary>
			/// Return an RTTI* value pointed by the Iterator
			/// </summary>
			/// <returns>An RTTI* value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			RTTI* const& AsPointer() const;

			/// <summary>
			/// Return an Scope value pointed by the Iterator
			/// </summary>
			/// <returns>An Scope value pointed by the Iterator</returns>
			/// <exception cref="std::exception">Iterator has no owner</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Iterator is end()</exception>
			const Scope& AsTable() const;

			/// <summary>
			/// Get the type of the item iterator points to
			/// </summary>
			/// <returns>The type of item</returns>
			/// <exception cref="std::exception">Iterator doesn't have owner</exception>
			DatumType Type() const;

		private:
			/// <summary>
			/// Construct Iterator with owner and index
			/// </summary>
			/// <param name="owner">The Datum Iterator belongs to</param>
			/// <param name="index">The index of item in Datum</param>
			ConstIterator(const Datum& owner, const size_t& index);

			/// <summary>
			/// The Datum Iterator belongs to
			/// </summary>
			const Datum* mOwner = nullptr;

			/// <summary>
			/// The index of item in Datum
			/// </summary>
			size_t mIndex = 0;
		};
#pragma endregion

#pragma region Public
		/// <summary>
		/// Default constructor, construct a Datum without type and without memory allocation
		/// </summary>
		Datum(DatumType type = DatumType::Unknown);

		/// <summary>
		/// Copy constructor, if other has internal storage, a deep copy will be performed, otherwise it will be shallow copy
		/// </summary>
		/// <param name="other">The other Datum to copy from</param>
		Datum(const Datum& other);

		/// <summary>
		/// Move constructor, if other has internal storage, a deep copy will be performed, otherwise it will be shallow copy.
		/// The other Datum will be set to invalid state
		/// </summary>
		/// <param name="other">The other Datum to move from</param>
		Datum(Datum&& other);

		/// <summary>
		/// Construct a datum with type = int32_t and initial value
		/// </summary>
		/// <param name="value">Initial int value</param>
		Datum(const int32_t& value);

		/// <summary>
		/// Construct a datum with type = float and initial value
		/// </summary>
		/// <param name="value">Initial float value</param>
		Datum(const float& value);

		/// <summary>
		/// Construct a datum with type = vec4 and initial value
		/// </summary>
		/// <param name="value">Initial vector value</param>
		Datum(const glm::vec4& value);

		/// <summary>
		/// Construct a datum with type = mat4 and initial value
		/// </summary>
		/// <param name="value">Initial matrix value</param>
		Datum(const glm::mat4& value);

		/// <summary>
		/// Construct a datum with type = string and initial value
		/// </summary>
		/// <param name="value">Initial string value</param>
		Datum(const std::string& value);

		/// <summary>
		/// Construct a datum with type = RTTI* and initial value
		/// </summary>
		/// <param name="value">Initial pointer value</param>
		Datum(RTTI* const& value);

		/// <summary>
		/// Construct a datum with type = Scope* and initial value
		/// </summary>
		/// <param name="value">Initial pointer value</param>
		Datum(const Scope& value);

		/// <summary>
		/// Construct a datum with type = int32_t and initial values
		/// </summary>
		/// <param name="value">Initial int value list</param>
		Datum(const std::initializer_list<int32_t>& list);

		/// <summary>
		/// Construct a datum with type = float and initial values
		/// </summary>
		/// <param name="value">Initial float value list</param>
		Datum(const std::initializer_list<float>& list);

		/// <summary>
		/// Construct a datum with type = vec4 and initial values
		/// </summary>
		/// <param name="value">Initial vec4 value list</param>
		Datum(const std::initializer_list<glm::vec4>& list);

		/// <summary>
		/// Construct a datum with type = mat4 and initial values
		/// </summary>
		/// <param name="value">Initial mat4 value list</param>
		Datum(const std::initializer_list<glm::mat4>& list);

		/// <summary>
		/// Construct a datum with type = string and initial values
		/// </summary>
		/// <param name="value">Initial string value list</param>
		Datum(const std::initializer_list<std::string>& list);

		/// <summary>
		/// Construct a datum with type = RTTI* and initial values
		/// </summary>
		/// <param name="value">Initial pointer value list</param>
		Datum(const std::initializer_list<RTTI*>& list);

		/// <summary>
		/// Copy assignment, make this Datum exactly the same as other
		/// If this Datum has internal storage, all memory will be freed before copying
		/// If other has internal storage, a deep copy will be performed, otherwise it will be shallow copy
		/// </summary>
		/// <param name="other">The other Datum to copy from</param>
		/// <returns>Datum after copying</returns>
		Datum& operator=(const Datum& other);

		/// <summary>
		/// Move assignment, make this Datum exactly the same as other and invalidate other
		/// If this Datum has internal storage, all memory will be freed before moving
		/// If other has internal storage, a deep copy will be performed, otherwise it will be shallow copy
		/// </summary>
		/// <param name="other">The other Datum to move from</param>
		/// <returns>Datum after moving</returns>
		Datum& operator=(Datum&& other);

		/// <summary>
		/// Assign an int value to the first item of Datum.
		/// If the datum is uninitialized, will set Datum type to int32_t.
		/// If datum is of type int32_t but has no element, it will push the value at first slot.
		/// If datum is initialized but is not of type int32_t, exception will be thrown
		/// </summary>
		/// <param name="value">int value to set</param>
		/// <returns>Datum after setting</returns>
		/// <exception cref="std::exception">Datum has incompatible type</exception>
		/// <exception cref="std::exception">Datum is empty and is using external storage</exception>
		Datum& operator=(const int32_t& value);

		/// <summary>
		/// Assign a float value to the first item of Datum.
		/// If the datum is uninitialized, will set Datum type to float.
		/// If datum is of type float but has no element, it will push the value at first slot.
		/// If datum is initialized but is not of type float, exception will be thrown
		/// </summary>
		/// <param name="value">float value to set</param>
		/// <returns>Datum after setting</returns>
		/// <exception cref="std::exception">Datum has incompatible type</exception>
		/// <exception cref="std::exception">Datum is empty and is using external storage</exception>
		Datum& operator=(const float& value);

		/// <summary>
		/// Assign a vec4 value to the first item of Datum.
		/// If the datum is uninitialized, will set Datum type to vec4.
		/// If datum is of type vec4 but has no element, it will push the value at first slot.
		/// If datum is initialized but is not of type vec4, exception will be thrown
		/// </summary>
		/// <param name="value">vec4 value to set</param>
		/// <returns>Datum after setting</returns>
		/// <exception cref="std::exception">Datum has incompatible type</exception>
		/// <exception cref="std::exception">Datum is empty and is using external storage</exception>
		Datum& operator=(const glm::vec4& value);

		/// <summary>
		/// Assign a mat4 value to the first item of Datum.
		/// If the datum is uninitialized, will set Datum type to mat4.
		/// If datum is of type mat4 but has no element, it will push the value at first slot.
		/// If datum is initialized but is not of type mat4, exception will be thrown
		/// </summary>
		/// <param name="value">mat4 value to set</param>
		/// <returns>Datum after setting</returns>
		/// <exception cref="std::exception">Datum has incompatible type</exception>
		/// <exception cref="std::exception">Datum is empty and is using external storage</exception>
		Datum& operator=(const glm::mat4& value);

		/// <summary>
		/// Assign a string value to the first item of Datum.
		/// If the datum is uninitialized, will set Datum type to string.
		/// If datum is of type string but has no element, it will push the value at first slot.
		/// If datum is initialized but is not of type string, exception will be thrown
		/// </summary>
		/// <param name="value">string value to set</param>
		/// <returns>Datum after setting</returns>
		/// <exception cref="std::exception">Datum has incompatible type</exception>
		/// <exception cref="std::exception">Datum is empty and is using external storage</exception>
		Datum& operator=(const std::string& value);

		/// <summary>
		/// Assign a RTTI* value to the first item of Datum.
		/// If the datum is uninitialized, will set Datum type to RTTI*.
		/// If datum is of type RTTI* but has no element, it will push the value at first slot.
		/// If datum is initialized but is not of type RTTI*, exception will be thrown
		/// </summary>
		/// <param name="value">RTTI* value to set</param>
		/// <returns>Datum after setting</returns>
		/// <exception cref="std::exception">Datum has incompatible type</exception>
		/// <exception cref="std::exception">Datum is empty and is using external storage</exception>
		Datum& operator=(RTTI* const& value);

		private:
			/// <summary>
			/// Assign a Scope* value to the first item of Datum.
			/// If the datum is uninitialized, will set Datum type to Scope*.
			/// If datum is of type Scope* but has no element, it will push the value at first slot.
			/// If datum is initialized but is not of type Scope*, exception will be thrown
			/// </summary>
			/// <param name="value">RTTI* value to set</param>
			/// <returns>Datum after setting</returns>
			/// <exception cref="std::exception">Datum has incompatible type</exception>
			/// <exception cref="std::exception">Datum is empty and is using external storage</exception>
			Datum& operator=(const Scope& value);

		public:
			/// <summary>
			/// Compare this Datum with other.
			/// Compare each element with their equality operator, two datums are equals if they have same type, same size, and all elements are equal
			/// </summary>
			/// <param name="other">The other Datum to compare with</param>
			/// <returns>True if both datums are equal, False otherwise</returns>
			bool operator==(const Datum& other) const;

			/// <summary>
			/// Compare this Datum with other.
			/// Compare each element with their equality operator, two datums are equals if they have same type, same size, and all elements are equal
			/// </summary>
			/// <param name="other">The other Datum to compare with</param>
			/// <returns>True if both datums are equal, False otherwise</returns>
			bool operator!=(const Datum& other) const;

			/// <summary>
			/// Compare the first element of datum with value
			/// </summary>
			/// <param name="value">The int value to compare with</param>
			/// <returns>True if first element equals to value, False otherwise</returns>
			bool operator==(const int32_t& value) const;

			/// <summary>
			/// Compare the first element of datum with value
			/// </summary>
			/// <param name="value">The float value to compare with</param>
			/// <returns>True if first element equals to value, False otherwise</returns>
			bool operator==(const float& value) const;

			/// <summary>
			/// Compare the first element of datum with value
			/// </summary>
			/// <param name="value">The vec4 value to compare with</param>
			/// <returns>True if first element equals to value, False otherwise</returns>
			bool operator==(const glm::vec4& value) const;

			/// <summary>
			/// Compare the first element of datum with value
			/// </summary>
			/// <param name="value">The mat4 value to compare with</param>
			/// <returns>True if first element equals to value, False otherwise</returns>
			bool operator==(const glm::mat4& value) const;

			/// <summary>
			/// Compare the first element of datum with value
			/// </summary>
			/// <param name="value">The string value to compare with</param>
			/// <returns>True if first element equals to value, False otherwise</returns>
			bool operator==(const std::string& value) const;

			/// <summary>
			/// Compare the first element of datum with value
			/// </summary>
			/// <param name="value">The RTTI* value to compare with</param>
			/// <returns>True if first element equals to value, False otherwise</returns>
			bool operator==(const RTTI* const& value) const;

			/// <summary>
			/// Compare the first element of datum with value
			/// </summary>
			/// <param name="value">The Scope value to compare with</param>
			/// <returns>True if first element equals to value, False otherwise</returns>
			bool operator==(const Scope& value) const;

			/// <summary>
			/// Compare the first element of datum with value
			/// </summary>
			/// <param name="value">The int value to compare with</param>
			/// <returns>False if first element equals to value, True otherwise</returns>
			bool operator!=(const int32_t& value) const;

			/// <summary>
			/// Compare the first element of datum with value
			/// </summary>
			/// <param name="value">The float value to compare with</param>
			/// <returns>False if first element equals to value, True otherwise</returns>
			bool operator!=(const float& value) const;

			/// <summary>
			/// Compare the first element of datum with value
			/// </summary>
			/// <param name="value">The vec4 value to compare with</param>
			/// <returns>False if first element equals to value, True otherwise</returns>
			bool operator!=(const glm::vec4& value) const;

			/// <summary>
			/// Compare the first element of datum with value
			/// </summary>
			/// <param name="value">The mat4 value to compare with</param>
			/// <returns>False if first element equals to value, True otherwise</returns>
			bool operator!=(const glm::mat4& value) const;

			/// <summary>
			/// Compare the first element of datum with value
			/// </summary>
			/// <param name="value">The string value to compare with</param>
			/// <returns>False if first element equals to value, True otherwise</returns>
			bool operator!=(const std::string& value) const;

			/// <summary>
			/// Compare the first element of datum with value
			/// </summary>
			/// <param name="value">The RTTI* value to compare with</param>
			/// <returns>False if first element equals to value, True otherwise</returns>
			bool operator!=(const RTTI *const& value) const;

			/// <summary>
			/// Compare the first element of datum with value
			/// </summary>
			/// <param name="value">The Scope value to compare with</param>
			/// <returns>True if first element equals to value, False otherwise</returns>
			bool operator!=(const Scope& value) const;

			/// <summary>
			/// Get the nested scope by index, if this Datum is Table type
			/// </summary>
			/// <param name="index">Index of scope</param>
			/// <returns>The nested at given index</returns>
			/// <exception cref="std::exception">This Datum is not table type</exception>
			/// <exception cref="std::exception">Index out of range</exception>
			Scope& operator[](std::uint32_t index);

			/// <summary>
			/// Get the nested scope by index, if this Datum is Table type
			/// </summary>
			/// <param name="index">Index of scope</param>
			/// <returns>The nested at given index</returns>
			/// <exception cref="std::exception">This Datum is not table type</exception>
			/// <exception cref="std::exception">Index out of range</exception>
			const Scope& operator[](std::uint32_t index) const;

			/// <summary>
			/// Add another datum and return the result.
			/// Works for int, float, vector, matrix and string
			/// </summary>
			/// <param name="other">Other datum to add</param>
			/// <returns>Sum of two datums</returns>
			Datum operator+(const Datum& other) const;

			/// <summary>
			/// Subtract another datum from this one and return the result.
			/// Works for int, float, vector, matrix
			/// </summary>
			/// <param name="other">Other datum to subtract</param>
			/// <returns>Datum after subtraction</returns>
			Datum operator-(const Datum& other) const;

			/// <summary>
			/// Multiply with another datum and return the result.
			/// Works for int, float, vector, matrix
			/// </summary>
			/// <param name="other">Other datum to multiply with</param>
			/// <returns>Datum after multipilication</returns>
			Datum operator*(const Datum& other) const;

			/// <summary>
			/// Divide this datum by another one
			/// Works for int, float, vector, matrix
			/// </summary>
			/// <param name="other">Other datum to divide</param>
			/// <returns>Datum after dividing</returns>
			Datum operator/(const Datum& other) const;

			/// <summary>
			/// Get the modulus.
			/// Works for int and float
			/// </summary>
			/// <param name="other">Dividend</param>
			/// <returns>Datum after modulus</returns>
			Datum operator%(const Datum& other) const;

			/// <summary>
			/// Negate this datum, works for int and float
			/// </summary>
			/// <returns>This datum after negating</returns>
			Datum operator-() const;

			/// <summary>
			/// Get the bool representation of this datum
			/// Works only if this datum is int or float. 0 is false, other values are true
			/// </summary>
			/// <returns>Bool representation of this datum</returns>
			operator bool() const;

			/// <summary>
			/// Negate this datum logically
			/// Works only if this datum is int or float.
			/// </summary>
			/// <returns>The datum value after logical flip</returns>
			Datum operator!() const;

			/// <summary>
			/// Compare this datum with another
			/// Works for int and float
			/// </summary>
			/// <param name="other">Other datum to compare with</param>
			/// <returns>True if this datum value is larger than the other, False otherwise</returns>
			bool operator>(const Datum& other) const;

			/// <summary>
			/// Compare this datum with another
			/// Works for int and float
			/// </summary>
			/// <param name="other">Other datum to compare with</param>
			/// <returns>True if this datum value is larger or equal than the other, False otherwise</returns>
			bool operator>=(const Datum& other) const;

			/// <summary>
			/// Compare this datum with another
			/// Works for int and float
			/// </summary>
			/// <param name="other">Other datum to compare with</param>
			/// <returns>True if this datum value is less than the other, False otherwise</returns>
			bool operator<(const Datum& other) const;

			/// <summary>
			/// Compare this datum with another
			/// Works for int and float
			/// </summary>
			/// <param name="other">Other datum to compare with</param>
			/// <returns>True if this datum value is less or equal than the other, False otherwise</returns>
			bool operator<=(const Datum& other) const;

			/// <summary>
			/// Perform logical "AND" on two datums
			/// Works only if both are int or float type
			/// </summary>
			/// <param name="other">Other datum to and</param>
			/// <returns>The bool result after and</returns>
			bool operator&&(const Datum& other) const;

			/// <summary>
			/// Perform logical "OR" on two datums
			/// Works only if both are int or float type
			/// </summary>
			/// <param name="other">Other datum to or</param>
			/// <returns>The bool result after or</returns>
			bool operator||(const Datum& other) const;

			/// <summary>
			/// Desctructor, free memory if Datum has internal storage
			/// </summary>
			~Datum();

			/// <summary>
			/// Copy the raw value from another datum into this one, no matter it's internal or external
			/// This function doesn't not make any type of check, so use with care
			/// </summary>
			/// <param name="other">Other datum to copy from</param>
			void CopyValue(const Datum& other);

			/// <summary>
			/// Get the type of Datum
			/// </summary>
			/// <returns>Type of datum</returns>
			DatumType Type() const;

			/// <summary>
			/// Set the type for uninitialized datum
			/// </summary>
			/// <param name="type">Type of datum</param>
			/// <exception cref="std::exception">Datum has a type</exception>
			void SetType(DatumType type);

			/// <summary>
			/// Get the number elements in datum
			/// </summary>
			/// <returns>Number of elements in datum</returns>
			size_t Size() const;

			/// <summary>
			/// Get the number of possible slots to hold element
			/// </summary>
			/// <returns>Number of possible slots to hold element</returns>
			size_t Capacity() const;

			/// <summary>
			/// Check if Datum is internal storage
			/// </summary>
			/// <returns>True if Datum has internal storage, False otherwise</returns>
			bool IsInternal() const;

			/// <summary>
			/// Set number of elements for Datum.
			/// If size is smaller than current size, remaining elements will be desctructed.
			/// If size is larger than current size, new elements will be default constructed, capacity will grow in case new size is larger than capacity
			/// </summary>
			/// <param name="size">New number of elements</param>
			/// <exception cref="std::exception">Datum has external storage</exception>
			/// <exception cref="std::exception">Datum has no type</exception>
			void SetSize(size_t size);

			/// <summary>
			/// Destruct all elements in datum, but keep capacity
			/// </summary>
			/// <exception cref="std::exception">Datum has external storage</exception>
			void Clear();

			/// <summary>
			/// Allocate new slots for capacity but don't construct objects
			/// </summary>
			/// <param name="capacity">New capacity</param>
			/// <exception cref="std::exception">Datum has external storage</exception>
			/// <exception cref="std::exception">Datum has no type</exception>
			void Reserve(size_t capacity);

			/// <summary>
			/// Free unused memory so that capacity = size
			/// </summary>
			/// <exception cref="std::exception">Datum has external storage</exception>
			/// <exception cref="std::exception">Datum has no type</exception>
			void ShrinkToFit();

			/// <summary>
			/// Append new element to the end of Datum.
			/// If Datum has no type, will set type to int32_t
			/// </summary>
			/// <param name="value">new element to append</param>
			/// <exception cref="std::exception">Datum has external storage</exception>
			/// <exception cref="std::exception">Datum has type other than int32_t</exception>
			void PushBack(const int32_t& value);

			/// <summary>
			/// Append new element to the end of Datum.
			/// If Datum has no type, will set type to float
			/// </summary>
			/// <param name="value">new element to append</param>
			/// <exception cref="std::exception">Datum has external storage</exception>
			/// <exception cref="std::exception">Datum has type other than float</exception>
			void PushBack(const float& value);

			/// <summary>
			/// Append new element to the end of Datum.
			/// If Datum has no type, will set type to vec4
			/// </summary>
			/// <param name="value">new element to append</param>
			/// <exception cref="std::exception">Datum has external storage</exception>
			/// <exception cref="std::exception">Datum has type other than vec4</exception>
			void PushBack(const glm::vec4& value);

			/// <summary>
			/// Append new element to the end of Datum.
			/// If Datum has no type, will set type to mat4
			/// </summary>
			/// <param name="value">new element to append</param>
			/// <exception cref="std::exception">Datum has external storage</exception>
			/// <exception cref="std::exception">Datum has type other than mat4</exception>
			void PushBack(const glm::mat4& value);

			/// <summary>
			/// Append new element to the end of Datum.
			/// If Datum has no type, will set type to string
			/// </summary>
			/// <param name="value">new element to append</param>
			/// <exception cref="std::exception">Datum has external storage</exception>
			/// <exception cref="std::exception">Datum has type other than string</exception>
			void PushBack(const std::string& value);

			/// <summary>
			/// Append new element to the end of Datum.
			/// If Datum has no type, will set type to RTTI*
			/// </summary>
			/// <param name="value">new element to append</param>
			/// <exception cref="std::exception">Datum has external storage</exception>
			/// <exception cref="std::exception">Datum has type other than RTTI*</exception>
			void PushBack(RTTI *const& value);

		private:
			/// <summary>
			/// Append new element to the end of Datum.
			/// If Datum has no type, will set type to Scope*
			/// </summary>
			/// <param name="value">new element to append</param>
			/// <exception cref="std::exception">Datum has external storage</exception>
			/// <exception cref="std::exception">Datum has type other than Scope*</exception>
			void PushBack(const Scope& value);

		public:
			/// <summary>
			/// Remove the last element
			/// </summary>
			/// <exception cref="std::exception">Datum has external storage</exception>
			void PopBack();

			/// <summary>
			/// Get the first element
			/// </summary>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Datum is empty</exception>
			template <typename T>
			T& Front() { static_assert(false, "Unsupported data type"); }

			/// <summary>
			/// Get the first element
			/// </summary>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Datum is empty</exception>
			template <typename T>
			const T& Front() const { static_assert(false, "Unsupported data type"); }

			/// <summary>
			/// Get the last element
			/// </summary>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Datum is empty</exception>
			template<typename T>
			T& Back() { static_assert(false, "Unsupported data type"); }

			/// <summary>
			/// Get the last element
			/// </summary>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Datum is empty</exception>
			template <typename T>
			const T& Back() const { static_assert(false, "Unsupported data type"); }

			/// <summary>
			/// Set the value to specific position
			/// </summary>
			/// <param name="value">The value to set</param>
			/// <param name="index">Position</param>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than datum size</exception>
			void Set(const int32_t& value, size_t index = 0);

			/// <summary>
			/// Set the value to specific position
			/// </summary>
			/// <param name="value">The value to set</param>
			/// <param name="index">Position</param>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than datum size</exception>
			void Set(const float& value, size_t index = 0);

			/// <summary>
			/// Set the value to specific position
			/// </summary>
			/// <param name="value">The value to set</param>
			/// <param name="index">Position</param>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than datum size</exception>
			void Set(const glm::vec4& value, size_t index = 0);

			/// <summary>
			/// Set the value to specific position
			/// </summary>
			/// <param name="value">The value to set</param>
			/// <param name="index">Position</param>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than datum size</exception>
			void Set(const glm::mat4& value, size_t index = 0);

			/// <summary>
			/// Set the value to specific position
			/// </summary>
			/// <param name="value">The value to set</param>
			/// <param name="index">Position</param>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than datum size</exception>
			void Set(const std::string& value, size_t index = 0);

			/// <summary>
			/// Set the value to specific position
			/// </summary>
			/// <param name="value">The value to set</param>
			/// <param name="index">Position</param>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than datum size</exception>
			void Set(RTTI* const& value, size_t index = 0);

			/// <summary>
			/// Set the value to specific position
			/// </summary>
			/// <param name="value">The value to set</param>
			/// <param name="index">Position</param>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than datum size</exception>
			void Set(const Scope& value, size_t index = 0);

			/// <summary>
			/// Get the value at specific position as int
			/// </summary>
			/// <param name="index">Position</param>
			/// <returns>Int value at position</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than Datum size</exception>
			int32_t& AsInt(size_t index = 0);

			/// <summary>
			/// Get the value at specific position as float
			/// </summary>
			/// <param name="index">Position</param>
			/// <returns>Float value at position</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than Datum size</exception>
			float& AsFloat(size_t index = 0);

			/// <summary>
			/// Get the value at specific position as vec4
			/// </summary>
			/// <param name="index">Position</param>
			/// <returns>Vector value at position</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than Datum size</exception>
			glm::vec4& AsVector(size_t index = 0);

			/// <summary>
			/// Get the value at specific position as mat4
			/// </summary>
			/// <param name="index">Position</param>
			/// <returns>Matrix value at position</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than Datum size</exception>
			glm::mat4& AsMatrix(size_t index = 0);

			/// <summary>
			/// Get the value at specific position as string
			/// </summary>
			/// <param name="index">Position</param>
			/// <returns>String value at position</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than Datum size</exception>
			std::string& AsString(size_t index = 0);

			/// <summary>
			/// Get the value at specific position as RTTI*
			/// </summary>
			/// <param name="index">Position</param>
			/// <returns>RTTI* value at position</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than Datum size</exception>
			RTTI*& AsPointer(size_t index = 0);

			/// <summary>
			/// Get the value at specific position as Scope*
			/// </summary>
			/// <param name="index">Position</param>
			/// <returns>Scope* value at position</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than Datum size</exception>
			Scope& AsTable(size_t index = 0);

			/// <summary>
			/// Get the value at specific position as int
			/// </summary>
			/// <param name="index">Position</param>
			/// <returns>Int value at position</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than Datum size</exception>
			const int32_t& AsInt(size_t index = 0) const;

			/// <summary>
			/// Get the value at specific position as float
			/// </summary>
			/// <param name="index">Position</param>
			/// <returns>Float value at position</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than Datum size</exception>
			const float& AsFloat(size_t index = 0) const;

			/// <summary>
			/// Get the value at specific position as vec4
			/// </summary>
			/// <param name="index">Position</param>
			/// <returns>Vector value at position</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than Datum size</exception>
			const glm::vec4& AsVector(size_t index = 0) const;

			/// <summary>
			/// Get the value at specific position as mat4
			/// </summary>
			/// <param name="index">Position</param>
			/// <returns>Matrix value at position</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than Datum size</exception>
			const glm::mat4& AsMatrix(size_t index = 0) const;

			/// <summary>
			/// Get the value at specific position as string
			/// </summary>
			/// <param name="index">Position</param>
			/// <returns>String value at position</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than Datum size</exception>
			const std::string& AsString(size_t index = 0) const;

			/// <summary>
			/// Get the value at specific position as RTTI*
			/// </summary>
			/// <param name="index">Position</param>
			/// <returns>RTTI* value at position</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than Datum size</exception>
			RTTI* const& AsPointer(size_t index = 0) const;

			/// <summary>
			/// Get the value at specific position as Scope*
			/// </summary>
			/// <param name="index">Position</param>
			/// <returns>Scope* value at position</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			/// <exception cref="std::exception">Index larger than Datum size</exception>
			const Scope& AsTable(size_t index = 0) const;

			/// <summary>
			/// Remove the first element equal to value
			/// </summary>
			/// <param name="value">The value to remove</param>
			/// <returns>True if remove succeeds, False otherwise</returns>
			/// <exception cref="std::exception">Datum has external storage</exception>
			bool Remove(const int32_t& value);

			/// <summary>
			/// Remove the first element equal to value
			/// </summary>
			/// <param name="value">The value to remove</param>
			/// <returns>True if remove succeeds, False otherwise</returns>
			/// <exception cref="std::exception">Datum has external storage</exception>
			bool Remove(const float& value);

			/// <summary>
			/// Remove the first element equal to value
			/// </summary>
			/// <param name="value">The value to remove</param>
			/// <returns>True if remove succeeds, False otherwise</returns>
			/// <exception cref="std::exception">Datum has external storage</exception>
			bool Remove(const glm::vec4& value);

			/// <summary>
			/// Remove the first element equal to value
			/// </summary>
			/// <param name="value">The value to remove</param>
			/// <returns>True if remove succeeds, False otherwise</returns>
			/// <exception cref="std::exception">Datum has external storage</exception>
			bool Remove(const glm::mat4& value);

			/// <summary>
			/// Remove the first element equal to value
			/// </summary>
			/// <param name="value">The value to remove</param>
			/// <returns>True if remove succeeds, False otherwise</returns>
			/// <exception cref="std::exception">Datum has external storage</exception>
			bool Remove(const std::string& value);

			/// <summary>
			/// Remove the first element equal to value
			/// </summary>
			/// <param name="value">The value to remove</param>
			/// <returns>True if remove succeeds, False otherwise</returns>
			/// <exception cref="std::exception">Datum has external storage</exception>
			bool Remove(const RTTI* const& value);

			/// <summary>
			/// Remove the element at specific position
			/// </summary>
			/// <param name="index">The position of element</param>
			/// <returns>True if remove succeeds, False otherwise</returns>
			/// <exception cref="std::exception">Datum has external storage</exception>
			bool RemoveAt(size_t index);

			/// <summary>
			/// Remove the element pointed by Iterator
			/// </summary>
			/// <param name="it">Iterator pointing to the element to be removed</param>
			/// <returns>True if remove succeeds, False otherwise</returns>
			/// <exception cref="std::exception">Datum has external storage</exception>
			/// <exception cref="std::exception">Iterator doesn't belong to this Datum</exception>
			bool RemoveAt(const Iterator& it);

			/// <summary>
			/// Search for the element matching given value
			/// </summary>
			/// <param name="value">The value to search</param>
			/// <returns>Iterator pointing to the found element, or end() if not found</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			Iterator Find(const int32_t& value);

			/// <summary>
			/// Search for the element matching given value
			/// </summary>
			/// <param name="value">The value to search</param>
			/// <returns>Iterator pointing to the found element, or end() if not found</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			Iterator Find(const float& value);

			/// <summary>
			/// Search for the element matching given value
			/// </summary>
			/// <param name="value">The value to search</param>
			/// <returns>Iterator pointing to the found element, or end() if not found</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			Iterator Find(const glm::vec4& value);

			/// <summary>
			/// Search for the element matching given value
			/// </summary>
			/// <param name="value">The value to search</param>
			/// <returns>Iterator pointing to the found element, or end() if not found</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			Iterator Find(const glm::mat4& value);

			/// <summary>
			/// Search for the element matching given value
			/// </summary>
			/// <param name="value">The value to search</param>
			/// <returns>Iterator pointing to the found element, or end() if not found</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			Iterator Find(const std::string& value);

			/// <summary>
			/// Search for the element matching given value
			/// </summary>
			/// <param name="value">The value to search</param>
			/// <returns>Iterator pointing to the found element, or end() if not found</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			Iterator Find(const RTTI* const& value);

			/// <summary>
			/// Search for the element matching given value
			/// </summary>
			/// <param name="value">The value to search</param>
			/// <returns>Iterator pointing to the found element, or end() if not found</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			Iterator Find(const Scope& value);

			/// <summary>
			/// Search for the element matching given value
			/// </summary>
			/// <param name="value">The value to search</param>
			/// <returns>Iterator pointing to the found element, or end() if not found</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			ConstIterator Find(const int32_t& value) const;

			/// <summary>
			/// Search for the element matching given value
			/// </summary>
			/// <param name="value">The value to search</param>
			/// <returns>Iterator pointing to the found element, or end() if not found</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			ConstIterator Find(const float& value) const;

			/// <summary>
			/// Search for the element matching given value
			/// </summary>
			/// <param name="value">The value to search</param>
			/// <returns>Iterator pointing to the found element, or end() if not found</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			ConstIterator Find(const glm::vec4& value) const;

			/// <summary>
			/// Search for the element matching given value
			/// </summary>
			/// <param name="value">The value to search</param>
			/// <returns>Iterator pointing to the found element, or end() if not found</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			ConstIterator Find(const glm::mat4& value) const;

			/// <summary>
			/// Search for the element matching given value
			/// </summary>
			/// <param name="value">The value to search</param>
			/// <returns>Iterator pointing to the found element, or end() if not found</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			ConstIterator Find(const std::string& value) const;

			/// <summary>
			/// Search for the element matching given value
			/// </summary>
			/// <param name="value">The value to search</param>
			/// <returns>Iterator pointing to the found element, or end() if not found</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			ConstIterator Find(const RTTI* const& value) const;

			/// <summary>
			/// Search for the element matching given value
			/// </summary>
			/// <param name="value">The value to search</param>
			/// <returns>Iterator pointing to the found element, or end() if not found</returns>
			/// <exception cref="std::exception">Type mismatch</exception>
			ConstIterator Find(const Scope& value) const;

			/// <summary>
			/// Convert a string to certain data type and set the value to given position
			/// </summary>
			/// <param name="str">The string representing the value</param>
			/// <param name="index">Position</param>
			/// <returns>True if the value is set, False otherwise</returns>
			/// <exception cref="std::exception">Datum has no type</exception>
			bool SetFromString(const std::string& str, size_t index = 0);

			/// <summary>
			/// Convert an element at given position to string
			/// </summary>
			/// <param name="index">Position</param>
			/// <returns>String representation of that element</returns>
			/// <exception cref="std::exception">Datum has no type</exception>
			/// <exception cref="std::exception">Index larger than size</exception>
			std::string ToString(size_t index = 0) const;

			void SetStorage(void* address, size_t size);

			/// <summary>
			/// Set this Datum to point to an existing memory space, make it a wrapper
			/// </summary>
			/// <param name="value">Address of int32_t</param>
			/// <param name="size">Number of elements at the address</param>
			/// <exception cref="std::exception">Address is nullptr</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			void SetStorage(int32_t* value, size_t size);

			/// <summary>
			/// Set this Datum to point to an existing memory space, make it a wrapper
			/// </summary>
			/// <param name="value">Address of float</param>
			/// <param name="size">Number of elements at the address</param>
			/// <exception cref="std::exception">Address is nullptr</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			void SetStorage(float* value, size_t size);

			/// <summary>
			/// Set this Datum to point to an existing memory space, make it a wrapper
			/// </summary>
			/// <param name="value">Address of vec4</param>
			/// <param name="size">Number of elements at the address</param>
			/// <exception cref="std::exception">Address is nullptr</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			void SetStorage(glm::vec4* value, size_t size);

			/// <summary>
			/// Set this Datum to point to an existing memory space, make it a wrapper
			/// </summary>
			/// <param name="value">Address of mat4</param>
			/// <param name="size">Number of elements at the address</param>
			/// <exception cref="std::exception">Address is nullptr</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			void SetStorage(glm::mat4* value, size_t size);

			/// <summary>
			/// Set this Datum to point to an existing memory space, make it a wrapper
			/// </summary>
			/// <param name="value">Address of string</param>
			/// <param name="size">Number of elements at the address</param>
			/// <exception cref="std::exception">Address is nullptr</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			void SetStorage(std::string* value, size_t size);

			/// <summary>
			/// Set this Datum to point to an existing memory space, make it a wrapper
			/// </summary>
			/// <param name="value">Address of RTTI*</param>
			/// <param name="size">Number of elements at the address</param>
			/// <exception cref="std::exception">Address is nullptr</exception>
			/// <exception cref="std::exception">Type mismatch</exception>
			void SetStorage(RTTI** value, size_t size);

			/// <summary>
			/// Return the Iterator pointing the first element
			/// </summary>
			/// <returns>The Iterator pointing the first element</returns>
			Iterator begin();

			/// <summary>
			/// Return the Iterator pointing to one after the last element (not in datum)
			/// </summary>
			/// <returns>The Iterator pointing to one after the last elemenet</returns>
			Iterator end();

			/// <summary>
			/// Return the ConstIterator pointing the first element
			/// </summary>
			/// <returns>The ConstIterator pointing the first element</returns>
			ConstIterator begin() const;

			/// <summary>
			/// Return the ConstIterator pointing to one after the last element (not in datum)
			/// </summary>
			/// <returns>The ConstIterator pointing to one after the last elemenet</returns>
			ConstIterator end() const;

			/// <summary>
			/// Return the ConstIterator pointing the first element
			/// </summary>
			/// <returns>The ConstIterator pointing the first element</returns>
			ConstIterator cbegin() const;

			/// <summary>
			/// Return the ConstIterator pointing to one after the last element (not in datum)
			/// </summary>
			/// <returns>The ConstIterator pointing to one after the last elemenet</returns>
			ConstIterator cend() const;
#pragma endregion

#pragma region Private
		private:
			/// <summary>
			/// A discriminated union to represent different data types at runtime
			/// </summary>
			union DataUnion
			{
				int32_t* Integer = nullptr;
				float* Float;
				glm::vec4* Vector;
				glm::mat4* Matrix;
				std::string* String;
				RTTI** Pointer;
				Scope** Table;
				void* Universe;
			};

			/// <summary>
			/// Type of Datum
			/// </summary>
			DatumType mType = DatumType::Unknown;

			/// <summary>
			/// Data stored in Datum
			/// </summary>
			DataUnion mData;

			/// <summary>
			/// Number of elements
			/// </summary>
			size_t mSize = 0;

			/// <summary>
			/// Number of slots
			/// </summary>
			size_t mCapacity = 0;

			/// <summary>
			/// Whether this Datum is using internal storage
			/// </summary>
			bool mIsInternal = true;

			/// <summary>
			/// Convert a string to int32_t and store it
			/// </summary>
			/// <param name="str">String representing int32_t</param>
			/// <param name="index">Position</param>
			/// <returns>Whether or not set is successful</returns>
			bool SetFromStringInt(const std::string& str, size_t index);

			/// <summary>
			/// Convert a string to float and store it
			/// </summary>
			/// <param name="str">String representing float</param>
			/// <param name="index">Position</param>
			/// <returns>Whether or not set is successful</returns>
			bool SetFromStringFloat(const std::string& str, size_t index);

			/// <summary>
			/// Convert a string to vec4 and store it
			/// </summary>
			/// <param name="str">String representing vec4</param>
			/// <param name="index">Position</param>
			/// <returns>Whether or not set is successful</returns>
			bool SetFromStringVector(const std::string& str, size_t index);

			/// <summary>
			/// Convert a string to mat4 and store it
			/// </summary>
			/// <param name="str">String representing mat4</param>
			/// <param name="index">Position</param>
			/// <returns>Whether or not set is successful</returns>
			bool SetFromStringMatrix(const std::string& str, size_t index);

			/// <summary>
			/// Store a string
			/// </summary>
			/// <param name="str">String</param>
			/// <param name="index">Position</param>
			/// <returns>Whether or not set is successful</returns>
			bool SetFromStringString(const std::string& str, size_t index);

			/// <summary>
			/// Copy the content in another Datum to this one.
			/// In another is internal storage, each elements will be deep-copied.
			/// Otherwise just a shallow copy
			/// </summary>
			/// <param name="other">The other Datum to copy from</param>
			void DeepCopy(const Datum& other);

			/// <summary>
			/// Make sure current capacity is greater than size, allocate new memory if not
			/// </summary>
			void EnsureCapacity();

			/// <summary>
			/// Check Datum is using internal memory, throw exception if not
			/// </summary>
			/// <exception cref="std::exception">Datum is using external storage</exception>
			void CheckIsInternal() const;

			/// <summary>
			/// Check Datum has type other than Unknown
			/// </summary>
			/// <exception cref="std::exception">Datum type is Unknown</exception>
			void CheckHasType() const;

			/// <summary>
			/// Allocate memory to meet new capacity. If capacity is 0, will free all memory and nullify mData
			/// </summary>
			/// <param name="capacity">New capacity</param>
			void AllocateMemory(size_t capacity);

			/// <summary>
			/// Before copying from another Datum, make sure this Datum is in clean state
			/// </summary>
			void ResetSelf();

			/// <summary>
			/// Invoke template specializations, to let compiler generate those code.
			/// This function should NEVER be invoked.
			/// </summary>
			void DummyFunction() const;

			static Vector<size_t> sTypeSizeTable;

	#pragma region CompareFunction
			using CompareFunction = bool(Datum::*)(void* data) const;

			bool ComparePrimitive(void* data) const;
			bool CompareString(void* data) const;
			bool ComparePointer(void* data) const;

			static Vector<CompareFunction> sCompareFunctions;
	#pragma endregion

	#pragma region CreateDefaultFunction
			using CreateDefaultFunction = void(Datum::*)(size_t size);

			void CreateDefaultPrimitive(size_t size);
			void CreateDefaultString(size_t size);

			static Vector<CreateDefaultFunction> sCreateDefaultFunctions;
	#pragma endregion

	#pragma region CopyFunction
			using CopyFunction = void(Datum::*)(void* data);

			void CopyPrimitive(void* data);
			void CopyString(void* data);

			static Vector<CopyFunction> sCopyFunctions;
	#pragma endregion


#pragma endregion
	};

	template<> int32_t& Datum::Front<int32_t>();
	template<> float& Datum::Front<float>();
	template<> glm::vec4& Datum::Front<glm::vec4>();
	template<> glm::mat4& Datum::Front<glm::mat4>();
	template<> std::string& Datum::Front<std::string>();
	template<> RTTI*& Datum::Front<RTTI*>();
	template<> const int32_t& Datum::Front<int32_t>() const;
	template<> const float& Datum::Front<float>() const;
	template<> const glm::vec4& Datum::Front<glm::vec4>() const;
	template<> const glm::mat4& Datum::Front<glm::mat4>() const;
	template<> const std::string& Datum::Front<std::string>() const;
	template<> RTTI* const& Datum::Front<RTTI*>() const;

	template<> int32_t& Datum::Back();
	template<> float& Datum::Back();
	template<> glm::vec4& Datum::Back();
	template<> glm::mat4& Datum::Back();
	template<> std::string& Datum::Back();
	template<> RTTI*& Datum::Back();
	template<> const int32_t& Datum::Back() const;
	template<> const float& Datum::Back() const;
	template<> const glm::vec4& Datum::Back() const;
	template<> const glm::mat4& Datum::Back() const;
	template<> const std::string& Datum::Back() const;
	template<> RTTI* const& Datum::Back() const;
}
