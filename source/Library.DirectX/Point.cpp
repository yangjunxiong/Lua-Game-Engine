#include "pch.h"
#include "Point.h"

namespace GameEngine
{
	const Point Point::Zero(0, 0);

	Point::Point(const int32_t x, const int32_t y) :
		X(x), Y(y)
	{
	}
}