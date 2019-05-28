#pragma once

/// \file vector.h
/// \brief Contains the declaration of Vector

#include <initializer_list>
#include <cstddef>
#include <functional>
#include <algorithm>

namespace GameEngine
{
	/// <summary>
	/// Vector is a dynamic array that behaves almost the same as regular array except that its size can change dynamically based on the number of items in it.
	/// In Vector size means number of existing items, and capacity means current possible number of items
	/// </summary>
	template <typename T>
	class Vector
	{
	private:
		/// <summary>
		/// Base address of container array
		/// </summary>
		T* mArray = nullptr;

		/// <summary>
		/// Number of existing items in the Vector
		/// </summary>
		size_t mSize = 0;

		/// <summary>
		/// Number of possible slots for items in the Vector (i.e. length of mArray)
		/// </summary>
		size_t mCapacity = 0;

		/// <summary>
		/// Deep copy the contents in other Vector to this one
		/// </summary>
		/// <param name="other">The other Vector to copy from</param>
		void DeepCopy(const Vector & other);

		/// <summary>
		/// Default functor for reserve strategy
		/// </summary>
		struct DefaultReserveFunction
		{
			size_t operator()(size_t size, size_t capacity) const;
		};

		/// <summary>
		/// Reserve strategy function, vector uses this to decide new capacity when it needs to grow
		/// </summary>
		std::function<size_t(size_t, size_t)> mReserveFunction { DefaultReserveFunction() };

	public:
		/// <summary>
		/// An iterator is a container that points to one data item in the vector.
		/// Generally it is used to traverse vector, or specify a "position" in the vector so that user can remove / insert based on that position.
		/// It can be compared with another iterator by evaluating the data they are pointing to.
		/// It can also be incremented to point to next item in vector.
		/// </summary>
		class Iterator
		{
		public:
			using size_type = std::size_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;

			/// <summary>
			/// Default constructor, create an Iterator without parent and without a item to point to
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Copy constructor. Shallow copy the content in other Iterator to this one
			/// </summary>
			/// <param name="other">The other Iterator to copy from</param>
			Iterator(const Iterator & other) = default;
			
			/// <summary>
			/// Move constructor. Shallow copy the content in other Iterator to this one
			/// </summary>
			/// <param name="other">The other Iterator to move from</param>
			Iterator(Iterator && other) = default;

			/// <summary>
			/// Destructor
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// Copy assignment, shallow copy the content of another Iterator to this one.
			/// </summary>
			/// <param name="other">The other Iterator to copy from</param>
			/// <returns>This Iterator after copy</returns>
			Iterator & operator=(const Iterator & other) = default;

			/// <summary>
			/// Move assignment, shallow copy the content of another Iterator to this one.
			/// </summary>
			/// <param name="other">The other Iterator to move from</param>
			/// <returns>This Iterator after move</returns>
			Iterator & operator=(Iterator && other) = default;

			/// <summary>
			/// Pre-increment operator. Increment the Iterator to point to next item in the vector and return it
			/// </summary>
			/// <returns>The Iterator after increment</returns>
			Iterator & operator++();

			/// <summary>
			/// Post-increment operator. Increment the Iterator to point to next item in the vector and return the copy of the original Iterator before copying
			/// </summary>
			/// <param name="">A signature parameter to tell this is a post increment</param>
			/// <returns>The copy of Iterator before increment</returns>
			Iterator operator++(int);

			/// <summary>
			/// Get the Iterator after moving current one by step items forward. This doesn't change original Iterator
			/// </summary>
			/// <param name="step">Number of items to move</param>
			/// <returns>The Iterator after move</returns>
			Iterator operator+(size_t step) const;

			/// <summary>
			/// Check if two Iterators are the equal. Two Iterators are equal if they belong to the same container and they point to the same item.
			/// Note that Iterators without parent container are considered in the same container, and Iterators pointing to nothing are considered to point to the same item.
			/// </summary>
			/// <param name="other">The other Iterator to compare with</param>
			/// <returns>True if two Iterators are equal, False otherwise</returns>
			inline bool operator==(const Iterator & other) const;

			/// <summary>
			/// Check if two Iterators are the equal. Two Iterators are equal if they belong to the same container and they point to the same item.
			/// Note that Iterators without parent container are considered in the same container, and Iterators pointing to nothing are considered to point to the same item.
			/// </summary>
			/// <param name="other">The other Iterator to compare with</param>
			/// <returns>True if two Iterators are not equal, False otherwise</returns>
			inline bool operator!=(const Iterator & other) const;

			/// <summary>
			/// Get the reference of item pointed by the Iterator
			/// </summary>
			/// <returns>The reference of item pointed by the Iterator</returns>
			/// <exception cref="std::exception">The Iterator points to nothing</exception>
			T & operator*();

			/// <summary>
			/// Get the reference of item pointed by the Iterator
			/// </summary>
			/// <returns>The reference of item pointed by the Iterator</returns>
			/// <exception cref="std::exception">The Iterator points to nothing</exception>
			const T & operator*() const;

		private:
			/// <summary>
			/// Construct an Iterator with parent list and spcific node in the vector
			/// </summary>
			/// <param name="owner">Pointer to parent node</param>
			/// <param name="node">Pointer to node in the vector</param>
			explicit Iterator(const Vector & owner, size_t index = 0);

			/// <summary>
			/// Parent list, a free Iterator doesn't have parent so this value will be nullptr
			/// </summary>
			const Vector* mOwner = nullptr;

			/// <summary>
			/// The item index of this Iterator
			/// </summary>
			size_t mIndex = 0;

			friend Vector;
		};

		/// <summary>
		/// A ConstIterator is same as Iterator except that its operator*() only returns const reference, which disallows end-user to change the content
		/// </summary>
		class ConstIterator
		{
		public:
			using value_type = T;
			using pointer = T * ;
			using reference = T & ;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;

			/// <summary>
			/// Default constructor, create an ConstIterator without parent and without a item to point to
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Copy constructor. Shallow copy the content in other ConstIterator to this one
			/// </summary>
			/// <param name="other">The other ConstIterator to copy from</param>
			ConstIterator(const ConstIterator & other) = default;

			/// <summary>
			/// Move constructor. Shallow copy the content in other ConstIterator to this one
			/// </summary>
			/// <param name="other">The other ConstIterator to move from</param>
			ConstIterator(ConstIterator && other) = default;

			/// <summary>
			/// Copy constructor. Shallow copy the content in other Iterator to this one. Promote Iterator to ConstIterator
			/// </summary>
			/// <param name="other">The other Iterator to copy from</param>
			ConstIterator(const Iterator & other);

			/// <summary>
			/// Destructor
			/// </summary>
			~ConstIterator() = default;

			/// <summary>
			/// Copy assignment, shallow copy the content of another ConstIterator to this one.
			/// </summary>
			/// <param name="other">The other ConstIterator to copy from</param>
			/// <returns>This ConstIterator after copy</returns>
			ConstIterator & operator=(const ConstIterator & other) = default;

			/// <summary>
			/// Move assignment, shallow copy the content of another ConstIterator to this one.
			/// </summary>
			/// <param name="other">The other ConstIterator to move from</param>
			/// <returns>This ConstIterator after move</returns>
			ConstIterator & operator=(ConstIterator && other) = default;

			/// <summary>
			/// Copy assignment, copy the contents from other Iterator to this ConstIterator. Promote Iterator to ConstIterator
			/// </summary>
			/// <param name="other">The other Iterator to copy from</param>
			/// <returns>This ConstIterator after copy</returns>
			ConstIterator & operator=(const Iterator & other);

			/// <summary>
			/// Pre-increment operator. Increment the ConstIterator to point to next item in the vector and return it
			/// </summary>
			/// <returns>The ConstIterator after increment</returns>
			ConstIterator & operator++();

			/// <summary>
			/// Post-increment operator. Increment the ConstIterator to point to next item in the vector and return the copy of the original ConstIterator before copying
			/// </summary>
			/// <param name="">A signature parameter to tell this is a post increment</param>
			/// <returns>The copy of ConstIterator before increment</returns>
			ConstIterator operator++(int);

			/// <summary>
			/// Get the ConstIterator after moving current one by step items forward. This doesn't change original ConstIterator
			/// </summary>
			/// <param name="step">Number of items to move</param>
			/// <returns>The ConstIterator after move</returns>
			ConstIterator operator+(size_t step);

			/// <summary>
			/// Check if two ConstIterators are the equal. Two ConstIterators are equal if they belong to the same container and they point to the same item.
			/// Note that ConstIterators without parent container are considered in the same container, and ConstIterators pointing to nothing are considered to point to the same item.
			/// </summary>
			/// <param name="other">The other ConstIterator to compare with</param>
			/// <returns>True if two ConstIterators are equal, False otherwise</returns>
			inline bool operator==(const ConstIterator & other) const;

			/// <summary>
			/// Check if two ConstIterators are the equal. Two ConstIterators are equal if they belong to the same container and they point to the same item.
			/// Note that ConstIterators without parent container are considered in the same container, and ConstIterators pointing to nothing are considered to point to the same item.
			/// </summary>
			/// <param name="other">The other ConstIterator to compare with</param>
			/// <returns>True if two ConstIterators are equal, False otherwise</returns>
			inline bool operator!=(const ConstIterator & other) const;

			/// <summary>
			/// Get the reference of item pointed by the ConstIterator in const form
			/// </summary>
			/// <returns>The reference of item pointed by the ConstIterator</returns>
			/// <exception cref="std::exception">The ConstIterator points to nothing</exception>
			const T & operator*() const;

		private:
			/// <summary>
			/// Construct an ConstIterator with parent list and spcific node in the vector
			/// </summary>
			/// <param name="owner">Pointer to parent node</param>
			/// <param name="node">Pointer to node in the vector</param>
			explicit ConstIterator(const Vector & owner, size_t index = 0);

			/// <summary>
			/// Parent list, a free ConstIterator doesn't have parent so this value will be nullptr
			/// </summary>
			const Vector* mOwner = nullptr;

			/// <summary>
			/// The item index of this ConstIterator
			/// </summary>
			size_t mIndex = 0;

			friend Vector;
			friend Iterator;
		};

		/// <summary>
		/// Default constructor. Create an empty Vector with given capacity (default = 0) and given reserve function (default is to double capacity every time)
		/// </summary>
		/// <param name="capacity">The capaicty to reserve on construction</param>
		/// <param name="reserveFunc">Reserve function used to decide new capacity when the vector needs to grow</param>
		explicit Vector(size_t capacity = 0, std::function<size_t(size_t, size_t)> reserveFunc = DefaultReserveFunction());

		/// <summary>
		/// Copy constructor. Copy the contents from other Vector to this one, clear any existing contents
		/// </summary>
		/// <param name="other">The other Vector to copy from</param>
		Vector(const Vector & other);

		/// <summary>
		/// Move constructor. Move the contents from other Vector to this one and invalidate other
		/// </summary>
		/// <param name="other">The other Vector to move from</param>
		Vector(Vector && other);

		/// <summary>
		/// Initilizaer list constructor. Will set both size and capacity to list length
		/// </summary>
		/// <param name="list">List of contents used to initilize Vector</param>
		/// <param name="reserveFunc">Reserve function used to decide new capacity when the vector needs to grow</param>
		Vector(std::initializer_list<T> list, std::function<size_t(size_t, size_t)> reserveFunc = DefaultReserveFunction());

		/// <summary>
		/// Desctructor, destroy all items and free memory
		/// </summary>
		~Vector();

		/// <summary>
		/// Copy assignment, copy the contents from other Vector to this one, clear any existing contents
		/// </summary>
		/// <param name="other">The other Vector to copy from</param>
		/// <returns>This Vector after copy</returns>
		Vector & operator=(const Vector & other);

		/// <summary>
		/// Move assignment, move the contents from other Vector to this one, clear any existing contents in this one and invalidate other Vector
		/// </summary>
		/// <param name="other">The other Vector to move from</param>
		/// <returns>This Vector after move</returns>
		Vector & operator=(Vector && other);

		/// <summary>
		/// Get the reference to an item in the Vector at given index
		/// </summary>
		/// <param name="index">The index of item</param>
		/// <returns>The reference to the item</returns>
		/// <exception cref="std::exception">The index is out of range</exception>
		T & operator[](size_t index);

		/// <summary>
		/// Get const the reference to an item in the Vector at given index
		/// </summary>
		/// <param name="index">The index of item</param>
		/// <returns>The const reference to the item</returns>
		/// /// <exception cref="std::exception">The index is out of range</exception>
		const T & operator[](size_t index) const;

		/// <summary>
		/// Get the reference to an item in the Vector at given index
		/// </summary>
		/// <param name="index">The index of item</param>
		/// <returns>The reference to the item</returns>
		/// <exception cref="std::exception">The index is out of range</exception>
		T & At(size_t index);

		/// <summary>
		/// Get const the reference to an item in the Vector at given index
		/// </summary>
		/// <param name="index">The index of item</param>
		/// <returns>The const reference to the item</returns>
		/// /// <exception cref="std::exception">The index is out of range</exception>
		const T & At(size_t index) const;

		/// <summary>
		/// Get the reference to the first item in the Vector
		/// </summary>
		/// <returns>The reference to the first item</returns>
		/// <exception cref="std::exception">The Vector is empty</exception>
		T & Front();

		/// <summary>
		/// Get the const reference to the first item in the Vector
		/// </summary>
		/// <returns>The const reference to the first item</returns>
		/// <exception cref="std::exception">The Vector is empty</exception>
		const T & Front() const;

		/// <summary>
		/// Get the reference to the last item in the Vector
		/// </summary>
		/// <returns>The reference to the last item</returns>
		/// <exception cref="std::exception">The Vector is empty</exception>
		T & Back();

		/// <summary>
		/// Get the const reference to the last item in the Vector
		/// </summary>
		/// <returns>The const reference to the last item</returns>
		/// <exception cref="std::exception">The Vector is empty</exception>
		const T & Back() const;

		/// <summary>
		/// Check is Vector is empty
		/// </summary>
		/// <returns>True if Vector is empty, False otherwise</returns>
		inline bool IsEmpty() const;

		/// <summary>
		/// Get the number of items in the Vector, not the capacity
		/// </summary>
		/// <returns>The number of items in the Vector</returns>
		inline size_t Size() const;

		/// <summary>
		/// Get the capacity of the Vector
		/// </summary>
		/// <returns>The capacity of the Vector</returns>
		inline size_t Capacity() const;

		/// <summary>
		/// Append an item to the end of Vector. Reserve new capacity when necessary
		/// </summary>
		/// <param name="data">The data to append</param>
		/// <returns>The Iterator pointing to the newly appended item</returns>
		Iterator PushBack(const T & data);

		template <typename... Args>
		Iterator EmplaceBack(Args&&... args);

		/// <summary>
		/// Remove the last item in the Vector and keep the capacity unchanged. This does nothing if Vector is empty
		/// </summary>
		void PopBack();

		/// <summary>
		/// Allocate new memory for given capacity. Do nothing if parameter is not larger than current capacity
		/// </summary>
		/// <param name="capacity">New capacity to reserve</param>
		void Reserve(size_t capacity);

		/// <summary>
		/// Shrink the vector so that capacity is same as size.
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// Resize the vector, set both size and capacity to given size.
		/// If new size is larger than current size, vector will grow and default-construct items in new slots.
		/// If new size is less than current size, items whose indexes are larger than new size will be destructed.
		/// </summary>
		/// <param name="size">New size of vector</param>
		/// <param name="sample">In cast T doesn't have default constructor, pass in a T object to allow copy construction</param>
		void Resize(size_t size, const T& sample = T());

		/// <summary>
		/// Remove all items in the Vector but keep the memory
		/// </summary>
		void Clear();

		/// <summary>
		/// Remove all items in the Vector and free all memory. Set the Vector to a default-constructed state
		/// </summary>
		void Destroy();

		/// <summary>
		/// Try to find the item equal to given data and return the Iterator pointing to it. Return end() if item is not found.
		/// </summary>
		/// <param name="data">The data to find</param>
		/// <returns>The Iterator pointing to the found item. Or end() if item is not found</returns>
		Iterator Find(const T & data);

		/// <summary>
		/// Try to find the item equal to given data and return the ConstIterator pointing to it. Return end() if item is not found.
		/// </summary>
		/// <param name="data">The data to find</param>
		/// <returns>The ConstIterator pointing to the found item. Or end() if item is not found</returns>
		ConstIterator Find(const T & data) const;

		/// <summary>
		/// Remove the first item equal to given data. If no such item is found, this function does nothing
		/// </summary>
		/// <param name="data">The data to remove</param>
		void Remove(const T & data);

		/// <summary>
		/// Remove the item pointed by the given Iterator. If Iterator is invalid, this function does nothing
		/// </summary>
		/// <param name="iterator">The Iterator pointing to item to be removed</param>
		void Remove(const Iterator& iterator);

		/// <summary>
		/// Remove the items starting from begin[inclusive] to end[exclusive]
		/// </summary>
		/// <param name="begin">The Iterator pointing to the beginning item</param>
		/// <param name="end">The Iterator pointing to the item after the last item to be removed</param>
		void Remove(Iterator begin, Iterator end);

		/// <summary>
		/// Get the Iterator pointing to the first item of Vector
		/// </summary>
		/// <returns>Iterator pointing to the first item of Vector</returns>
		Iterator begin();

		/// <summary>
		/// Get the Iterator pointing to the first item of Vector
		/// </summary>
		/// <returns>Iterator pointing to the first item of Vector</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Get the Iterator pointing to one item after the last item of Vector (i.e. not in the Vector)
		/// </summary>
		/// <returns>Iterator pointing to one item after the last item of Vector</returns>
		Iterator end();

		/// <summary>
		/// Get the Iterator pointing to one item after the last item of Vector (i.e. not in the Vector)
		/// </summary>
		/// <returns>Iterator pointing to one item after the last item of Vector</returns>
		ConstIterator end() const;

		/// <summary>
		/// Get the ConstIterator pointing to the first item of Vector
		/// </summary>
		/// <returns>ConstIterator pointing to the first item of Vector</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Get the ConstIterator pointing to the first item of Vector
		/// </summary>
		/// <returns>ConstIterator pointing to the first item of Vector</returns>
		ConstIterator cend() const;

		using value_type = T;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;
		using reference = T&;
		using const_reference = const T&;
		using iterator = Iterator;
		using const_iterator = ConstIterator;
	};
}

#include "vector.inl"