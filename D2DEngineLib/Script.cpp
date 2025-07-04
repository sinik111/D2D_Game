#include "pch.h"
#include "Script.h"

#include "ComponentSystem.h"
#include "SceneManager.h"

Script::Script()
{
	ComponentSystem::Get().Script().Register(this);
}

Script::~Script()
{
	ComponentSystem::Get().Script().Unregister(this);
}

GameObject* Script::CreateGameObject(const std::wstring& name)
{
	return SceneManager::Get().GetCurrentScene()->CreateGameObject(name);
}

void Script::Initialize()
{

}

void Script::Start()
{

}

void Script::Update()
{
	// �ڽĿ��� Update()�� override���� �ʾ��� ���
	// Update ����Ʈ���� ������
	ComponentSystem::Get().Script().UnregisterUpdate(this);
}

void Script::LateUpdate()
{
	// �ڽĿ��� LateUpdate()�� override���� �ʾ��� ���
	// LateUpdate ����Ʈ���� ������
	ComponentSystem::Get().Script().UnregisterLateUpdate(this);
}

void Script::OnDestroy()
{

}