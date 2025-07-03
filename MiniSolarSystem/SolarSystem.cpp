#include "../D2DEngineLib/framework.h"
#include "SolarSystem.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/MyTime.h"
#include "CameraController.h"
#include "Sun.h"
#include "Earth.h"
#include "Moon.h"
#include "Title.h"
#include "InfoViewer.h"
#include "HpViewer.h"
#include "Health.h"

void SolarSystem::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();
	camera->AddComponent<CameraController>();


	GameObject* sun = CreateGameObject(L"Sun");
	sun->AddComponent<Sun>();


	GameObject* earth = CreateGameObject(L"Earth");
	Earth* earthComp = earth->AddComponent<Earth>();
	Health* earthHp = earth->AddComponent<Health>();
	earth->GetTransform()->SetParent(sun->GetTransform());
	earth->GetTransform()->SetLocalPosition({ 200.0f, 0.0f });


	GameObject* moon = CreateGameObject(L"Moon");
	Moon* moonComp = moon->AddComponent<Moon>();
	Health* moonHp = moon->AddComponent<Health>();
	moon->GetTransform()->SetParent(earth->GetTransform());


	GameObject* title = CreateGameObject(L"Title");
	title->AddComponent<Title>();


	GameObject* infoViewer = CreateGameObject(L"InfoViewer");
	InfoViewer* infoViewerComp = infoViewer->AddComponent<InfoViewer>();
	infoViewerComp->SetSpaceObjects(sun);
	earthComp->GetOnRotationChange().Add(infoViewerComp, &InfoViewer::SetEarthInfo);
	moonComp->GetOnRotationChange().Add(infoViewerComp, &InfoViewer::SetMoonInfo);


	GameObject* earthHpViewer = CreateGameObject(L"EarthHpViewer");
	HpViewer* earthHpViewerComp = earthHpViewer->AddComponent<HpViewer>();
	TextRenderer* earthHpText = earthHpViewer->AddComponent<TextRenderer>();
	earthHpViewerComp->SetTargetName(L"Áö±¸");
	earthHpText->SetPoint({ -100.0f, 300.0f });
	earthHp->GetOnChangeHp().Add(earthHpViewerComp, &HpViewer::ChangeHpText);


	GameObject* moonHpViewer = CreateGameObject(L"MoonHpViewer");
	HpViewer* moonHpViewerComp = moonHpViewer->AddComponent<HpViewer>();
	TextRenderer* moonHpText = moonHpViewer->AddComponent<TextRenderer>();
	moonHpViewerComp->SetTargetName(L"´Þ");
	moonHpText->SetPoint({ 100.0f, 300.0f });
	moonHp->GetOnChangeHp().Add(moonHpViewerComp, &HpViewer::ChangeHpText);

	//MyTime::SetTimeScale(2.0f);
}