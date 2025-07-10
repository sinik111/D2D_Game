#pragma once

#include <functional>
#include "GameObject.h"

#define CreateGameObjectF(name, addingComponents)\
__CreateGameObject(name);\
__AddComponentAndInitialize([&]() {\
addingComponents })

class Scene
{
protected:
	std::vector<std::unique_ptr<GameObject>> m_gameObjects;
	std::vector<std::unique_ptr<GameObject>> m_pendingInitializeGameObjects;

public:
	virtual ~Scene() = default;

public:
	virtual void Enter() = 0;
	virtual void Exit();

public:
	void CleanupDestroyedGameObjects();

private:
	void Clear();

public:
	GameObject* CreateGameObject(const std::wstring& name = L"GameObject");
	GameObject* __CreateGameObject(const std::wstring& name = L"GameObject"); // 직접 호출 금지. 매크로를 통해서만 호출

	void InitializeGameObjectsCreatedLastFrame();

	template<typename F>
	void __AddComponentAndInitialize(F&& addComponentsFunc)
	{
		GameObject* newGameObject = m_gameObjects.back().get();

		addComponentsFunc();

		newGameObject->Initialize();
	}

	GameObject* Find(const std::wstring& name) const;
};