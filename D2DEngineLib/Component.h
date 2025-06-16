#pragma once

class GameObject;

class Component
{
protected:
	GameObject* m_owner;

public:
	Component();
	virtual ~Component() = default;

public:
	void OnEnable();
	void OnDestroy();

private:
	void SetOwner(GameObject* gameObject);

	friend class GameObject;
};