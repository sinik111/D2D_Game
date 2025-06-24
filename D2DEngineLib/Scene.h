#pragma once

#include "GameObject.h"

class Scene
{
protected:
	std::vector<std::unique_ptr<GameObject>> m_gameObjects;
	std::vector<std::unique_ptr<GameObject>> m_pendingCreatedGameObjects;
	std::vector<std::unique_ptr<GameObject>> m_pendingDestroyedGameObjects;
	std::unordered_set<GameObject*> m_validGameObjects;

public:
	virtual ~Scene();

public:
	virtual void Load() = 0;
	virtual void Enter();
	virtual void Exit();
	virtual void Unload() = 0;

public:
	void Update();

private:
	void Clear();

public:
	GameObject* CreateGameObject(const std::wstring& name = L"GameObject");
	GameObject* Find(const std::wstring& name) const;
	bool IsValid(GameObject* gameObject) const;
};