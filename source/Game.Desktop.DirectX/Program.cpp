#include "pch.h"
#include "GameException.h"
#include "UtilityWin32.h"
#include "RenderingGame.h"

using namespace GameEngine;
using namespace Rendering;
using namespace std;
using namespace std::string_literals;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int showCommand)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	ThrowIfFailed(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED), "Error initializing COM.");

	current_path(UtilityWin32::ExecutableDirectory());
	
	const wstring windowClassName = L"RenderingClass"s;
	const wstring windowTitle = L"DirectX Essentials"s;

	const SIZE RenderTargetSize = { 1024, 768 };
	HWND windowHandle;
	WNDCLASSEX window;

	UtilityWin32::InitializeWindow(window, windowHandle, instance, windowClassName, windowTitle, RenderTargetSize, showCommand);

	auto getRenderTargetSize = [&RenderTargetSize](SIZE & renderTargetSize)
	{
		renderTargetSize = RenderTargetSize;
	};

	auto getWindow = [&]() -> void*
	{
		return reinterpret_cast<void*>(windowHandle);
	};

	RenderingGame game(getWindow, getRenderTargetSize);
	game.UpdateRenderTargetSize();
	game.Initialize();

	MSG message{ 0 };

	try
	{
		while (message.message != WM_QUIT)
		{
			if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			else
			{
				game.Run();
			}
		}
	}
	catch (GameException ex)
	{
		MessageBox(windowHandle, ex.whatw().c_str(), windowTitle.c_str(), MB_ABORTRETRYIGNORE);
	}

	game.Shutdown();
	UnregisterClass(windowClassName.c_str(), window.hInstance);
	CoUninitialize();

	return static_cast<int>(message.wParam);
}