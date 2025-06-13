#include "pch.h"
#include "WinApp.h"

WinApp::WinApp()
	: m_hWnd(nullptr), m_hInstance(nullptr), m_width(0), m_height(0),
	m_classStyle(0), m_hIcon(nullptr), m_hCursor(nullptr), m_hIconSmall(nullptr),
	m_windowStyle(0), m_x(0), m_y(0), m_isRunning(false), m_renderer(nullptr)
{
	
}

WinApp::~WinApp()
{

}

// �����츦 �����Ҷ� ������ Ŭ���� �ν��Ͻ��� ���� �ٸ��� �ִ�.
// �ʱ� WM_NCCREATE ���� ������ �ڵ鿡 �ν��Ͻ� �ַθ� �����ϰ�
// �޼����� ������ ���� �������ڵ鿡 ������ �ּҷ� MessageProc �� ȣ���Ѵ�.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WinApp* winApp = nullptr;
	// WM_NCCREATE: ������ ���� ���� �ʱ⿡, ������ ���� ����. WM_CREATE���� �������߻�
	if (uMsg == WM_NCCREATE)
	{
		// lParam�� CREATESTRUCT* �̴�
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		winApp = reinterpret_cast<WinApp*>(cs->lpCreateParams);

		// HWND�� this ������ ����
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(winApp));
	}
	else
	{
		// WM_NCCREATE�� �ƴ� ���� HWND���� this �����͸� �����´�
		winApp = reinterpret_cast<WinApp*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
	}

	if (winApp != nullptr)
	{
		winApp->MessageProc(hWnd, uMsg, wParam, lParam); // ��� �Լ� ȣ��
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
	m_modulePath = szPath;

	GetCurrentDirectoryW(MAX_PATH, szPath);
	m_workingPath = szPath;

	OutputDebugStringW(std::wstring(m_modulePath + L"\n").c_str());
	OutputDebugStringW(std::wstring(m_workingPath + L"\n").c_str());

	m_hInstance = GetModuleHandleW(NULL);

	WNDCLASSEX wc = {};

	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = m_classStyle;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = m_className.c_str();
	wc.hCursor = m_hCursor;
	wc.hIcon = m_hIcon;
	wc.hCursor = m_hCursor;
	wc.hIconSm = m_hIconSmall;

	RegisterClassExW(&wc);

	SIZE clientSize = { (LONG)m_width, (LONG)m_height };
	RECT clientRect = { 0, 0, clientSize.cx, clientSize.cy };
	AdjustWindowRect(&clientRect, m_windowStyle, FALSE);

	m_hWnd = CreateWindowExW(
		0,
		m_className.c_str(),
		m_windowName.c_str(),
		m_windowStyle,
		m_x, m_y,
		clientRect.right - clientRect.left, // �ʺ�
		clientRect.bottom - clientRect.top, // ����
		NULL,
		NULL,
		m_hInstance,
		this // �ν��Ͻ� �ּҸ� NCREATESTRUCT�� lpCreateParams�� ����
	);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	HRESULT hr = CoInitialize(nullptr);

	m_renderer = new D2DRenderer(m_hWnd, m_width, m_height);
	m_renderer->Initialize();
}

void WinApp::Shutdown()
{
	delete m_renderer;

	CoUninitialize();
}

void WinApp::Run()
{
	MSG msg = {};

	while (m_isRunning)
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
	return m_isRunning;
}

void WinApp::Update()
{

}

void WinApp::Render()
{
	
}
