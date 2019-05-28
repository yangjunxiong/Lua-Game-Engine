namespace GameEngine
{
#pragma region Public
	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::HashMap(size_t capacity)
	{
		mBuckets.Resize(std::max(size_t(1), capacity));
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::HashMap(const std::initializer_list<PairType>& list, size_t capacity)
	{
		mBuckets.Resize(std::max(size_t(1), capacity));
		for (const auto& pair : list)
		{
			Insert(pair);
		}
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::HashMap(HashMap&& other) :
		mSize(other.mSize),
		mHashFunction(other.mHashFunction),
		mKeyEqualityFunction(other.mKeyEqualityFunction)
	{
		mBuckets = std::move(other.mBuckets);
		other.mSize = 0;
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>& HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::operator=(const HashMap& other)
	{
		if (this != &other)
		{
			// Don't need to Clear() here, because Vector operator=() will clear itself before copying
			DeepCopy(other);
		}
		return *this;
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>& HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::operator=(HashMap&& other)
	{
		if (this != &other)
		{
			// Don't need to Clear() here, because Vector operator=() will clear itself before moving
			mBuckets = std::move(other.mBuckets);
			mSize = other.mSize;
			mHashFunction = other.mHashFunction;
			mKeyEqualityFunction = other.mKeyEqualityFunction;
			other.mSize = 0;
		}
		return *this;
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	TValue& HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::operator[](const TKey& key)
	{
		// Find existing entry
		ChainType& chain = mBuckets[mHashFunction(key) % mBuckets.Size()];
		for (auto& pair : chain)
		{
			if (mKeyEqualityFunction(pair.first, key))
			{
				return pair.second;
			}
		}

		// Create new entry
		auto it = chain.PushBack(std::make_pair(key, TValue()));
		++mSize;
		return (*it).second;
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	const TValue& HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::operator[](const TKey& key) const
	{
		return At(key);
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	void HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Clear()
	{
		for (auto& chain : mBuckets)
		{
			chain.Clear();
		}
		mSize = 0;
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	TValue& HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::At(const TKey& key)
	{
		auto it = Find(key);
		if (it != end())
		{
			return (*it).second;
		}
		else
		{
			throw std::exception("Entry with given key does not exist");
		}
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	const TValue& HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::At(const TKey& key) const
	{
		return const_cast<HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>*>(this)->At(key);
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	inline size_t HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Size() const
	{
		return mSize;
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	bool HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::IsEmpty() const
	{
		return mSize == 0;
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	bool HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ContainsKey(const TKey& key) const
	{
		return Find(key) != end();
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	bool HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ContainsKey(const TKey& key, Iterator& iterator)
	{
		iterator = Find(key);
		return iterator != end();
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	bool HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ContainsKey(const TKey& key, ConstIterator& iterator) const
	{
		iterator = Find(key);
		return iterator != end();
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Iterator HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Find(const TKey& key)
	{
		// Find bucket
		const size_t hash = mHashFunction(key);
		const size_t index = hash % mBuckets.Size();
		ChainType& chain = mBuckets[index];

		for (auto it = chain.begin(); it != chain.end(); ++it)
		{
			if (mKeyEqualityFunction((*it).first, key))
			{
				return Iterator(*this, index, it);
			}
		}

		return end();
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Find(const TKey& key) const
	{
		return ConstIterator(const_cast<HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>*>(this)->Find(key));
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	std::pair<typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Iterator, bool> HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Insert(const PairType& pair)
	{
		// Find bucket
		const size_t hash = mHashFunction(pair.first);
		const size_t index = hash % mBuckets.Size();
		ChainType& chain = mBuckets[index];

		// Check if there exists same key
		for (auto it = chain.begin(); it != chain.end(); ++it)
		{
			if (mKeyEqualityFunction((*it).first, pair.first))
			{
				return std::make_pair(Iterator(*this, index, it), false);
			}
		}

		// Insert to bucket
		ChainIterator chainIterator = chain.PushBack(pair);
		++mSize;
		return std::make_pair(Iterator(*this, index, chainIterator), true);
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	void HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Remove(const TKey& key)
	{
		Remove(Find(key));
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	void HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Remove(const Iterator& position)
	{
		if (position.mOwner != this)
		{
			throw std::exception("Iterator doesn't belong to the container");
		}

		if (position.mIndex < mBuckets.Size())
		{
			ChainType& chain = mBuckets[position.mIndex];
			if (position.mChainIterator != chain.end())
			{
				chain.Remove(position.mChainIterator);
				--mSize;
			}
		}
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	void HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Resize(size_t capacity)
	{
		capacity = std::max(size_t(1), capacity);
		BucketType temp = std::move(mBuckets);
		mSize = 0;
		mBuckets.Resize(capacity);
		for (const auto& chain : temp)
		{
			for (const auto& pair : chain)
			{
				Insert(pair);
			}
		}
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Iterator HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::begin()
	{
		for (size_t i = 0; i < mBuckets.Size(); ++i)
		{
			ChainType& chain = mBuckets[i];
			if (chain.Size() > 0)
			{
				return Iterator(*this, i, chain.begin());
			}
		}

		return end();
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::begin() const
	{
		return ConstIterator(const_cast<HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>*>(this)->begin());
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Iterator HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::end()
	{
		return Iterator(*this, mBuckets.Size(), ChainIterator());
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::end() const
	{
		return ConstIterator(*this, mBuckets.Size(), ChainIterator());
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::cbegin() const
	{
		return begin();
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::cend() const
	{
		return end();
	}
#pragma endregion

#pragma region Private
	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	void HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::DeepCopy(const HashMap& other)
	{
		mBuckets = other.mBuckets;
		mSize = other.mSize;
		mHashFunction = other.mHashFunction;
		mKeyEqualityFunction = other.mKeyEqualityFunction;
	}
#pragma endregion

#pragma region Iterator
	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Iterator::Iterator(const HashMap& owner, const size_t& index, const typename ChainIterator& chainIterator) :
		mOwner(&owner),
		mIndex(index),
		mChainIterator(chainIterator)
	{}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	inline bool HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Iterator::operator==(const Iterator& other) const
	{
		return mOwner == other.mOwner && mIndex == other.mIndex && (mChainIterator == other.mChainIterator || mIndex == mOwner->mBuckets.Size());
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	inline bool HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Iterator::operator!=(const Iterator& other) const
	{
		return !operator==(other);
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::PairType& HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Iterator::operator*()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Iterator doesn't belong to any hashmap");
		}

		if (mIndex >= mOwner->mBuckets.Size() || mChainIterator == mOwner->mBuckets[mIndex].end())
		{
			throw std::exception("Iterator doesn't point to any item in hashmap");
		}

		return *mChainIterator;
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename const HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::PairType& HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Iterator::operator*() const
	{
		return const_cast<Iterator*>(this)->operator*();
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::PairType* HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Iterator::operator->()
	{
		return &operator*();
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename const HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::PairType* HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Iterator::operator->() const
	{
		return &operator*();
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Iterator& HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Iterator does not belong to any hashmap");
		}

		if (mIndex < mOwner->mBuckets.Size())
		{
			++mChainIterator;                                        // Move to next list node
			if (mChainIterator == mOwner->mBuckets[mIndex].end())    // If no more nodes, move to next bucket
			{
				while (++mIndex < mOwner->mBuckets.Size())
				{
					ChainType& chain = const_cast<HashMap*>(mOwner)->mBuckets[mIndex];
					if (chain.Size() > 0)
					{
						mChainIterator = chain.begin();
						break;
					}
				}
			}
		}
		
		return *this;
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Iterator HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::Iterator::operator++(int)
	{
		Iterator copy = *this;
		operator++();
		return copy;
	}
#pragma endregion

#pragma region ConstIterator
	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator::ConstIterator(const HashMap& owner, const size_t& index, const typename ChainIterator& chainIterator) :
		mOwner(&owner),
		mIndex(index),
		mChainIterator(chainIterator)
	{}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator::ConstIterator(const Iterator& other) :
		mOwner(other.mOwner),
		mIndex(other.mIndex),
		mChainIterator(other.mChainIterator)
	{}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator& HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator::operator=(const Iterator& other)
	{
		mOwner = other.mOwner;
		mIndex = other.mIndex;
		mChainIterator = other.mChainIterator;
		return *this;
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	inline bool HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return mOwner == other.mOwner && mIndex == other.mIndex && (mChainIterator == other.mChainIterator || mIndex == mOwner->mBuckets.Size());
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	inline bool HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return !operator==(other);
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	const typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::PairType& HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Iterator doesn't belong to any hashmap");
		}

		if (mIndex >= mOwner->mBuckets.Size() || mChainIterator == mOwner->mBuckets[mIndex].end())
		{
			throw std::exception("Iterator doesn't point to any item in hashmap");
		}

		return *mChainIterator;
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	const typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::PairType* HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator::operator->() const
	{
		return &operator*();
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator& HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Iterator does not belong to any hashmap");
		}

		if (mIndex < mOwner->mBuckets.Size())
		{
			++mChainIterator;                                        // Move to next list node
			if (mChainIterator == mOwner->mBuckets[mIndex].end())    // If no more nodes, move to next bucket
			{
				while (++mIndex < mOwner->mBuckets.Size())
				{
					const ChainType& chain = mOwner->mBuckets[mIndex];
					if (chain.Size() > 0)
					{
						mChainIterator = chain.begin();
						break;
					}
				}
			}
		}

		return *this;
	}

	template <typename TKey, typename TValue, typename HashFunctor, typename KeyEqualityFunctor>
	typename HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator HashMap<TKey, TValue, HashFunctor, KeyEqualityFunctor>::ConstIterator::operator++(int)
	{
		ConstIterator copy = *this;
		operator++();
		return copy;
	}
#pragma endregion
}