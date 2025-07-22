#include "../D2DEngineLib/framework.h"
#include "KinematicTest.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "KinematicSun.h"
#include "StaticMoon.h"

void KinematicTest::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();

	GameObject* go = CreateGameObject(L"KinematicSun");
	go->AddComponent<KinematicSun>();
	auto rigidbody = go->AddComponent<RigidBody2D>();
	rigidbody->SetBodyType(RigidBody2D::BodyType::Kinematic);
	BoxCollider2D* sunCollider = go->AddComponent<BoxCollider2D>();
	sunCollider->SetSize({ 50.0f, 50.f });
	go->AddComponent<BitmapRenderer>(L"Sun.png");


	for (size_t i = 0; i < 50; ++i)
	{
		go = CreateGameObject(L"StaticMoon");
		go->AddComponent<StaticMoon>();
		BoxCollider2D* moonCollider = go->AddComponent<BoxCollider2D>();
		moonCollider->SetTrigger(true);
		moonCollider->SetSize({ 20.0f, 20.f });
		go->AddComponent<BitmapRenderer>(L"Moon.png");
		go->GetTransform()->SetLocalPosition(Random::Float(-500, 500), Random::Float(-400, 400));
	}

	go = CreateGameObject();
	auto textRenderer = go->AddComponent<TextRenderer>();
	textRenderer->SetText(L"Press 1 to go main");
	textRenderer->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	textRenderer->SetFontSize(20.0f);

	Debug::Log("kine load done");
}