#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/SoundManager.h"

#include "JC_DemoGameApp.h"
#include "EnemyTestScene.h"


void JC_DemoGameApp::Initialize() 
{
	// [Tip] WinApp::Initialize()을 호출하기 전에 Window 설정을 하면 반영됨.
	// default값이 적용되어 있어서 아무것도 설정하지 않아도 문제는 없음.

	//m_windowStyle = WS_MAXIMIZE | WS_POPUP; // 전체화면
	m_windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	m_className = L"JC_D2DGame";
	m_windowName = L"JC_DemoGameApp";

	//m_width = 960;
	//m_height = 540;

	m_width = 1920;
	m_height = 1080;

	WinApp::Initialize();

	// [Tip] ResourceManager를 초기화 하면서 실행 파일 폴더 위치와 리소스 폴더 위치를 입력해야함.
	// binaryFolderName은 resourceFolderName을 찾을 수 있는 위치여야 함.
	//
	// (예시)
	// Solution
	// |---x64 <--- Debug <--- .exe
	// | (동일위치)
	// |---> MiniSolarSystem ---> Resource
	// 
	// .exe 에서 상위 폴더를 타고 나오면서 L"Bin"를 만나면 멈춤.
	// 그리고 거기서 L"MiniSolarSystem/Resource" 안으로 타고 들어감.

	ResourceManager::Get().Initialize(m_d2dRenderer.GetDeviceContext(),
		m_modulePath, L"Bin", L"JC_Part/Resource");

	SoundManager::Get().Initialize(ResourceManager::Get().GetResourcePathA());

	// [Tip] Scene 생성하기. 인자의 name은 SceneChange할 때 쓰이는 key
	SceneManager::Get().CreateScene<EnemyTestScene>(L"Test");
	//SceneManager::Get().CreateScene<SolarSystem>(L"SolarSystem");
	//SceneManager::Get().CreateScene<AnimationTestScene>(L"AnimationTest");
	//SceneManager::Get().CreateScene<KinematicTest>(L"KinematicTest");


	// [Tip] Scene 변경하기
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