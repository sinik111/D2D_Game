#include "../D2DEngineLib/framework.h"
#include "MainTitle.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Button.h"

#include "TitleSceneChange.h"
#include "MainSceneChangeButton.h"

void MainTitle::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();
	
	GameObject* go = CreateGameObject(L"SceneChange");
	go->AddComponent<TitleSceneChange>();

	GameObject* buttonGo = CreateGameObject(L"Button");
	buttonGo->AddComponent<BitmapRenderer>(L"ChangeSceneButton.png");
	buttonGo->AddComponent<Button>();
	buttonGo->AddComponent<MainSceneChangeButton>();

	Debug::Log("main load done");
}