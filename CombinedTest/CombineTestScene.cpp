#include "../D2DEngineLib/framework.h"
#include "CombineTestScene.h"
#include "../D2DEngineLib/GameObject.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/Component.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/ImGuiSystem.h"

#include "Maps.h"


void CombineTestScene::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();
}

void CombineTestScene::CreateMap()
{
}

void CombineTestScene::CreateEnemy()
{
}

void CombineTestScene::CreateBuilding(int index, float x, float y, int id)
{
}
