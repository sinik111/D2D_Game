#include "pch.h"
#include "Script.h"

#include "ComponentSystem.h"
#include "SceneManager.h"

Script::Script()
{
	ComponentSystem::Script().Register(this);
}

Script::~Script()
{
	ComponentSystem::Script().Unregister(this);
}

GameObject* Script::CreateGameObject(const std::wstring& name)
{
	return SceneManager::GetCurrentScene()->CreateGameObject(name);
}

void Script::Start()
{

}

void Script::Update()
{
	// �ڽĿ��� Update()�� ����, ���ǵ��� �ʾ��� ���
	// Update ����Ʈ���� ������
	ComponentSystem::Script().UnregisterUpdate(this);
}

void Script::LateUpdate()
{
	// �ڽĿ��� LateUpdate()�� ����, ���ǵ��� �ʾ��� ���
	// LateUpdate ����Ʈ���� ������
	ComponentSystem::Script().UnregisterLateUpdate(this);
}