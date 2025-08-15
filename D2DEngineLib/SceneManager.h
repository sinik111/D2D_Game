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

	std::wstring m_previousSceneName;

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
	void InitializeObjects();
	void Shutdown();

public:
	void SetD2DRenderer(D2DRenderer* d2dRenderer);
	Scene* GetCurrentScene();
	const std::wstring& GetPreviousSceneName() const;
	void ChangeScene(const std::wstring& name);
	void CheckSceneChanged();

public:
	template<typename T>
	void CreateScene(const std::wstring& name)
	{
		if (m_scenes.find(name) == m_scenes.end())
		{
			std::unique_ptr<Scene> newScene = std::make_unique<T>();
			newScene->SetName(name);
			m_scenes.emplace(name, std::move(newScene));
		}
	}

public:
	GameObject* Find(const std::wstring& name) const;
};