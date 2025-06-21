#pragma once

#include "CoInitializer.h"
#include "D2DRenderer.h"

class WinApp
{
protected:
	HWND m_hWnd{};
	HINSTANCE m_hInstance{};
	HICON m_hIcon{};
	HCURSOR m_hCursor{};
	HICON m_hIconSmall{};

	std::wstring m_className{ L"DefaultClassName" };
	std::wstring m_windowName{ L"DefaultWindowName" };
	std::wstring m_modulePath;
	std::wstring m_workingPath;

	CoInitializer m_coInitializer;
	std::unique_ptr<D2DRenderer> m_d2dRenderer;

	UINT m_classStyle{ CS_HREDRAW | CS_VREDRAW };
	DWORD m_windowStyle{ WS_OVERLAPPEDWINDOW };
	int m_x{ CW_USEDEFAULT };
	int m_y{ CW_USEDEFAULT };
	int m_width{ 800 };
	int m_height{ 600 };


public:
	virtual ~WinApp() = default;

public:
	virtual void Initialize();
	void Shutdown();

	void Run();

private:
	void Update();
	void Render();

protected:
	virtual void MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	friend LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);	
};