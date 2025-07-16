#pragma once

#include "Component.h"
#include "GameObject.h"
#include "SceneManager.h"

struct Collision;

class Script :
	public Component
{
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

	virtual void OnCollisionEnter(const Collision& collision);
	virtual void OnCollisionStay(const Collision& collision);
	virtual void OnCollisionExit(const Collision& collision);
	virtual void OnTriggerEnter(const Collision& collision);
	virtual void OnTriggerStay(const Collision& collision);
	virtual void OnTriggerExit(const Collision& collision);

	friend class ScriptSystem;
	friend class GameObject;
	friend class BoxCollider2D;
};