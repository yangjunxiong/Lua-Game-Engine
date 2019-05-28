#pragma once

#include <utility>
#include <exception>
#include <algorithm>
#include "DefaultHashFunction.h"
#include "SList.h"
#include "vector.h"

namespace GameEngine
{
	/// <summary>
	/// HashMap is associative container that stores elements formed by a combination of a key value and a mapped value, without specific order.
	/// Ideally, hashMap allows constant time lookup and modification with certain key.
	/// </summary>
	template <typename TKey, typename TValue, typename HashFunctor = DefaultHash<TKey>, typename KeyEqualityFunctor = DefaultKeyEquality<TKey>>
	class HashMap
	{
	public:
		using PairType = std::pair<const TKey, TValue>;
		using ChainIterator = typename SList<PairType>::Iterator;

	private:
		using ChainType = SList<PairType>;
		using BucketType = Vector<ChainType>;

	public:
		/// <summary>
		/// An iterator is a container that points to one pair in the HashMap.
		/// Generally it is used to traverse HashMap, or specify a "position" in the HashMap so that user can remove / modify based on that position.
		/// It can be compared with another iterator by evaluating the pair they are pointing to.
		/// It can also be incremented to point to next pair in HashMap.
		/// </summary>
		class Iterator final
		{
			friend HashMap;
			friend class ConstIterator;

		public:
			/// <summary>
			/// Default constructor, create an Iterator without parent and without a item to point to
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Copy constructor. Shallow copy the content in other Iterator to this one
			/// </summary>
			/// <param name="other">The other Iterator to copy from</param>
			Iterator(const Iterator& other) = default;

			/// <summary>
			/// Move constructor. Shallow copy the content in other Iterator to this one
			/// </summary>
			/// <param name="other">The other Iterator to move from</param>
			Iterator(Iterator&& other) = default;

			/// <summary>
			/// Copy assignment, shallow copy the content of another Iterator to this one.
			/// </summary>
			/// <param name="other">The other Iterator to copy from</param>
			/// <returns>This Iterator after copy</returns>
			Iterator& operator=(const Iterator& other) = default;

			/// <summary>
			/// Move assignment, shallow copy the content of another Iterator to this one.
			/// </summary>
			/// <param name="other">The other Iterator to move from</param>
			/// <returns>This Iterator after move</returns>
			Iterator& operator=(Iterator&& other) = default;

			/// <summary>
			/// Destructor
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// Check if two Iterators are the equal. Two Iterators are equal if they belong to the same container and they point to the same item.
			/// Note that Iterators without parent container are considered in the same container, and Iterators pointing to nothing are considered to point to the same item.
			/// </summary>
			/// <param name="other">The other Iterator to compare with</param>
			/// <returns>True if two Iterators are equal, False otherwise</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// Check if two Iterators are the equal. Two Iterators are equal if they belong to the same container and they point to the same item.
			/// Note that Iterators without parent container are considered in the same container, and Iterators pointing to nothing are considered to point to the same item.
			/// </summary>
			/// <param name="other">The other Iterator to compare with</param>
			/// <returns>True if two Iterators are not equal, False otherwise</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Get the reference of pair pointed by the Iterator
			/// </summary>
			/// <returns>The reference of pair pointed by the Iterator</returns>
			/// <exception cref="std::exception">The Iterator points to nothing</exception>
			PairType& operator*();

			/// <summary>
			/// Get the reference of pair pointed by the Iterator
			/// </summary>
			/// <returns>The reference of pair pointed by the Iterator</returns>
			/// <exception cref="std::exception">The Iterator points to nothing</exception>
			const PairType& operator*() const;

			/// <summary>
			/// Get the pointer to pair pointed by the Iterator
			/// </summary>
			/// <returns>The pointer to pair pointed by the Iterator</returns>
			/// <exception cref="std::exception">The Iterator points to nothing</exception>
			PairType* operator->();

			/// <summary>
			/// Get the pointer to pair pointed by the Iterator
			/// </summary>
			/// <returns>The pointer to pair pointed by the Iterator</returns>
			/// <exception cref="std::exception">The Iterator points to nothing</exception>
			const PairType* operator->() const;

			/// <summary>
			/// Pre-increment operator. Increment the Iterator to point to next pair in the HashMap and return it
			/// </summary>
			/// <returns>The Iterator after increment</returns>
			/// <exception cref="std::exception">The Iterator has no parent</exception>
			Iterator& operator++();

			/// <summary>
			/// Post-increment operator. Increment the Iterator to point to next pair in the HashMap and return the copy of the original Iterator before copying
			/// </summary>
			/// <param name="">A signature parameter to tell this is a post increment</param>
			/// <returns>The copy of Iterator before increment</returns>
			/// <exception cref="std::exception">The Iterator has no parent</exception>
			Iterator operator++(int);

		private:
			/// <summary>
			/// Construct Iterator with given HashMap, index of bucket, and Iterator of SList in that bucket
			/// </summary>
			/// <param name="owner">Parent HashMap</param>
			/// <param name="index">Index of bucket list</param>
			/// <param name="chainIterator">Iterator to an item in SList in the bucket</param>
			Iterator(const HashMap& owner, const size_t& index, const typename ChainType::Iterator& chainIterator);

			/// <summary>
			/// Parent HashMap
			/// </summary>
			const HashMap* mOwner = nullptr;

			/// <summary>
			/// Index of bucket list
			/// </summary>
			size_t mIndex = 0;

			/// <summary>
			/// Iterator to an item in SList in the bucket
			/// </summary>
			typename ChainType::Iterator mChainIterator;
		};

		/// <summary>
		/// A ConstIterator is same as Iterator except that its operator*() only returns const reference, which disallows end-user to change the content
		/// </summary>
		class ConstIterator final
		{
			friend HashMap;

		public:
			/// <summary>
			/// Default constructor, create an ConstIterator without parent and without a item to point to
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Copy constructor. Shallow copy the content in other ConstIterator to this one
			/// </summary>
			/// <param name="other">The other ConstIterator to copy from</param>
			ConstIterator(const ConstIterator& other) = default;

			/// <summary>
			/// Move constructor. Shallow copy the content in other ConstIterator to this one
			/// </summary>
			/// <param name="other">The other ConstIterator to move from</param>
			ConstIterator(ConstIterator&& other) = default;

			/// <summary>
			/// Copy constructor. Shallow copy the content in other Iterator to this one. Promote Iterator to ConstIterator
			/// </summary>
			/// <param name="other">The other Iterator to copy from</param>
			ConstIterator(const Iterator& other);

			/// <summary>
			/// Copy assignment, shallow copy the content of another ConstIterator to this one.
			/// </summary>
			/// <param name="other">The other ConstIterator to copy from</param>
			/// <returns>This ConstIterator after copy</returns>
			ConstIterator& operator=(const ConstIterator& other) = default;

			/// <summary>
			/// Move assignment, shallow copy the content of another ConstIterator to this one.
			/// </summary>
			/// <param name="other">The other ConstIterator to move from</param>
			/// <returns>This ConstIterator after move</returns>
			ConstIterator& operator=(ConstIterator&& other) = default;

			/// <summary>
			/// Copy assignment, copy the contents from other Iterator to this ConstIterator. Promote Iterator to ConstIterator
			/// </summary>
			/// <param name="other">The other Iterator to copy from</param>
			/// <returns>This ConstIterator after copy</returns>
			ConstIterator& operator=(const Iterator& other);

			/// <summary>
			/// Destructor
			/// </summary>
			~ConstIterator() = default;

			/// <summary>
			/// Check if two ConstIterators are the equal. Two ConstIterators are equal if they belong to the same container and they point to the same item.
			/// Note that ConstIterators without parent container are considered in the same container, and ConstIterators pointing to nothing are considered to point to the same item.
			/// </summary>
			/// <param name="other">The other ConstIterator to compare with</param>
			/// <returns>True if two ConstIterators are equal, False otherwise</returns>
			bool operator==(const ConstIterator& other) const;

			/// <summary>
			/// Check if two ConstIterators are the equal. Two ConstIterators are equal if they belong to the same container and they point to the same item.
			/// Note that ConstIterators without parent container are considered in the same container, and ConstIterators pointing to nothing are considered to point to the same item.
			/// </summary>
			/// <param name="other">The other ConstIterator to compare with</param>
			/// <returns>True if two ConstIterators are equal, False otherwise</returns>
			bool operator!=(const ConstIterator& other) const;

			/// <summary>
			/// Get the reference of pair pointed by the ConstIterator in const form
			/// </summary>
			/// <returns>The reference of pair pointed by the ConstIterator</returns>
			/// <exception cref="std::exception">The ConstIterator points to nothing</exception>
			const PairType& operator*() const;

			/// <summary>
			/// Get the pointer to pair pointed by the ConstIterator in const form
			/// </summary>
			/// <returns>The pointer to pair pointed by the ConstIterator</returns>
			/// <exception cref="std::exception">The ConstIterator points to nothing</exception>
			const PairType* operator->() const;

			/// <summary>
			/// Pre-increment operator. Increment the ConstIterator to point to next item in the HashMap and return it
			/// </summary>
			/// <returns>The ConstIterator after increment</returns>
			/// <exception cref="std::exception">The ConstIterator has no parent</exception>
			ConstIterator& operator++();

			/// <summary>
			/// Post-increment operator. Increment the ConstIterator to point to next item in the HashMap and return the copy of the original ConstIterator before copying
			/// </summary>
			/// <param name="">A signature parameter to tell this is a post increment</param>
			/// <returns>The copy of ConstIterator before increment</returns>
			/// <exception cref="std::exception">The ConstIterator has no parent</exception>
			ConstIterator operator++(int);

		private:
			/// <summary>
			/// Construct Iterator with given HashMap, index of bucket, and Iterator of SList in that bucket
			/// </summary>
			/// <param name="owner">Parent HashMap</param>
			/// <param name="index">Index of bucket list</param>
			/// <param name="chainIterator">Iterator to an item in SList in the bucket</param>
			ConstIterator(const HashMap& owner, const size_t& index, const typename ChainType::Iterator& chainIterator);

			/// <summary>
			/// Parent HashMap
			/// </summary>
			const HashMap* mOwner = nullptr;

			/// <summary>
			/// Index of bucket list
			/// </summary>
			size_t mIndex = 0;

			/// <summary>
			/// Iterator to an item in SList in the bucket
			/// </summary>
			typename ChainType::Iterator mChainIterator;
		};

		/// <summary>
		/// Default constructor, construct HashMap with given bucket size
		/// </summary>
		/// <param name="capacity">Bucket size</param>
		explicit HashMap(size_t capacity = 32);

		/// <summary>
		/// Constructor that takes a list of initial pairs and bucket size
		/// </summary>
		/// <param name="list">Initial list of pairs</param>
		/// <param name="capacity">Bucket size</param>
		HashMap(const std::initializer_list<PairType>& list, size_t capacity = 32);

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="other">The other HashMap to copy from</param>
		HashMap(const HashMap& other) = default;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other">The other HashMap to move from</param>
		HashMap(HashMap&& other);

		/// <summary>
		/// Destructor, release all memory
		/// </summary>
		~HashMap() = default;

		/// <summary>
		/// Copy assignment, clear current HashMap and deep copy contents in other to this one
		/// </summary>
		/// <param name="other">The other HashMap to copy from</param>
		/// <returns>This HashMap</returns>
		HashMap& operator=(const HashMap& other);

		/// <summary>
		/// Move assignment, clear current HashMap and move contents in other to this one
		/// </summary>
		/// <param name="other">The other HashMap to move from</param>
		/// <returns>This HashMap</returns>
		HashMap& operator=(HashMap&& other);

		/// <summary>
		/// Get the reference to mapped value with given key. If key doesn't exist, one such pair will be default-constructed
		/// </summary>
		/// <param name="key">Key value to look for</param>
		/// <returns>Reference to mapped value at key position</returns>
		TValue& operator[](const TKey& key);

		/// <summary>
		/// Equivalent to calling "At()", get the pair at position specified by key
		/// </summary>
		/// <param name="key">Key value to look for</param>
		/// <returns>Reference to mapped value at key position, if exists</returns>
		/// <exception cref="std::exception">The given key doesn't exist</exception>
		const TValue& operator[](const TKey& key) const;

		/// <summary>
		/// Get reference to mapped value with given key
		/// </summary>
		/// <param name="key">Key value to look for</param>
		/// <returns>Reference to mapped value at key position, if exists</returns>
		/// <exception cref="std::exception">The given key doesn't exist</exception>
		TValue& At(const TKey& key);

		/// <summary>
		/// Get reference to mapped value with given key
		/// </summary>
		/// <param name="key">Key value to look for</param>
		/// <returns>Reference to mapped value at key position, if exists</returns>
		/// <exception cref="std::exception">The given key doesn't exist</exception>
		const TValue& At(const TKey& key) const;

		/// <summary>
		/// Destroy all pairs in HashMap and free all memory
		/// </summary>
		void Clear();

		/// <summary>
		/// Get the number of pairs
		/// </summary>
		/// <returns>Number of pairs</returns>
		size_t Size() const;

		/// <summary>
		/// Check if HashMap is empty
		/// </summary>
		/// <returns>True if empty, False otherwise</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Check if given key exists in HashMap
		/// </summary>
		/// <param name="key">The key value to check</param>
		/// <returns>True if key exists, False otherwise</returns>
		bool ContainsKey(const TKey& key) const;

		/// <summary>
		/// Check if given key exists in HashMap, and set the iterator to point to that pair
		/// </summary>
		/// <param name="key">The key value to check</param>
		/// <param name="iterator">Return paramter, points to the pair with given key, or end() if that key doesn't exist</param>
		/// <returns>True if key exists, False otherwise</returns>
		bool ContainsKey(const TKey& key, Iterator& iterator);

		/// <summary>
		/// Check if given key exists in HashMap, and set the iterator to point to that pair
		/// </summary>
		/// <param name="key">The key value to check</param>
		/// <param name="iterator">Return paramter, points to the pair with given key, or end() if that key doesn't exist</param>
		/// <returns>True if key exists, False otherwise</returns>
		bool ContainsKey(const TKey& key, ConstIterator& iterator) const;

		/// <summary>
		/// Search for the pair with given key and return the iterator pointing that pair
		/// </summary>
		/// <param name="key">Key value to search</param>
		/// <returns>Iterator pointing that pair with given key, or end() if such pair doesn't exist</returns>
		Iterator Find(const TKey& key);

		/// <summary>
		/// Search for the pair with given key and return the iterator pointing that pair
		/// </summary>
		/// <param name="key">Key value to search</param>
		/// <returns>Iterator pointing that pair with given key, or end() if such pair doesn't exist</returns>
		ConstIterator Find(const TKey& key) const;

		/// <summary>
		/// Insert the pair to HashMap. If there is a pair with same key already, insertion will not happen
		/// </summary>
		/// <param name="pair">The pair to be inserted</param>
		/// <returns>Iterator to inserted pair, or an existing pair with same key if such pair exists</returns>
		std::pair<Iterator, bool> Insert(const PairType& pair);

		/// <summary>
		/// Remove the pair with given key
		/// </summary>
		/// <param name="key">The key value to search for</param>
		void Remove(const TKey& key);

		/// <summary>
		/// Remove the pair pointed by iterator
		/// </summary>
		/// <param name="position">The iterator pointing to the pair to be deleted</param>
		/// <exception cref="std::exception">The iterator has no parent</exception>
		void Remove(const Iterator& position);

		/// <summary>
		/// Change the size of buckets and rehash the entire map. The new size can be greater of less than current size
		/// </summary>
		/// <param name="capacity">New size of buckets</param>
		void Resize(size_t capacity);

		/// <summary>
		/// Return the Iterator pointing to first pair, or pointing to nothing if HashMap is empty
		/// </summary>
		/// <returns>Iterator pointing to first pair</returns>
		Iterator begin();

		/// <summary>
		/// Return the ConstIterator pointing to first pair, or pointing to nothing if HashMap is empty
		/// </summary>
		/// <returns>ConstIterator pointing to first pair</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Return the Iterator pointing to nothing in the HashMap
		/// </summary>
		/// <returns>Iterator pointing to nothing in the HashMap</returns>
		Iterator end();

		/// <summary>
		/// Return the ConstIterator pointing to nothing in the HashMap
		/// </summary>
		/// <returns>ConstIterator pointing to nothing in the HashMap</returns>
		ConstIterator end() const;

		/// <summary>
		/// Return the ConstIterator pointing to first pair, or pointing to nothing if HashMap is empty
		/// </summary>
		/// <returns>ConstIterator pointing to first pair</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Return the ConstIterator pointing to nothing in the HashMap
		/// </summary>
		/// <returns>ConstIterator pointing to nothing in the HashMap</returns>
		ConstIterator cend() const;

	private:
		/// <summary>
		/// Deep copy contents from another HashMap to this one
		/// </summary>
		/// <param name="other">The other HashMap to copy from</param>
		void DeepCopy(const HashMap& other);

		/// <summary>
		/// List of bucket
		/// </summary>
		BucketType mBuckets;

		/// <summary>
		/// Total number of pairs in HashMap
		/// </summary>
		size_t mSize = 0;

		/// <summary>
		/// Function to use when calculating hash value
		/// </summary>
		HashFunctor mHashFunction = HashFunctor();

		/// <summary>
		/// Function to use when comparing two key values
		/// </summary>
		KeyEqualityFunctor mKeyEqualityFunction = KeyEqualityFunctor();
	};
}

#include "HashMap.inl"