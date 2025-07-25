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

	// [Tip] Camera�� �����̷��� GameObject�� �����̴� Script Component�� �ۼ��ؼ�
	// GameObject�� �������ָ� ��.

	camera->AddComponent<CameraController>();


	GameObject* sun = CreateGameObject(L"Sun");
	sun->AddComponent<Sun>();


	GameObject* earth = CreateGameObject(L"Earth");
	Earth* earthComp = earth->AddComponent<Earth>();
	Health* earthHp = earth->AddComponent<Health>();

	// [Tip] �ٸ� GameObject�� �ڽ����� ����� ���� ���� Transform::SetParent�� �����.
	// �ڽ��� Transform�� �θ� Transform Component�� ������ ����.
	// �θ� GameObject�� �ı��Ǹ� �ڽ� GameObject�� �ı���.
	// Transform::SetParent�� ���ڷ� nullptr�� ������ �θ𿡼� �����.
	// �̶� �θ� Transform Component�� ���⿡�� ����� ������ ��ġ�� �ٲ� �� ����.
	// Transform::SetParent�� �� ������ World Transform�� �����ϴ� ����� �ʿ��ϴٸ� ���� �ʿ�.

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

	// [Tip] UI ���� �� �ʿ��� Delegate�� ���� �Լ� ȣ��
	// earthComp, moonComp���� �ڽ��� ȸ�� ������ ����Ǿ��� �� ȣ������ �Լ��� ����ϴ� �ڵ�.

	earthComp->GetOnRotationChange().Add(infoViewerComp, &InfoViewer::SetEarthInfo);
	moonComp->GetOnRotationChange().Add(infoViewerComp, &InfoViewer::SetMoonInfo);


	GameObject* earthHpViewer = CreateGameObject(L"EarthHpViewer");
	HpViewer* earthHpViewerComp = earthHpViewer->AddComponent<HpViewer>();
	TextRenderer* earthHpText = earthHpViewer->AddComponent<TextRenderer>();
	earthHpViewerComp->SetTargetName(L"����");
	earthHp->GetOnChangeHp().Add(earthHpViewerComp, &HpViewer::ChangeHpText);
	earthHpViewer->GetTransform()->SetLocalPosition({ -100.0f, 300.0f });


	GameObject* moonHpViewer = CreateGameObject(L"MoonHpViewer");
	HpViewer* moonHpViewerComp = moonHpViewer->AddComponent<HpViewer>();
	TextRenderer* moonHpText = moonHpViewer->AddComponent<TextRenderer>();
	moonHpViewerComp->SetTargetName(L"��");
	moonHp->GetOnChangeHp().Add(moonHpViewerComp, &HpViewer::ChangeHpText);
	moonHpViewer->GetTransform()->SetLocalPosition({ 100.0f, 300.0f });


	// [Tip] Particle Component�� �̿��ؼ� ����Ʈ, ��ƼŬ ����.
	// Particle Component�� BatchRenderer Component�� �ʼ������� �����Ǿ��־����.
	// Particle Component �Ǵ� BatchRenderer Component���� SetBitmap�� SetSpriteSheet�� ȣ���ؼ�
	// BatchRenderer Component�� ����� Bitmap�� SpriteSheet(json ������)�� �ε�����.

	GameObject* particleTest = CreateGameObject(L"ParticleTest");
	particleTest->GetTransform()->SetParent(moon->GetTransform());
	auto batchRenderer = particleTest->AddComponent<BatchRenderer>();
	batchRenderer->SetLocalRect({ -300.0f, -300.0f, 300.0f, 300.0f });
	auto particle = particleTest->AddComponent<Particle>();
	particle->SetBitmap(L"effect_test.png");
	particle->SetSpriteSheet(L"effect_test_sprites.json");

	// [Tip] Particle Component ����
	// Particle::SetLoop�� ��ƼŬ�� duration�� ������ �� ������ ��� ������� ������.
	// Particle::SetDuration�� ��ƼŬ ��ü�� �� ����ð�.
	// 
	// ParticleUnit��
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
	// �� ���� ������ ��� ����.
	// BatchUnit�� BatchUnit Component ������ ����.
	// duration		 - ��ƼŬ �� ������ ��� �ð�.
	// startTime	 - ��ƼŬ �� ������ ��� ���� �ð�.
	// timer		 - ��ƼŬ �� ������ ���� ���� �ð�.
	// original		 - isFollowing�� false�� ��쿡
	//				   ��� ���� �ð��� GameObject�� World Transform ��.
	// isPlaying	 - ���� �� ���ڰ� ��� ������.
	// doInterpolate - �� BatchUnit�� ���� ������ �������� ����.
	//				   �������� ������ t(��ü �ð�)/n(��ü BatchUnit����)��
	//				   �ð� ��ŭ �� BatchUnit ���� �����
	// isFollowing	 - ��ƼŬ ���ڰ� GameObject Transform�� �ǽð����� ��������,
	//				   ��� ���� �ð��� GameObject Transform�� �������.
	// 
	// Particle::AddParticleUnit�� �ۼ��� ParticleUnit�� �߰���.

	particle->SetLoop(true);
	particle->SetDuration(1.0f);

	for (size_t i = 0; i < 100; ++i)
	{
		ParticleUnit particleUnit;

		// �� ParticleUnit�� �� �������� ������.
		
		// ù��°
		particleUnit.batchUnits.push_back(
			{ 
				i % 5,						// i % 5�� �ε����� �̹����� ���.
				{ 0.0f, 0.0f },				// position: 0.0f, 0.0f
				{ 0.3f, 0.3f },				// scale: 0.3f, 0.3f
				0.0f,						// rotation: 0.0f
				{ 1.0f, 1.0f, 1.0f, 1.0f }	// color: 1.0f, 1.0f, 1.0f, 1.0f (���. �������� �״�� ����Ѵٴ� ��)
			}
		);

		// �ι�°
		particleUnit.batchUnits.push_back(
			{
				i % 5,						// i % 5�� �ε����� �̹����� ���.
				Random::Direction() * Random::Float(100.0f, 300.0f), // position: random ������ random(100~300) �Ÿ��� ��ġ
				{ 0.5f, 0.5f },						// scale: 0.5f, 0.5f
				Random::Float(-1060.0f, 1060.0f),	// rotation: random(-1060.0f ~ 1060.0f)�� ����
				{ 1.0f, 1.0f, 1.0f, 0.0f }			// color: 1.0f, 1.0f, 1.0f, 0.0f (���, ���� 0)
			}
		);

		particleUnit.duration = 1.0f;		// ����ð� 1.0f
		particleUnit.startTime = 0.01f * i; // ���۽ð� 0.01f ~ 0.99f
		particleUnit.isFollowing = false;   // GameObject�� ������ ����.

		// ù��°�� �ι�°�� BatchUnit���� 1�ʵ��� ������ ���� ��ġ��
		// SpriteSheet�� i % 5 �ε����� �ش��ϴ� �̹����� �����.

		particle->AddParticleUnit(particleUnit); // ParticleUnit �߰�
	}
}