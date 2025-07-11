#pragma once

#include "Component.h"
#include "GameObject.h"
#include "SceneManager.h"

#define CreateGameObjectWithComponents(name, addingComponents)\
__CreateGameObject(name);\
__AddComponentAndInitialize([&]() {\
addingComponents })

class Script :
	public Component
{
public:
	virtual ~Script() override;

public:
	GameObject* CreateGameObject(const std::wstring& name = L"GameObject");
	GameObject* __CreateGameObject(const std::wstring& name = L"GameObject");

	template<typename F>
	void __AddComponentAndInitialize(F&& componentAdder)
	{
		SceneManager::Get().GetCurrentScene()->__AddComponentAndInitialize(componentAdder);
	}

public:
	void RegisterToSystem() override;
	void UnregisterFromSystem() override;

private:
	virtual void Initialize() override;
	virtual void Start();
	virtual void FixedUpdate();
	virtual void Update();
	virtual void LateUpdate();
	virtual void OnDestroy();

	friend class ScriptSystem;
	friend class GameObject;
};