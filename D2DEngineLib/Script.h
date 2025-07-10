#pragma once

#include "Component.h"
#include "GameObject.h"

class Script :
	public Component
{
private:
	bool m_isStarted = false;

public:
	GameObject* CreateGameObject(const std::wstring& name = L"GameObject");

	void SetEnabled(bool enabled) override;

private:
	void RegisterToSystem() override;
	void UnregisterFromSystem() override;

	void Initialize() override;
	virtual void OnEnable();
	virtual void Start();
	virtual void FixedUpdate();
	virtual void Update();
	virtual void LateUpdate();
	virtual void OnDisable();
	void OnDestroy() override;

	friend class ScriptSystem;
	friend class GameObject;
};