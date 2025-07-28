#include "../D2DEngineLib/framework.h"
#include "PlayerMakingTestScene.h"

#include <sstream>

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Screen.h"

#include "Player.h"

void PlayerMakingTestScene::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();

	GameObject* map = CreateGameObject(L"Map");
	auto mapBitmapRenderer = map->AddComponent<BitmapRenderer>(L"TempMap.png");
	mapBitmapRenderer->SetSortOrder(0);
	mapBitmapRenderer->SetSpaceType(SpaceType::World);

	GameObject* player = CreateGameObject(L"Player");
	player->AddComponent<BitmapRenderer>(L"TempPlayer.bmp");
	player->AddComponent<TextRenderer>();
	player->AddComponent<RigidBody2D>();
	player->AddComponent<PlayerInput>();
	player->AddComponent<Player>();


	GameObject* keyInfo = CreateGameObject(L"KeyInfo");
	keyInfo->GetTransform()->SetLocalPosition(-Screen::Get().GetWidth() * 0.5f, Screen::Get().GetHeight() * 0.5f);
	auto keyInfoTextRenderer = keyInfo->AddComponent<TextRenderer>();
	keyInfoTextRenderer->SetPivot({ 0.0f, 0.0f });
	keyInfoTextRenderer->SetRectSize({ 800.0f, 600.0f });

	std::wostringstream woss;
	woss << L"이동: 방향키\n"
		<< L"회피: L-Shift 짧게\n"
		<< L"대시: 회피 후 L-Shift 유지\n";

	keyInfoTextRenderer->SetText(woss.str());
}