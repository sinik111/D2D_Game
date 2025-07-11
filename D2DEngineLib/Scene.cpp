#include "pch.h"
#include "Scene.h"

#include "GameObject.h"
#include "ResourceManager.h"

void Scene::Exit()
{
	Clear();

	ResourceManager::Get().ReleaseResources();
}

void Scene::InitializeObjectsCreatedLastFrame()
{
	if (!m_createdLastFrameGameObjects.empty())
	{
		for (const auto& gameObject : m_createdLastFrameGameObjects)
		{
			if (Object::IsValid(gameObject))
			{
				gameObject->Initialize();
			}
		}

		m_createdLastFrameGameObjects.clear();
	}

	for (const auto& gameObject : m_gameObjects)
	{
		if (gameObject->HasAddedComponent())
		{
			gameObject->InitializeAddedComponents();
		}
	}
}

void Scene::CleanupDestroyedObjects()
{
	for (size_t i = 0; i < m_gameObjects.size(); )
	{
		auto& gameObject = m_gameObjects[i];

		if (gameObject->m_isDestroyed)
		{
			std::swap(gameObject, m_gameObjects.back());

			m_gameObjects.pop_back();

			continue;
		}
		else if (gameObject->HasDestroyedComponents())
		{
			gameObject->CleanupDestroyedComponents();
		}

		++i;
	}
}

void Scene::Clear()
{
	m_gameObjects.clear();
	m_createdLastFrameGameObjects.clear();
}

GameObject* Scene::CreateGameObject(const std::wstring& name)
{
	std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>(name);

	m_createdLastFrameGameObjects.push_back(gameObject.get());

	m_gameObjects.push_back(std::move(gameObject));

	return m_gameObjects.back().get();
}

GameObject* Scene::__CreateGameObject(const std::wstring& name)
{
	m_gameObjects.push_back(std::make_unique<GameObject>(name));

	return m_gameObjects.back().get();
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
