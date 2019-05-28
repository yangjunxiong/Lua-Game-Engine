/// \file pch.h
/// \brief Contains the precompiled headers

#ifndef PCH_H
#define PCH_H
// Standard library
#include <assert.h>
#include <exception>
#include <initializer_list>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <algorithm>
#include <string>
#include <thread>
#include <future>
#include <mutex>
#include <type_traits>

// Lua
#include <lua.hpp>

// OpenGL
#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/glm.hpp>
#pragma warning(pop)
#endif
