#include "pch.h"
#include "Script.h"

#include "ComponentSystem.h"
#include "SceneManager.h"
#include "BoxCollider2D.h"

GameObject* Script::CreateGameObject(const std::wstring& name)
{
	return SceneManager::Get().GetCurrentScene()->CreateGameObject(name);
}

void Script::RegisterToSystem()
{
	ComponentSystem::Get().Script().Register(this);

	auto components = GetGameObject()->GetComponents<BoxCollider2D>();

	for (const auto& collider : components)
	{
		collider->RegisterScript(this);
	}
}

void Script::UnregisterFromSystem()
{
	ComponentSystem::Get().Script().Unregister(this);

	auto components = GetGameObject()->GetComponents<BoxCollider2D>();

	for (const auto& collider : components)
	{
		collider->UnregisterScript(this);
	}
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
	// �ڽĿ��� Update()�� override���� �ʾ��� ��� Update ����Ʈ���� ������
	ComponentSystem::Get().Script().UnregisterUpdate(this);
}

void Script::LateUpdate()
{
	// �ڽĿ��� LateUpdate()�� override���� �ʾ��� ��� LateUpdate ����Ʈ���� ������
	ComponentSystem::Get().Script().UnregisterLateUpdate(this);
}

void Script::OnDestroy()
{

}

void Script::OnCollisionEnter()
{
}

void Script::OnCollisionStay()
{
}

void Script::OnCollisionExit()
{
}

void Script::OnTriggerEnter()
{
}

void Script::OnTriggerStay()
{
}

void Script::OnTriggerExit()
{
}
