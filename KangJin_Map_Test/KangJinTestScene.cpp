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
#include "Building03.h"
#include "Building04.h"
#include "Building05.h"
#include "Building06.h"
#include "Building07.h"
#include "Building08.h"
#include "Building09.h"
#include "Building10.h"
#include "Building11.h"

//#include "Building08.h"
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

	//CreateBuilding1(100, 100);
	//CreateBuilding2(1,100, 100);
	CreateBuilding2(2,100, 100);
	//CreateBuilding2(8,1400, 1600);
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

	imgui.SetValue(structure);
	imgui.Add();
}

void KangJinTestScene::CreateBuilding2(int number,float x, float y)
{
	GameObject* structure = CreateGameObject(L"Building");
	
	std::wstring wfilepath;
	if(number < 10)
	{
		wfilepath = L"Building0" + std::to_wstring(number) + L".png";
	}
	else if (number >= 10)
	{
		wfilepath = L"Building" + std::to_wstring(number) + L".png";
	}
	
	/*if (number == 2)
	{
		wfilepath = L"Building0" + std::to_wstring(number);
	}*/

	structure->AddComponent<BitmapRenderer>(wfilepath)->SetSortOrder(1);
	structure->GetTransform()->SetLocalPosition(x, y);
	Structure* st;
	switch (number)
	{
		case 1:
			st = structure->AddComponent<Building01>();
			break;
		case 2:
			st = structure->AddComponent<Building02>();
			//GameObject* structure2 = CreateGameObject
			break;
		case 3:
			st = structure->AddComponent<Building03>();
			break;
		case 4:
			st = structure->AddComponent<Building04>();
			break;
		case 5:
			st = structure->AddComponent<Building05>();
			break;
		case 6:
			st = structure->AddComponent<Building06>();
			break;
		case 7:
			st = structure->AddComponent<Building07>();
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
			break;
		default:
			st = structure->AddComponent<Structure>();
			break;
	}

	st->SetSize(4);
	imgui.Add();
	imgui.SetValue(structure);
}

