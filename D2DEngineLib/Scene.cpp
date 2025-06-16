#include "pch.h"
#include "Scene.h"

#include "GameObject.h"

Scene::Scene()
{

}

Scene::~Scene()
{
	Clear();
}

GameObject* Scene::CreateGameObject(const std::wstring& name)
{
	auto newGameObject = std::make_unique<GameObject>();

	m_pendingCreatedGameObjects.push_back(std::move(newGameObject));

	return newGameObject.get();
}

void Scene::Enter()
{
	
}

void Scene::Exit()
{
	Clear();
}

void Scene::Update()
{
	if (!m_pendingCreatedGameObjects.empty())
	{
		m_gameObjects.insert(m_gameObjects.end(),
			std::make_move_iterator(m_pendingCreatedGameObjects.begin()),
			std::make_move_iterator(m_pendingCreatedGameObjects.end()));

		m_pendingCreatedGameObjects.clear();
	}

	for (size_t i = 0; i < m_gameObjects.size(); )
	{
		//m_gameObjects[i]->Update();

		if (m_gameObjects[i])
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
		m_pendingDestroyedGameObjects.clear();
	}
}


void Scene::Clear()
{
	m_pendingCreatedGameObjects.clear();
	m_gameObjects.clear();
	m_pendingDestroyedGameObjects.clear();
}
