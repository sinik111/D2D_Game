#include "../D2DEngineLib/framework.h"
#include "SolarSystem.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/Camera.h"
#include "CameraController.h"
#include "Sun.h"
#include "Earth.h"
#include "Moon.h"
#include "Title.h"
#include "InfoViewer.h"
#include "HpViewer.h"
#include "Health.h"

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
	sun->AddComponent<Sun>();


	GameObject* earth = CreateGameObject(L"Earth");
	Earth* earthComp = earth->AddComponent<Earth>();
	earth->GetTransform()->SetParent(sun->GetTransform());
	earth->GetTransform()->SetPosition(Vector2(200.0f, 0.0f));
	Health* earthHp = earth->AddComponent<Health>();


	GameObject* moon = CreateGameObject(L"Moon");
	Moon* moonComp = moon->AddComponent<Moon>();
	moon->GetTransform()->SetParent(earth->GetTransform());
	Health* moonHp = moon->AddComponent<Health>();


	GameObject* title = CreateGameObject(L"Title");
	title->AddComponent<Title>();


	GameObject* infoViewer = CreateGameObject(L"InfoViewer");
	InfoViewer* infoViewerComp = infoViewer->AddComponent<InfoViewer>();
	infoViewerComp->SetSpaceObjects(std::vector<GameObject*>{ sun, earth, moon });
	earthComp->GetOnRotationChange().Add(infoViewerComp, &InfoViewer::SetEarthRotation);
	moonComp->GetOnRotationChange().Add(infoViewerComp, &InfoViewer::SetMoonRotation);


	GameObject* earthHpViewer = CreateGameObject(L"EarthHpViewer");
	HpViewer* earthHpViewerComp = earthHpViewer->AddComponent<HpViewer>();
	earthHpViewerComp->SetTargetName(L"Áö±¸");
	TextRenderer* earthHpText = earthHpViewer->AddComponent<TextRenderer>();
	earthHpText->SetPoint({ -100.0f, 300.0f });

	earthHp->GetOnChangeHp().Add(earthHpViewerComp, &HpViewer::ChangeHpText);


	GameObject* moonHpViewer = CreateGameObject(L"MoonHpViewer");
	HpViewer* moonHpViewerComp = moonHpViewer->AddComponent<HpViewer>();
	moonHpViewerComp->SetTargetName(L"´Þ");
	TextRenderer* moonHpText = moonHpViewer->AddComponent<TextRenderer>();
	moonHpText->SetPoint({ 100.0f, 300.0f });

	moonHp->GetOnChangeHp().Add(moonHpViewerComp, &HpViewer::ChangeHpText);
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
