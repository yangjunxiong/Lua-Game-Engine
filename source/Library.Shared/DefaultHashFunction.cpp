#include "pch.h"
#include "DefaultHashFunction.h"

namespace GameEngine
{
	size_t Hash(const char* data, size_t size)
	{
		size_t hash = 0;
		for (size_t i = 0; i < size; ++i)
		{
			hash += 13 * *data;
			++data;
		}
		return hash;
	}

	size_t DefaultHash<char*>::operator()(const char* key) const
	{
		if (key == nullptr)
		{
			throw std::exception("Default hash function (char*): Key is nullptr!");
		}
		return Hash(key, strlen(key));
	}

	size_t DefaultHash<std::string>::operator()(const std::string& key) const
	{
		return Hash(key.c_str(), key.length());
	}

	size_t DefaultHash<const std::string>::operator()(const std::string& key) const
	{
		return Hash(key.c_str(), key.length());
	}

	bool DefaultKeyEquality<char*>::operator()(const char* one, const char* two) const
	{
		return strcmp(one, two) == 0;
	}
}