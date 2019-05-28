#pragma once

// Windows
#include <windows.h>
#include <wrl.h>

// Standard
#include <string>
#include <sstream>
#include <algorithm>
#include <exception>
#include <initializer_list>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <thread>
#include <future>
#include <mutex>

// OpenGL
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#pragma warning(push)
#pragma warning(disable : 4201)
#pragma warning(push)
#pragma warning(disable : 4127)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>
#pragma warning(pop)
#pragma warning(pop)

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC

#include <stdlib.h>
#include <crtdbg.h>
#endif

// DirectX
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <DirectXMath.h>

// Local
#include "GameException.h"