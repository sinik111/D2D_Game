#include "pch.h"
#include "WinApp.h"

#include "SceneManager.h"
#include "ComponentSystem.h"
#include "D2DRenderer.h"
#include "ResourceManager.h"
#include "Debug.h"
#include "MyTime.h"
#include "MyTimeSystem.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void WinApp::Initialize()
{
	Debug::CreateConsole();

	wchar_t szPath[MAX_PATH]{};

	GetModuleFileNameW(nullptr, szPath, MAX_PATH);
	m_modulePath = szPath;

	GetCurrentDirectoryW(MAX_PATH, szPath);
	m_workingPath = szPath;

	Debug::Log(m_modulePath);
	Debug::Log(m_workingPath);

	m_hInstance = GetModuleHandleW(nullptr);
	m_hCursor = m_hCursor != nullptr ? m_hCursor : LoadCursorW(NULL, IDC_ARROW);

	WNDCLASSEX wc{};
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

	RECT clientRect{ 0, 0, (LONG)m_width, (LONG)m_height };

	AdjustWindowRect(&clientRect, m_windowStyle, FALSE);

	m_hWnd = CreateWindowExW(
		0,
		m_className.c_str(),
		m_windowName.c_str(),
		m_windowStyle,
		m_x, m_y,
		clientRect.right - clientRect.left, // �ʺ�
		clientRect.bottom - clientRect.top, // ����
		nullptr,
		nullptr,
		m_hInstance,
		this // �ν��Ͻ� �ּҸ� NCREATESTRUCT�� lpCreateParams�� ����
	);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	m_d2dRenderer = std::make_unique<D2DRenderer>(m_hWnd, m_width, m_height);
	m_d2dRenderer->Initialize();

	ComponentSystem::Get().BitmapRenderer().SetD2DRenderer(m_d2dRenderer.get());
	ComponentSystem::Get().TextRenderer().SetD2DRenderer(m_d2dRenderer.get());
	ComponentSystem::Get().PlayerInput().SetWindow(m_hWnd);
}

void WinApp::Shutdown()
{
	Debug::ReleaseConsole();

	SceneManager::Get().Shutdown();
	ResourceManager::Get().Release();
	m_d2dRenderer->Shutdown();
}

void WinApp::Run()
{
	MSG msg{};

	while (true)
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

void WinApp::Update()
{
	MyTimeSystem::Get().Update();
	Debug::UpdateFPS(true);

	ComponentSystem::Get().PlayerInput().Update();
	ComponentSystem::Get().PlayerInput().ProcessInput();

	ComponentSystem::Get().Script().UpdateSystem();

	SceneManager::Get().Update();

	ComponentSystem::Get().BitmapRenderer().MakeRenderCommands();
	ComponentSystem::Get().TextRenderer().MakeRenderCommands();
}

void WinApp::Render()
{
	m_d2dRenderer->BeginDraw(D2D1::ColorF(D2D1::ColorF::Black));
	m_d2dRenderer->ExecuteRenderCommands();
	m_d2dRenderer->EndDraw();
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