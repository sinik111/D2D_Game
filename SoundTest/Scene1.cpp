#include "../D2DEngineLib/framework.h"
#include "Scene1.h"

#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/SoundManager.h"

#include "SoundPlayer.h"

void Scene1::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();

	SoundManager::Get().LoadSound("Kawaii Side Of Life.mp3", L"MainBGM");
	SoundManager::Get().LoadSound("laser-shoot-38126.mp3", L"LaserSound");
	SoundManager::Get().LoadSound("laser-zap-90575.mp3", L"ZapSound");
	SoundManager::Get().LoadSound("machine-gun-burst-43670.mp3", L"MachinegunSound");
	SoundManager::Get().LoadSound("pixel-explosion-319166.mp3", L"ExplodeSound");
	SoundManager::Get().LoadSound("retro-laser-1-236669.mp3", L"Laser2Sound");
	SoundManager::Get().LoadSound("TheFatRat - Arcadia.mp3", L"ArcadiaBGM");
	SoundManager::Get().LoadSound("TheFatRat - Fire.mp3", L"FireBGM");

	GameObject* soundPlayer = CreateGameObject(L"SoundPlayer");
	soundPlayer->AddComponent<SoundPlayer>();
}