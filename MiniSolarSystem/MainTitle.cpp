#include "../D2DEngineLib/framework.h"
#include "MainTitle.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Button.h"

#include "TitleSceneChange.h"
#include "MainSceneChangeButton.h"

void MainTitle::Enter()
{
	// [Tip] Scene 생성시 Camera를 무조건 하나는 생성해줘야함.
	// Camera Component를 부착한 GameObject가 Camera의 역할을 하게됨.
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();
	

	// [Tip] GameObject 생성시에 넣는 인자가 GameObject의 이름이 됨.
	// 아직은 tag와 같은 다른 구분 요소가 없기 때문에 이름으로 구분을 하면 됨.
	// 필요시 tag 추가 구현 필요.
	// 
	// GameObject::Find 함수를 사용하면 이름으로 GameObject를 찾을 수 있음.
	// 같은 이름이 여러개면 순회 시 가장 처음 발견한 GameObject 반환.
	
	GameObject* go = CreateGameObject(L"SceneChange");
	go->AddComponent<TitleSceneChange>();


	// [Tip] Button을 만드려면 GameObject에 Button Component를 부착하면 됨.
	// Button Component는 BitmapRenderer가 필수적으로 필요함.
	// Button의 크기는 BitmapRenderer가 그리는 이미지의 크기와 같음.
	// 현재는 클릭 시 색상이 { 0.97f, 0.97f, 0.97f, 1.0f }(색을 어둡게 함)로 고정인데
	// 필요시 색 지정 가능하게 구현 필요.

	GameObject* buttonGo = CreateGameObject(L"Button");
	buttonGo->AddComponent<BitmapRenderer>(L"ChangeSceneButton.png");
	buttonGo->AddComponent<Button>();
	buttonGo->AddComponent<MainSceneChangeButton>();
}