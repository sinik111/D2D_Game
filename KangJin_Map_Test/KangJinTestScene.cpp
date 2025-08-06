#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/GameObject.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/Component.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "KangJinTestScene.h"
#include "CameraController.h"
#include "Maps.h"
#include "Building01.h"
#include "Building02.h"
#include "Building02_01.h"
#include "Building02_02.h"
#include "Building02_03.h"
#include "Building02_04.h"
#include "Building03.h"
#include "Building04.h"
#include "Building05.h"
#include "Building051.h"
#include "Building052.h"
#include "Building06.h"
#include "Building07.h"
#include "Building071.h"
#include "Building072.h"
#include "Building08.h"
#include "Building09.h"
#include "Building10.h"
#include "Building11.h"
#include "Building12.h"
#include "Building13_01.h"
#include "Building13_02.h"
#include "Building14_01.h"
#include "Building14_02.h"

#include "PlayerController.h"
void KangJinTestScene::Enter()
{
	//카메라
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();
	camera->AddComponent<CameraController>();
	camera->AddComponent<Maps>();
	

	CreateMap1();

	CreatePlayer();

	CreateBuilding2(5, 0,0);

	//CreateBuilding2(14, 0, 0);
}

void KangJinTestScene::CreateMap1()
{
	GameObject* gameobject = CreateGameObject(L"Map");
	gameobject->AddComponent<BitmapRenderer>(L"map01_ver1.jpg")->SetSortOrder(0);
}

void KangJinTestScene::CreatePlayer()
{
	// 플레이어 관련 코드
	GameObject* player = CreateGameObject(L"Player");
	player->GetTransform()->SetLocalScale(0.5, 0.5);

	RigidBody2D* rigidbody = player->AddComponent<RigidBody2D>();
	rigidbody->SetGravityScale(0.0f);

	CircleCollider* playercircle = player->AddComponent<CircleCollider>();
	playercircle->SetLayer(CollisionLayer::PlayerMove);
	playercircle->SetRadius(10.0f);

	BitmapRenderer* bitmap = player->AddComponent<BitmapRenderer>(L"Building04.png");
	bitmap->SetPivot({ 0.5f, 1.0f });
	bitmap->SetSortOrder(1);

	PlayerController* playercontroller = player->AddComponent<PlayerController>();
}

void KangJinTestScene::CreateBuilding1(float x, float y)
{
	GameObject* structure = CreateGameObject(L"Building08");
	structure->AddComponent<BitmapRenderer>(L"Building08.png")->SetSortOrder(1);
	structure->GetTransform()->SetLocalPosition(x, y);

	Building08* st = structure->AddComponent<Building08>();
	st->SetSize(4);

	//imgui.SetValue(structure);
	//imgui.Add();
}

void KangJinTestScene::CreateBuilding2(int number,float x, float y)
{
	GameObject* structure = CreateGameObject(L"Building");
	std::string jsonfilepath;
	std::wstring wfilepath;
	if(number < 10)
	{
		jsonfilepath = "building_circles0" + std::to_string(number) + ".json";
		wfilepath = L"Building0" + std::to_wstring(number) + L".png";
	}
	else if (number >= 10)
	{
		jsonfilepath = "building_circles" + std::to_string(number) + ".json";
		wfilepath = L"Building" + std::to_wstring(number) + L".png";
	}

	if (number == 2 || number == 5 || number == 7)
	{
		wfilepath = L"Building0" + std::to_wstring(number) + L"_01.png";
	}
	else if (number == 13 || number == 14)
	{
		wfilepath = L"Building" + std::to_wstring(number) + L"_01.png";
	}

	structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
	structure->GetTransform()->SetLocalPosition(x, y);
	
	Vector2 pos = structure->GetTransform()->GetLocalPosition();
	Structure* st;
	switch (number)
	{
		case 1:
			st = structure->AddComponent<Building01>();

			break;
		case 2:
			st = structure->AddComponent<Building02_01>();
			
			structure = CreateGameObject(L"Building2");
			structure->GetTransform()->SetLocalPosition(pos.x+1, pos.y+265.0f);
			wfilepath = L"Building0" + std::to_wstring(number) + L"_02.png";
			structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
			st = structure->AddComponent<Building02_02>();
			
			structure = CreateGameObject(L"Building3");
			structure->GetTransform()->SetLocalPosition(pos.x+2, pos.y+560.0f);
			wfilepath = L"Building0" + std::to_wstring(number) + L"_03.png";
			structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
			st = structure->AddComponent<Building02_03>();
			
			structure = CreateGameObject(L"Building4");
			wfilepath = L"Building0" + std::to_wstring(number) + L"_04.png";
			structure->GetTransform()->SetLocalPosition(pos.x + 3, pos.y + 560.0f);
			structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
			st = structure->AddComponent<Building02_04>();

			st->SetSize(4);
			im1.SetName(jsonfilepath);
			im1.Add();
			im1.SetValue(structure);
			break;
		case 3:
			st = structure->AddComponent<Building03>();
			break;
		case 4:
			st = structure->AddComponent<Building04>();
			break;
		case 5:
			st = structure->AddComponent<Building051>();

			structure = CreateGameObject(L"Building2");
			structure->GetTransform()->SetLocalPosition(pos.x + 2, pos.y + 291);
			wfilepath = L"Building0" + std::to_wstring(number) + L"_02.png";
			structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
			st = structure->AddComponent<Building052>();
			break;
		case 6:
			st = structure->AddComponent<Building06>();
			break;
		case 7:
			st = structure->AddComponent<Building071>();

			structure = CreateGameObject(L"Building2");
			structure->GetTransform()->SetLocalPosition(pos.x, pos.y + 136.0f);
			wfilepath = L"Building0" + std::to_wstring(number) + L"_02.png";
			structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
			structure->AddComponent<Building072>();
			break;
		case 8:
			st = structure->AddComponent<Building08>();
			break;
		case 9:
			st = structure->AddComponent<Building09>();
			break;
		case 10:
			st = structure->AddComponent<Building10>();
			break;
		case 11:
			st = structure->AddComponent<Building11>();

			//st->SetSize(4);
			//im2.SetName(jsonfilepath);
			//im2.Add();
			//im2.SetValue(structure);
			break;
		case 12:
			st = structure->AddComponent<Building12>();

			/*st->SetSize(4);
			im2.SetName(jsonfilepath);
			im2.Add();
			im2.SetValue(structure);*/
			break;
		case 13:
			st = structure->AddComponent<Building13_01>();

			structure = CreateGameObject(L"Building2");
			structure->GetTransform()->SetLocalPosition(pos.x - 0.5f, pos.y + 285);
			wfilepath = L"Building" + std::to_wstring(number) + L"_02.png";
			structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
			st = structure->AddComponent<Building13_02>();
			break;
		case 14:
			st = structure->AddComponent<Building14_01>();
			
			structure = CreateGameObject(L"Building2");
			structure->GetTransform()->SetLocalPosition(pos.x - 0.5f, pos.y + 204);
			wfilepath = L"Building" + std::to_wstring(number) + L"_02.png";
			structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
			st = structure->AddComponent<Building14_02>();
			break;
		default:
			st = structure->AddComponent<Structure>();
			break;
	}
	st->SetSize(4);
	im1.SetName(jsonfilepath);
	im1.Add();
	im1.SetValue(structure);
}

