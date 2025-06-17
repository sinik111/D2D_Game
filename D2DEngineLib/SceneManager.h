#pragma once

#include "Scene.h"

class SceneManager
{
private:
	std::unordered_map<std::wstring, std::unique_ptr<Scene>> m_scenes;

	Scene* m_currentScene;
	Scene* m_nextScene;

private:
	SceneManager();
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;
	~SceneManager() = default;

private:
	static SceneManager& Get();

public:
	static void Shutdown();

public:
	static void Update();

public:
	static void ChangeScene(const std::wstring& name);
	static Scene* GetCurrentScene();

private:
	static void CheckSceneChanged();

public:
	template<typename T>
	static void CreateScene(const std::wstring& name)
	{
		std::unordered_map<std::wstring, std::unique_ptr<Scene>>& scenes = Get().m_scenes;

		if (scenes.find(name) != scenes.end())
		{
			return;
		}

		scenes.emplace(name, std::make_unique<T>());
	}
};