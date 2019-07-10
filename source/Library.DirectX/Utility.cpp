#include "pch.h"
#include "Utility.h"

using namespace std;

namespace GameEngine
{
	void Utility::LoadBinaryFile(const wstring& filename, vector<char>& data)
	{
		ifstream file(filename.c_str(), ios::binary);
		if (!file.good())
		{
			throw exception("Could not open file.");
		}

		file.seekg(0, ios::end);
		uint32_t size = (uint32_t)file.tellg();

		if (size > 0)
		{
			data.resize(size);
			file.seekg(0, ios::beg);
			file.read(&data.front(), size);
		}

		file.close();
	}

#pragma warning(push)
#pragma warning(disable: 4996)
	void Utility::ToWideString(const string& source, wstring& dest)
	{
		dest = wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(source);
	}

	wstring Utility::ToWideString(const string& source)
	{
		return wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(source);
	}

	void Utility::Totring(const wstring& source, string& dest)
	{
		dest = wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(source);
	}

	string Utility::ToString(const wstring& source)
	{
		return wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(source);
	}
#pragma warning(pop)
}