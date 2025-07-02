#include "pch.h"
#include "SceneManager.h"

#include "D2DRenderer.h"

SceneManager& SceneManager::Get()
{
	static SceneManager s_instance;

	return s_instance;
}

void SceneManager::Shutdown()
{
	if (m_currentScene != nullptr)
	{
		m_currentScene->Exit();
	}

	m_scenes.clear();
}

void SceneManager::Update()
{
	m_currentScene->Update();
}

void SceneManager::ChangeScene(const std::wstring& name)
{
	if (m_scenes.find(name) == m_scenes.end())
	{
		return;
	}

	m_nextScene = m_scenes[name].get();
}

void SceneManager::SetD2DRenderer(D2DRenderer* d2dRenderer)
{
	m_d2dRenderer = d2dRenderer;
}

Scene* SceneManager::GetCurrentScene()
{
	return m_currentScene;
}

void SceneManager::CheckSceneChanged()
{
	if (m_nextScene != nullptr)
	{
		if (m_currentScene != nullptr)
		{
			m_currentScene->Exit();

			m_d2dRenderer->Trim();
		}
		
		m_currentScene = m_nextScene;

		m_nextScene = nullptr;

		m_currentScene->Enter();
	}
}

GameObject* SceneManager::Find(const std::wstring& name) const
{
	return m_currentScene->Find(name);
}

bool SceneManager::IsValid(GameObject* gameObject) const
{
	if (m_currentScene == nullptr)
	{
		return false;
	}

	return m_currentScene->IsValid(gameObject);
}