#pragma once

namespace GameEngine
{
	template <typename TKey>
	size_t DefaultHash<TKey>::operator()(const TKey& key) const
	{
		const char* address = reinterpret_cast<const char*>(&key);
		return Hash(address, sizeof(key));
	}

	template <typename TKey>
	bool DefaultKeyEquality<TKey>::operator()(const TKey& one, const TKey& two) const
	{
		return one == two;
	}
}