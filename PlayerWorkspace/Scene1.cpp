#include "../D2DEngineLib/framework.h"
#include "Scene1.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Screen.h"
#include "../D2DEngineLib/CircleCollider.h"
#include "../D2DEngineLib/LineCollider.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/Animator.h"

#include "Player.h"
#include "DummyEnemy.h"

void Scene1::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();

	GameObject* map = CreateGameObject(L"Map");
	auto mapBitmapRenderer = map->AddComponent<BitmapRenderer>(L"map01_ver1.jpg");
	mapBitmapRenderer->SetSortOrder(0);

	GameObject* player = CreateGameObject(L"Player");
	auto playerBr = player->AddComponent<BitmapRenderer>();
	playerBr->SetSortOrder(1);
	auto playerAnimator = player->AddComponent<Animator>();

	playerAnimator->AddSpriteSheet(L"runs_E_001_sprites.json");
	playerAnimator->AddSpriteSheet(L"runs_N_001_sprites.json");
	playerAnimator->AddSpriteSheet(L"runs_NE_001_sprites.json");
	playerAnimator->AddSpriteSheet(L"runs_NW_001_sprites.json");
	playerAnimator->AddSpriteSheet(L"runs_S_001_sprites.json");
	playerAnimator->AddSpriteSheet(L"runs_SE_001_sprites.json");
	playerAnimator->AddSpriteSheet(L"runs_SW_001_sprites.json");
	playerAnimator->AddSpriteSheet(L"runs_W_001_sprites.json");

	playerAnimator->AddAnimationClip(L"runs_E_001_anim.json");
	playerAnimator->AddAnimationClip(L"runs_N_001_anim.json");
	playerAnimator->AddAnimationClip(L"runs_NE_001_anim.json");
	playerAnimator->AddAnimationClip(L"runs_NW_001_anim.json");
	playerAnimator->AddAnimationClip(L"runs_S_001_anim.json");
	playerAnimator->AddAnimationClip(L"runs_SE_001_anim.json");
	playerAnimator->AddAnimationClip(L"runs_SW_001_anim.json");
	playerAnimator->AddAnimationClip(L"runs_W_001_anim.json");

	player->AddComponent<TextRenderer>();
	player->AddComponent<RigidBody2D>();
	player->AddComponent<PlayerInput>();
	auto playerComp = player->AddComponent<Player>();
	auto cc = player->AddComponent<CircleCollider>();
	cc->SetOffset({ 0.0f, -50.0f });
	cc->SetRadius(40.0f);
	cc->SetLayer(CollisionLayer::PlayerMove);
	auto bc = player->AddComponent<BoxCollider2D>();
	bc->SetLayer(CollisionLayer::PlayerHitBox);
	bc->SetSize({ 35.0f, 75.0f });
	bc->SetTrigger(true);

	// temp map colider

	const float radius = 450.0f;
	const float segment = MyMath::PI * 2.0f / 8.0f;

	for (int i = 0; i < 8; i++)
	{
		float startRadian = (i + 1) * segment;
		float endRadian = i * segment;

		Vector2 startPoint{ radius * std::cos(startRadian), radius * std::sin(startRadian) };
		Vector2 endPoint{ radius * std::cos(endRadian), radius * std::sin(endRadian) };
		Vector2 center{ Vector2(endPoint + startPoint) * 0.5f };

		startPoint = startPoint - center;
		endPoint = endPoint - center;

		GameObject* wall = CreateGameObject(L"Wall");
		wall->GetTransform()->SetLocalPosition(center);

		auto lineCollider = wall->AddComponent<LineCollider>();
		lineCollider->SetLine(startPoint, endPoint);
		lineCollider->SetLayer(CollisionLayer::Wall);
	}

	GameObject* playerInfo = CreateGameObject(L"PlayerInfo");
	playerInfo->GetTransform()->SetLocalPosition(-Screen::Get().GetWidth() * 0.5f + 20.0f, Screen::Get().GetHeight() * 0.5f - 20.0f);
	auto playerInfoTextRenderer = playerInfo->AddComponent<TextRenderer>();

	playerComp->SetPlayerInfoTextRenderer(playerInfoTextRenderer);


	GameObject* dummyEnemy = CreateGameObject(L"DummyEnemy");
	dummyEnemy->GetTransform()->SetLocalPosition(250.0f, 250.0f);

	auto enemyBr = dummyEnemy->AddComponent<BitmapRenderer>(L"TempEnemy.bmp");
	enemyBr->SetSortOrder(1);
	dummyEnemy->AddComponent<DummyEnemy>();
	auto dummyRb = dummyEnemy->AddComponent<RigidBody2D>();
	dummyRb->SetGravityScale(0.0f);
	dummyRb->SetMass(100000.0f);

	auto dummyCircleCol = dummyEnemy->AddComponent<CircleCollider>();
	dummyCircleCol->SetOffset({ 0.0f, -50.0f });
	dummyCircleCol->SetRadius(40.0f);
	dummyCircleCol->SetLayer(CollisionLayer::EnemyMove);
	
	auto dummyBoxCol = dummyEnemy->AddComponent<BoxCollider2D>();
	dummyBoxCol->SetSize({ 35.0f, 75.0f });
	dummyBoxCol->SetTrigger(true);
	dummyBoxCol->SetLayer(CollisionLayer::EnemyHitBox);
}
