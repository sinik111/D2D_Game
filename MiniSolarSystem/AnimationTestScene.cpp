#include "../D2DEngineLib/framework.h"
#include "AnimationTestScene.h"

#include <sstream>

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/BoxCollider2D.h"

#include "Ken.h"
#include "HpViewer.h"
#include "Health.h"
#include "FollowCameraController.h"
#include "Sun.h"

void AnimationTestScene::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();
	auto controller = camera->AddComponent<FollowCameraController>();

	GameObject* go = CreateGameObject(L"Ken");
	go->AddComponent<BitmapRenderer>();
	go->AddComponent<Animator>();
	go->AddComponent<PlayerInput>();
	go->AddComponent<Ken>();
	Health* health = go->AddComponent<Health>();
	auto rigidBody = go->AddComponent<RigidBody2D>();
	BoxCollider2D* boxCollider2d = go->AddComponent<BoxCollider2D>();
	boxCollider2d->SetSize({ 30.0f, 45.0f });
	boxCollider2d->SetOffset({ 0.0f, 45.0f });

	controller->SetTarget(go->GetTransform());

	//go = CreateGameObject(L"Ken2");
	//go->AddComponent<BitmapRenderer>();
	//go->AddComponent<Animator>();
	//go->AddComponent<PlayerInput>();
	//go->AddComponent<Ken>();
	//go->AddComponent<RigidBody2D>();
	//go->AddComponent<Health>();
	//boxCollider2d = go->AddComponent<BoxCollider2D>();
	//boxCollider2d->SetSize({ 30.0f, 45.0f });
	//boxCollider2d->SetOffset({ 0.0f, 45.0f });
	//go->GetTransform()->SetLocalPosition(0.0f, 100.0f);


	GameObject* floor = CreateGameObject(L"Floor");
	boxCollider2d = floor->AddComponent<BoxCollider2D>();
	boxCollider2d->SetSize({ 800.0f, 20.0f });
	floor->GetTransform()->SetLocalPosition({ 0.0f, -50.0f });

	GameObject* wall = CreateGameObject(L"Wall");
	boxCollider2d = wall->AddComponent<BoxCollider2D>();
	boxCollider2d->SetSize({ 20.0f, 300.0f });
	wall->GetTransform()->SetLocalPosition({ -400.0f, 100.0f });

	wall = CreateGameObject(L"Wall");
	boxCollider2d = wall->AddComponent<BoxCollider2D>();
	boxCollider2d->SetSize({ 20.0f, 300.0f });
	wall->GetTransform()->SetLocalPosition({ 400.0f, 100.0f });

	floor = CreateGameObject(L"Floor");
	boxCollider2d = floor->AddComponent<BoxCollider2D>();
	boxCollider2d->SetSize({ 40.0f, 10.0f });
	floor->GetTransform()->SetLocalPosition({ 50.0f, 30.0f });

	floor = CreateGameObject(L"Floor");
	boxCollider2d = floor->AddComponent<BoxCollider2D>();
	boxCollider2d->SetSize({ 40.0f, 10.0f });
	floor->GetTransform()->SetLocalPosition({ 200.0f, 90.0f });

	floor = CreateGameObject(L"Floor");
	boxCollider2d = floor->AddComponent<BoxCollider2D>();
	boxCollider2d->SetSize({ 40.0f, 10.0f });
	floor->GetTransform()->SetLocalPosition({ 350.0f, 150.0f });

	go = CreateGameObject(L"KenKeyInfo");
	TextRenderer* textRenderer = go->AddComponent<TextRenderer>();
	textRenderer->SetSpaceType(SpaceType::Screen);
	textRenderer->SetFontSize(15.0f);
	textRenderer->SetPivot({ 0.5f, 1.0f });
	textRenderer->SetRectSize({ 300.0f, 400.0f });
	textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	textRenderer->SetSortOrder(3);
	textRenderer->SetVerticalAlignment(VerticalAlignment::Top);
	textRenderer->SetHorizontalAlignment(HorizontalAlignment::Left);

	go->GetTransform()->SetLocalPosition({ 100.0f, 0.0f });

	std::wostringstream oss;
	oss << L"[ 4 ]: KinematicTestScene\n\n[ ¡ç ¡æ ]: Back/FrontDash\n[ Q ]: Jump\n"
		<< L"[ W ]: SpinningKick - FireEarth\n[ E ]: Take 10 damage\n"
		<< L"[ R ]: Revive";

	textRenderer->SetText(oss.str());
	
	go = CreateGameObject(L"KenHpViewer");
	HpViewer* hpViewer = go->AddComponent<HpViewer>();
	hpViewer->SetTargetName(L"Ken");
	go->GetTransform()->SetLocalPosition({ 0.0f, 300.0f });

	textRenderer = go->AddComponent<TextRenderer>();

	health->GetOnChangeHp().Add(hpViewer, &HpViewer::ChangeHpText);

	go = CreateGameObject(L"Sun");
	go->AddComponent<Sun>();
	go->GetTransform()->SetLocalScale(0.5f, 0.5f);
	go->GetTransform()->SetLocalPosition(-200.0f, 50.0f);
	auto collider = go->AddComponent<BoxCollider2D>();
	auto rigidBody2 = go->AddComponent<RigidBody2D>();
	rigidBody2->SetMass(2.0f);
	//collider->SetTrigger(true);
	collider->SetSize({ 25.0f, 25.f });
}

void AnimationTestScene::Exit()
{
	Scene::Exit();

	MyTime::SetTimeScale(1.0f);
}