#pragma once

class Transform;

class TransformSystem
{
private:
	std::vector<Transform*> m_transforms;

public:
	void Register(Transform* transform);
	void Unregister(Transform* transform);
};