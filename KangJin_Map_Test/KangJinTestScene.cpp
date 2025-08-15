#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/GameObject.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/Component.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/ImGuiSystem.h"

#include "KangJinTestScene.h"
#include "CameraController.h"
#include "PlayerController.h"
#include "StructureCreater.h"
#include "Maps.h"
#include "Structure.h"

void KangJinTestScene::Enter()
{
	//카메라
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();
	camera->AddComponent<CameraController>();
	m_map = camera->AddComponent<Maps>();

	GameObject* terrain = CreateGameObject(L"Terrain");
	BitmapRenderer* terrainBitmap = terrain->AddComponent<BitmapRenderer>(L"belows_tuto.png");
	terrainBitmap->SetSortOrder(2);
	terrainBitmap->SetPivot({ 0.49f, 1});
	terrainBitmap = terrain->AddComponent<BitmapRenderer>(L"shadow_tuto_ver2.png");
	terrainBitmap->SetSortOrder(1);
	terrainBitmap->SetPivot({0.5f, 1});

	auto imguiObject = ImGuiSystem::Get().CreateImGui<Objectimgui>();
	imguiObject->SetObject(terrain);
	imguiObject->SetName("Terrain");
	terrain->GetTransform()->SetLocalPosition(0, -1920);

	//맵 생성
	CreateMap();
	
	//적 생상
	CreateEnemy();
}

void KangJinTestScene::CreateMap()
{
	GameObject* map1 = CreateGameObject(L"Map1");
	BitmapRenderer* mapbitmap1 = map1->AddComponent<BitmapRenderer>(L"map01/map01_ver3.png");
	mapbitmap1->SetSortOrder(0);

	GameObject* terrain = CreateGameObject(L"Terrain");
	BitmapRenderer* terrainBitmap = terrain->AddComponent<BitmapRenderer>(L"belows_tuto_ver2.png");
	terrainBitmap->SetSortOrder(2);
	terrainBitmap = terrain->AddComponent<BitmapRenderer>(L"shadow_tuto_ver2.png");
	terrainBitmap->SetSortOrder(1);
	terrain->GetTransform()->SetLocalPosition(0, 0);

	GameObject* map2 = CreateGameObject(L"Map2");
	BitmapRenderer* mapbitmap2 = map2->AddComponent<BitmapRenderer>(L"map02/map02_Lowwithshadow.png");
	mapbitmap2 = map2->AddComponent<BitmapRenderer>(L"map02/map02_Highwithshadow.png");
	mapbitmap2->SetSortOrder(1);
	mapbitmap2 = map2->AddComponent<BitmapRenderer>(L"map02/map02_Front.png");
	
	GameObject* map3 = CreateGameObject(L"Map3");
	auto mapbitmap3 = map3->AddComponent<BitmapRenderer>(L"map03/map03_lowwithshadow.png");
	mapbitmap3 = map3->AddComponent<BitmapRenderer>(L"map03/map03_high.png");
	mapbitmap3 = map3->AddComponent<BitmapRenderer>(L"map03/map03_front.png");
	mapbitmap3->SetSortOrder(2);

	Vector2 mapsize = {
		mapbitmap1->GetBitmap()->GetSize().width / 2 + mapbitmap2->GetBitmap()->GetSize().width / 2,
		mapbitmap1->GetBitmap()->GetSize().height / 2 + mapbitmap2->GetBitmap()->GetSize().height / 2
	};

	map2->GetTransform()->SetLocalPosition({ 0, mapsize.y });
	Vector2 map2pos = map2->GetTransform()->GetLocalPosition();
	map3->GetTransform()->SetLocalPosition(mapsize.x-1, mapsize.y);

	std::vector<ImportStructData> imported = m_map->ImportStructure();
	for (auto& data : imported)
	{
		CreateBuilding(data.type, data.position.x, data.position.y, data.id);
	}

}

void KangJinTestScene::CreateEnemy()
{
	// 적 오브젝트 관련 코드
	GameObject* Enemy1 = CreateGameObject(L"Enemy1");
	Enemy1->GetTransform()->SetLocalPosition({ 8000, 5000});
	//m_map->PushBackNormalEnemyPosition(Enemy1->GetTransform()->GetLocalPosition());
	RigidBody2D* rigidbody = Enemy1->AddComponent<RigidBody2D>();
	rigidbody->SetGravityScale(0.0f);
	CircleCollider* playercircle = Enemy1->AddComponent<CircleCollider>();
	playercircle->SetLayer(CollisionLayer::PlayerMove);
	playercircle->SetRadius(30.0f);
	BitmapRenderer* bitmap = Enemy1->AddComponent<BitmapRenderer>(L"WalkIn_N.png");
	bitmap->SetPivot({ 0.5f, 0.8f });
	bitmap->SetSortOrder(1);
	PlayerController* playercontroller = Enemy1->AddComponent<PlayerController>();

	GameObject* Enemy2 = CreateGameObject(L"Enemy2");
	Enemy2->GetTransform()->SetLocalPosition({ 2900,2950 });
	CircleCollider* circlecollider = Enemy2->AddComponent<CircleCollider>();
	circlecollider->SetLayer(CollisionLayer::EnemyMove);
	circlecollider->SetRadius(10.0f);
	BitmapRenderer* bitmap2 = Enemy2->AddComponent<BitmapRenderer>(L"EmemyIdle_E_001.png");
	bitmap2->SetPivot({ 0.47f, 0.56f });
	bitmap2->SetSortOrder(1);
	//m_map->PushBackNormalEnemyPosition(Enemy2->GetTransform()->GetLocalPosition());
}

void KangJinTestScene::CreateBuilding(int number, float x, float y, int id)
{
	GameObject* structure = CreateGameObject(L"Building");
	m_map->InsertStructure(number, structure, id);

	std::string jsonfilepath;
	std::wstring wfilepath;
	
	if (number < 10)
	{

		jsonfilepath = "Resource/building_circles0" + std::to_string(number);
		wfilepath = L"Building0" + std::to_wstring(number);
	}
	else if (number >= 10)
	{
		jsonfilepath = "Resource/building_circles" + std::to_string(number);
		wfilepath = L"Building" + std::to_wstring(number);
	}

	if (number == 2 || number == 5 || number == 7 || number == 13 || number == 14)
	{
		wfilepath += L"_01";
		jsonfilepath += "_01";
	}
	wfilepath += L".png";
	jsonfilepath += ".json";

	BitmapRenderer* pbitmap = structure->AddComponent<BitmapRenderer>(wfilepath);
	pbitmap->SetSortOrder(1);
	structure->GetTransform()->SetLocalPosition(x, y);

	auto imguiObject = ImGuiSystem::Get().CreateImGui<Objectimgui>();

	Vector2 pos = structure->GetTransform()->GetLocalPosition();
	Structure* st = structure->AddComponent<Structure>(jsonfilepath);;
	switch (number)
	{
	case 2:
		imguiObject->SetName(jsonfilepath);
		imguiObject->SetObject(structure);

		structure = CreateGameObject(L"Building2");
		structure->GetTransform()->SetLocalPosition(pos.x + 1, pos.y + 265.0f);
		wfilepath = L"Building0" + std::to_wstring(number) + L"_02.png";
		jsonfilepath = "Resource/building_circles0" + std::to_string(number) + "_02.json";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		imguiObject->SetName(jsonfilepath);
		imguiObject->SetObject(structure);

		structure = CreateGameObject(L"Building3");
		structure->GetTransform()->SetLocalPosition(pos.x + 2, pos.y + 560.0f);
		wfilepath = L"Building0" + std::to_wstring(number) + L"_03.png";
		jsonfilepath = "Resource/building_circles0" + std::to_string(number) + "_03.json";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		imguiObject->SetName(jsonfilepath);
		imguiObject->SetObject(structure);

		structure = CreateGameObject(L"Building4");
		wfilepath = L"Building0" + std::to_wstring(number) + L"_04.png";
		jsonfilepath = "Resource/building_circles0" + std::to_string(number) + "_04.json";
		structure->GetTransform()->SetLocalPosition(pos.x + 3, pos.y + 560.0f);
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		break;

	case 5:
		imguiObject->SetName(jsonfilepath);
		imguiObject->SetObject(structure);

		structure = CreateGameObject(L"Building2");
		structure->GetTransform()->SetLocalPosition(pos.x + 2, pos.y + 291);
		wfilepath = L"Building0" + std::to_wstring(number) + L"_02.png";
		jsonfilepath = "Resource/building_circles0" + std::to_string(number) + "_02.json";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		break;
	
	case 7:
		imguiObject->SetName(jsonfilepath);
		imguiObject->SetObject(structure);

		structure = CreateGameObject(L"Building2");
		structure->GetTransform()->SetLocalPosition(pos.x, pos.y + 136.0f);
		jsonfilepath = "Resource/building_circles0" + std::to_string(number) + "_02.json";
		wfilepath = L"Building0" + std::to_wstring(number) + L"_02.png";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		break;
	
	case 13:
		
		imguiObject->SetName(jsonfilepath);
		imguiObject->SetObject(structure);

		structure = CreateGameObject(L"Building2");
		structure->GetTransform()->SetLocalPosition(pos.x - 0.5f, pos.y + 285);
		wfilepath = L"Building" + std::to_wstring(number) + L"_02.png";
		jsonfilepath = "Resource/building_circles" + std::to_string(number) + "_02.json";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(0);
		st = structure->AddComponent<Structure>(jsonfilepath);
		break;

	case 14:
		imguiObject->SetName(jsonfilepath);
		imguiObject->SetObject(structure);

		structure = CreateGameObject(L"Building2");
		structure->GetTransform()->SetLocalPosition(pos.x - 0.5f, pos.y + 204);
		wfilepath = L"Building" + std::to_wstring(number) + L"_02.png";
		jsonfilepath = "Resource/building_circles" + std::to_string(number) + "_02.json";
		structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
		st = structure->AddComponent<Structure>(jsonfilepath);
		break;
	}
	st->SetSize(4);
	imguiObject->SetObject(structure);
	imguiObject->SetName(jsonfilepath + std::to_string(id));
}

