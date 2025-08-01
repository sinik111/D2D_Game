#include "pch.h"
#include "Script.h"

#include "ComponentSystem.h"
#include "SceneManager.h"
#include "Collider.h"

GameObject* Script::CreateGameObject(const std::wstring& name)
{
	return SceneManager::Get().GetCurrentScene()->CreateGameObject(name);
}

void Script::RegisterToSystem()
{
	ComponentSystem::Get().Script().Register(this);

	auto components = GetGameObject()->GetComponents<Collider>();

	for (const auto& collider : components)
	{
		collider->RegisterScript(this);
	}
}

void Script::UnregisterFromSystem()
{
	ComponentSystem::Get().Script().Unregister(this);

	auto components = GetGameObject()->GetComponents<Collider>();

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

void Script::OnCollisionEnter(const Collision& collision)
{
}

void Script::OnCollisionStay(const Collision& collision)
{
}

void Script::OnCollisionExit(const Collision& collision)
{
}

void Script::OnTriggerEnter(const Collision& collision)
{
}

void Script::OnTriggerStay(const Collision& collision)
{
}

void Script::OnTriggerExit(const Collision& collision)
{
}