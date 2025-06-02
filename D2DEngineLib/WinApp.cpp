#include "pch.h"
#include "WinApp.h"

WinApp::WinApp()
	: m_hWnd(nullptr), m_hInstance(nullptr), m_Width(0), m_Height(0),
	m_ClassStyle(0), m_hIcon(nullptr), m_hCursor(nullptr), m_hIconSmall(nullptr),
	m_WindowStyle(0), m_X(0), m_Y(0), m_IsRunning(false)
{
	
}

// 윈도우를 생성할때 설정한 클래스 인스턴스는 각각 다를수 있다.
// 초기 WM_NCCREATE 에서 윈도우 핸들에 인스턴스 주로를 설정하고
// 메세지를 받을때 마다 윈도우핸들에 설정된 주소로 MessageProc 를 호출한다.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WinApp* pWinApp = nullptr;
	// WM_NCCREATE: 윈도우 생성 아주 초기에, 프레임 생성 전에. WM_CREATE보다 이전에발생
	if (uMsg == WM_NCCREATE)
	{
		// lParam은 CREATESTRUCT* 이다
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		pWinApp = reinterpret_cast<WinApp*>(cs->lpCreateParams);

		// HWND에 this 포인터 저장
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWinApp));
	}
	else
	{
		// WM_NCCREATE가 아닐 때는 HWND에서 this 포인터를 가져온다
		pWinApp = reinterpret_cast<WinApp*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
	}

	if (pWinApp != nullptr)
	{
		pWinApp->MessageProc(hWnd, uMsg, wParam, lParam); // 멤버 함수 호출
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

void WinApp::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
}

void WinApp::Initialize()
{
	wchar_t szPath[MAX_PATH] = {};

	GetModuleFileNameW(NULL, szPath, MAX_PATH);
	m_ModulePath = szPath;

	GetCurrentDirectoryW(MAX_PATH, szPath);
	m_WorkingPath = szPath;

	OutputDebugStringW(std::wstring(m_ModulePath + L"\n").c_str());
	OutputDebugStringW(std::wstring(m_WorkingPath + L"\n").c_str());

	m_hInstance = GetModuleHandleW(NULL);

	WNDCLASSEX wc = {};

	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = m_ClassStyle;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = m_ClassName.c_str();
	wc.hCursor = m_hCursor;
	wc.hIcon = m_hIcon;
	wc.hCursor = m_hCursor;
	wc.hIconSm = m_hIconSmall;

	RegisterClassExW(&wc);

	SIZE clientSize = { (LONG)m_Width, (LONG)m_Height };
	RECT clientRect = { 0, 0, clientSize.cx, clientSize.cy };
	AdjustWindowRect(&clientRect, m_WindowStyle, FALSE);

	m_hWnd = CreateWindowExW(
		0,
		m_ClassName.c_str(),
		m_WindowName.c_str(),
		m_WindowStyle,
		m_X, m_Y,
		clientRect.right - clientRect.left, // 너비
		clientRect.bottom - clientRect.top, // 높이
		NULL,
		NULL,
		m_hInstance,
		this // 인스턴스 주소를 NCREATESTRUCT의 lpCreateParams에 저장
	);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	HRESULT hr = CoInitialize(nullptr);

	m_pRenderer = std::make_unique<D2DRenderer>(m_hWnd, m_Width, m_Height);
	m_pRenderer->Initialize();
}

void WinApp::Shutdown()
{
	CoUninitialize();
}

void WinApp::Run()
{
	MSG msg = {};

	while (m_IsRunning)
	{
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		Update();
		Render();
	}
}

bool WinApp::IsRunning()
{
	return m_IsRunning;
}

void WinApp::Update()
{

}

void WinApp::Render()
{
	
}
