#include "../D2DEngineLib/framework.h"
#include "SolarSystem.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/Particle.h"
#include "../D2DEngineLib/BatchRenderer.h"

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
	earthHp->GetOnChangeHp().Add(earthHpViewerComp, &HpViewer::ChangeHpText);
	earthHpViewer->GetTransform()->SetLocalPosition({ -100.0f, 300.0f });


	GameObject* moonHpViewer = CreateGameObject(L"MoonHpViewer");
	HpViewer* moonHpViewerComp = moonHpViewer->AddComponent<HpViewer>();
	TextRenderer* moonHpText = moonHpViewer->AddComponent<TextRenderer>();
	moonHpViewerComp->SetTargetName(L"´Þ");
	moonHp->GetOnChangeHp().Add(moonHpViewerComp, &HpViewer::ChangeHpText);
	moonHpViewer->GetTransform()->SetLocalPosition({ 100.0f, 300.0f });


	GameObject* particleTest = CreateGameObject(L"ParticleTest");
	particleTest->GetTransform()->SetParent(moon->GetTransform());
	//particleTest->GetTransform()->SetLocalPosition(-300.0f, 200.0f);
	particleTest->AddComponent<BatchRenderer>();
	auto particle = particleTest->AddComponent<Particle>();
	particle->SetBitmap(L"effect_test.png");
	particle->SetSpriteSheet(L"effect_test_sprites.json");
	particle->SetLoop(true);
	particle->SetDuration(1.0f);

	for (size_t i = 0; i < 100; ++i)
	{
		ParticleUnit particleUnit;

		particleUnit.batchUnits.push_back(
			{ 
				i % 5,
				{ 0.0f, 0.0f },
				{ 0.3f, 0.3f },
				0.0f,
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);
		particleUnit.batchUnits.push_back(
			{
				i % 5,
				Random::Direction() * Random::Float(100.0f, 300.0f),
				{ 0.5f, 0.5f },
				Random::Float(-1060.0f, 1060.0f),
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			}
		);
		particleUnit.duration = 1.0f;
		particleUnit.startTime = 0.01f * i;
		particleUnit.isFollowing = false;

		particle->AddParticleUnit(particleUnit);
	}
}