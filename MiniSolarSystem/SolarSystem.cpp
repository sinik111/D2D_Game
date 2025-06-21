#include "../D2DEngineLib/framework.h"
#include "SolarSystem.h"

#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/Camera.h"
#include "CameraController.h"
#include "Sun.h"
#include "Earth.h"
#include "Moon.h"
#include "Title.h"
#include "InfoViewer.h"

SolarSystem::~SolarSystem()
{
	Unload();
}

void SolarSystem::Load()
{
	LoadResources();
}

void SolarSystem::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();
	camera->AddComponent<CameraController>();

	GameObject* sun = CreateGameObject(L"Sun");
	GameObject* earth = CreateGameObject(L"Earth");
	GameObject* moon = CreateGameObject(L"Moon");

	moon->GetTransform()->SetParent(earth->GetTransform());
	earth->GetTransform()->SetParent(sun->GetTransform());

	sun->AddComponent<Sun>();
	earth->AddComponent<Earth>();
	moon->AddComponent<Moon>();

	earth->GetTransform()->SetPosition(Vector2(200.0f, 0.0f));

	GameObject* title = CreateGameObject(L"Title");
	title->AddComponent<Title>();

	GameObject* infoViewer = CreateGameObject(L"InfoViewer");
	InfoViewer* comp = infoViewer->AddComponent<InfoViewer>();
	comp->SetSpaceObjects(std::vector<GameObject*>{ sun, earth, moon });
}

void SolarSystem::Unload()
{
	ResourceManager::Get().ReleaseResources(L"SolarSystem");
}

void SolarSystem::LoadResources()
{
	ResourceManager::Get().LoadBitmapFromFile(L"SolarSystem", L"Sun", L"Sun.png");
	ResourceManager::Get().LoadBitmapFromFile(L"SolarSystem", L"Earth", L"Earth.png");
	ResourceManager::Get().LoadBitmapFromFile(L"SolarSystem", L"Moon", L"Moon.png");
}
