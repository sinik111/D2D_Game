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

	// [Tip] Camera를 움직이려면 GameObject를 움직이는 Script Component를 작성해서
	// GameObject에 부착해주면 됨.

	camera->AddComponent<CameraController>();


	GameObject* sun = CreateGameObject(L"Sun");
	sun->AddComponent<Sun>();


	GameObject* earth = CreateGameObject(L"Earth");
	Earth* earthComp = earth->AddComponent<Earth>();
	Health* earthHp = earth->AddComponent<Health>();

	// [Tip] 다른 GameObject의 자식으로 만들고 싶을 때는 Transform::SetParent를 사용함.
	// 자식의 Transform은 부모 Transform Component의 영향을 받음.
	// 부모 GameObject가 파괴되면 자식 GameObject도 파괴됨.
	// Transform::SetParent의 인자로 nullptr을 넣으면 부모에서 떼어내짐.
	// 이때 부모 Transform Component의 영향에서 벗어나기 때문에 위치가 바뀔 수 있음.
	// Transform::SetParent할 때 본인의 World Transform을 유지하는 기능이 필요하다면 구현 필요.

	earth->GetTransform()->SetParent(sun->GetTransform());
	earth->GetTransform()->SetLocalPosition({ 300.0f, 0.0f });


	GameObject* moon = CreateGameObject(L"Moon");
	Moon* moonComp = moon->AddComponent<Moon>();
	Health* moonHp = moon->AddComponent<Health>();
	moon->GetTransform()->SetParent(earth->GetTransform());


	GameObject* title = CreateGameObject(L"Title");
	title->AddComponent<Title>();


	GameObject* infoViewer = CreateGameObject(L"InfoViewer");
	InfoViewer* infoViewerComp = infoViewer->AddComponent<InfoViewer>();
	infoViewerComp->SetSpaceObjects(sun);

	// [Tip] UI 구현 시 필요한 Delegate를 통한 함수 호출
	// earthComp, moonComp에서 자신의 회전 각도가 변경되었을 시 호출해줄 함수를 등록하는 코드.

	earthComp->GetOnRotationChange().Add(infoViewerComp, &InfoViewer::SetEarthInfo);
	moonComp->GetOnRotationChange().Add(infoViewerComp, &InfoViewer::SetMoonInfo);


	GameObject* earthHpViewer = CreateGameObject(L"EarthHpViewer");
	HpViewer* earthHpViewerComp = earthHpViewer->AddComponent<HpViewer>();
	TextRenderer* earthHpText = earthHpViewer->AddComponent<TextRenderer>();
	earthHpViewerComp->SetTargetName(L"지구");
	earthHp->GetOnChangeHp().Add(earthHpViewerComp, &HpViewer::ChangeHpText);
	earthHpViewer->GetTransform()->SetLocalPosition({ -100.0f, 300.0f });


	GameObject* moonHpViewer = CreateGameObject(L"MoonHpViewer");
	HpViewer* moonHpViewerComp = moonHpViewer->AddComponent<HpViewer>();
	TextRenderer* moonHpText = moonHpViewer->AddComponent<TextRenderer>();
	moonHpViewerComp->SetTargetName(L"달");
	moonHp->GetOnChangeHp().Add(moonHpViewerComp, &HpViewer::ChangeHpText);
	moonHpViewer->GetTransform()->SetLocalPosition({ 100.0f, 300.0f });


	// [Tip] Particle Component를 이용해서 이펙트, 파티클 구현.
	// Particle Component는 BatchRenderer Component가 필수적으로 부착되어있어야함.
	// Particle Component 또는 BatchRenderer Component에서 SetBitmap과 SetSpriteSheet를 호출해서
	// BatchRenderer Component가 사용할 Bitmap과 SpriteSheet(json 데이터)를 로드해줌.

	GameObject* particleTest = CreateGameObject(L"ParticleTest");
	particleTest->GetTransform()->SetParent(moon->GetTransform());
	auto batchRenderer = particleTest->AddComponent<BatchRenderer>();
	batchRenderer->SetLocalRect({ -300.0f, -300.0f, 300.0f, 300.0f });
	auto particle = particleTest->AddComponent<Particle>();
	particle->SetBitmap(L"effect_test.png");
	particle->SetSpriteSheet(L"effect_test_sprites.json");

	// [Tip] Particle Component 사용법
	// Particle::SetLoop는 파티클의 duration이 지났을 때 멈출지 계속 재생될지 설정함.
	// Particle::SetDuration은 파티클 전체의 총 재생시간.
	// 
	// ParticleUnit은
	// 
	// std::vector<BatchUnit> batchUnits;
	// float duration = 0.0f;
	// float startTime = 0.0f;
	// float timer = 0.0f;
	// Matrix3x2 original;
	// bool isPlaying = false;
	// bool doInterpolate = true;
	// bool isFollowing = true;
	// 
	// 와 같은 정보를 담고 있음.
	// BatchUnit은 BatchUnit Component 사용법을 참고.
	// duration		 - 파티클 한 입자의 재생 시간.
	// startTime	 - 파티클 한 입자의 재생 시작 시간.
	// timer		 - 파티클 한 입자의 현재 진행 시간.
	// original		 - isFollowing이 false일 경우에
	//				   재생 시작 시간에 GameObject의 World Transform 값.
	// isPlaying	 - 현재 이 입자가 재생 중인지.
	// doInterpolate - 각 BatchUnit들 간에 값들을 보간할지 말지.
	//				   보간하지 않으면 t(전체 시간)/n(전체 BatchUnit개수)의
	//				   시간 만큼 각 BatchUnit 값이 적용됨
	// isFollowing	 - 파티클 입자가 GameObject Transform을 실시간으로 적용할지,
	//				   재생 시작 시간의 GameObject Transform을 사용할지.
	// 
	// Particle::AddParticleUnit은 작성된 ParticleUnit을 추가함.

	particle->SetLoop(true);
	particle->SetDuration(1.0f);

	for (size_t i = 0; i < 100; ++i)
	{
		ParticleUnit particleUnit;

		// 이 ParticleUnit은 두 구간으로 구성됨.
		
		// 첫번째
		particleUnit.batchUnits.push_back(
			{ 
				i % 5,						// i % 5의 인덱스의 이미지를 사용.
				{ 0.0f, 0.0f },				// position: 0.0f, 0.0f
				{ 0.3f, 0.3f },				// scale: 0.3f, 0.3f
				0.0f,						// rotation: 0.0f
				{ 1.0f, 1.0f, 1.0f, 1.0f }	// color: 1.0f, 1.0f, 1.0f, 1.0f (흰색. 기존색을 그대로 사용한다는 뜻)
			}
		);

		// 두번째
		particleUnit.batchUnits.push_back(
			{
				i % 5,						// i % 5의 인덱스의 이미지를 사용.
				Random::Direction() * Random::Float(100.0f, 300.0f), // position: random 방향의 random(100~300) 거리의 위치
				{ 0.5f, 0.5f },						// scale: 0.5f, 0.5f
				Random::Float(-1060.0f, 1060.0f),	// rotation: random(-1060.0f ~ 1060.0f)의 각도
				{ 1.0f, 1.0f, 1.0f, 0.0f }			// color: 1.0f, 1.0f, 1.0f, 0.0f (흰색, 알파 0)
			}
		);

		particleUnit.duration = 1.0f;		// 재생시간 1.0f
		particleUnit.startTime = 0.01f * i; // 시작시간 0.01f ~ 0.99f
		particleUnit.isFollowing = false;   // GameObject를 따라가지 않음.

		// 첫번째와 두번째의 BatchUnit들을 1초동안 보간한 값의 위치에
		// SpriteSheet의 i % 5 인덱스에 해당하는 이미지를 출력함.

		particle->AddParticleUnit(particleUnit); // ParticleUnit 추가
	}
}