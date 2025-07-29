#include "../D2DEngineLib/framework.h"
#include "EnemyTestScene.h"

#include <sstream>

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/BoxCollider2D.h"


#include "EnemyBase.h"
#include "FollowCameraController.h"



void EnemyTestScene::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();
	//auto controller = camera->AddComponent<FollowCameraController>();

	GameObject* go = CreateGameObject(L"EnemyBase");
	go->AddComponent<BitmapRenderer>();
	go->AddComponent<Animator>();
	go->AddComponent<PlayerInput>();
	go->AddComponent<TextRenderer>();
	go->AddComponent<EnemyBase>();


	//auto rigidBody = go->AddComponent<RigidBody2D>();
	//BoxCollider2D* boxCollider2d = go->AddComponent<BoxCollider2D>();
	//boxCollider2d->SetSize({ 30.0f, 45.0f });
	//boxCollider2d->SetOffset({ 0.0f, 45.0f });

	//controller->SetTarget(go->GetTransform());

	//go = CreateGameObject(L"EnemyBaseInfo");
	//TextRenderer* textRenderer = go->AddComponent<TextRenderer>();
	//textRenderer->SetSpaceType(SpaceType::Screen);
	//textRenderer->SetFontSize(15.0f);
	//textRenderer->SetPivot({ 0.5f, 1.0f });
	//textRenderer->SetRectSize({ 300.0f, 400.0f });
	//textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	//textRenderer->SetSortOrder(3);
	//textRenderer->SetVerticalAlignment(VerticalAlignment::Top);
	//textRenderer->SetHorizontalAlignment(HorizontalAlignment::Left);

	//go->GetTransform()->SetLocalPosition({ 100.0f, 0.0f });

	//std::wostringstream oss;
	//oss << L"EnemyBase\n"
	//	<< L"Testing\n"
	//	<< L"¤·¤·";

	//textRenderer->SetText(oss.str());

}

void EnemyTestScene::Exit()
{
	Scene::Exit();

	MyTime::SetTimeScale(1.0f);
}