#pragma once

class GameObject;
class Transform;

class Component
{
private:
	GameObject* m_owner;

public:
	Component();
	virtual ~Component() = default;

public:
	GameObject* GetGameObject();
	Transform* GetTransform();

private:
	void SetOwner(GameObject* gameObject);

	friend class GameObject;
};