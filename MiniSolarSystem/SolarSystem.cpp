#include "../D2DEngineLib/framework.h"
#include "SolarSystem.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Camera.h"

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
	GameObject* camera = CreateGameObjectWithComponents(L"Camera",
		camera->AddComponent<Camera>();
		camera->AddComponent<CameraController>();
		);

	GameObject* sun = CreateGameObjectWithComponents(L"Sun",
		sun->AddComponent<Sun>();
		);


	Earth* earthComp;
	Health* earthHp;

	GameObject* earth = CreateGameObjectWithComponents(L"Earth",
		earthComp = earth->AddComponent<Earth>();
		earthHp = earth->AddComponent<Health>();
		earth->GetTransform()->SetParent(sun->GetTransform());
		earth->GetTransform()->SetLocalPosition({ 200.0f, 0.0f });
		);


	Moon* moonComp;
	Health* moonHp;

	GameObject* moon = CreateGameObjectWithComponents(L"Moon",
		moonComp = moon->AddComponent<Moon>();
		moonHp = moon->AddComponent<Health>();
		moon->GetTransform()->SetParent(earth->GetTransform());
		);


	GameObject* title = CreateGameObjectWithComponents(L"Title",
		title->AddComponent<Title>();
		);


	GameObject* infoViewer = CreateGameObjectWithComponents(L"InfoViewer",
		InfoViewer* infoViewerComp = infoViewer->AddComponent<InfoViewer>();
		infoViewerComp->SetSpaceObjects(sun);
		earthComp->GetOnRotationChange().Add(infoViewerComp, &InfoViewer::SetEarthInfo);
		moonComp->GetOnRotationChange().Add(infoViewerComp, &InfoViewer::SetMoonInfo);
		);


	GameObject* earthHpViewer = CreateGameObjectWithComponents(L"EarthHpViewer",
		HpViewer* earthHpViewerComp = earthHpViewer->AddComponent<HpViewer>();
		TextRenderer* earthHpText = earthHpViewer->AddComponent<TextRenderer>();
		earthHpViewerComp->SetTargetName(L"Áö±¸");
		earthHp->GetOnChangeHp().Add(earthHpViewerComp, &HpViewer::ChangeHpText);
		earthHpViewer->GetTransform()->SetLocalPosition({ -100.0f, 300.0f });
		);


	GameObject* moonHpViewer = CreateGameObjectWithComponents(L"MoonHpViewer",
		HpViewer* moonHpViewerComp = moonHpViewer->AddComponent<HpViewer>();
		TextRenderer* moonHpText = moonHpViewer->AddComponent<TextRenderer>();
		moonHpViewerComp->SetTargetName(L"´Þ");
		moonHp->GetOnChangeHp().Add(moonHpViewerComp, &HpViewer::ChangeHpText);
		moonHpViewer->GetTransform()->SetLocalPosition({ 100.0f, 300.0f });
		);
}