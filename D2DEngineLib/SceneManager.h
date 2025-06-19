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

public:
	static SceneManager& Get();

public:
	void Shutdown();

public:
	void Update();

public:
	void ChangeScene(const std::wstring& name);
	Scene* GetCurrentScene();

private:
	void CheckSceneChanged();

public:
	template<typename T>
	void CreateScene(const std::wstring& name)
	{
		std::unordered_map<std::wstring, std::unique_ptr<Scene>>& scenes = Get().m_scenes;

		if (scenes.find(name) != scenes.end())
		{
			return;
		}

		scenes.emplace(name, std::make_unique<T>());
	}
};