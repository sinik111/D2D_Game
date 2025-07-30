#include "../D2DEngineLib/framework.h"
#include "TestScene1.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/CircleCollider.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/LineCollider.h"

#include "PlayerController.h"

void TestScene1::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();

	GameObject* floor2 = CreateGameObject(L"Floor");
	auto lineCollider = floor2->AddComponent<LineCollider>();
	lineCollider->SetLayer(CollisionLayer::Wall);
	lineCollider->SetLine({ -100.0f, -100.0f }, { 100.0f, 100.0f });
	floor2->GetTransform()->SetLocalPosition(0.0f, 100.0f);

	//for (int i = 0; i < 10; ++i)
	//{
	//	GameObject* floor = CreateGameObject(L"Floor");
	//	auto boxCollider = floor->AddComponent<ConeCollider2D>();
	//	boxCollider->SetLayer(CollisionLayer::Wall);
	//	//boxCollider->SetSize({ 15.0f, 15.0f });
	//	floor->GetTransform()->SetLocalPosition({ Random::Float(-400.0f, 400.0f), Random::Float(-300.0f, 300.0f) });
	//	boxCollider->SetCone(100.0f, Random::Direction(), 11.25f);
	//}

	//for (int i = 0; i < 2; ++i)
	//{
	//	GameObject* floor = CreateGameObject(L"Floor");
	//	auto boxCollider = floor->AddComponent<BoxCollider2D>();
	//	boxCollider->SetLayer(CollisionLayer::Wall);
	//	boxCollider->SetSize({ 20.0f, 20.0f });
	//	floor->GetTransform()->SetLocalPosition({ 100.0f, 100.0f });
	//}

	GameObject* player = CreateGameObject(L"Player");
	auto rigidBody = player->AddComponent<RigidBody2D>();
	rigidBody->SetGravityScale(0.0f);
	auto circleCollider = player->AddComponent<CircleCollider>();
	circleCollider->SetLayer(CollisionLayer::PlayerMove);
	circleCollider->SetRadius(10.0f);
	//circleCollider->SetCone(100.0f, Vector2::EllipseRightUp, 11.25f);
	player->AddComponent<PlayerController>();
}
