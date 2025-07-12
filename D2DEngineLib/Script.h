#pragma once

#include "Component.h"
#include "GameObject.h"
#include "SceneManager.h"

class Script :
	public Component
{
public:
	virtual ~Script() override;

public:
	GameObject* CreateGameObject(const std::wstring& name = L"GameObject");

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