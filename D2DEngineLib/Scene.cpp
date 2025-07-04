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
	for (size_t i = 0; i < m_gameObjects.size(); )
	{
		if (m_gameObjects[i]->m_isDestroyed)
		{
			std::swap(m_gameObjects[i], m_gameObjects.back());

			m_gameObjects.pop_back();

			continue;
		}

		m_gameObjects[i]->Update();

		++i;
	}
}

void Scene::Clear()
{
	m_gameObjects.clear();
}

GameObject* Scene::CreateGameObject(const std::wstring& name)
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
