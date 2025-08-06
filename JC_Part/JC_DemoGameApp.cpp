#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/SoundManager.h"

#include "JC_DemoGameApp.h"
#include "EnemyTestScene.h"


void JC_DemoGameApp::Initialize() 
{
	// [Tip] WinApp::Initialize()�� ȣ���ϱ� ���� Window ������ �ϸ� �ݿ���.
	// default���� ����Ǿ� �־ �ƹ��͵� �������� �ʾƵ� ������ ����.

	//m_windowStyle = WS_MAXIMIZE | WS_POPUP; // ��üȭ��
	m_windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	m_className = L"JC_D2DGame";
	m_windowName = L"JC_DemoGameApp";

	//m_width = 960;
	//m_height = 540;

	m_width = 1920;
	m_height = 1080;

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
	// .exe ���� ���� ������ Ÿ�� �����鼭 L"Bin"�� ������ ����.
	// �׸��� �ű⼭ L"MiniSolarSystem/Resource" ������ Ÿ�� ��.

	ResourceManager::Get().Initialize(m_d2dRenderer.GetDeviceContext(),
		m_modulePath, L"Bin", L"JC_Part/Resource");

	SoundManager::Get().Initialize(ResourceManager::Get().GetResourcePathA());

	// [Tip] Scene �����ϱ�. ������ name�� SceneChange�� �� ���̴� key
	SceneManager::Get().CreateScene<EnemyTestScene>(L"Test");
	//SceneManager::Get().CreateScene<SolarSystem>(L"SolarSystem");
	//SceneManager::Get().CreateScene<AnimationTestScene>(L"AnimationTest");
	//SceneManager::Get().CreateScene<KinematicTest>(L"KinematicTest");


	// [Tip] Scene �����ϱ�
	SceneManager::Get().ChangeScene(L"Test");
}

void JC_DemoGameApp::MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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