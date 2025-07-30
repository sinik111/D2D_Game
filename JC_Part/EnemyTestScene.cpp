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
#include "DummyP.h"
#include "FollowCameraController.h"


void EnemyTestScene::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();
	//auto controller = camera->AddComponent<FollowCameraController>();

	GameObject* go = CreateGameObject(L"DummyPlayer");
	go->AddComponent<PlayerInput>();
	go->AddComponent<TextRenderer>();
	go->GetTransform()->SetLocalPosition(0.0f, 0.0f);
	go->AddComponent<DummyP>();
	go->AddComponent<BitmapRenderer>(L"Ramiel.png");
	go->AddComponent<RigidBody2D>();
	go->AddComponent<BoxCollider2D>();


	go = CreateGameObject(L"EnemyBase");
	go->AddComponent<BitmapRenderer>();
	go->AddComponent<Animator>();
	go->AddComponent<PlayerInput>();
	go->AddComponent<TextRenderer>();	
	go->AddComponent<EnemyBase>();
	go->AddComponent<RigidBody2D>();
	go->AddComponent<BoxCollider2D>();
	go->GetComponent<EnemyBase>()->SetTargetPlayerAndCamera(Find(L"DummyPlayer"), camera);

	
	//controller->SetTarget(go->GetTransform());

}

void EnemyTestScene::Exit()
{
	Scene::Exit();

	//MyTime::SetTimeScale(1.0f);
}