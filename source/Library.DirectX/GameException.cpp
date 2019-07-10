#include "pch.h"
#include "GameException.h"

namespace GameEngine
{
	GameException::GameException(const char* const message, HRESULT hr) :
		runtime_error(message), mHR(hr)
	{
	}

	HRESULT GameException::HR() const
	{
		return mHR;
	}

	std::wstring GameException::whatw() const
	{
		std::wstringstream whatw;
		whatw << what();
		return whatw.str();
	}
}