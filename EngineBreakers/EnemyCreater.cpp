#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/json.hpp"
#include "../D2DEngineLib/CircleCollider.h"
#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/BoxCollider2D.h"

#include <fstream>

#include "EnemyCreater.h"
#include "EnemyZako.h"
#include "EnemyElite.h"
#include "EnemyBoss.h"

using json = nlohmann::json;

void EnemyCreater::Start()
{

}

void EnemyCreater::ImportEnemyPositionFromJSON()
{
	//.json 파일로부터 적 위치 정보 읽어오고 생성
	std::ifstream infile(ResourceManager::Get().GetResourcePath() + L"Map/Enemy_Positions.json");
	if(infile.is_open())
	{
		json j;
		infile >> j;
		infile.close();

		for (auto& normalEnemies : j["normalEnemy"])
		{
			Vector2 normalEnemyPosition = { normalEnemies["position"]["x"],normalEnemies["position"]["y"] };
			GameObject* gameobject = CreateGameObject(L"ZakoEnemy");
			gameobject->GetTransform()->SetLocalPosition(normalEnemyPosition);
			gameobject->AddComponent<BitmapRenderer>()->SetSortOrder(1);
			gameobject->AddComponent<Animator>();
			//gameobject->AddComponent<TextRenderer>();
			gameobject->AddComponent<EnemyZako>()->SetTargetPlayerAndCamera(player,camera);
			gameobject->AddComponent<RigidBody2D>();
			gameobject->AddComponent<BoxCollider2D>();
			gameobject->AddComponent<CircleCollider>();
		}

		for (auto& eliteEnemies : j["eliteEnemy"])
		{
			Vector2 eliteEnemyPosition = { eliteEnemies["position"]["x"], eliteEnemies["position"]["y"] };
			GameObject* elite = CreateGameObject(L"EliteEnemy");
			elite->GetTransform()->SetLocalPosition(eliteEnemyPosition);
			elite->AddComponent<BitmapRenderer>()->SetSortOrder(1);
			elite->AddComponent<Animator>();
			//elite->AddComponent<TextRenderer>();
			elite->AddComponent<EnemyElite>()->SetTargetPlayerAndCamera(player, camera);;
			elite->AddComponent<RigidBody2D>();
			elite->AddComponent<BoxCollider2D>();
			elite->AddComponent<CircleCollider>();
		}
		
		Vector2 bossPosition = { j["bossEnemy"]["position"]["x"], j["bossEnemy"]["position"]["y"]};
		GameObject* boss = CreateGameObject(L"BossEnemy");
		boss->GetTransform()->SetLocalPosition(bossPosition);
		boss->AddComponent<BitmapRenderer>()->SetSortOrder(1);
		boss->AddComponent<Animator>();
		//boss->AddComponent<TextRenderer>();
		boss->AddComponent<EnemyBoss>()->SetTargetPlayerAndCamera(player, camera);;
		boss->AddComponent<RigidBody2D>();
		boss->AddComponent<BoxCollider2D>();
		boss->AddComponent<CircleCollider>();
	}
}

void EnemyCreater::SetPlayer(GameObject* object)
{
	player = object;
}

void EnemyCreater::SetCamera(GameObject* object)
{
	camera = object;
}
