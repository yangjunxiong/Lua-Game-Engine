#pragma once
#include <string>

namespace GameEngine
{
	/// <summary>
	/// Generic hash function to handle byte stream
	/// </summary>
	size_t Hash(const char* data, size_t size);

	/// <summary>
	/// Default hash functor for generic types. Converts any object to byte stream and use additive hash to all byte values
	/// </summary>
	template <typename TKey>
	struct DefaultHash
	{
		size_t operator()(const TKey& key) const;
	};

	/// <summary>
	/// Specialization of DefaulHash for char* type, will use additive hash on all characters in the string, string should be null-terminated
	/// </summary>
	/// <exception cref="std::exception">char* is nullptr</exception>
	template<>
	struct DefaultHash<char*>
	{
		size_t operator()(const char* key) const;
	};

	/// <summary>
	/// Specialization of DefaulHash for std::string type, will use additive hash on all characters in the string
	/// </summary>
	template<>
	struct DefaultHash<std::string>
	{
		size_t operator()(const std::string& key) const;
	};

	/// <summary>
	/// Specialization of DefaulHash for std::string type, will use additive hash on all characters in the string
	/// </summary>
	template<>
	struct DefaultHash<const std::string>
	{
		size_t operator()(const std::string& key) const;
	};

	/// <summary>
	/// Default functor to compare two key values for HashMap, invoke "==" on two objects
	/// </summary>
	template <typename TKey>
	struct DefaultKeyEquality
	{
		bool operator()(const TKey& one, const TKey& two) const;
	};

	/// <summary>
	/// Specialization of DefaultKeyEquality for char* type, invoke "strcmp" to compare two strings
	/// </summary>
	template<>
	struct DefaultKeyEquality<char*>
	{
		bool operator()(const char* one, const char* two) const;
	};
}

#include "DefaultHashFunction.inl"
