#pragma once

#include <SDKDDKVer.h>

// Headers for CppUnitTest
#include "CppUnitTest.h"
#include <exception>
#include <initializer_list>
#include <cstddef>
#include <functional>
#include <algorithm>
#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/glm.hpp>
#pragma warning(pop)

#if defined(DEBUG) || defined(_DEBUG)
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif