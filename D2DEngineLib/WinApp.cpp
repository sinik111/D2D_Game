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

	m_d2dRenderer.Initialize(m_hWnd, m_width, m_height);

	Screen::Get().SetWidth(m_width);
	Screen::Get().SetHeight(m_height);

	ComponentSystem::Get().Renderer().SetD2DRenderer(&m_d2dRenderer);
	ComponentSystem::Get().Physics().SetD2DRenderer(&m_d2dRenderer);
	ComponentSystem::Get().PlayerInput().SetWindow(m_hWnd);
	SceneManager::Get().SetD2DRenderer(&m_d2dRenderer);
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
	// [Tip] 실행순서

	MyTimeSystem::Get().Update();
	
	Profiling();

	ComponentSystem::Get().Transform().UnmarkDirtyThisFrame();

	SceneManager::Get().CheckSceneChanged();

	// [Tip] Scene Enter/이전 프레임에 생성된 오브젝트(GameObject, Component)들 초기화
	SceneManager::Get().InitializeObjects();

	ComponentSystem::Get().Script().CallStart();

	m_accumulatedDeltaTime += MyTime::DeltaTime();

	const float fixedDeltaTime = MyTime::FixedDeltaTime();

	while (m_accumulatedDeltaTime >= fixedDeltaTime)
	{
		m_accumulatedDeltaTime -= fixedDeltaTime;

		ComponentSystem::Get().Script().CallFixedUpdate();

		ComponentSystem::Get().Physics().ProcessPhysics();
	}

	ComponentSystem::Get().Physics().Interpolate();

	ComponentSystem::Get().Physics().UpdateColliders();

	ComponentSystem::Get().PlayerInput().Update();

	ComponentSystem::Get().PlayerInput().ProcessInput();

	ComponentSystem::Get().Button().Update();

	ComponentSystem::Get().Script().CallUpdate();

	ComponentSystem::Get().Animator().Update();

	ComponentSystem::Get().Particle().Update();

	ComponentSystem::Get().Script().CallLateUpdate();

	Object::UpdateDelayDestroy();

	// [Tip] 이번 프레임에 파괴된 오브젝트(GameObject, Component)들 정리
	SceneManager::Get().CleanupDestroyedObjects();

	ResourceManager::Get().Update();
}

void WinApp::Render()
{
	Camera::s_mainCamera->Update();

	ComponentSystem::Get().Renderer().RegisterRendererToRenderQueue();

	m_d2dRenderer.BeginDraw(m_clearColor);

	m_d2dRenderer.ExecuteRenderQueue();

//#ifdef _DEBUG
	ComponentSystem::Get().Physics().RenderColliders();
//#endif // _DEBUG

	m_d2dRenderer.EndDraw();
}

void WinApp::Profiling()
{
	Debug::UpdateFPS(true);

	HANDLE hProcess = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS_EX pmc{};
	pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);

	// 현재 프로세스의 메모리 사용 정보 조회
	GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

	DebugSystem::Get().SetDRAMCurrentUsage(pmc.WorkingSetSize);
	DebugSystem::Get().SetPageFileCurrentUsage(pmc.PagefileUsage);
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
			m_d2dRenderer.Trim();
		}
		break;
	}
}