#pragma once

#include "Scene.h"

class D2DRenderer;

class SceneManager
{
private:
	std::unordered_map<std::wstring, std::unique_ptr<Scene>> m_scenes;

	Scene* m_currentScene = nullptr;
	Scene* m_nextScene = nullptr;

	D2DRenderer* m_d2dRenderer = nullptr;

private:
	SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;
	~SceneManager() = default;

public:
	static SceneManager& Get();

public:
	void CleanupDestroyedObjects();
	void Shutdown();

public:
	void SetD2DRenderer(D2DRenderer* d2dRenderer);
	Scene* GetCurrentScene();
	void ChangeScene(const std::wstring& name);
	void CheckSceneChanged();
	void InitializeGameObjectsCreatedLastFrame();

public:
	template<typename T>
	void CreateScene(const std::wstring& name)
	{
		if (m_scenes.find(name) == m_scenes.end())
		{
			m_scenes.emplace(name, std::make_unique<T>());
		}
	}

public:
	GameObject* Find(const std::wstring& name) const;
};