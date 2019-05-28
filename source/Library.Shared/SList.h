#pragma once

/// \file SList.h
/// \brief Contains the declaration of SList

#include <initializer_list>
#include <cstddef>

/// <summary>
/// All game-related code goes in here
/// </summary>
namespace GameEngine
{
	/// <summary>
	/// A templated singly-linked list.
	/// The list supports push to front, push to back, pop front, pop back and clear manipunation.
	/// It allows user to query the size, emptiness, the first item and the last item.
	/// To create a SList, provide a specification such as "SList<int> list".
	/// </summary>
	template <typename T>
	class SList
	{
	private:
		/// <summary>
		/// A structure to represent a node in linked list.
		/// It contains actual user data and a pointer to next node.
		/// </summary>
		struct Node
		{
			/// <summary>
			/// Default constructor, create a empty node without data
			/// </summary>
			Node() = default;

			/// <summary>
			/// Constructor that takes the data and a pointer to its next node
			/// </summary>
			/// <param name="data">The data of this node</param>
			/// <param name="next">The pointer to next node</param>
			explicit Node(const T & data, Node* next = nullptr);

			/// <summary>
			/// Copy constructor
			/// </summary>
			/// <param name="other">The other node to copy from</param>
			Node(const Node & other) = delete;

			/// <summary>
			/// Copy assignment
			/// </summary>
			/// <param name="other">The other node to copy from</param>
			/// <returns>This node itself after copying</returns>
			Node & operator=(const Node & other) = delete;

			/// <summary>
			/// Desctructor
			/// </summary>
			~Node() = default;

			/// <summary>
			/// The pointer to next node
			/// </summary>
			Node* mNext = nullptr;

			/// <summary>
			/// Actual data in this node
			/// </summary>
			T mData;
		};

		/// <summary>
		/// Pointer to the front of the list. By default it's nullptr
		/// </summary>
		Node* mFront = nullptr;

		/// <summary>
		/// Pointer to the back of the list. By default it's nullptr
		/// </summary>
		Node* mBack = nullptr;

		/// <summary>
		/// Current size of the list
		/// </summary>
		size_t mSize = 0;

		/// <summary>
		/// Copy the data in each node in other list to this one
		/// </summary>
		/// <param name="other">The other list to copy from</param>
		void DeepCopy(const SList & other);

	public:
		/// <summary>
		/// An iterator is a container that points to one data item in the list.
		/// Generally it is used to traverse SList, or specify a "position" in the SList so that user can remove / insert based on that position.
		/// It can be compared with another iterator by evaluating the data they are pointing to.
		/// It can also be incremented to point to next item in SList.
		/// </summary>
		class Iterator
		{
		public:
			/// <summary>
			/// Default constructor, create an iterator without parent and without a node pointer
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Copy constructor. Shallow copy the content in other iterator to this one
			/// </summary>
			/// <param name="other">The other iterator to copy from</param>
			Iterator(const Iterator & other) = default;

			/// <summary>
			/// Destructor
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// Pre-increment operator. Increment the iterator to point to next item in the list and return it
			/// </summary>
			/// <returns>The iterator after increment</returns>
			Iterator & operator++();

			/// <summary>
			/// Post-increment operator. Increment the iterator to point to next item in the list and return the copy of the original iterator before copying
			/// </summary>
			/// <param name="">A signature parameter to tell this is a prefix increment</param>
			/// <returns>The copy of iterator before increment</returns>
			Iterator operator++(int);

			/// <summary>
			/// Check if two iterators are the equal. Two iterators are equal if they belong to the same container and they point to the same item.
			/// Note that iterators without parent container are considered in the same container, and iterators pointing to nothing are considered to point to the same item.
			/// </summary>
			/// <param name="other">The other iterator to compare with</param>
			/// <returns>True if two iterators are equal, False otherwise</returns>
			bool operator==(const Iterator & other) const;

			/// <summary>
			/// Check if two iterators are the equal. Two iterators are equal if they belong to the same container and they point to the same item.
			/// Note that iterators without parent container are considered in the same container, and iterators pointing to nothing are considered to point to the same item.
			/// </summary>
			/// <param name="other">The other iterator to compare with</param>
			/// <returns>True if two iterators are not equal, False otherwise</returns>
			bool operator!=(const Iterator & other) const;

			/// <summary>
			/// Get the content pointed by the iterator
			/// </summary>
			/// <returns>The content pointed by the iterator</returns>
			/// <exception cref="std::exception">The iterator points to nothing</exception>
			T & operator*() const;

		private:
			/// <summary>
			/// Construct an iterator with parent list and spcific node in the list
			/// </summary>
			/// <param name="owner">Pointer to parent node</param>
			/// <param name="node">Pointer to node in the list</param>
			Iterator(const SList & owner, Node* node = nullptr);

			/// <summary>
			/// Parent list, a free iterator doesn't have parent so this value will be nullptr
			/// </summary>
			const SList* mOwner = nullptr;

			/// <summary>
			/// The node this iterator points to
			/// </summary>
			Node* mNode = nullptr;

			friend class SList;  // Must let list access private members
		};

		/// <summary>
		/// Default constructor. Create an empty linked list
		/// </summary>
		SList() = default;

		/// <summary>
		/// Copy constructor. Deep copy the contents from the other list into this one
		/// </summary>
		/// <param name="other">The other list to copy from</param>
		SList(const SList & other);

		/// <summary>
		/// Move constructor. Move the contents from the other list into this one
		/// </summary>
		/// <param name="other">The other SList to move from</param>
		SList(SList && other);

		/// <summary>
		/// Create a list from initializer list (i.e. SList<T> list = { data1, data2, data3, ... })
		/// </summary>
		/// <param name="list">The list of data used to initialize SList</param>
		SList(std::initializer_list<T> list);

		/// <summary>
		/// Clear current list (release memory) and deep copy the contents from the other list into this one
		/// </summary>
		/// <param name="other">The other list to copy from</param>
		SList & operator=(const SList & other);

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="other">The other SList to move from</param>
		/// <returns>This list after moving</returns>
		SList & operator=(SList && other);

		/// <summary>
		/// Destructor. Release all memory in this list
		/// </summary>
		~SList();

		/// <summary>
		/// Add a new item to the front of this list.
		/// If there is a front item already, the added item will be new front.
		/// If the list is empty, the added item will be both front and back.
		/// </summary>
		/// <param name="data">The data item to add</param>
		/// <returns>The iterator to the new item being pushed</returns>
		Iterator PushFront(const T & data);

		/// <summary>
		/// Append a new item after the back of this list.
		/// If there is a back item already, the appended item will be new back.
		/// If the list is empty, the added item will be both front and back.
		/// </summary>
		/// <param name="data">The data item to append</param>
		/// <returns>The iterator to the new item being pushed</returns>
		Iterator PushBack(const T & data);

		/// <summary>
		/// Remove the first item from the list, this function releases memory of that item.
		/// If list is empty, this function does nothing
		/// </summary>
		void PopFront();

		/// <summary>
		/// Remove the last item from the list, this function releases memory of that item.
		/// If list is empty, this function does nothing
		/// </summary>
		void PopBack();

		/// <summary>
		/// Get the reference (non-const) to the first item of the list. If the list is empty, this function throws an exception
		/// </summary>
		/// <returns>The non-const reference to the first item of the list</returns>
		/// <exception cref="std::exception">The list is empty</exception>
		T & Front();

		/// <summary>
		/// Get the reference (const) to the first item of the list. If the list is empty, this function throws an exception
		/// </summary>
		/// <returns>The const reference to the first item of the list</returns>
		/// <exception cref="std::exception">The list is empty</exception>
		const T & Front() const;

		/// <summary>
		/// Get the reference (non-const) to the last item of the list. If the list is empty, this function throws an exception
		/// </summary>
		/// <returns>The not-const reference to last item of the list</returns>
		/// <exception cref="std::exception">The list is empty</exception>
		T & Back();

		/// <summary>
		/// Get the reference (const) to the last item of the list. If the list is empty, this function throws an exception
		/// </summary>
		/// <returns>The const reference to last item of the list</returns>
		/// <exception cref="std::exception">The list is empty</exception>
		const T & Back() const;

		/// <summary>
		/// Check whether the list is empty or not
		/// </summary>
		/// <returns>True if list is empty, False otherwise</returns>
		inline bool IsEmpty() const;

		/// <summary>
		/// Get the size (number of items) of the list
		/// </summary>
		/// <returns>Size of the list</returns>
		inline size_t Size() const;

		/// <summary>
		/// Remove all items from the list and release their memory.
		/// Use with caution, if you hold a reference to an item in the list, it will become invalid.
		/// </summary>
		void Clear();

		/// <summary>
		/// Get the iterator to the first item of the list
		/// </summary>
		/// <returns>The iterator to the beginning of the list. If list is empty, the iterator is also the end</returns>
		Iterator begin();

		/// <summary>
		/// Get the const iterator to the first item of the list, currently it's just a fake one to pass compiler
		/// </summary>
		/// <returns>The iterator to the beginning of the list. If list is empty, the iterator is also the end</returns>
		Iterator begin() const;

		/// <summary>
		/// Get the iterator after the last item of the list. Note the returned iterator points to nothing so trying to dereference it will cause exception
		/// </summary>
		/// <returns>The iterator after the last item of the list</returns>
		Iterator end();

		/// <summary>
		/// Get the const iterator after the last item of the list, currently it's just a fake one to pass compiler
		/// </summary>
		/// <returns>The iterator after the last item of the list</returns>
		Iterator end() const;

		/// <summary>
		/// Insert a new data item after the given iterator.
		/// Note that the iterator must be valid (i.e. it points to some data of certain list, iterator returned by end() doesn't count as valid).
		/// </summary>
		/// <param name="iterator">The iterator after which new item will be inserted</param>
		/// <param name="data">The new data item to be inserted</param>
		/// <returns>The iterator of newly inserted item, if insertion is invalid, it will be end()</returns>
		Iterator InsertAfter(const Iterator & iterator, const T & data);

		/// <summary>
		/// Search for the given data in the list and return the iterator to the item if such item is found.
		/// This method will invoke operator==() of given data type, so such method must exist.
		/// </summary>
		/// <param name="data">The data to search</param>
		/// <returns>The iterator to the requested item, if none of the items matches the data, an iterator from end() will be returned</returns>
		Iterator Find(const T & data);

		/// <summary>
		/// Search for the given data in the list and return the const iterator to the item if such item is found.
		/// This method will invoke operator==() of given data type, so such method must exist.
		/// Note currently this method is just a fake one since we don't have constIterator yet
		/// </summary>
		/// <param name="data">The data to search</param>
		/// <returns>The const iterator to the requested item, if none of the items matches the data, an iterator from end() will be returned</returns>
		Iterator Find(const T & data) const;

		/// <summary>
		/// Search for the given data item in the list and remove it from the list should it is found
		/// </summary>
		/// <param name="data">The data item to be removed</param>
		void Remove(const T & data);

		/// <summary>
		/// Remove the data item pointed by the given iterator. The iterator must be valid (i.e. from the same list and points to an item)
		/// </summary>
		/// <param name="iterator">The iterator pointing to the data to be removed</param>
		void Remove(const Iterator & iterator);
	};
}

#include "SList.inl"
