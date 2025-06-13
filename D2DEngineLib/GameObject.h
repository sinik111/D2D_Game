#pragma once

class Transform;

class GameObject
{
protected:
	Transform* m_transform;

public:
	GameObject();
	virtual ~GameObject();

public:
	Transform* GetTransform();

	virtual void Update() = 0;
	virtual void Render() = 0;
};