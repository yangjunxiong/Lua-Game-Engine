#pragma once

namespace GameEngine
{
#pragma region SList
	template <typename T>
	SList<T>::SList(const SList<T> & other)
	{
		DeepCopy(other);
	}

	template <typename T>
	SList<T>::SList(SList<T> && other) :
		mFront(other.mFront),
		mBack(other.mBack),
		mSize(other.mSize)
	{
		other.mFront = nullptr;
		other.mBack = nullptr;
		other.mSize = 0;
	}

	template <typename T>
	SList<T>::SList(std::initializer_list<T> list)
	{
		for (const auto data : list)
		{
			PushBack(data);
		}
	}

	template <typename T>
	SList<T> & SList<T>::operator=(const SList<T> & other)
	{
		if (this != &other)
		{
			Clear();
			DeepCopy(other);
		}

		return *this;
	}

	template <typename T>
	SList<T> & SList<T>::operator=(SList<T> && other)
	{
		if (this != &other)
		{
			Clear();
			mFront = other.mFront;
			mBack = other.mBack;
			mSize = other.mSize;
			other.mFront = nullptr;
			other.mBack = nullptr;
			other.mSize = 0;
		}

		return *this;
	}

	template <typename T>
	SList<T>::~SList()
	{
		Clear();
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushFront(const T & data)
	{
		Node* front = new Node(data, mFront);

		// List is empty, point back to front too
		if (mSize == 0)
		{
			mBack = front;
		}

		mSize++;
		mFront = front;

		return begin();
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T & data)
	{
		if (mSize == 0)  // List is empty
		{
			PushFront(data);
		}
		else  // List has a back already
		{
			Node* newBack = new Node(data);
			mBack->mNext = newBack;
			mBack = newBack;
			mSize++;
		}

		return Iterator(*this, mBack);
	}

	template <typename T>
	void SList<T>::PopFront()
	{
		if (mFront != nullptr)
		{
			// Delete old front and point to new front
			Node* newFront = mFront->mNext;
			delete mFront;
			mFront = newFront;

			// Decrement list size
			mSize--;

			// Handle the last item being removed
			if (mSize == 0)
			{
				mBack = nullptr;
			}
		}
	}

	template <typename T>
	void SList<T>::PopBack()
	{
		if (mSize == 1)
		{
			PopFront();
		}
		else if (mSize > 1)
		{
			Node* current = mFront;
			while (current->mNext != mBack)
			{
				current = current->mNext;
			}
			delete mBack;
			current->mNext = nullptr;
			mBack = current;
			mSize--;
		}
	}

	template <typename T>
	T & SList<T>::Front()
	{
		if (mFront != nullptr)
		{
			return mFront->mData;
		}
		else
		{
			throw std::exception("List is empty!");
		}
	}

	template <typename T>
	const T & SList<T>::Front() const
	{
		return const_cast<SList*>(this)->Front();
	}

	template <typename T>
	T & SList<T>::Back()
	{
		if (mBack != nullptr)
		{
			return mBack->mData;
		}
		else
		{
			throw std::exception("List is empty!");
		}
	}

	template <typename T>
	const T & SList<T>::Back() const
	{
		return const_cast<SList*>(this)->Back();
	}

	template <typename T>
	bool SList<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template <typename T>
	size_t SList<T>::Size() const
	{
		return mSize;
	}

	template <typename T>
	void SList<T>::Clear()
	{
		while (mSize > 0)
		{
			PopFront();
		}
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::begin()
	{
		return Iterator(*this, mFront);
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::begin() const
	{
		return Iterator(*this, mFront);
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::end()
	{
		return Iterator(*this);
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::end() const
	{
		return Iterator(*this);
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::InsertAfter(const SList<T>::Iterator & iterator, const T & data)
	{
		if (iterator.mOwner == this)
		{
			if (iterator == end())
			{
				return PushBack(data);
			}
			else
			{
				// Insert node
				Node* node = iterator.mNode;
				Node* next = node->mNext;
				Node* newNode = new Node(data, next);
				node->mNext = newNode;
				mSize++;

				// Update back
				if (next == nullptr)
				{
					mBack = newNode;
				}

				return Iterator(*this, newNode);
			}
		}
		else
		{
			throw std::exception("Iterator doesn't belong to this SList!");
		}
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Find(const T & data)
	{
		SList<T>::Iterator it = begin();
		while (it.mNode != nullptr)
		{
			if (it.mNode->mData == data)
			{
				return it;
			}
			++it;
		}
		
		return end();
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Find(const T & data) const
	{
		return const_cast<SList<T>*>(this)->Find(data);
	}

	template <typename T>
	void SList<T>::Remove(const T & data)
	{
		Remove(Find(data));
	}

	template <typename T>
	void SList<T>::Remove(const Iterator & iterator)
	{
		if (iterator.mOwner == this && iterator.mNode != nullptr)
		{
			Node* node = iterator.mNode;
			if (node == mFront)
			{
				PopFront();
			}
			else if (node == mBack)
			{
				PopBack();
			}
			else
			{
				Node* next = node->mNext;
				node->mData.~T();
				new(&node->mData) T(std::move(next->mData));
				node->mNext = next->mNext;
				delete next;
				mSize--;
				if (node->mNext == nullptr)
				{
					mBack = node;
				}
			}
		}
	}

	template <typename T>
	void SList<T>::DeepCopy(const SList & other)
	{
		for (auto & data : other)
		{
			PushBack(data);
		}
	}
#pragma endregion

#pragma region SListIterator
	template <typename T>
	SList<T>::Iterator::Iterator(const SList & owner, Node* node) :
		mOwner(&owner),
		mNode(node)
	{
	}

	template <typename T>
	typename SList<T>::Iterator & SList<T>::Iterator::operator++()
	{
		if (mNode != nullptr)
		{
			mNode = mNode->mNext;
		}

		return *this;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator copy = *this;
		operator++();
		return copy;
	}

	template <typename T>
	bool SList<T>::Iterator::operator==(const Iterator & other) const
	{
		return (mOwner == other.mOwner && mNode == other.mNode);
	}

	template <typename T>
	bool SList<T>::Iterator::operator!=(const Iterator & other) const
	{
		return !operator==(other);
	}

	template <typename T>
	T & SList<T>::Iterator::operator*() const
	{
		if (mNode == nullptr)
		{
			throw std::exception("Try to dereference an iterator pointing to nothing!");
		}
		else
		{
			return mNode->mData;
		}
	}

#pragma endregion

	template <typename T>
	SList<T>::Node::Node(const T & data, Node* next) :
		mData(data),
		mNext(next)
	{
	}
}
