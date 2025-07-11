#pragma once

#include "GameObject.h"

#define CreateGameObjectWithComponents(name, addingComponents)\
__CreateGameObject(name);\
__AddComponentAndInitialize([&]() {\
addingComponents })

class Scene
{
protected:
	std::vector<std::unique_ptr<GameObject>> m_gameObjects;
	std::vector<GameObject*> m_createdLastFrameGameObjects;

public:
	virtual ~Scene() = default;

public:
	virtual void Enter() = 0;
	virtual void Exit();

public:
	void InitializeObjectsCreatedLastFrame();
	void CleanupDestroyedObjects();

private:
	void Clear();

public:
	GameObject* CreateGameObject(const std::wstring& name = L"GameObject");
	GameObject* __CreateGameObject(const std::wstring& name = L"GameObject"); // 직접 호출 금지. 매크로를 통해서만 호출

	template<typename F>
	void __AddComponentAndInitialize(F&& componentAdder)
	{
		GameObject* gameObject = m_gameObjects.back().get();

		componentAdder();

		gameObject->Initialize();
	}

	GameObject* Find(const std::wstring& name) const;
};