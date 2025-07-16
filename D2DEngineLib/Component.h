#pragma once

#include "Object.h"

class GameObject;
class Transform;

class Component :
	public Object
{
private:
	GameObject* m_owner = nullptr;

public:
	virtual ~Component() override = default;

public:
	GameObject* GetGameObject() const;
	Transform* GetTransform() const;
	void Destroy(Object* object, float delay = 0.0f) override;

public:
	virtual void RegisterToSystem();
	virtual void UnregisterFromSystem();

public:
	virtual void Initialize();

private:
	void Destroy() override;

private:
	virtual void SetOwner(GameObject* gameObject);

	friend class GameObject;
};