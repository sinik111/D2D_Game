#pragma once

#include "Scene.h"

class SceneManager
{
private:
	std::unordered_map<std::wstring, std::unique_ptr<Scene>> m_Scenes;

	Scene* m_pCurrentScene;
	Scene* m_pNextScene;

private:
	SceneManager();
	~SceneManager() = default;

public:
	void Shutdown();

public:
	void Update();
	//void Render();

public:
	void ChangeScene(const std::wstring& name);
	Scene* GetCurrentScene();

private:
	void CheckSceneChanged();

public:
	template<typename T>
	T* CreateScene(const std::wstring& name)
	{
		if (m_Scenes.find(name) != m_Scenes.end())
		{
			return nullptr;
		}

		T* pScene = new T();
		m_Scenes.emplace(name, pScene);

		return pScene;
	}
};