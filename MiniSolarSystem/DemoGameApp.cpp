#include "../D2DEngineLib/framework.h"
#include "DemoGameApp.h"

#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/SceneManager.h"

#include "SolarSystem.h"
#include "MainTitle.h"
#include "AnimationTestScene.h"
#include "KinematicTest.h"

void DemoGameApp::Initialize()
{
	// [Tip] WinApp::Initialize()�� ȣ���ϱ� ���� Window ������ �ϸ� �ݿ���.
	// default���� ����Ǿ� �־ �ƹ��͵� �������� �ʾƵ� ������ ����.

	//m_windowStyle = WS_MAXIMIZE | WS_POPUP; // ��üȭ��
	m_windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	m_className = L"D2DGame";
	m_windowName = L"MiniSolarSystem";

	//m_width = 960;
	//m_height = 540;

	m_width = 1280;
	m_height = 720;

	WinApp::Initialize();

	// [Tip] ResourceManager�� �ʱ�ȭ �ϸ鼭 ���� ���� ���� ��ġ�� ���ҽ� ���� ��ġ�� �Է��ؾ���.
	// binaryFolderName�� resourceFolderName�� ã�� �� �ִ� ��ġ���� ��.
	//
	// (����)
	// Solution
	// |---x64 <--- Debug <--- .exe
	// | (������ġ)
	// |---> MiniSolarSystem ---> Resource
	// 
	// .exe ���� ���� ������ Ÿ�� �����鼭 L"x64"�� ������ ����.
	// �׸��� �ű⼭ L"MiniSolarSystem/Resource" ������ Ÿ�� ��.
	
	ResourceManager::Get().Initialize(m_d2dRenderer.GetDeviceContext(),
		m_modulePath, L"x64", L"MiniSolarSystem/Resource");

	// [Tip] Scene �����ϱ�. ������ name�� SceneChange�� �� ���̴� key
	SceneManager::Get().CreateScene<MainTitle>(L"Title");
	SceneManager::Get().CreateScene<SolarSystem>(L"SolarSystem");
	SceneManager::Get().CreateScene<AnimationTestScene>(L"AnimationTest");
	SceneManager::Get().CreateScene<KinematicTest>(L"KinematicTest");

	// [Tip] Scene �����ϱ�
	SceneManager::Get().ChangeScene(L"Title");
}

void DemoGameApp::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WinApp::MessageProc(hWnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_CREATE:
		break;

	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		break;
	}
}