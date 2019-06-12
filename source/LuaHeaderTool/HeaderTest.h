#pragma once
#include "Macro.h"
#include <string>

// This is comment
CLASS()
class HeaderTest
{
public:
	/// <summary>
	/// This is another comment
	/// </summary>
	/// <returns></returns>
	PROPERTY();
	static inline int mInt = 10;

	/*
	This is multiline comment
	*/
	PROPERTY();
	float mFloat = 12.125f;

	PROPERTY();
	std::string mString = "123 \" 456 \" \\ 789";

	FUNCTION();
	inline float Do()
	{
		float f = mFloat + 100.f;
		return f;
	};
};

CLASS()
class HeaderTest2 : public HeaderTest
{
public:
	PROPERTY();
	double mDouble = 0;

	double mDouble2 = 1;
};
