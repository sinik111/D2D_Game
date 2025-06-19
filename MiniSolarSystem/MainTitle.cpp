#include "../D2DEngineLib/framework.h"
#include "MainTitle.h"

#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/Camera.h"
#include "TitleSceneChange.h"

MainTitle::~MainTitle()
{
	Unload();
}

void MainTitle::Load()
{
	ResourceManager::Get().LoadBitmapFromFile(L"Title", L"MainTitle", L"MainTitle.png");
}

void MainTitle::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();

	GameObject* go = CreateGameObject(L"SceneChange");
	go->AddComponent<TitleSceneChange>();
}

void MainTitle::Exit()
{
	Scene::Exit();
}

void MainTitle::Unload()
{
	ResourceManager::Get().ReleaseResources(L"Title");
}
