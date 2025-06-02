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

	std::wstring m_ClassName;
	std::wstring m_WindowName;
	std::wstring m_ModulePath;
	std::wstring m_WorkingPath;

	std::unique_ptr<D2DRenderer> m_pRenderer;

	UINT m_ClassStyle;
	UINT m_X;
	UINT m_Y;
	UINT m_Width;
	UINT m_Height;

	DWORD m_WindowStyle;

	bool m_IsRunning;

public:
	WinApp();
	virtual ~WinApp() = default;

public:
	virtual void Initialize();
	virtual void Shutdown();

	void Run();
	bool IsRunning();

private:
	void Update();
	virtual void Render();

protected:
	virtual void MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	friend LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
};