#pragma once

#include "GameObject.h"

class Scene
{
protected:
	std::vector<std::unique_ptr<GameObject>> m_gameObjects;
	std::vector<std::unique_ptr<GameObject>> m_pendingCreatedGameObjects;
	std::vector<std::unique_ptr<GameObject>> m_pendingDestroyedGameObjects;

public:
	virtual ~Scene();

public:
	GameObject* CreateGameObject(const std::wstring& name = L"GameObject");

public:
	virtual void Load() = 0;
	virtual void Enter();
	virtual void Exit();
	virtual void Unload() = 0;

public:
	void Update();

private:
	void Clear();
};