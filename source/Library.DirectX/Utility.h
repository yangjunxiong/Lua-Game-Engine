#pragma once

#include <string>
#include <vector>
#include <limits>
#include <functional>

namespace GameEngine
{
	class Utility final
	{
	public:
		static void LoadBinaryFile(const std::wstring& filename, std::vector<char>& data);
		static void ToWideString(const std::string& source, std::wstring& dest);
		static std::wstring ToWideString(const std::string& source);
		static void Totring(const std::wstring& source, std::string& dest);
		static std::string ToString(const std::wstring& source);

		Utility() = delete;
		Utility(const Utility&) = delete;
		Utility& operator=(const Utility&) = delete;
		Utility(Utility&&) = delete;
		Utility& operator=(Utility&&) = delete;
		~Utility() = default;
	};

	template <typename T>
	void UpdateValue(std::function<bool()> increasePredicate, std::function<bool()> decreasePredicate, T& value, const T& delta, std::function<void(const T&)> updateFunc = nullptr, const T& minValue = std::numeric_limits<T>::min(), const T& maxValue = std::numeric_limits<T>::max())
	{
		bool update = false;
		if (increasePredicate() && value < maxValue)
		{
			value += delta;
			value = std::min(value, maxValue);
			update = true;
		}
		else if (decreasePredicate() && value > minValue)
		{
			value -= delta;
			value = std::max(value, minValue);
			update = true;
		}

		if (update && updateFunc != nullptr)
		{
			updateFunc(value);
		}
	}
}