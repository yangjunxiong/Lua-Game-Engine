#pragma once

#include <windows.h>
#include <exception>
#include <string>

namespace GameEngine
{
	class GameException : public std::runtime_error
	{
	public:
		GameException(const char* const message, HRESULT hr = S_OK);

		HRESULT HR() const;
		std::wstring whatw() const;

	private:
		HRESULT mHR;
	};

	inline void ThrowIfFailed(HRESULT hr, const char* const message = "")
	{
		if (FAILED(hr))
		{
			throw GameException(message, hr);
		}
	}
}