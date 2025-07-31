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

#include "Player.h"

void Scene1::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();

	GameObject* map = CreateGameObject(L"Map");
	auto mapBitmapRenderer = map->AddComponent<BitmapRenderer>(L"TempMap.png");
	mapBitmapRenderer->SetSortOrder(0);

	GameObject* player = CreateGameObject(L"Player");
	player->AddComponent<BitmapRenderer>(L"TempPlayer.bmp");
	player->AddComponent<TextRenderer>();
	player->AddComponent<RigidBody2D>();
	player->AddComponent<PlayerInput>();
	auto playerComp = player->AddComponent<Player>();
	auto cc = player->AddComponent<CircleCollider>();
	cc->SetOffset({ 0.0f, -50.0f });
	cc->SetRadius(40.0f);
	cc->SetLayer(CollisionLayer::PlayerMove);

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
}
