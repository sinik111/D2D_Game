#pragma once

class GameObject;
class Transform;

class Component
{
private:
	GameObject* m_owner = nullptr;

public:
	Component();
	virtual ~Component();

public:
	GameObject* GetGameObject() const;
	Transform* GetTransform() const;

	bool IsValid(Component* component) const;

private:
	void SetOwner(GameObject* gameObject);

	friend class GameObject;
};