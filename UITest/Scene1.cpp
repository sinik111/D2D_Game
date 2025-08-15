#include "../D2DEngineLib/framework.h"
#include "Scene1.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/SoundManager.h"
#include "../D2DEngineLib/BitmapRenderer.h"

#include "BitmapFillTest.h"

void Scene1::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();

	GameObject* bitmapFillTest = CreateGameObject(L"BitmapFillTest");
	bitmapFillTest->AddComponent<BitmapFillTest>();
	bitmapFillTest->AddComponent<BitmapRenderer>(L"HpBarTest.png");
}