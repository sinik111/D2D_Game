#include "../D2DEngineLib/framework.h"
#include "TestScene1.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "PlayerController.h"

void TestScene1::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();

	for (int i = 0; i < 30; ++i)
	{
		GameObject* floor = CreateGameObject(L"Floor");
		auto boxCollider = floor->AddComponent<BoxCollider2D>();
		boxCollider->SetLayer(CollisionLayer::Wall);
		boxCollider->SetSize({ 20.0f, 20.0f });
		floor->GetTransform()->SetLocalPosition({ Random::Float(-400.0f, 400.0f), Random::Float(-300.0f, 300.0f) });
	}

	GameObject* player = CreateGameObject(L"Player");
	auto rigidBody = player->AddComponent<RigidBody2D>();
	rigidBody->SetGravityScale(0.0f);
	auto boxCollider = player->AddComponent<BoxCollider2D>();
	boxCollider->SetLayer(CollisionLayer::PlayerMove);
	boxCollider->SetSize({ 30.0f, 30.0f });
	player->AddComponent<PlayerController>();
}
