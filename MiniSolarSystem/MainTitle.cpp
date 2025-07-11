#include "../D2DEngineLib/framework.h"
#include "MainTitle.h"

#include "../D2DEngineLib/Camera.h"

#include "TitleSceneChange.h"

void MainTitle::Enter()
{
	GameObject* camera = CreateGameObjectWithComponents(L"Camera",
		camera->AddComponent<Camera>();
	);
	
	GameObject* go = CreateGameObjectWithComponents(L"SceneChange",
		go->AddComponent<TitleSceneChange>();
	);
}