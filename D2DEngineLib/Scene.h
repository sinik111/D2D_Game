#pragma once

#include "GameObject.h"

class Scene
{
protected:
	std::vector<std::unique_ptr<GameObject>> m_gameObjects;
	std::vector<GameObject*> m_createdGameObjects;
	std::wstring m_name;

public:
	virtual ~Scene() = default;

public:
	virtual void Enter() = 0;
	virtual void Exit();

public:
	void InitializeObjects();
	void CleanupDestroyedObjects();
	void SetName(const std::wstring& name);
	const std::wstring& GetName() const;

private:
	void Clear();

public:
	GameObject* CreateGameObject(const std::wstring& name = L"GameObject");

	GameObject* Find(const std::wstring& name) const;
};