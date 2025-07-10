#include "pch.h"
#include "Scene.h"

#include "GameObject.h"
#include "ResourceManager.h"

void Scene::Exit()
{
	ResourceManager::Get().ReleaseResources();

	Clear();
}

void Scene::CleanupDestroyedGameObjects()
{
	for (size_t i = 0; i < m_gameObjects.size(); )
	{
		if (m_gameObjects[i]->m_isDestroyed)
		{
			std::swap(m_gameObjects[i], m_gameObjects.back());

			m_gameObjects.pop_back();

			continue;
		}

		m_gameObjects[i]->CleanupDestroyedComponents();

		++i;
	}
}

void Scene::Clear()
{
	m_gameObjects.clear();
	m_pendingInitializeGameObjects.clear();
}

GameObject* Scene::CreateGameObject(const std::wstring& name)
{
	m_pendingInitializeGameObjects.push_back(std::make_unique<GameObject>(name));

	return m_pendingInitializeGameObjects.back().get();
}

GameObject* Scene::__CreateGameObject(const std::wstring& name)
{
	m_gameObjects.push_back(std::make_unique<GameObject>(name));

	return m_gameObjects.back().get();
}

void Scene::InitializeGameObjectsCreatedLastFrame()
{
	if (!m_pendingInitializeGameObjects.empty())
	{
		for (size_t i = 0; i < m_pendingInitializeGameObjects.size(); ++i)
		{
			m_pendingInitializeGameObjects[i]->Initialize();
		}
		
		m_gameObjects.insert(m_gameObjects.end(),
			std::make_move_iterator(m_pendingInitializeGameObjects.begin()),
			std::make_move_iterator(m_pendingInitializeGameObjects.end()));

		m_pendingInitializeGameObjects.clear();
	}
}

GameObject* Scene::Find(const std::wstring& name) const
{
	const auto& find = std::find_if(
		m_gameObjects.begin(),
		m_gameObjects.end(),
		[name](const std::unique_ptr<GameObject>& gameObject) {
			return gameObject->GetName() == name;
		}
	);

	if (find != m_gameObjects.end())
	{
		return find->get();
	}

	const auto& findFromInit = std::find_if(
		m_pendingInitializeGameObjects.begin(),
		m_pendingInitializeGameObjects.end(),
		[name](const std::unique_ptr<GameObject>& gameObject) {
			return gameObject->GetName() == name;
		}
	);

	if (findFromInit != m_pendingInitializeGameObjects.end())
	{
		return findFromInit->get();
	}

	return nullptr;
}
