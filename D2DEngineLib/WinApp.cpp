#include "pch.h"
#include "WinApp.h"

#include <psapi.h>                // GetProcessMemoryInfo, PROCESS_MEMORY_COUNTERS_EX
#pragma comment(lib, "psapi.lib")

#include "SceneManager.h"
#include "ComponentSystem.h"
#include "D2DRenderer.h"
#include "ResourceManager.h"
#include "Debug.h"
#include "MyTime.h"
#include "MyTimeSystem.h"
#include "DebugSystem.h"
#include "Screen.h"
#include "Camera.h"

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
		clientRect.right - clientRect.left, // 너비
		clientRect.bottom - clientRect.top, // 높이
		nullptr,
		nullptr,
		m_hInstance,
		this // 인스턴스 주소를 NCREATESTRUCT의 lpCreateParams에 저장
	);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	m_d2dRenderer = std::make_unique<D2DRenderer>(m_hWnd, m_width, m_height);
	m_d2dRenderer->Initialize();

	Screen::Get().SetWidth(m_width);
	Screen::Get().SetHeight(m_height);

	ComponentSystem::Get().BitmapRenderer().SetD2DRenderer(m_d2dRenderer.get());
	ComponentSystem::Get().TextRenderer().SetD2DRenderer(m_d2dRenderer.get());
	ComponentSystem::Get().PlayerInput().SetWindow(m_hWnd);
	SceneManager::Get().SetD2DRenderer(m_d2dRenderer.get());
}

void WinApp::Shutdown()
{
	Debug::ReleaseConsole();

	SceneManager::Get().Shutdown();
	ResourceManager::Get().Release();
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
	HANDLE hProcess = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS_EX pmc;
	pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);

	// 현재 프로세스의 메모리 사용 정보 조회
	GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

	DebugSystem::Get().SetDRAMCurrentUsage(pmc.WorkingSetSize);
	DebugSystem::Get().SetPageFileCurrentUsage(pmc.PagefileUsage);
	

	MyTimeSystem::Get().Update();
	Debug::UpdateFPS(true);

	SceneManager::Get().CheckSceneChanged();

	ComponentSystem::Get().Transform().UnmarkDirtyThisFrame();

	ComponentSystem::Get().PlayerInput().Update();
	ComponentSystem::Get().PlayerInput().ProcessInput();

	ComponentSystem::Get().Script().Update();
	ComponentSystem::Get().Animator().Update();

	SceneManager::Get().Update();

	Camera::s_mainCamera->Update();

	ComponentSystem::Get().BitmapRenderer().Update();
	ComponentSystem::Get().TextRenderer().Update();
}

void WinApp::Render()
{
	m_d2dRenderer->BeginDraw(D2D1::ColorF(D2D1::ColorF::Black));
	m_d2dRenderer->ExecuteRenderQueue();
	m_d2dRenderer->EndDraw();
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WinApp* winApp = nullptr;

	if (uMsg == WM_NCCREATE)
	{
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		winApp = reinterpret_cast<WinApp*>(cs->lpCreateParams);

		SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(winApp));
	}
	else
	{
		winApp = reinterpret_cast<WinApp*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
	}

	if (winApp != nullptr)
	{
		winApp->MessageProc(hWnd, uMsg, wParam, lParam);
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

	case WM_KEYUP:
		if (wParam == 'P')
		{
			m_d2dRenderer->Trim();
		}
		break;
	}
}