#pragma once

#include "D2DRenderer.h"

class WinApp
{
protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	HICON m_hIcon;
	HCURSOR m_hCursor;
	HICON m_hIconSmall;

	std::wstring m_className;
	std::wstring m_windowName;
	std::wstring m_modulePath;
	std::wstring m_workingPath;

	D2DRenderer* m_renderer;

	UINT m_classStyle;
	UINT m_x;
	UINT m_y;
	UINT m_width;
	UINT m_height;

	DWORD m_windowStyle;

	bool m_isRunning;

public:
	WinApp();
	virtual ~WinApp();

public:
	virtual void Initialize();
	virtual void Shutdown();

	void Run();
	bool IsRunning();

private:
	virtual void Update();
	virtual void Render();

protected:
	virtual void MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	friend LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
};