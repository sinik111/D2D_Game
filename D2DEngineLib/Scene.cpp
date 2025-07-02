#include "pch.h"
#include "Scene.h"

#include "GameObject.h"
#include "ResourceManager.h"

void Scene::Exit()
{
	ResourceManager::Get().ReleaseResources();

	Clear();
}

void Scene::Update()
{
	if (!m_pendingCreatedGameObjects.empty())
	{
		m_gameObjects.insert(
			m_gameObjects.end(),
			std::make_move_iterator(m_pendingCreatedGameObjects.begin()),
			std::make_move_iterator(m_pendingCreatedGameObjects.end())
		);

		m_pendingCreatedGameObjects.clear();
	}

	for (size_t i = 0; i < m_gameObjects.size(); )
	{
		if (m_gameObjects[i]->IsDestroyed())
		{
			std::swap(m_gameObjects[i], m_gameObjects.back());

			m_pendingDestroyedGameObjects.push_back(std::move(m_gameObjects.back()));

			m_gameObjects.pop_back();

			continue;
		}

		++i;
	}

	if (!m_pendingDestroyedGameObjects.empty())
	{
		for (const auto& gameObject : m_pendingDestroyedGameObjects)
		{
			m_validGameObjects.erase(gameObject.get());
		}

		m_pendingDestroyedGameObjects.clear();
	}
}


void Scene::Clear()
{
	m_pendingCreatedGameObjects.clear();
	m_gameObjects.clear();
	m_pendingDestroyedGameObjects.clear();
	m_validGameObjects.clear();
}

GameObject* Scene::CreateGameObject(const std::wstring& name)
{
	m_pendingCreatedGameObjects.push_back(std::make_unique<GameObject>(name));

	m_validGameObjects.insert(m_pendingCreatedGameObjects.back().get());

	return m_pendingCreatedGameObjects.back().get();
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

	return nullptr;
}

bool Scene::IsValid(GameObject* gameObject) const
{
	return m_validGameObjects.find(gameObject) != m_validGameObjects.end();
}
