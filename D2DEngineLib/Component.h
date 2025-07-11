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
	virtual ~Component() = default;

public:
	GameObject* GetGameObject() const;
	Transform* GetTransform() const;
	void Destroy(Object* object) override;

private:
	void Destroy() override;

private:
	virtual void SetOwner(GameObject* gameObject);

	friend class GameObject;
};