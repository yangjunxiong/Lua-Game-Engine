#pragma once

namespace GameEngine
{
	template <typename T>
	Vector<T>::Vector(size_t capacity, std::function<size_t(size_t, size_t)> reserveFunc) :
		mReserveFunction(reserveFunc)
	{
		Reserve(capacity);
	}

	template <typename T>
	Vector<T>::Vector(const Vector & other)
	{
		DeepCopy(other);
	}

	template <typename T>
	Vector<T>::Vector(Vector && other) :
		mSize(other.mSize),
		mCapacity(other.mCapacity),
		mArray(other.mArray),
		mReserveFunction(other.mReserveFunction)
	{
		other.mCapacity = 0;
		other.mSize = 0;
		other.mArray = nullptr;
	}

	template <typename T>
	Vector<T>::Vector(std::initializer_list<T> list, std::function<size_t(size_t, size_t)> reserveFunc) :
		mReserveFunction(reserveFunc)
	{
		Reserve(list.size());
		for (const auto& item : list)
		{
			PushBack(item);
		}
	}

	template <typename T>
	Vector<T>::~Vector()
	{
		Destroy();
	}

	template <typename T>
	Vector<T> & Vector<T>::operator=(const Vector & other)
	{
		if (this != &other)
		{
			Destroy();
			DeepCopy(other);
		}
		return *this;
	}

	template <typename T>
	Vector<T> & Vector<T>::operator=(Vector && other)
	{
		if (this != &other)
		{
			Destroy();
			mSize = other.mSize;
			mCapacity = other.mCapacity;
			mArray = other.mArray;
			mReserveFunction = other.mReserveFunction;
			other.mSize = 0;
			other.mCapacity = 0;
			other.mArray = nullptr;
		}
		return *this;
	}

	template <typename T>
	T & Vector<T>::operator[](size_t index)
	{
		if (index < mSize)
		{
			return mArray[index];
		}
		else
		{
			throw std::exception("Index out of range!");
		}
	}

	template <typename T>
	const T & Vector<T>::operator[](size_t index) const
	{
		return const_cast<Vector*>(this)->operator[](index);
	}

	template <typename T>
	T & Vector<T>::At(size_t index)
	{
		return operator[](index);
	}

	template <typename T>
	const T & Vector<T>::At(size_t index) const
	{
		return operator[](index);
	}

	template <typename T>
	T & Vector<T>::Front()
	{
		if (mSize > 0)
		{
			return mArray[0];
		}
		else
		{
			throw std::exception("Vector is empty!");
		}
	}

	template <typename T>
	const T & Vector<T>::Front() const
	{
		return const_cast<Vector*>(this)->Front();
	}

	template <typename T>
	T & Vector<T>::Back()
	{
		if (mSize > 0)
		{
			return mArray[mSize - 1];
		}
		else
		{
			throw std::exception("Vector is empty!");
		}
	}

	template <typename T>
	const T & Vector<T>::Back() const
	{
		return const_cast<Vector*>(this)->Back();
	}

	template <typename T>
	bool Vector<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template <typename T>
	size_t Vector<T>::Size() const
	{
		return mSize;
	}

	template <typename T>
	size_t Vector<T>::Capacity() const
	{
		return mCapacity;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::PushBack(const T & data)
	{
		return EmplaceBack(data);
	}

	template <typename T>
	template <typename... Args>
	typename Vector<T>::Iterator Vector<T>::EmplaceBack(Args&&... args)
	{
		// Increse capacity if necessary
		if (mSize == mCapacity)
		{
			Reserve(std::max(mCapacity + 1, mReserveFunction(mSize, mCapacity)));
		}

		new(mArray + mSize) T(std::forward<Args>(args)...);
		++mSize;

		return Iterator(*this, mSize - 1);
	}

	template <typename T>
	void Vector<T>::PopBack()
	{
		if (mSize > 0)
		{
			mArray[mSize - 1].~T();
			--mSize;
		}
	}

	template <typename T>
	void Vector<T>::Reserve(size_t capacity)
	{
		if (capacity > mCapacity)
		{
			T* newArray = reinterpret_cast<T*>(malloc(sizeof(T) * capacity));
			if (mArray != nullptr)
			{
				memmove(newArray, mArray, sizeof(T) * mSize);   // Move contents to new array
				free(mArray);
			}
			mArray = newArray;
			mCapacity = capacity;
		}
	}

	template <typename T>
	void Vector<T>::ShrinkToFit()
	{
		if (mSize == 0)
		{
			mCapacity = 0;
			free(mArray);
			mArray = nullptr;
		}
		else if (mSize < mCapacity)
		{
			mCapacity = mSize;
			mArray = reinterpret_cast<T*>(realloc(mArray, sizeof(T) * mSize));
		}
	}

	template <typename T>
	void Vector<T>::Resize(size_t newSize, const T& sample)
	{
		if (newSize > mSize)
		{
			if (newSize > mCapacity)
			{
				mArray = reinterpret_cast<T*>(realloc(mArray, sizeof(T) * newSize));
				mCapacity = newSize;
			}
			for (size_t i = mSize; i < newSize; ++i)
			{
				new(mArray + i) T(sample);
			}
		}
		else if (newSize < mSize)
		{
			for (size_t i = newSize; i < mSize; ++i)
			{
				mArray[i].~T();
			}
		}
		mSize = newSize;
	}

	template <typename T>
	void Vector<T>::Clear()
	{
		while (mSize > 0)
		{
			PopBack();
		}
	}

	template <typename T>
	void Vector<T>::Destroy()
	{
		if (mCapacity > 0)
		{
			Clear();
			free(mArray);
			mArray = nullptr;
			mCapacity = 0;
		}
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Find(const T & data)
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			if (mArray[i] == data)
			{
				return Iterator(*this, i);
			}
		}
		return end();
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::Find(const T & data) const
	{
		return ConstIterator(const_cast<Vector*>(this)->Find(data));
	}

	template <typename T>
	void Vector<T>::Remove(const T & data)
	{
		Remove(Find(data));
	}

	template <typename T>
	void Vector<T>::Remove(const Vector<T>::Iterator& iterator)
	{
		Remove(iterator, iterator + 1);
	}

	template <typename T>
	void Vector<T>::Remove(Vector<T>::Iterator begin, Vector<T>::Iterator end)
	{
		if (begin.mOwner == this && begin.mOwner == end.mOwner)
		{
			// Handle backward range
			if (begin.mIndex > end.mIndex)
			{
				std::swap(begin.mIndex, end.mIndex);
			}

			if (begin.mIndex < mSize)
			{
				// Make sure end index doesn't exceed size
				end.mIndex = end.mIndex > mSize ? mSize : end.mIndex;

				// Destruct objects
				for (size_t i = begin.mIndex; i < end.mIndex; ++i)
				{
					mArray[i].~T();
				}

				// Fill gap
				size_t moveNumber = this->end().mIndex - end.mIndex;
				memmove(mArray + begin.mIndex, mArray + end.mIndex, sizeof(T) * moveNumber);

				// Change size
				mSize -= end.mIndex - begin.mIndex;
			}
		}
		else
		{
			throw std::exception("begin and end iterator don't belong to the same vector or don't have owner!");
		}
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Iterator(*this, 0);
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::begin() const
	{
		return ConstIterator(*this, 0);
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::end()
	{
		return Iterator(*this, mSize);
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::end() const
	{
		return ConstIterator(*this, mSize);
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return ConstIterator(*this, 0);
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return ConstIterator(*this, mSize);
	}

	template <typename T>
	void Vector<T>::DeepCopy(const Vector & other)
	{
		mCapacity = other.mCapacity;
		mSize = other.mSize;
		mReserveFunction = other.mReserveFunction;

		if (mCapacity > 0)
		{
			mArray = reinterpret_cast<T*>(malloc(sizeof(T) * mCapacity));
			for (size_t i = 0; i < mSize; ++i)
			{
				new(mArray + i) T(other.mArray[i]);
			}
		}
	}

	template <typename T>
	size_t Vector<T>::DefaultReserveFunction::operator()(size_t size, size_t capacity) const
	{
		size_t currentCapacity = capacity == 0 ? 8 : capacity;
		while (currentCapacity <= size)
		{
			currentCapacity *= 2;
		}
		return currentCapacity;
	}

#pragma region Iterator
	template <typename T>
	Vector<T>::Iterator::Iterator(const Vector & owner, size_t index) :
		mOwner(&owner),
		mIndex(index)
	{}

	template <typename T>
	typename Vector<T>::Iterator & Vector<T>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Try to increment uninitialized iterator");
		}

		if (mIndex < mOwner->mSize)
		{
			++mIndex;
		}
		return *this;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator copy = *this;
		operator++();
		return copy;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator+(size_t step) const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Try to increment uninitialized iterator");
		}

		size_t newIndex = std::min(mIndex + step, mOwner->mSize);
		return Iterator(*mOwner, newIndex);
	}

	template <typename T>
	bool Vector<T>::Iterator::operator==(const Iterator & other) const
	{
		return (mOwner == other.mOwner && mIndex == other.mIndex);
	}

	template <typename T>
	bool Vector<T>::Iterator::operator!=(const Iterator & other) const
	{
		return !operator==(other);
	}

	template <typename T>
	T & Vector<T>::Iterator::operator*()
	{
		if (mOwner != nullptr && mOwner->Size() > mIndex)
		{
			return mOwner->mArray[mIndex];
		}
		else
		{
			throw std::exception("Try to dereference an iterator pointing to invalid data!");
		}
	}

	template <typename T>
	const T & Vector<T>::Iterator::operator*() const
	{
		return const_cast<Iterator*>(this)->operator*();
	}

	template <typename T>
	Vector<T>::ConstIterator::ConstIterator(const Vector & owner, size_t index) :
		mOwner(&owner),
		mIndex(index)
	{}

	template <typename T>
	Vector<T>::ConstIterator::ConstIterator(const Iterator & other) :
		mOwner(other.mOwner),
		mIndex(other.mIndex)
	{}

	template <typename T>
	typename Vector<T>::ConstIterator & Vector<T>::ConstIterator::operator=(const Iterator & other)
	{
		mOwner = other.mOwner;
		mIndex = other.mIndex;
		return *this;
	}

	template <typename T>
	typename Vector<T>::ConstIterator & Vector<T>::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Try to increment uninitialized iterator");
		}

		if (mIndex < mOwner->mSize)
		{
			++mIndex;
		}
		return *this;
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator copy = *this;
		operator++();
		return copy;
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator+(size_t step)
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Try to increment uninitialized iterator");
		}

		size_t newIndex = std::min(mIndex + step, mOwner->mSize);
		return Iterator(*mOwner, newIndex);
	}

	template <typename T>
	bool Vector<T>::ConstIterator::operator==(const ConstIterator & other) const
	{
		return (mOwner == other.mOwner && mIndex == other.mIndex);
	}

	template <typename T>
	bool Vector<T>::ConstIterator::operator!=(const ConstIterator & other) const
	{
		return !operator==(other);
	}

	template <typename T>
	const T & Vector<T>::ConstIterator::operator*() const
	{
		if (mOwner != nullptr && mOwner->Size() > mIndex)
		{
			return mOwner->At(mIndex);
		}
		else
		{
			throw std::exception("Try to dereference an iterator pointing to invalid data!");
		}
	}
#pragma endregion
};