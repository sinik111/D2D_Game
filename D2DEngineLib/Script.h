#pragma once

#include "Component.h"

class Script :
	public Component
{
public:
	Script();
	~Script() override;

public:
	GameObject* CreateGameObject(const std::wstring& name = L"GameObject");

private:
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	friend class ScriptSystem;
};