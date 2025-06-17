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
	Get().m_scenes.clear();
}

void SceneManager::Update()
{
	CheckSceneChanged();

	Get().m_currentScene->Update();
}

void SceneManager::ChangeScene(const std::wstring& name)
{
	std::unordered_map<std::wstring, std::unique_ptr<Scene>>& scenes = Get().m_scenes;

	if (scenes.find(name) == scenes.end())
	{
		return;
	}

	Get().m_nextScene = scenes[name].get();
}

Scene* SceneManager::GetCurrentScene()
{
	return Get().m_currentScene;
}

void SceneManager::CheckSceneChanged() // 예외처리 추가 필요
{
	if (Get().m_nextScene != nullptr)
	{	
		Get().m_nextScene->Load();

		if (Get().m_currentScene != nullptr)
		{
			Get().m_currentScene->Exit();

			Get().m_currentScene->Unload();
		}
		
		Get().m_currentScene = Get().m_nextScene;

		Get().m_nextScene = nullptr;

		Get().m_currentScene->Enter();
	}
}
