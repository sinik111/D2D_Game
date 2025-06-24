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
		if (m_scenes.find(name) != m_scenes.end())
		{
			return;
		}

		m_scenes.emplace(name, std::make_unique<T>());
	}

public:
	GameObject* Find(const std::wstring& name) const;
	bool IsValid(GameObject* gameObject) const;
};