#include "../D2DEngineLib/framework.h"
#include "GamePlayScene.h"

#include "../D2DEngineLib/GameObject.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/Component.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Screen.h"
#include "../D2DEngineLib/CircleCollider.h"
#include "../D2DEngineLib/LineCollider.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/BatchRenderer.h"
#include "../D2DEngineLib/SoundManager.h"
#include "../D2DEngineLib/ResourceManager.h"

#include "Maps.h"
#include "Structure.h"
#include "HealthGaugeController.h"
#include "StaminaGaugeController.h"
#include "Player.h"
#include "FollowCameraController.h"
#include "PlayerCameraController.h"
#include "HitStopController.h"
#include "EnemyBase.h"
#include "EnemyZako.h"
#include "EnemyElite.h"
#include "EnemyBoss.h"

#include "GameDataManager.h"
#include "InGameUIInfoController.h"
#include "PauseController.h"
#include "EventChecker.h"
#include "ScreenCurtainController.h"
#include "EnemyCreater.h"

void GamePlayScene::Enter()
{
	GameDataManager::Get().Reset();

	SoundManager::Get().StopSound(L"MainTitleBGM");

	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();
	
	m_map = camera->AddComponent<Maps>();


	auto controller = camera->AddComponent<PlayerCameraController>();
	auto hsController = camera->AddComponent<HitStopController>();
	auto enemycreater = camera->AddComponent<EnemyCreater>();

	GameObject* player = CreateGameObject(L"Player");
	m_Player = player;
	controller->SetTarget(player->GetTransform());
	auto playerBr = player->AddComponent<BitmapRenderer>();
	playerBr->SetSortOrder(1);
	player->AddComponent<Animator>();
	auto rb = player->AddComponent<RigidBody2D>();
	player->AddComponent<PlayerInput>();
	auto playerComp = player->AddComponent<Player>();
	auto circle = player->AddComponent<CircleCollider>();
	player->AddComponent<BoxCollider2D>();
	GameDataManager::Get().SetPlayer(playerComp);
	player->GetTransform()->SetLocalPosition(-2040.0f, -1455.0f);

	enemycreater->SetPlayer(player);
	enemycreater->SetCamera(camera);
	enemycreater->ImportEnemyPositionFromJSON();

	CreateMap();

	CreateInGameUI();

	LoadSound();

	GameObject* screenCurtainController = CreateGameObject(L"ScreenCurtainController");
	screenCurtainController->AddComponent<ScreenCurtainController>(true, 1.0f);
}

void GamePlayScene::CreateMap()
{
	GameObject* map1 = CreateGameObject(L"Map1");
	BitmapRenderer* mapbitmap1 = map1->AddComponent<BitmapRenderer>(L"Map/map01/map01_ver3.png");
	mapbitmap1->SetSortOrder(0);
	
	GameObject* terrain = CreateGameObject(L"Terrain");
	BitmapRenderer* terrainBitmap = terrain->AddComponent<BitmapRenderer>(L"Map/belows_tuto.png");
	terrainBitmap->SetSortOrder(2);
	terrainBitmap->SetPivot({ 0.49f, 1 });
	terrainBitmap = terrain->AddComponent<BitmapRenderer>(L"Map/shadow_tuto_ver2.png");
	terrainBitmap->SetSortOrder(1);
	terrainBitmap->SetPivot({ 0.5f, 1 });
	terrain->GetTransform()->SetLocalPosition(0, -1920);

	GameObject* map2 = CreateGameObject(L"Map2");
	BitmapRenderer* mapbitmap2 = map2->AddComponent<BitmapRenderer>(L"Map/map02/map02_Lowwithshadow.png");
	mapbitmap2 = map2->AddComponent<BitmapRenderer>(L"Map/map02/map02_Highwithshadow.png");
	mapbitmap2 = map2->AddComponent<BitmapRenderer>(L"Map/map02/map02_Front.png");

	GameObject* map3 = CreateGameObject(L"Map3");
	auto mapbitmap3 = map3->AddComponent<BitmapRenderer>(L"Map/map03/map03_lowwithshadow.png");
	mapbitmap3 = map3->AddComponent<BitmapRenderer>(L"Map/map03/map03_high.png");
	mapbitmap3 = map3->AddComponent<BitmapRenderer>(L"Map/map03/map03_front.png");
	mapbitmap3->SetSortOrder(2);

	Vector2 mapsize = {
		mapbitmap1->GetBitmap()->GetSize().width / 2 + mapbitmap2->GetBitmap()->GetSize().width / 2,
		mapbitmap1->GetBitmap()->GetSize().height / 2 + mapbitmap2->GetBitmap()->GetSize().height / 2
	};

	map2->GetTransform()->SetLocalPosition({ 0, mapsize.y });
	Vector2 map2pos = map2->GetTransform()->GetLocalPosition();
	map3->GetTransform()->SetLocalPosition(mapsize.x, mapsize.y);

	std::vector<ImportStructData> imported = m_map->ImportStructure();
	for (auto& data : imported)
	{
		CreateBuilding(data.type, data.position.x, data.position.y, data.id);
	}

	GameObject* EventChecker1 = CreateGameObject(L"EventChecker1");
	EventChecker* ec = EventChecker1->AddComponent<EventChecker>();
	ec->SetEventCheckPosition({ 2000, 3200 });
	ec->SetEventEndPosition({ 2040, 3028});
	ec->SetPlayer(m_Player);
	ec->SetIsJumpUp(false);

	GameObject* Eventchecker2 = CreateGameObject(L"EventChecker2");
	ec = Eventchecker2->AddComponent<EventChecker>();
	ec->SetEventCheckPosition({ 1950, 4300});
	ec->SetEventEndPosition({ 1250, 4580});
	ec->SetEventJumpStartPosition({ 1330, 4660 });
	ec->SetPlayer(m_Player);
	ec->SetIsJumpUp(true);

	GameObject* Eventchecker3 = CreateGameObject(L"EventChecker3");
	ec = Eventchecker3->AddComponent<EventChecker>();
	ec->SetEventCheckPosition({ 3200, 5440});
	ec->SetEventEndPosition({ 3450, 5258});
	ec->SetPlayer(m_Player);
	ec->SetIsJumpUp(false);

	auto cameraController = Camera::s_mainCamera->GetGameObject()->GetComponent<PlayerCameraController>();
	cameraController->SetMapObjects({ map1, map2, map3 });
}

void GamePlayScene::CreateBuilding(int number, float x, float y, int id)
{
	GameObject* structure = CreateGameObject(L"Building");
	m_map->InsertStructure(number, structure, id);

	//std::string jsonfilepath;
	std::wstring jsonfilepath;
	std::wstring wfilepath;

	if (number < 10)
	{
		jsonfilepath = ResourceManager::Get().GetResourcePath() + L"Map/building_circles0" + std::to_wstring(number);
		wfilepath = L"Map/Building0" + std::to_wstring(number);
	}
	else if (number >= 10)
	{
		jsonfilepath = ResourceManager::Get().GetResourcePath() + L"Map/building_circles" + std::to_wstring(number);
		wfilepath = L"Map/Building" + std::to_wstring(number);
	}

	if (number == 2 || number == 5 || number == 7 || number == 13 || number == 14)
	{
		jsonfilepath += L"_01";
		wfilepath += L"_01";
	}
	jsonfilepath += L".json";
	wfilepath += L".png";

	structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
	structure->GetTransform()->SetLocalPosition(x, y);

	Vector2 pos = structure->GetTransform()->GetLocalPosition();
	Structure* st = structure->AddComponent<Structure>(jsonfilepath);;
	switch (number)
	{
	case 2:
		structure = CreateGameObject(L"Building2");
		structure->GetTransform()->SetLocalPosition(pos.x + 1, pos.y + 265.0f);
		wfilepath = L"Map/Building0" + std::to_wstring(number) + L"_02.png";
		jsonfilepath = ResourceManager::Get().GetResourcePath() + L"Map/building_circles0" + std::to_wstring(number) + L"_02.json";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);


		structure = CreateGameObject(L"Building3");
		structure->GetTransform()->SetLocalPosition(pos.x + 2, pos.y + 560.0f);
		jsonfilepath = ResourceManager::Get().GetResourcePath() + L"Map/building_circles0" + std::to_wstring(number) + L"_03.json";
		wfilepath = L"Map/Building0" + std::to_wstring(number) + L"_03.png";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);


		structure = CreateGameObject(L"Building4");
		wfilepath = L"Map/Building0" + std::to_wstring(number) + L"_04.png";
		jsonfilepath = ResourceManager::Get().GetResourcePath() + L"Map/building_circles0" + std::to_wstring(number) + L"_04.json";
		structure->GetTransform()->SetLocalPosition(pos.x + 3, pos.y + 560.0f);
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		break;

	case 5:
		structure = CreateGameObject(L"Building2");
		structure->GetTransform()->SetLocalPosition(pos.x + 2, pos.y + 291);
		wfilepath = L"Map/Building0" + std::to_wstring(number) + L"_02.png";
		jsonfilepath = ResourceManager::Get().GetResourcePath() + L"Map/building_circles0" + std::to_wstring(number) + L"_02.json";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		break;

	case 7:
		structure = CreateGameObject(L"Building2");
		structure->GetTransform()->SetLocalPosition(pos.x, pos.y + 136.0f);
		jsonfilepath = ResourceManager::Get().GetResourcePath() + L"Map/building_circles0" + std::to_wstring(number) + L"_02.json";
		wfilepath = L"Map/Building0" + std::to_wstring(number) + L"_02.png";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		break;

	case 13:
		structure = CreateGameObject(L"Building2");
		structure->GetTransform()->SetLocalPosition(pos.x - 0.5f, pos.y + 285);
		wfilepath = L"Map/Building" + std::to_wstring(number) + L"_02.png";
		jsonfilepath = ResourceManager::Get().GetResourcePath() + L"Map/building_circles" + std::to_wstring(number) + L"_02.json";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		break;

	case 14:
		structure = CreateGameObject(L"Building2");
		structure->GetTransform()->SetLocalPosition(pos.x - 0.5f, pos.y + 204);
		wfilepath = L"Map/Building" + std::to_wstring(number) + L"_02.png";
		jsonfilepath = ResourceManager::Get().GetResourcePath() + L"Map/building_circles" + std::to_wstring(number) + L"_02.json";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		break;
	}
	st->SetSize(4);
}

void GamePlayScene::CreateInGameUI()
{
	GameObject* inGameUIOpacity = CreateGameObject(L"InGameUIOpacity");
	auto br = inGameUIOpacity->AddComponent<BitmapRenderer>(L"Image/UI/MAP_OPACITY.png");
	br->SetSortOrder(0);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* inGameUIFrame = CreateGameObject(L"InGameUIFrame");
	br = inGameUIFrame->AddComponent<BitmapRenderer>(L"Image/UI/MAP_UI.png");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);


	GameObject* inGameUIKeyInfo = CreateGameObject(L"InGameUIKeyInfo");
	br = inGameUIKeyInfo->AddComponent<BitmapRenderer>(L"Image/UI/ACTION_KEYS_ICON.png");
	br->SetSortOrder(1);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 1.0f, 1.0f });
	inGameUIKeyInfo->GetTransform()->SetLocalPosition({ 920.0f, -440.0f });


	GameObject* hpBarFrame = CreateGameObject(L"HpBarFrame");
	br = hpBarFrame->AddComponent<BitmapRenderer>(L"Image/UI/HEALTH_BAR_02.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });
	hpBarFrame->GetTransform()->SetLocalPosition({ -860.0f, -440.0f });


	GameObject* staminaBarFrame = CreateGameObject(L"StaminaBarFrame");
	br = staminaBarFrame->AddComponent<BitmapRenderer>(L"Image/UI/ENERGY_BAR_02.png");
	br->SetSortOrder(2);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });
	staminaBarFrame->GetTransform()->SetLocalPosition({ -860.0f, -455.0f });


	GameObject* hpBar = CreateGameObject(L"HpBar");
	br = hpBar->AddComponent<BitmapRenderer>(L"Image/UI/HEALTH_BAR_01.png");
	br->SetSortOrder(3);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });
	hpBar->GetTransform()->SetLocalPosition({ -860.0f, -440.0f });
	auto healthGaugeController = hpBar->AddComponent<HealthGaugeController>();
	GameDataManager::Get().SetHealthGaugeController(healthGaugeController);


	GameObject* staminaBar = CreateGameObject(L"StaminaBar");
	br = staminaBar->AddComponent<BitmapRenderer>(L"Image/UI/ENERGY_BAR_01.png");
	br->SetSortOrder(3);
	br->SetSpaceType(SpaceType::Screen);
	br->SetPivot({ 0.0f, 0.5f });
	staminaBar->GetTransform()->SetLocalPosition({ -860.0f, -455.0f });
	auto staminaGaugeController = staminaBar->AddComponent<StaminaGaugeController>();
	GameDataManager::Get().SetStaminaGaugeController(staminaGaugeController);


	GameObject* infoController = CreateGameObject(L"InfoController");
	infoController->AddComponent<BatchRenderer>();
	auto inGameUIInfoController = infoController->AddComponent<InGameUIInfoController>();

	GameDataManager::Get().SetInGameUIInfoController(inGameUIInfoController);
	
	GameObject* pauseController = CreateGameObject(L"PauseController");
	pauseController->AddComponent<PauseController>();
}

void GamePlayScene::LoadSound()
{
	SoundManager::Get().LoadSound("Sound/BGM/GameBGAll.mp3", L"GamePlayBGM", true);
	SoundManager::Get().LoadSound("Sound/BGM/CountDown.mp3", L"CountDown", true);

	SoundManager::Get().LoadSound("Sound/Player/Player_AttackSuccess.mp3", L"PlayerAttackSuccess", true);
	SoundManager::Get().LoadSound("Sound/Player/Player_AttackTry1st.mp3", L"PlayerAttackTry1", true);
	SoundManager::Get().LoadSound("Sound/Player/Player_AttackTry2nd3rd.mp3", L"PlayerAttackTry23", true);
	SoundManager::Get().LoadSound("Sound/Player/Player_Damaged.mp3", L"PlayerDamaged", true);
	SoundManager::Get().LoadSound("Sound/Player/Player_DashIn.mp3", L"PlayerDashIn", true);
	SoundManager::Get().LoadSound("Sound/Player/Player_Dead.mp3", L"PlayerDead", true);
	SoundManager::Get().LoadSound("Sound/Player/Player_Footstep01.mp3", L"PlayerFootstep1", true);
	SoundManager::Get().LoadSound("Sound/Player/Player_Footstep02.mp3", L"PlayerFootstep2", true);
	SoundManager::Get().LoadSound("Sound/Player/PlayerDashEnd.mp3", L"PlayerDashEnd", true);
	SoundManager::Get().LoadSound("Sound/Player/PlayerDashIng.mp3", L"PlayerDashing", true);
	SoundManager::Get().LoadSound("Sound/Player/PlayerJump.mp3", L"PlayerJump", true);

	SoundManager::Get().LoadSound("Sound/Boss/Boss_Dead.mp3", L"BossDead", false);
	SoundManager::Get().LoadSound("Sound/Boss/Boss_Footstep1.mp3", L"BossFootstep1", false);
	SoundManager::Get().LoadSound("Sound/Boss/Boss_Footstep2.mp3", L"BossFootstep2", false);
	SoundManager::Get().LoadSound("Sound/Boss/Boss_HorizontalAttack1.mp3", L"BossHAttack1", false);
	SoundManager::Get().LoadSound("Sound/Boss/Boss_HorizontalAttack2.mp3", L"BossHAttack2", false);
	SoundManager::Get().LoadSound("Sound/Boss/Boss_HorizontalAttack3.mp3", L"BossHAttack3", false);
	SoundManager::Get().LoadSound("Sound/Boss/Boss_Idle.mp3", L"BossIdle", false);
	SoundManager::Get().LoadSound("Sound/Boss/Boss_StabAttacking.mp3", L"BossSAttacking", false);
	SoundManager::Get().LoadSound("Sound/Boss/Boss_StabPreAttack.mp3", L"BossSPreAttack", false);

	SoundManager::Get().LoadSound("Sound/Enemy/Eite_Enemy_StabAttack.mp3", L"EnemyStabAttack", false);
	SoundManager::Get().LoadSound("Sound/Enemy/Enemy_Atteck001.mp3", L"EnemyAttack", false);
	SoundManager::Get().LoadSound("Sound/Enemy/Enemy_Idle.mp3", L"EnemyIdle", false);
}
