#include "pch.h"
#include "Script.h"

#include "ComponentSystem.h"
#include "SceneManager.h"

Script::~Script()
{
	
}

GameObject* Script::CreateGameObject(const std::wstring& name)
{
	return SceneManager::Get().GetCurrentScene()->CreateGameObject(name);
}

void Script::RegisterToSystem()
{
	ComponentSystem::Get().Script().Register(this);
}

void Script::UnregisterFromSystem()
{
	ComponentSystem::Get().Script().Unregister(this);
}

void Script::Initialize()
{

}

void Script::Start()
{

}

void Script::FixedUpdate()
{
	ComponentSystem::Get().Script().UnregisterFixedUpdate(this);
}

void Script::Update()
{
	// 자식에서 Update()가 override되지 않았을 경우 Update 리스트에서 제거함
	ComponentSystem::Get().Script().UnregisterUpdate(this);
}

void Script::LateUpdate()
{
	// 자식에서 LateUpdate()가 override되지 않았을 경우 LateUpdate 리스트에서 제거함
	ComponentSystem::Get().Script().UnregisterLateUpdate(this);
}

void Script::OnDestroy()
{

}