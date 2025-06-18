#include "../D2DEngineLib/framework.h"
#include "SolarSystem.h"

#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/Camera.h"
#include "SpaceObject.h"
#include "CameraController.h"
#include "Sun.h"
#include "Earth.h"
#include "Moon.h"

SolarSystem::~SolarSystem()
{
	Unload();
}

void SolarSystem::Load()
{
	LoadResources();
}

void SolarSystem::Enter()
{
	GameObject* camera = CreateGameObject(L"Camera");
	camera->AddComponent<Camera>();
	camera->AddComponent<CameraController>();

	GameObject* sun = CreateGameObject(L"Sun");
	GameObject* earth = CreateGameObject(L"Earth");
	GameObject* moon = CreateGameObject(L"Moon");

	moon->GetTransform()->SetParent(earth->GetTransform());
	earth->GetTransform()->SetParent(sun->GetTransform());

	sun->AddComponent<Sun>();
	earth->AddComponent<Earth>();
	moon->AddComponent<Moon>();

	sun->AddComponent<Sun>();
}

void SolarSystem::Exit()
{
}

void SolarSystem::Unload()
{
	ResourceManager::ReleaseResources();
}

void SolarSystem::LoadResources()
{
	ResourceManager::LoadBitmapFromFile(L"Sun", L"Sun.png");
	ResourceManager::LoadBitmapFromFile(L"Earth", L"Earth.png");
	ResourceManager::LoadBitmapFromFile(L"Moon", L"Moon.png");
}
