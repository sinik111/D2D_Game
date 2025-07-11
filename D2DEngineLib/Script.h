#pragma once

#include "Component.h"
#include "GameObject.h"

class Script :
	public Component
{
public:
	Script();
	~Script() override;

public:
	GameObject* CreateGameObject(const std::wstring& name = L"GameObject");

private:
	virtual void Initialize();
	virtual void Start();
	virtual void FixedUpdate();
	virtual void Update();
	virtual void LateUpdate();
	virtual void OnDestroy();

	friend class ScriptSystem;
	friend void GameObject::CallOnDestroy(Script* script);
};