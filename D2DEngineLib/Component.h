#pragma once

#include "Object.h"

class GameObject;
class Transform;

class Component :
	public Object
{
private:
	GameObject* m_owner = nullptr;
	bool m_isEnabled = true;

public:
	virtual ~Component() = default;

public:
	GameObject* GetGameObject() const;
	Transform* GetTransform() const;
	void Destroy(Object* object) override;
	bool IsEnabled() const;
	bool IsActiveAndEnabled() const;
	virtual void SetEnabled(bool enabled);

private:
	virtual void Initialize();
	virtual void RegisterToSystem();
	virtual void UnregisterFromSystem();
	virtual void OnDestroy();
	void Destroy() override;

private:
	virtual void SetOwner(GameObject* gameObject);

	friend class GameObject;
};