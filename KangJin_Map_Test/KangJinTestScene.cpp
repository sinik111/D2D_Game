#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/GameObject.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/Component.h"

#include "KangJinTestScene.h"
#include "CameraController.h"
#include "Maps.h"
#include "Structure.h"
void KangJinTestScene::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();
	camera->AddComponent<CameraController>();
	camera->AddComponent<Maps>();

	GameObject* gameobject = CreateGameObject(L"Player");
	gameobject->AddComponent<BitmapRenderer>(L"map01_ver1_colision.jpg")->SetSortOrder(0);
	
	GameObject* structure = CreateGameObject(L"Structure1");
	structure->AddComponent<BitmapRenderer>(L"belows_tuto.png")->SetSortOrder(1);
	Structure* st = structure->AddComponent<Structure>();
	st->SetSize(3);
	
	imgui.SetValue(structure);
	imgui.Add();
}
