#include "pch.h"
#include "SceneManager.h"

SceneManager::SceneManager()
	: m_currentScene{ nullptr }, m_nextScene{ nullptr }
{
}

SceneManager& SceneManager::Get()
{
	static SceneManager s_instance;

	return s_instance;
}

void SceneManager::Shutdown()
{
	m_scenes.clear();
}

void SceneManager::Update()
{
	CheckSceneChanged();

	m_currentScene->Update();
}

void SceneManager::ChangeScene(const std::wstring& name)
{
	std::unordered_map<std::wstring, std::unique_ptr<Scene>>& scenes = m_scenes;

	if (scenes.find(name) == scenes.end())
	{
		return;
	}

	m_nextScene = scenes[name].get();
}

Scene* SceneManager::GetCurrentScene()
{
	return m_currentScene;
}

void SceneManager::CheckSceneChanged() // 예외처리 추가 필요
{
	if (m_nextScene != nullptr)
	{	
		m_nextScene->Load();

		if (m_currentScene != nullptr)
		{
			m_currentScene->Exit();

			m_currentScene->Unload();
		}
		
		m_currentScene = m_nextScene;

		m_nextScene = nullptr;

		m_currentScene->Enter();
	}
}
